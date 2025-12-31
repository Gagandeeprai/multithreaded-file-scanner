#include <dirent.h>
#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "browser.h"
#include "menu.h"
#include "scanner.h"
#include "ui.h"

#define MAX_THREADS 4096

pthread_t threads[MAX_THREADS];
int thread_count = 0;

static volatile int scan_done = 0;
static volatile int cancel_scan = 0;
static volatile int active_threads = 0;
pthread_mutex_t active_mutex = PTHREAD_MUTEX_INITIALIZER;

/* ---------- Worker thread ---------- */
void *thread_scan(void *arg) {
  char *path = (char *)arg;

  /* Increment active threads */
  pthread_mutex_lock(&active_mutex);
  active_threads++;
  pthread_mutex_unlock(&active_mutex);

  if (!cancel_scan) {
    int infected = scan_file(path);
    ui_log(infected ? "INFECTED" : "CLEAN", path);
  }

  /* Decrement active threads */
  pthread_mutex_lock(&active_mutex);
  active_threads--;
  pthread_mutex_unlock(&active_mutex);

  free(path);
  return NULL;
}

/* ---------- Recursive traversal ---------- */
void scan_path(const char *path) {
  if (cancel_scan)
    return;

  struct stat st;
  if (stat(path, &st) != 0)
    return;

  if (S_ISREG(st.st_mode)) {
    if (thread_count >= MAX_THREADS)
      return;
    pthread_create(&threads[thread_count++], NULL, thread_scan, strdup(path));
  } else if (S_ISDIR(st.st_mode)) {
    DIR *d = opendir(path);
    if (!d)
      return;

    struct dirent *e;
    while ((e = readdir(d))) {
      if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, ".."))
        continue;

      char full[1024];
      snprintf(full, sizeof(full), "%s/%s", path, e->d_name);
      scan_path(full);
    }
    closedir(d);
  }
}

/* ---------- UI refresh loop ---------- */
void *ui_loop(void *arg) {
  (void)arg;
  nodelay(stdscr, TRUE); /* Non-blocking input for this loop */

  while (!scan_done) {
    ui_update_status(scanned_count, infected_count, active_threads,
                     cancel_scan ? "Cancelling..." : "Scanning...",
                     "Press 'q' to cancel");
    usleep(100000);

    int ch = getch();
    if (ch == 'q' || ch == 'Q')
      cancel_scan = 1;
  }

  /* Final update to ensure "0 active" is shown if finished */
  ui_update_status(scanned_count, infected_count, 0, "Scan Complete",
                   "Press any key to return");

  nodelay(stdscr, FALSE); /* Restore blocking input */
  return NULL;
}

/* ---------- VIEW LOGS ---------- */
void show_logs() {
  clear();
  box(stdscr, 0, 0);
  mvprintw(1, 2, "Previous Scan Reports");

  FILE *fp = fopen("scan.log", "r");
  int row = 3;

  if (!fp) {
    mvprintw(row, 2, "No reports found.");
  } else {
    char line[512];
    while (fgets(line, sizeof(line), fp) && row < LINES - 2) {
      mvprintw(row++, 2, "%s", line);
    }
    fclose(fp);
  }

  mvprintw(LINES - 2, 2, "Press any key to return");
  refresh();
  getch();
}

/* ---------- ABOUT ---------- */
void show_about() {
  clear();
  box(stdscr, 0, 0);

  mvprintw(2, 4, "Multithreaded File Scanner v1.0");
  mvprintw(4, 4, "Features:");
  mvprintw(5, 6, "- Interactive folder browser");
  mvprintw(6, 6, "- Multithreaded scanning");
  mvprintw(7, 6, "- Signature-based detection");
  mvprintw(8, 6, "- ncurses terminal UI");
  mvprintw(9, 6, "- Progress bar & cancel scan");

  mvprintw(LINES - 2, 4, "Press any key to return");
  refresh();
  getch();
}

/* ---------- MAIN ---------- */
int main() {
  ui_init();
  load_signatures("signatures.txt");

  while (1) {
    int choice = show_menu();

    /* ===== Scan ===== */
    if (choice == 0) {
      char *folder = browse_folders(".");
      if (!folder)
        continue;

      /* Clear screen to remove browser artifacts before scan UI starts */
      clear();
      refresh();

      /* Reset logs from previous run */
      ui_reset();

      scanned_count = infected_count = 0;
      thread_count = 0;
      cancel_scan = 0;
      scan_done = 0;

      pthread_t ui_thread;
      pthread_create(&ui_thread, NULL, ui_loop, NULL);

      scan_path(folder);

      for (int i = 0; i < thread_count; i++)
        pthread_join(threads[i], NULL);

      scan_done = 1;
      pthread_join(ui_thread, NULL);

      ui_update_status(scanned_count, infected_count, 0,
                       cancel_scan ? "Scan cancelled by user."
                                   : "Scan complete.",
                       "Press any key to return");
      // Force refresh of log to ensure last items are seen ? ui_log handles
      // wrefresh
      getch();

      free(folder);
    }

    /* ===== View Reports ===== */
    else if (choice == 1) {
      show_logs();
    }

    /* ===== About ===== */
    else if (choice == 2) {
      show_about();
    }

    /* ===== Exit ===== */
    else if (choice == 3) {
      break;
    }
  }

  ui_finish();
  return 0;
}

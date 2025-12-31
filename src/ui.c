#include "ui.h"
#include <ncurses.h>
#include <pthread.h>
#include <string.h>

static WINDOW *header_win = NULL;
static WINDOW *log_win = NULL;
static pthread_mutex_t ui_mutex = PTHREAD_MUTEX_INITIALIZER;

void ui_init() {
  pthread_mutex_lock(&ui_mutex);

  initscr();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);
  // nodelay is managed by specific loops now
  start_color();

  init_pair(1, COLOR_WHITE, COLOR_BLUE);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_RED, COLOR_BLACK);

  int h, w;
  getmaxyx(stdscr, h, w);

  header_win = newwin(6, w, 0, 0);
  wbkgd(header_win, COLOR_PAIR(1));

  log_win = newwin(h - 6, w, 6, 0);
  scrollok(log_win, TRUE);

  refresh();

  pthread_mutex_unlock(&ui_mutex);
}

void ui_reset() {
  pthread_mutex_lock(&ui_mutex);
  if (log_win) {
    werase(log_win);
    wrefresh(log_win);
  }
  pthread_mutex_unlock(&ui_mutex);
}

/* Need unistd.h for getpid */
#include <unistd.h>

void ui_update_status(int files, int infected, int active_threads,
                      const char *msg, const char *footer) {
  pthread_mutex_lock(&ui_mutex);

  /* Overwrite instead of erase to reduce flicker */
  werase(header_win);
  box(header_win, 0, 0);

  mvwprintw(header_win, 1, 2, "Multithreaded File Scanner - OS Project");
  mvwprintw(header_win, 1, 40, "[ PID: %d ]", getpid());

  /* Stat Line 1 */
  mvwprintw(header_win, 2, 2, "Files: %-6d | Infected: %-6d", files, infected);
  /* Stat Line 2: OS Elements */
  mvwprintw(header_win, 2, 40, "Active Threads: %-4d", active_threads);

  mvwprintw(header_win, 3, 2, "%-60s", msg);

  /* ---- Activity Spinner ---- */
  const char *spinner = "|/-\\";
  if (active_threads > 0)
    mvwprintw(header_win, 4, 2, "[ %c ] Scanning Activity", spinner[files % 4]);
  else
    mvwprintw(header_win, 4, 2, "[ - ] Idle/Finished");

  mvwprintw(header_win, 5, 2, "%s", footer);

  wrefresh(header_win);
  pthread_mutex_unlock(&ui_mutex);
}

void ui_log(const char *type, const char *file) {
  pthread_mutex_lock(&ui_mutex);

  if (strcmp(type, "INFECTED") == 0) {
    wattron(log_win, COLOR_PAIR(3) | A_BOLD);
    wprintw(log_win, "[INFECTED] ");
    wattroff(log_win, COLOR_PAIR(3) | A_BOLD);
  } else {
    wattron(log_win, COLOR_PAIR(2));
    wprintw(log_win, "[CLEAN]    ");
    wattroff(log_win, COLOR_PAIR(2));
  }

  wprintw(log_win, "%s\n", file);
  wrefresh(log_win);

  pthread_mutex_unlock(&ui_mutex);
}

void ui_finish() {
  pthread_mutex_lock(&ui_mutex);
  if (header_win)
    delwin(header_win);
  if (log_win)
    delwin(log_win);
  endwin();
  pthread_mutex_unlock(&ui_mutex);
}

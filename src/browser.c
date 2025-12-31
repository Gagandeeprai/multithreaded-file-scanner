#include "browser.h"
#include <dirent.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_ITEMS 512
#define PATH_LEN 1024

typedef struct {
  char name[PATH_LEN];
  int is_dir;
} Item;

static int load_dir(const char *path, Item items[], int *count) {
  DIR *dir = opendir(path);
  if (!dir)
    return -1;

  *count = 0;
  struct dirent *e;

  /* Add .. manually */
  strcpy(items[*count].name, "..");
  items[*count].is_dir = 1;
  (*count)++;

  while ((e = readdir(dir)) && *count < MAX_ITEMS) {
    if (e->d_name[0] == '.')
      continue;

    char full[PATH_LEN];
    snprintf(full, sizeof(full), "%s/%s", path, e->d_name);

    struct stat st;
    if (stat(full, &st) == 0 && S_ISDIR(st.st_mode)) {
      strcpy(items[*count].name, e->d_name);
      items[*count].is_dir = 1;
      (*count)++;
    }
  }

  closedir(dir);
  return 0;
}

char *browse_folders(const char *start_path) {
  char cwd[PATH_LEN];
  realpath(start_path, cwd);

  Item items[MAX_ITEMS];
  int count = 0;
  int highlight = 0;

  keypad(stdscr, TRUE);

  if (!has_colors())
    start_color();

  /* Retrowave Theme (Same as Menu) */
  init_pair(10, COLOR_CYAN, COLOR_BLACK);  // Text
  init_pair(11, COLOR_BLACK, COLOR_CYAN);  // Selection
  init_pair(13, COLOR_WHITE, COLOR_BLACK); // Normal

  while (1) {
    load_dir(cwd, items, &count);

    erase();
    bkgd(COLOR_PAIR(10));
    box(stdscr, 0, 0);

    attron(COLOR_PAIR(10) | A_BOLD);
    mvprintw(1, 2, " Select Folder to Scan ");
    attroff(COLOR_PAIR(10) | A_BOLD);

    attron(COLOR_PAIR(13));
    mvprintw(2, 2, "Current: %s", cwd);
    attroff(COLOR_PAIR(13));

    /* Footer */
    attron(COLOR_PAIR(10) | A_DIM);
    mvprintw(LINES - 2, 2, " [ENTER] Open  [S] Scan Folder  [Q] Cancel ");
    attroff(COLOR_PAIR(10) | A_DIM);

    for (int i = 0; i < count && i < LINES - 5; i++) {
      if (i == highlight) {
        /* BLACK text on CYAN background */
        attron(COLOR_PAIR(11));
        mvprintw(4 + i, 4, " [DIR] %-30s ", items[i].name);
        attroff(COLOR_PAIR(11));
      } else {
        attron(COLOR_PAIR(13));
        mvprintw(4 + i, 4, " [DIR] %s", items[i].name);
        attroff(COLOR_PAIR(13));
      }
    }

    refresh();

    int ch = getch();
    if (ch == KEY_UP && highlight > 0)
      highlight--;
    else if (ch == KEY_DOWN && highlight < count - 1)
      highlight++;
    else if (ch == 10) { /* ENTER */
      if (strcmp(items[highlight].name, "..") == 0) {
        char *slash = strrchr(cwd, '/');
        if (slash && slash != cwd)
          *slash = '\0';
      } else {
        strcat(cwd, "/");
        strcat(cwd, items[highlight].name);
      }
      highlight = 0;
    } else if (ch == KEY_BACKSPACE || ch == 127) {
      char *slash = strrchr(cwd, '/');
      if (slash && slash != cwd)
        *slash = '\0';
    } else if (ch == 's' || ch == 'S') {
      return strdup(cwd); /* ✅ SELECTED */
    } else if (ch == 'q' || ch == 'Q') {
      return NULL;
    }
  }
}

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "menu.h"
#include "ui.h"

static const char *choices[] = {"Scan a folder", "View previous scan reports",
                                "About this project", "Exit"};

#define NUM_CHOICES 4

int show_menu() {
  int highlight = 0;
  int choice = -1;
  int ch;

  /* Initialize colors if not already done */
  if (!has_colors()) {
    start_color();
  }

  /* Retrowave / Tech Theme */
  init_pair(10, COLOR_CYAN, COLOR_BLACK);    // Text
  init_pair(11, COLOR_BLACK, COLOR_CYAN);    // Selection
  init_pair(12, COLOR_MAGENTA, COLOR_BLACK); // Title
  init_pair(13, COLOR_WHITE, COLOR_BLACK);   // Normal

  keypad(stdscr, TRUE);

  while (1) {
    erase(); // Cleaner than clear()
    bkgd(COLOR_PAIR(10));
    box(stdscr, 0, 0);

    /* Title */
    attron(COLOR_PAIR(12) | A_BOLD);
    mvprintw(2, 10, "  MULTITHREADED ANTIVIRUS SCANNNER  ");
    attroff(COLOR_PAIR(12) | A_BOLD);

    attron(COLOR_PAIR(10));
    mvprintw(3, 14, "v1.0 - OS Project");
    attroff(COLOR_PAIR(10));

    /* Menu Items */
    for (int i = 0; i < NUM_CHOICES; i++) {
      int y = 6 + (i * 2); // Spaced out

      if (i == highlight) {
        attron(COLOR_PAIR(11));
        mvprintw(y, 10, "  > %s  ", choices[i]); // Wider selection bar
        attroff(COLOR_PAIR(11));
      } else {
        attron(COLOR_PAIR(13));
        mvprintw(y, 10, "    %s  ", choices[i]);
        attroff(COLOR_PAIR(13));
      }
    }

    /* Footer */
    attron(COLOR_PAIR(10) | A_DIM);
    mvprintw(LINES - 2, 4, "[UP/DOWN] Select  [ENTER] Confirm");
    attroff(COLOR_PAIR(10) | A_DIM);

    refresh();

    ch = getch();
    switch (ch) {
    case KEY_UP:
      highlight = (highlight - 1 + NUM_CHOICES) % NUM_CHOICES;
      break;
    case KEY_DOWN:
      highlight = (highlight + 1) % NUM_CHOICES;
      break;
    case 10: // ENTER
      choice = highlight;
      return choice;
    }
  }
}

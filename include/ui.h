#ifndef UI_H
#define UI_H

/*
 * Text-based User Interface (TUI) using ncurses
 * NOTE: All UI functions must be called from the MAIN thread only.
 */

/* Initialize the ncurses UI */
void ui_init();

/* Restore terminal and clean up UI */
void ui_finish();

/* Reset log window */
void ui_reset();

/* Update the top status bar with current scan info */
void ui_update_status(int files_scanned, int infected_count, int active_threads,
                      const char *current_file, const char *footer);

/* Append a log entry for a scanned file */
void ui_log(const char *type, const char *filepath);

#endif /* UI_H */

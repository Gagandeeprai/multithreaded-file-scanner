#ifndef BROWSER_H
#define BROWSER_H

/* Opens an interactive folder browser.
 * Returns a malloc'ed string containing the selected folder path,
 * or NULL if user cancels.
 */
char *browse_folders(const char *start_path);

#endif

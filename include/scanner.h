#ifndef SCANNER_H
#define SCANNER_H

void load_signatures(const char *filename);
int scan_file(const char *filepath);

void increment_scanned();
void increment_infected();

/* NEW: store infected file path */
void record_infected(const char *filepath);

/* Access infected list */
char **get_infected_files(int *count);

/* Counters */
extern int scanned_count;
extern int infected_count;

#endif

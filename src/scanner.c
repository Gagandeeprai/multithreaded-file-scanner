#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scanner.h"

/* Counters */
int infected_count = 0;
int scanned_count = 0;

/* Signatures */
static char signatures[100][256];
static int sig_count = 0;

/* Infected files list */
static char *infected_files[4096];
static int infected_index = 0;

/* Mutex */
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Load signatures */
void load_signatures(const char *filename) {
  FILE *fp = fopen(filename, "r");
  if (!fp) {
    perror("signatures");
    exit(1);
  }

  while (fgets(signatures[sig_count], sizeof(signatures[sig_count]), fp)) {
    signatures[sig_count][strcspn(signatures[sig_count], "\r\n")] = 0;
    if (strlen(signatures[sig_count]) > 0)
      sig_count++;
  }
  fclose(fp);
}

/* Binary-safe search */
static int mem_search(const unsigned char *buf, size_t len, const char *pat) {
  size_t plen = strlen(pat);
  if (plen == 0 || plen > len)
    return 0;

  for (size_t i = 0; i <= len - plen; i++)
    if (memcmp(buf + i, pat, plen) == 0)
      return 1;

  return 0;
}

/* Record infected file */
/* Record infected file */
void record_infected(const char *filepath) {
  pthread_mutex_lock(&mutex);

  /* Update in-memory list */
  if (infected_index < 4096)
    infected_files[infected_index++] = strdup(filepath);

  /* Update log file */
  FILE *log = fopen("scan.log", "a");
  if (log) {
    fprintf(log, "INFECTED: %s\n", filepath);
    fclose(log);
  }

  pthread_mutex_unlock(&mutex);
}

/* Access infected list */
char **get_infected_files(int *count) {
  *count = infected_index;
  return infected_files;
}

/* Scan file */
int scan_file(const char *filepath) {

  /* Skip source/build files to prevent self-detection */
  const char *ext = strrchr(filepath, '.');
  if (ext && (!strcmp(ext, ".c") || !strcmp(ext, ".h") || !strcmp(ext, ".o")))
    return 0;

  FILE *fp = fopen(filepath, "rb");
  if (!fp)
    return 0;

  increment_scanned();

  /* Overlap buffer strategy */
  const size_t CHUNK_SIZE = 1024;
  const size_t OVERLAP = 256;
  unsigned char buf[CHUNK_SIZE + OVERLAP];

  size_t bytes_in_buffer = 0;
  size_t r;

  while ((r = fread(buf + bytes_in_buffer, 1, CHUNK_SIZE, fp)) > 0) {
    size_t total = bytes_in_buffer + r;

    for (int i = 0; i < sig_count; i++) {
      if (mem_search(buf, total, signatures[i])) {
        fclose(fp);
        increment_infected();
        record_infected(filepath);
        return 1;
      }
    }

    /* Keep overlap for next chunk */
    if (total > OVERLAP) {
      memcpy(buf, buf + total - OVERLAP, OVERLAP);
      bytes_in_buffer = OVERLAP;
    } else {
      bytes_in_buffer = 0;
    }
  }

  fclose(fp);
  return 0;
}

/* Counters */
void increment_scanned() {
  pthread_mutex_lock(&mutex);
  scanned_count++;
  pthread_mutex_unlock(&mutex);
}

void increment_infected() {
  pthread_mutex_lock(&mutex);
  infected_count++;
  pthread_mutex_unlock(&mutex);
}

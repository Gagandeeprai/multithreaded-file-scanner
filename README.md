# Multithreaded File Scanner

A high-performance file scanner built in C using Ncurses for the UI and POSIX threads for parallel scanning.

## Features
- **Parallel Scanning**: Uses multiple worker threads to scan files concurrently.
- **Ncurses UI**: Professional CLI interface with live logs and status indicators.
- **OS Integration**: Displays real-time Process ID and Active Thread counts.
- **Interactive Menu**: Navigate options and browse folders easily.
- **Robust Detection**: Signature-based detection with overlap buffering for reliability.

## Build and Run

### Prerequisites
- GCC
- Ncurses library (`libncurses5-dev` on Ubuntu/WSL)
- Pthreads library (usually included with GCC)

### Build
```bash
make clean && make
```

### Run
```bash
./multithreaded-file-scanner
```

## OS Concepts Demonstrated
- **Multithreading**: Use of `pthread_create` and `pthread_join`.
- **Synchronization**: Thread-safe counters using `pthread_mutex_t`.
- **Process Management**: Monitoring `getpid()` and active execution paths.
- **File Systems**: Recursive directory traversal using `dirent.h`.
- **Memory Management**: dynamic allocations and cleanup.

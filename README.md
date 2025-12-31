
# 🛡️ Multithreaded File Scanner

> A fast, interactive, terminal-based file scanner built in **C**  
> powered by **POSIX threads** and a **ncurses UI**

<p align="center">
  <b>⚙️ Operating Systems Mini Project • 💻 Linux / WSL • 🧵 Multithreaded</b>
</p>

---

## ✨ What is this?

**Multithreaded File Scanner** is a **real system-level CLI tool** that scans
files and folders concurrently for malicious signatures — all inside a
**professional ncurses interface**.

No typing long paths.  
No frozen terminal.  
Just clean UI, parallel scanning, and full control.

---

## 🚀 Features

✅ **Multithreaded Scanning**  
Scan hundreds of files simultaneously using `pthreads`.

✅ **Interactive Terminal UI (ncurses)**  
Menu-driven interface with live updates, logs, and progress display.

✅ **Folder Browser**  
Navigate directories using arrow keys — *no manual path typing*.

✅ **Progress Bar + Live Stats**  
See scanned file count, infections, and activity in real time.

✅ **Scan Cancellation**  
Press **`q`** anytime to safely stop scanning.

✅ **Signature-Based Detection**  
Binary-safe scanning with buffered reads.

✅ **User-Controlled Deletion**  
Detected infected files can be deleted **only after user confirmation**.

---

## 🖥️ Demo Flow

```text
./antivirus
 ├── Scan a folder
 │    ├── Browse folders
 │    ├── Scan in parallel
 │    ├── View progress
 │    └── Delete infected files (optional)
 ├── View previous reports
 ├── About
 └── Exit
````

---

## 🧠 OS Concepts Demonstrated

* 🧵 **Multithreading**

  * `pthread_create`, `pthread_join`
  * Worker-thread model

* 🔒 **Synchronization**

  * Mutex-protected shared counters
  * Safe data sharing across threads

* 📂 **File System Traversal**

  * Recursive directory walking (`dirent.h`)
  * Metadata inspection (`stat`)

* 🧠 **Memory Management**

  * Dynamic allocation and cleanup
  * Leak-safe design

* 🖥️ **Terminal I/O**

  * ncurses-based UI
  * Non-blocking input handling

---

## 🛠️ Build & Run

### Prerequisites

* GCC
* ncurses development library

```bash
sudo apt install libncurses5-dev
```

### Build

```bash
make clean
make
```

### Run

```bash
./multithreaded-file-scanner
```

---

## 📁 Project Structure

```text
.
├── src/
│   ├── main.c        # Application controller
│   ├── scanner.c     # File scanning engine
│   ├── ui.c          # ncurses UI
│   ├── menu.c        # Main menu
│   └── browser.c     # Folder navigation
├── include/
│   └── *.h
├── signatures.txt    # Malware signatures
├── scan.log          # Scan reports
└── Makefile
```

---

## ⚠️ Disclaimer

This project is **educational** and designed to demonstrate
**Operating System concepts**.

It is **not** a replacement for commercial antivirus software.

---

## 🧑‍💻 Author

Built as an **Operating Systems Mini Project**
with focus on **concurrency, synchronization, and system-level programming**.

---

⭐ If you like this project, star it.
💬 If you break it, you learned something.

```

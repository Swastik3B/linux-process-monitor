Here is the complete, properly formatted, production-ready `README.md` for your GitHub repository:

```markdown
# Linux Process Monitoring and Management Tool Using C

## 1. Project Title

**Linux Process Monitoring and Management Tool Using C**

---

## 2. Project Overview

The **Linux Process Monitoring and Management Tool** is a system-level utility developed in C designed to inspect, track, and manage processes executing on a Linux operating system. 

By querying the Linux `/proc` virtual filesystem directly, the tool extracts live kernel-level execution and resource metrics without relying on third-party wrapper libraries. It features an interactive command-line interface that allows users to list active processes, examine in-depth metadata for a target PID, review system-wide resource utilization, monitor CPU load, and terminate processes using system signals.

Additionally, the project includes an accompanying Linux character device driver implemented as a kernel module (`.ko`) to demonstrate user-space to kernel-space interactions, file operations, and device registration.

---

## 3. Problem Statement

Modern Linux environments execute hundreds of concurrent foreground and background processes. While standard utilities like `ps`, `top`, or `htop` exist, building a native C-based process manager demonstrates foundational systems engineering:
- Interfacing with kernel pseudo-filesystems (`/proc`).
- Parsing raw system state metrics.
- Invoking POSIX system calls for process signaling and control.
- Understanding the architectural boundary between user space and kernel space via a dedicated character device driver.

---

## 4. Objectives

- Implement a standalone process monitoring and management suite in C.
- Parse and extract process metrics from the Linux `/proc` virtual filesystem.
- Display a categorized inventory of active process identifiers (PIDs) and process names.
- Inspect granular process states, parent-child relationships (PPID), thread counts, and memory footprints (VmSize, VmRSS).
- Calculate live CPU utilization metrics from system tick counters (`/proc/stat`).
- Inspect global system statistics including CPU model, memory metrics (`/proc/meminfo`), and system uptime (`/proc/uptime`).
- Implement controlled process termination via POSIX signal handling (`kill()` / `SIGTERM`) with built-in safety constraints.
- Implement and compile an out-of-tree Linux character device driver (`process_monitor_driver.c`).
- Explore kernel module structure, device file abstractions, and compilation workflows using Kernel Makefiles.

---

## 5. Technologies Used

| Technology / Interface | Purpose |
|---|---|
| **C (C99/GNU99)** | Core application & device driver development |
| **Linux / WSL2** | Target operating system & execution environment |
| **GCC** | Compilation of user-space utilities |
| **GNU Make & Kbuild** | Automated compilation of kernel module and userspace bins |
| **`/proc` Filesystem** | Kernel interface for process and hardware telemetry |
| **POSIX System Calls** | Direct OS service invocation (`kill`, `open`, `read`, `write`, `close`) |
| **Linux Kernel Module (LKM)** | Kernel-space character device implementation |
| **Git / GitHub** | Source code management and version tracking |

---

## 6. System Requirements

### Software Requirements
- Linux distribution (Ubuntu 20.04/22.04/24.04 LTS, Debian, Fedora, or WSL2)
- GCC compiler toolchain (`build-essential`)
- GNU Make
- Linux kernel headers (`linux-headers-$(uname -r)`)
- Git

### Hardware Requirements
- x86_64 or ARM64 architecture
- Minimum 2 GB RAM (4 GB recommended)
- 500 MB free storage

---

## 7. Main Features

### 7.1 Running Process Monitoring
Scans the `/proc` directory, filters for numeric directory entries corresponding to active PIDs, and extracts process comm identifiers.

```text
PID        PROCESS NAME
--------------------------------------------
1          systemd
2          kthreadd
...

```

### 7.2 Process Details Inspection

Queries `/proc/<PID>/status` to provide detailed diagnostic metrics for a specific process:

* Process Name
* Execution State (e.g., `S (sleeping)`, `R (running)`)
* Process ID (PID)
* Parent Process ID (PPID)
* Virtual Memory Size (`VmSize`)
* Resident Set Size (`VmRSS`)
* Active Thread Count

```text
============================================
          PROCESS DETAILS
============================================
Name:    systemd
State:   S (sleeping)
Pid:     1
PPid:    0
VmSize:  21684 kB
VmRSS:   13172 kB
Threads: 1
============================================

```

### 7.3 System Resource Telemetry

Parses hardware and runtime telemetry from:

* `/proc/cpuinfo` (CPU model and architecture)
* `/proc/meminfo` (Total, free, and available system memory)
* `/proc/uptime` (Total system uptime and idle time)

### 7.4 CPU Usage Calculation

Reads CPU tick metrics (`user`, `nice`, `system`, `idle`, `iowait`, `irq`, `softirq`) from `/proc/stat` across a sampling interval, computing total utilization percentage:

$$\text{CPU Utilization} = \left( 1 - \frac{\Delta \text{Idle}}{\Delta \text{Total}} \right) \times 100$$

```text
============================================
              CPU INFORMATION
============================================
CPU Usage: 12.45%
============================================

```

### 7.5 Controlled Process Termination

Sends standard POSIX signals using the `kill()` system call with `SIGTERM` (Signal 15), allowing target processes to clean up descriptors and state before exiting.

* **Safety Interlock:** Automatically prohibits termination signals to `PID <= 1` to protect essential system daemons and prevent kernel panics.

### 7.6 Linux Character Device Driver

A modular kernel component demonstrating:

* Kernel module initialization (`module_init`) and cleanup (`module_exit`).
* Dynamic character device major/minor allocation.
* Registration of standard file operations (`file_operations` struct: `open`, `read`, `release`).
* Bridge handling between kernel memory space and user-space buffers (`copy_to_user`).

---

## 8. Application Menu

```text
====================================
   LINUX PROCESS MONITORING TOOL
====================================
1. Show Running Processes
2. Show Process Details
3. Show System Resources
4. Show CPU Usage
5. Terminate a Process
6. Exit
====================================

```

---

## 9. Project Architecture

```text
                 Linux Process Monitoring Tool
                              │
                              ▼
                       C Application
                              │
              ┌───────────────┼───────────────┐
              │               │               │
              ▼               ▼               ▼
        /proc virtual   System Calls    Process Parsing
         filesystem      (kill, open)    (PIDs, Status)
              │               │               │
       ┌──────┼───────┐       │          ┌────┴────┐
       ▼      ▼       ▼       ▼          ▼         ▼
      CPU   Memory  Status  SIGTERM     PID     Thread Count
              │               │
              └───────┬───────┘
                      ▼
              Linux Kernel Space
                      │
                      ▼
           Character Device Driver
                      │
                      ▼
           process_monitor_driver.ko

```

---

## 10. Project Structure

```text
linux-process-monitor/
├── main.c                          # Main monitoring CLI application source
├── process_monitor                 # Compiled monitoring binary
├── driver_test.c                   # Test program for character device driver
├── driver_test                     # Compiled driver test binary
├── driver/
│   ├── Makefile                    # Kbuild Makefile for driver module
│   └── process_monitor_driver.c    # Character device driver source
├── docs/                           # Documentation and design diagrams
├── screenshots/                    # Application and driver execution proofs
├── .gitignore                      # Git ignore file for binaries and object files
└── README.md                       # Project documentation

```

---

## 11. Build and Run Instructions

### 11.1 Compiling the Main Application

```bash
# Clone the repository
git clone [https://github.com/](https://github.com/)<your-username>/linux-process-monitor.git
cd linux-process-monitor

# Compile user-space process monitor
gcc main.c -o process_monitor

# Launch the tool
./process_monitor

```

### 11.2 Compiling the Driver Test Program

```bash
gcc driver_test.c -o driver_test

```

### 11.3 Building the Kernel Module

Navigate to the driver directory and compile using your installed kernel build directory:

```bash
cd driver
make

```

*Or invoke the Kbuild target directly:*

```bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules

```

This generates `process_monitor_driver.ko`.

---

## 12. Important WSL2 / Environment Note

This project was built and validated within a **WSL2 (Windows Subsystem for Linux)** environment.

In WSL2, the active kernel is a customized Microsoft Linux kernel, whereas standard package managers install standard distribution headers (e.g., `linux-headers-generic`). Because Linux enforces strict kernel version and signature matching (`vermagic`), kernel modules compiled against generic headers cannot be loaded directly into a non-matching WSL2 kernel without compiling against the exact WSL2 kernel source.

The driver code and compilation flow are structured to demonstrate valid Linux device driver architecture and compilation mechanics. For full insertion (`insmod`), deploy on a standard native Linux distribution or a matching custom WSL2 kernel build environment.

---

## 13. System Programming & Linux Concepts Demonstrated

* **Virtual Filesystems:** Deep parsing of synthetic interfaces provided by `/proc`.
* **POSIX Signal Interfacing:** Inter-process communication and signal dispatch via `kill()`.
* **Process Memory Models:** Distinguishing resident physical memory (`VmRSS`) from total address space allocation (`VmSize`).
* **File Descriptors and I/O:** Reading stream-based system metrics in user space.
* **Kernel/User Boundary:** Memory separation and safe pointer transfers via `copy_to_user()`.
* **Character Device Lifecycle:** Registering cdev structures, major numbers, and linking `file_operations` callbacks.

---

## 14. Testing and Verification

| Test Scenario | Action Taken | Expected Result | Status |
| --- | --- | --- | --- |
| **Process Listing** | Select Option `1` | All active system PIDs and names enumerated | Passed |
| **Valid Process Details** | Select Option `2`, Enter PID `1` | Displays memory, state, and thread metrics for PID 1 | Passed |
| **Invalid Process Details** | Select Option `2`, Enter invalid PID (`999999`) | Displays clean error handling ("Process not found") | Passed |
| **System Telemetry** | Select Option `3` | Displays CPU architecture, RAM, and uptime | Passed |
| **CPU Calculation** | Select Option `4` | Calculates delta and prints percentage load | Passed |
| **Process Termination** | Select Option `5` on dummy process (`sleep 300`) | Sends `SIGTERM`; process closes cleanly | Passed |
| **Safety Interlock** | Select Option `5`, attempt PID `1` | Rejection notice; critical system process spared | Passed |
| **Menu Validation** | Enter non-numeric or invalid option | Prompts user with invalid choice warning | Passed |

---

## 15. Limitations and Future Scope

### Current Limitations

* Pure command-line interface with manual refresh prompts.
* User permission boundaries dictate visibility into processes owned by `root`.

### Future Enhancements

* **Dynamic curses UI:** Build an interactive terminal dashboard using `ncurses`.
* **Sorting & Filters:** Add column sorting by CPU utilization or memory footprint.
* **Automated Polling:** Background daemon mode with configurable metric logging.
* **Extended Driver Telemetry:** Expand the character device interface to directly expose process block tracking metrics from kernel space.

---

## 16. Author

* **Developer:** Swastik Bishoi
* **Project:** Linux Process Monitoring and Management Tool
* **Language:** C (POSIX / GNU C)
* **Platform:** Linux / WSL2
* **Version:** 1.0.0

```


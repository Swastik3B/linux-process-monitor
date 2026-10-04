# System Architecture

## 1. Overview

The Linux Process Monitoring Tool is a C-based system utility designed to monitor and manage processes and system resources in a Linux environment.

The project uses Linux system interfaces such as `/proc`, POSIX signals, and a Linux character device driver.

## 2. Architecture

```text
+----------------------------------+
|       User / Terminal            |
+----------------+-----------------+
                 |
                 v
+----------------------------------+
|      Process Monitoring Tool     |
|             main.c               |
+----------------+-----------------+
                 |
        +--------+--------+
        |        |        |
        v        v        v
     /proc     Signals  System
     Files     SIGTERM  Resources
        |
        v
+----------------------------------+
|       Linux Kernel               |
+----------------+-----------------+
                 |
                 v
+----------------------------------+
|  Character Device Driver         |
| process_monitor_driver.c        |
+----------------+-----------------+
                 |
                 v
        /dev/process_monitor


3. Main Components
Process Monitoring Application

main.c provides the user interface and performs process monitoring using the Linux /proc filesystem.

Functions include:

Displaying running processes
Displaying process details
Displaying system memory and CPU information
Calculating CPU usage
Terminating processes using SIGTERM
Linux /proc Interface

The application reads information from:

/proc/[PID]/comm
/proc/[PID]/status
/proc/cpuinfo
/proc/meminfo
/proc/uptime
/proc/stat
Character Device Driver

process_monitor_driver.c implements a Linux character device driver.

The driver demonstrates:

Kernel module programming
Character device registration
File operations
Kernel-to-user data transfer
Device creation

The device is named:

/dev/process_monitor
4. Technologies
C Programming
Linux
Linux Kernel Modules
Character Device Driver
/proc filesystem
POSIX Signals
GCC
Make
WSL2
5. Development Note

The character driver was successfully compiled against the available Linux kernel headers (6.8.0-146-generic).

The current WSL2 running kernel uses a different kernel version, so the compiled module is not loaded into the running WSL2 kernel.

This limitation is documented rather than bypassed to maintain kernel compatibility and system safety.


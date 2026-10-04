#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
void show_processes() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if (dir == NULL) {
        perror("Unable to open /proc");
        return;
    }

    printf("\n============================================\n");
    printf("             RUNNING PROCESSES\n");
    printf("============================================\n");
    printf("%-10s %-30s\n", "PID", "PROCESS NAME");
    printf("--------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {

        if (isdigit(entry->d_name[0])) {

            char path[512];
            char process_name[256] = "Unknown";

            snprintf(path, sizeof(path),
                     "/proc/%s/comm", entry->d_name);

            FILE *file = fopen(path, "r");

            if (file != NULL) {
                fgets(process_name, sizeof(process_name), file);
                process_name[strcspn(process_name, "\n")] = '\0';
                fclose(file);
            }

            printf("%-10s %-30s\n",
                   entry->d_name, process_name);
        }
    }

    closedir(dir);
}

void show_process_details() {

    int pid;
    char path[512];
    char line[512];

    printf("\nEnter PID: ");
    scanf("%d", &pid);

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    FILE *file = fopen(path, "r");

    if (file == NULL) {
        printf("\nProcess with PID %d not found.\n", pid);
        return;
    }

    printf("\n============================================\n");
    printf("          PROCESS DETAILS\n");
    printf("============================================\n");

    while (fgets(line, sizeof(line), file) != NULL) {

        if (strncmp(line, "Name:", 5) == 0 ||
            strncmp(line, "State:", 6) == 0 ||
            strncmp(line, "Pid:", 4) == 0 ||
            strncmp(line, "PPid:", 5) == 0 ||
            strncmp(line, "VmSize:", 7) == 0 ||
            strncmp(line, "VmRSS:", 6) == 0 ||
            strncmp(line, "Threads:", 8) == 0) {

            printf("%s", line);
        }
    }

    printf("============================================\n");

    fclose(file);
}

void show_system_info() {
    FILE *file;
    char line[512];

    printf("\n============================================\n");
    printf("          SYSTEM RESOURCE INFORMATION\n");
    printf("============================================\n");

    /* CPU Information */
    file = fopen("/proc/cpuinfo", "r");

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "model name", 10) == 0) {
                printf("CPU %s", line + 11);
                break;
            }
        }
        fclose(file);
    }

    /* Memory Information */
    file = fopen("/proc/meminfo", "r");

    if (file != NULL) {
        while (fgets(line, sizeof(line), file)) {

            if (strncmp(line, "MemTotal:", 9) == 0 ||
                strncmp(line, "MemAvailable:", 13) == 0 ||
                strncmp(line, "MemFree:", 8) == 0) {

                printf("%s", line);
            }
        }

        fclose(file);
    }

    /* System Uptime */
    file = fopen("/proc/uptime", "r");

    if (file != NULL) {

        double uptime;

        if (fscanf(file, "%lf", &uptime) == 1) {

            int hours = (int)(uptime / 3600);
            int minutes = ((int)uptime % 3600) / 60;
            int seconds = (int)uptime % 60;

            printf("System Uptime: %d hours %d minutes %d seconds\n",
                   hours, minutes, seconds);
        }

        fclose(file);
    }

    printf("============================================\n");
}

void terminate_process() {

    int pid;

    printf("\nEnter PID to terminate: ");
    scanf("%d", &pid);

    if (pid <= 1) {
        printf("\nFor safety, PID 1 or lower cannot be terminated.\n");
        return;
    }

    if (kill(pid, SIGTERM) == 0) {
        printf("\nProcess %d termination signal sent successfully.\n", pid);
    } else {
        perror("\nUnable to terminate process");
    }
}

void show_cpu_usage() {
    FILE *file;
    char cpu[10];
    long long user, nice, system, idle, iowait, irq, softirq, steal;

    file = fopen("/proc/stat", "r");

    if (file == NULL) {
        perror("Unable to read CPU information");
        return;
    }

    if (fscanf(file, "%s %lld %lld %lld %lld %lld %lld %lld %lld",
               cpu, &user, &nice, &system, &idle,
               &iowait, &irq, &softirq, &steal) != 9) {
        printf("Unable to read CPU statistics.\n");
        fclose(file);
        return;
    }

    fclose(file);

    long long idle_time = idle + iowait;
    long long total_time = user + nice + system + idle +
                           iowait + irq + softirq + steal;

    double usage = 0.0;

    if (total_time > 0) {
        usage = ((double)(total_time - idle_time) /
                 total_time) * 100.0;
    }

    printf("\n============================================\n");
    printf("              CPU INFORMATION\n");
    printf("============================================\n");
    printf("CPU Usage: %.2f%%\n", usage);
    printf("============================================\n");
}

void show_menu() {

    printf("\n====================================\n");
    printf("   LINUX PROCESS MONITORING TOOL\n");
    printf("====================================\n");
    printf("1. Show Running Processes\n");
    printf("2. Show Process Details\n");
    printf("3. Show System Resources\n");
    printf("4. Show CPU Usage\n");
    printf("5. Terminate a Process\n");
    printf("6. Exit\n");
    printf("====================================\n");
}

int main() {

    int choice;

    while (1) {

        show_menu();

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                show_processes();
                break;

            case 2:
                show_process_details();
                break;
            case 3:
                show_system_info();
                break;
            case 4:
                show_cpu_usage();
                break;
            case 5:
                terminate_process();
                break;
            case 6:
                printf("\nExiting program...\n");
                return 0;

            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    }

    return 0;
}

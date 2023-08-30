//process_functions.cpp
//#include "process_functions.h"
#include <cstdlib> // Include necessary headers here
//#include "process_functions.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


void list_processes()
{
    system("ps aux");
}

// Show process details
void show_process_details()
{
    int pid;
    std::cout << "Enter process ID: ";
    std::cin >> pid;
    char command[256];
    snprintf(command, sizeof(command), "ps -p %d -o pid,ppid,cmd,%%cpu,%%mem", pid);
    system(command);
}

// Kill a process
void kill_process()
{
    int pid;
    std::cout << "Enter process ID: ";
    std::cin >> pid;
    kill(pid, SIGTERM);
}

// Kill all processes that have been running for more than a certain time
void kill_processes_by_time()
{
    int time_threshold;
    std::cout << "Enter time threshold (in seconds): ";
    std::cin >> time_threshold;
    std::string command = "ps -eo pid,etimes | awk '$2 > " + std::to_string(time_threshold) + " { print $1 }' | xargs kill";
    system(command.c_str());
}

// Show the process hierarchy of a specific process
void show_process_hierarchy()
{
    int pid;
    std::cout << "Enter process ID: ";
    std::cin >> pid;
    std::string command = "pstree -p " + std::to_string(pid);
    system(command.c_str());
}

// List all processes that have a certain string in their command line
void list_processes_by_string()
{
    std::string searchString;
    std::cout << "Enter string to search for: ";
    std::cin >> searchString;
    std::string command = "ps aux | grep " + searchString;
    system(command.c_str());
}





































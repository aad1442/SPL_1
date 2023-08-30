//system_function.cpp
//#include "system_functions.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void monitor_cpu_usage()
{
    pid_t pid = fork();
    if (pid == 0)
    {
        execlp("top", "top", NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
    {
        wait(NULL);
    }
    else
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
}


// Show disk usage details
void show_disk_usage()
{
    system("df -h");
}
// Show the number of running processes for each user
void show_running_processes_per_user()
{
    system("ps -eo user=|sort|uniq -c");
}

// Show memory usage details
void show_memory_usage()
{
    system("free -h");
}






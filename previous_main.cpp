#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include <csignal>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits>

void list_open_files()
{
    int pid;
    std::cout << "Enter process ID: ";
    std::cin >> pid;
    std::string command = "ls -l /proc/" + std::to_string(pid) + "/fd";
    system(command.c_str());
}


// List all open network connections and their corresponding process
void list_network_connections()
{
    system("netstat -tunap | grep ESTABLISHED");
}

//aad
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

// List all running processes
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

// Show memory usage details
void show_memory_usage()
{
    system("free -h");
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


void print_menu()
{
    std::cout << "\n";
    std::cout << "1. List all running processes\n";
    std::cout << "2. Show process details\n";
    std::cout << "3. Kill a process\n";
    std::cout << "4. List all open files for a process\n";
    std::cout << "5. List all open network connections\n";
    std::cout << "6. Monitor CPU usage in real-time\n";
    std::cout << "7. Show memory usage details\n";
    std::cout << "8. Show disk usage details\n";
    std::cout << "9. Show the number of running processes for each user\n";
    std::cout << "10. Kill all processes that have been running for more than a certain time\n";
    std::cout << "11. Show the process hierarchy of a specific process\n";
    std::cout << "12. List all processes that have a certain string in their command line\n";
    std::cout << "13. Exit\n";
    std::cout << "\n";
}





int main()
{
    int choice;
    while (true)
    {
        print_menu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        //To clear the input buffer
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice)
        {
        case 1:
            list_processes();
            break;
        case 2:
            show_process_details();
            break;
        case 3:
            kill_process();
            break;
        case 4:
            list_open_files();
            break;
        case 5:
            list_network_connections();
            break;
        case 6:
            monitor_cpu_usage();
            break;
        case 7:
            show_memory_usage();
            break;
        case 8:
            show_disk_usage();
            break;
        case 9:
            show_running_processes_per_user();
            break;
        case 10:
            kill_processes_by_time();
            break;
        case 11:
            show_process_hierarchy();
            break;
        case 12:
            list_processes_by_string();
            break;
        case 13:
            exit(EXIT_SUCCESS);
        default:
            std::cout << "Invalid choice. Try again." << std::endl;
            break;
        }
    }
    return 0;
}


//menu_function.cpp
//#include "../project_folder/menu_functions.h"

#include <iostream>

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
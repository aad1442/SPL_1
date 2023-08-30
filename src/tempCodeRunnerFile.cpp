#include <iostream>
#include "../project_folder/menu_functions.h"
#include "../project_folder/network_functions.h"
#include "../project_folder/process_functions.h"
#include "../project_folder/system_functions.h"

int main() {
    int choice;
    while (true) {
        print_menu();
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                list_processes();
                break;
            case 2:
                show_process_details();
                break;
            case 3:
                kill_process();
                break;
            // ... handle other cases ...
            case 13:
                exit(EXIT_SUCCESS);
            default:
                std::cout << "Invalid choice. Try again." << std::endl;
                break;
        }
    }
    return 0;
}

#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int flag=1;
void runServer() {
    system("gcc server5.c -o server");
    system("./server");
    flag=1;
}

void runClient() {
    system("gcc client5.c -o client");
    system("./client");
    flag=0;
}

int main() {
    int choice;
    
    int childCount = 0;

    while (flag) {
        std::cout << "Choose an option:\n";
        std::cout << "1. Run server\n";
        std::cout << "2. Run client\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                // Fork a child process to run the server in a different terminal
                if (fork() == 0) {
                    flag=0;
                    // Child process
                    runServer();
                    exit(0);  // Child process exits after running the server
                }
                childCount++;
                break;

            case 2:
                // Fork a child process to run the client in a different terminal
                if (fork() == 0) {
                    // Child process
                    flag=0;
                    runClient();
                    exit(0);  // Child process exits after running the client
                }
                childCount++;
                break;

            case 0:
                // Exit the main menu loop
                break;

            default:
                std::cout << "Invalid choice\n";
                break;
        }

        // Check for finished child processes
        int status;
        while (waitpid(-1, &status, WNOHANG) > 0) {
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                childCount--;
            }
        }

        if (choice == 0 && childCount > 0) {
            // Wait for all child processes to finish before exiting the program
            while (childCount > 0) {
                waitpid(-1, &status, 0);
                if (WIFEXITED(status) || WIFSIGNALED(status)) {
                    childCount--;
                }
            }
            return 0;
        }
    }

    return 0;
}

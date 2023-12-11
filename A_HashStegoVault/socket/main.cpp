#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>  // For waitpid function

void runServer() {
    system("gcc server5.c -o server");
    system("./server");
}

void runClient() {
    system("gcc client5.c -o client");
    system("./client img1.bmp");
}

int main() {
    int choice;

    while (true) {
        std::cout << "Choose an option:\n";
        std::cout << "1. Run server\n";
        std::cout << "2. Run client\n";
        std::cout << "0. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                // Fork a child process to run the server in a different terminal
                if (fork() == 0) {
                    // Child process
                    runServer();
                    exit(0);  // Child process exits after running the server
                }
                break;
            }

            case 2: {
                // Fork a child process to run the client in a different terminal
                if (fork() == 0) {
                    // Child process
                    runClient();
                    exit(0);  // Child process exits after running the client
                }
                break;
            }

            case 0:
                // Exit the program
                return 0;

            default:
                std::cout << "Invalid choice\n";
                break;
        }

        // Wait for any child process to finish
        waitpid(-1, NULL, 0);
    }

    return 0;
}

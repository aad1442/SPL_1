#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

int flag = 1;

void runServer() {
    system("gcc server5.c -o server");
    system("./server");
}

void runClient() {
    system("gcc client5.c -o client");
    system("./client");
}

int main() {
    int choice;

    std::cout << "Do you want to send a file?\n";
    std::cout << "1. Run server\n";
    std::cout << "2. Run client\n";
    std::cout << "3. Run both server and client\n";
    std::cout << "0. Exit\n";
    std::cout << "Enter your choice: ";
    std::cin >> choice;

    switch (choice) {
        case 1:
            runServer();
            break;

        case 2:
            runClient();
            break;

        case 3:
            if (fork() == 0) {
                // Child process for server
                runServer();
                exit(0);
            }

            if (fork() == 0) {
                // Child process for client
                runClient();
                exit(0);
            }

            // Parent process waits for both children to finish
            wait(nullptr);
            wait(nullptr);
            break;

        case 0:
            // Exit the program
            break;

        default:
            std::cout << "Invalid choice\n";
            break;
    }

    return 0;
}

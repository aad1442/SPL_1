#include <iostream>
#include <cstdlib>
#include <future>
#include <thread>

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
            case 1:
                // Run the server asynchronously
                std::async(std::launch::async, runServer);
                break;

            case 2:
                // Run the client asynchronously
                std::async(std::launch::async, runClient);
                break;

            case 0:
                // Exit the program
                return 0;

            default:
                std::cout << "Invalid choice\n";
                break;
        }
    }

    return 0;
}

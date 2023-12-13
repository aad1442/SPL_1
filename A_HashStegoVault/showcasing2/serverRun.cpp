#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>

void runServer() {
    system("gcc server5.c -o server");
    system("./server");
}

void runClient() {
    system("gcc client5.c -o client");
    system("./client");
}
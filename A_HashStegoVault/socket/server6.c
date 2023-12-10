#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SIZE 1024

void write_file(int sockfd) {
    int n;
    FILE *fp;
    char filename[20] = "recv."; // Assuming a maximum extension length of 9 characters
    char extension[10]; // Assuming a maximum extension length of 9 characters

    // Receive file extension
    n = recv(sockfd, extension, sizeof(extension) - 1, 0);
    if (n <= 0) {
        perror("[-]Error in receiving file extension.");
        exit(1);
    }
    extension[n] = '\0'; // Null-terminate the received extension

    // Append received extension to the filename
    strcat(filename, extension);

    unsigned char buffer[SIZE];

    fp = fopen(filename, "wb"); // Open file in binary mode
    if (fp == NULL) {
        perror("[-]Error in opening file.");
        exit(1);
    }

    while (1) {
        n = recv(sockfd, buffer, SIZE, 0);
        if (n <= 0) {
            if (n < 0) {
                perror("[-]Error in receiving file.");
            }
            break;
        }
        size_t written = fwrite(buffer, 1, n, fp);
        if (written < n) {
            perror("[-]Error in writing to file.");
            break;
        }
        bzero(buffer, SIZE);
    }

    fclose(fp);
}

int main() {
    char *ip = "127.0.0.1";
    int port = 8080;
    int e;

    int sockfd, new_sock;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[SIZE];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[-]Error in socket");
        exit(1);
    }
    printf("[+]Server socket created successfully.\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = port;
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (e < 0) {
        perror("[-]Error in bind");
        exit(1);
    }
    printf("[+]Binding successful.\n");

    if (listen(sockfd, 10) == 0) {
        printf("[+]Listening....\n");
    } else {
        perror("[-]Error in listening");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_sock = accept(sockfd, (struct sockaddr *)&new_addr, &addr_size);
    if (new_sock < 0) {
        perror("[-]Error in accepting connection");
        exit(1);
    }

    write_file(new_sock);
    printf("[+]Data written to the file successfully.\n");

    close(new_sock);
    close(sockfd);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

char* addImageFileExtension(const char* imageFileName) {
    // Calculate the length of the new string
    size_t newLength = strlen(imageFileName) + 4;  // 4 for .bmp

    // Allocate memory for the new string
    char* newFileName = (char*)malloc(newLength + 1);  // +1 for the null terminator

    // Check if memory allocation was successful
    if (newFileName == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Copy the original string to the new string
    strcpy(newFileName, imageFileName);

    // Add the file extension
    strcat(newFileName, ".bmp");

    return newFileName;
}

void send_file(FILE *fp, int sockfd) {
  char data[SIZE];
  size_t bytesRead;

  while ((bytesRead = fread(data, 1, sizeof(data), fp)) > 0) {
    if (send(sockfd, data, bytesRead, 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}


int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char filename[256];

  printf("Enter the file name: ");
  fgets(filename, sizeof(filename), stdin);
  filename[strcspn(filename, "\n")] = 0;  // Remove newline character
  char* newFileName = addImageFileExtension(filename);
    strcpy(filename, newFileName);

  printf("\nfile name is %s\n",filename);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
	printf("[+]Connected to Server.\n");

  fp = fopen(filename, "rb");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");

	printf("[+]Closing the connection.\n");
  close(sockfd);
  

  return 0;
}

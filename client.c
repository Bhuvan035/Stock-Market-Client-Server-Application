#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent* server;

  char buffer[255];
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("Error opening the socket");
    exit(0);
  } else {
    server = gethostbyname(argv[1]);
    if (server == NULL) {
      printf("No such host");
    }
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr,
          server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
      printf("error in connection");
      exit(0);
    }
    while (1) {
      printf("> ");
      // printf("B at the start of the while loop %s",buffer);
      bzero(buffer, 255);
      fgets(buffer + 1, 254, stdin);
      int length = strlen(buffer + 1);
      buffer[0] = (char)length;
      // printf("Trying to write buffer[0] : %s", buffer[0]);
      n = write(sockfd, buffer, strlen(buffer));
      if (n < 0) {
        printf("Error writing");
        exit(0);
      }
      bzero(buffer, 255);
      n = read(sockfd, buffer, 255);
      if (n < 0) {
        printf("Error reading");
        exit(0);
      }
      if (strcmp(buffer, "quit") == 0) {
        exit(0);
      }
      int strLength = buffer[0];
      printf("%.*s\n", strLength, buffer + 1);
      // printf("B after reading from client %s",buffer);
    }
  }
  close(sockfd);
  return 0;
}
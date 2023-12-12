#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLI_NUM 4

int main() {

    int listen_fd, client_fd[MAX_CLI_NUM];
    int client_number = 0;
    char client_name[MAX_CLI_NUM][32];
    struct sockaddr_in server_address, client_address[MAX_CLI_NUM];
    socklen_t client_len[MAX_CLI_NUM];
    char sendline[BUFFER_SIZE], recvline[BUFFER_SIZE];

    // Create socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket
    if (bind(listen_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listen_fd, 10) == -1) {
        perror("Error listening for connections");
        close(listen_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);
    // Waiting for first user
    client_len[0] = sizeof(client_address[0]);
    client_fd[0] = accept(listen_fd, (struct sockaddr*)&client_address[0], &client_len[0]);


    bzero(&recvline, sizeof(recvline));
    read(client_fd[0], recvline, sizeof(recvline));
    sprintf(client_name[0], "%s", recvline);

    bzero(&sendline, sizeof(sendline));
    sprintf(sendline, "you are the room owner, select the number of the player(2~4).\n");
    write(client_fd[0], sendline, strlen(sendline));

    bzero(&recvline, sizeof(recvline));
    read(client_fd[0], recvline, sizeof(recvline));
    printf("%s", recvline);
    // Waiting for other users

    // Close the server socket
    close(listen_fd);

    exit(0);
}

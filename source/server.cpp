#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include "../include/game.hpp"
#include <sys/socket.h>
#include <iostream>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLI_NUM 4

void sig_chld(int sig){

    pid_t pid;
    int stat;
    
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        return;
}

void game(char client_name[][32], int client_fd[], int client_number) {
    // cerr << "Game server started..." << endl;
    Game game;
    for (int i = 0; i < client_number; ++i) {
        std::cerr << client_name << ' '<< client_fd[i] << std::endl;
        game.addPlayer(std::string(client_name[i]), i+1, client_fd[i]);
    }
    for (auto &player : game.players) {
        player.showStatus();
    }
    game.run();
    game.showStatus();
}

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

    for (;;){

        // Waiting for first user
        client_len[0] = sizeof(client_address[0]);
        client_fd[0] = accept(listen_fd, (struct sockaddr*)&client_address[0], &client_len[0]);

        bzero(&recvline, sizeof(recvline));
        read(client_fd[0], recvline, sizeof(recvline));
        sprintf(client_name[0], "%s", recvline);

        bzero(&sendline, sizeof(sendline));
        // sprintf(sendline, "你是房主, 請輸入 2~4 的數字來決定有幾位玩家。\n");
        sprintf(sendline, "you are the room owner, choose how many player to enter the game(2~4).\n");
        write(client_fd[0], sendline, strlen(sendline));

        bzero(&recvline, sizeof(recvline));
        while(read(client_fd[0], recvline, sizeof(recvline)) > 0){

            client_number = recvline[0] - 48;
            if(client_number >= 2 && client_number <= MAX_CLI_NUM){

                break;
            }
            else{

                bzero(&sendline, sizeof(sendline));
                // sprintf(sendline, "請輸入正確的數字(2~4).\n");
                sprintf(sendline, "please choose correct number(2~4)%d.\n",client_number);
                write(client_fd[0], sendline, strlen(sendline));
            }
        }

        // Waiting for other users
        for (int i = 1; i < client_number; i++){
            client_len[i] = sizeof(client_address[i]);
            client_fd[i] = accept(listen_fd, (struct sockaddr *)&client_address[i], &client_len[i]);

            bzero(&recvline, sizeof(recvline));
            read(client_fd[i], recvline, sizeof(recvline));
            sprintf(client_name[i], "%s", recvline);

            bzero(&sendline, sizeof(sendline));
            sprintf(sendline, "你是第 #%d 位玩家.\n", i + 1);
            write(client_fd[i], sendline, strlen(sendline));
        }
        
        signal(SIGCHLD, sig_chld);
        
        // fork a child process to start the game
        pid_t pid = fork();
        if(pid == 0){

            bzero(&sendline, sizeof(sendline));
            // sprintf(sendline, "總共有 %d 位玩家.\n", client_number);
            sprintf(sendline, "there's %d players in total.\n", client_number);
            for (int i = 0; i < client_number; i++){
                sprintf(sendline, "%s %d. %s\n", sendline, i, client_name[i]);
            }
            for (int i = 0; i < client_number; i++){
                write(client_fd[i], sendline, strlen(sendline));
            }

            close(listen_fd);

            // start the game
            game(client_name, client_fd, client_number);

            exit(0);
        }
        else{

            pause();
        }
    }

    exit(0);
}

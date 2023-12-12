#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

char name[BUFFER_SIZE];

void go_into_the_game(FILE *fp, int sock_fd){

    int       maxfdp1, stdineof, peer_exit, n;
    fd_set    rset;
    char recvline[BUFFER_SIZE], sendline[BUFFER_SIZE];

    bzero(&sendline, sizeof(sendline));
    sprintf(sendline, "%s", name);
    write(sock_fd, sendline, strlen(sendline));

    bzero(&recvline, sizeof(recvline));
    read(sock_fd, recvline, sizeof(recvline));
    printf("%s", recvline);
	
    stdineof = 0;
	peer_exit = 0;

    for ( ; ; ) {	
		FD_ZERO(&rset);
		maxfdp1 = 0;
        if (stdineof == 0) {
            FD_SET(fileno(fp), &rset);
			maxfdp1 = fileno(fp);
		};	
		if (peer_exit == 0) {
			FD_SET(sock_fd, &rset);
			if (sock_fd > maxfdp1)
				maxfdp1 = sock_fd;
		};	
        maxfdp1++;
        select(maxfdp1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(sock_fd, &rset)) {  /* socket is readable */
			n = read(sock_fd, recvline, BUFFER_SIZE);
			if (n == 0) {
 		   		if (stdineof == 1)
                    return;         /* normal termination */
		   		else {
					printf("(End of input from the peer!)");
					peer_exit = 1;
					return;
				};
            }
			else if (n > 0) {
				recvline[n] = '\0';
				printf(recvline);
			}
			else { // n < 0
			    printf("(server down)");
				return;
			};
        }
		
        if (FD_ISSET(fileno(fp), &rset)) {  /* input is readable */

            if (fgets(sendline, BUFFER_SIZE, fp) == NULL) {
				if (peer_exit)
					return;
				else {
					printf("(leaving...)\n");
					stdineof = 1;
					shutdown(sock_fd, SHUT_WR);      /* send FIN */
				};
            }
			else {
				
				write(sock_fd, sendline, strlen(sendline));
			};
        }
    }
}

int main(int argc, char **argv) {

    if(argc != 3){

        perror("where is your ip address?");
        exit(0);
    }

    int sock_fd;
    struct sockaddr_in server_address;

    // Create socket
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, argv[1], &server_address.sin_addr.s_addr);
    strcpy(name, argv[2]);

    // Connect to the server
    if (connect(sock_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to server on %s:%d\n", argv[1], PORT);

    go_into_the_game(stdin, sock_fd);

    // Close the client socket
    close(sock_fd);

    return 0;
}

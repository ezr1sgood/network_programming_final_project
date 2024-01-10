#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <locale.h>
#include <unistd.h>
#include <vector>
#include <deque>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <iomanip>

#define ADDR "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

#define window_first_row 1
#define window_last_row 20
#define window_first_col 1
#define window_last_col 75

#define status_first_row 1
#define status_last_row 31
#define status_first_col 81
#define status_last_col 120

#define recv_first_row 21
#define recv_last_row 30
#define recv_first_col 1
#define recv_last_col 80

#define player_first_row 30
#define player_last_row 30
#define player_first_col 1
#define player_last_col 80

char name[BUFFER_SIZE];
int player_num = 0;

void clear_screen(){

    printf("\x1B[2J\x1B[H");
}

void set_cursor_position(int x, int y) {

    printf("\x1B[%d;%dH", x, y);
}

void clear_recv_window(){

    for(int i = recv_first_row; i <= recv_last_row; i++){

        for(int j = recv_first_col; j <= recv_last_col; j++){

            set_cursor_position(i, j);
            printf(" ");
        }
    }
    set_cursor_position(recv_first_row, recv_first_col);
}

void draw_grid(){

    int up = 7, down = 13, left = 3, right = 23;

    for(int k = 0; k < 3; k++){

        if(k){

            left += 24;
            right += 24;
        }
        for(int i = up; i <= down; i++){

            for(int j = left; j <= right; j++){

                if(i == up || i == down || j == left || j == right){

                    set_cursor_position(i, j);
                    printf("@");
                }
            }
        }
        set_cursor_position(10, right + 1);
        printf("<--");
    }

    set_cursor_position(6, 33);
    printf("目前位置");
}

void draw_game_window(){

    for(int i = window_first_row; i <= window_last_row; i++){

        for(int j = window_first_col; j <= window_last_col; j++){

            set_cursor_position(i, j);
            if(i == window_first_row || i == window_last_row){

                printf("=");
            }
            else if(j == window_first_col || j == window_last_col){

                printf("|");
            }
            else{

                printf(" ");
            }
        }
    }

    draw_grid();
}

int print_buffer(std::vector<char> buffer, std::deque<std::vector<char> > message_queue, int status, int cnt_recv, int cnt_status, int cnt_map){

    if(std::equal(buffer.begin(), buffer.end(), "status of player")){

        return 1;
    }
    else if(std::equal(buffer.begin(), buffer.end(), "end of status")){

        return 3;
    }
    else if(std::equal(buffer.begin(), buffer.end(), "map of player")){

        return 2;
    }
    else if(std::equal(buffer.begin(), buffer.end(), "end of map")){

        return 3;
    }
    else if(std::equal(buffer.begin(), buffer.begin() + 7, "there's")){
        
        player_num = buffer[8] - 48;
    }
    else if(std::equal(buffer.begin(), buffer.begin() + 5, "Round")){

        clear_screen();
        draw_game_window();
    }

    if(status == 0){

        clear_recv_window();
        set_cursor_position(recv_first_row, recv_first_col);
        while(message_queue.size() >= recv_last_row - recv_first_row - 2) message_queue.pop_front();
        for(auto &message_it: message_queue){

            std::vector<char> tmp = message_it;
            for(auto &buffer_it: tmp){

                printf("%c", buffer_it);
            }
            printf("\n");
        }
    }
    else if(status == 1){

        set_cursor_position(status_first_row + cnt_status, status_first_col);
    }
    else if(status == 2){

        int tmp = 19 - buffer.size();
        set_cursor_position(8, 4 + 24 * (cnt_map - 1) + (abs(tmp) / 2));
    }
    else if(status == 3){

        // clear_recv_window();
        set_cursor_position(recv_first_row, recv_first_col);
    }

    for(auto &it: buffer){

        printf("%c", it);
    }
    printf("\n");

    if(status == 3){

        return 0;
    }

    return status;
}

void game(FILE *fp, int sock_fd){

    int       maxfdp1, stdineof, peer_exit, n, status = 0, cnt_recv = 0, cnt_status = 0, cnt_map = 0;
    fd_set    rset;
    char recvline[BUFFER_SIZE], sendline[BUFFER_SIZE];

    std::deque<std::vector<char> > message_queue;
    std::vector<char> buffer;

    clear_screen();
    draw_game_window();
    // draw_status_window();
    // draw_recv_window();

    bzero(&sendline, sizeof(sendline));
    sprintf(sendline, "%s", name);
    write(sock_fd, sendline, strlen(sendline));

    stdineof = 0;
    peer_exit = 0;

    bzero(&sendline, sizeof(sendline));
    while(true){

        FD_ZERO(&rset);
        maxfdp1 = 0;

        if(peer_exit == 0){

            FD_SET(sock_fd, &rset);
            maxfdp1 = sock_fd + 1;
        }
        if(stdineof == 0){

            FD_SET(fileno(fp), &rset);
            if(fileno(fp) + 1 > maxfdp1){

                maxfdp1 = fileno(fp) + 1;
            }
        }

        select(maxfdp1, &rset, NULL, NULL, NULL);

        if(FD_ISSET(sock_fd, &rset)){
            //recieve something from server
            n = read(sock_fd, recvline, BUFFER_SIZE);
			if (n == 0) {
 		   		if (stdineof == 1)
                    return;         /* normal termination */
		   		else {
					
					peer_exit = 1;
					return;
				};
            }
			else if (n > 0) {

				recvline[n] = '\0';
                //split receive with \n
                for(int i = 0; i < BUFFER_SIZE; i++){

                    if(recvline[i] == '\n'){

                        status = print_buffer(buffer, message_queue, status, cnt_recv, cnt_status, cnt_map); // 0 -> recvline, 1 -> status, 2 -> game window
                    
                        while(message_queue.size() >= recv_last_row - recv_first_row - 2){

                            message_queue.pop_front();
                        }
                        if(status == 0){

                            message_queue.push_back(buffer);
                        }

                        buffer.clear();

                        if(status == 0 || status == 3){

                            cnt_recv++;
                            cnt_status = 0;
                            cnt_map = 0;
                        }
                        else if(status == 1){

                            cnt_recv = 0;
                            cnt_status++;
                            cnt_map = 0;
                        }
                        else if(status == 2){

                            cnt_recv = 0;
                            cnt_status = 0; 
                            cnt_map++;
                        }
                    }
                    else if(recvline[i] == '\0'){
                        bzero(&recvline, sizeof(recvline));
                        break;
                    }
                    else{

                        buffer.push_back(recvline[i]);
                    }
                }

                set_cursor_position(recv_first_row, recv_first_col);
			}
			else { // n < 0
			    
				return;
			};
        }
        if(FD_ISSET(fileno(fp), &rset)){
            
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
                clear_recv_window();
                cnt_recv = 0;
                cnt_status = 0;
                cnt_map = 0;
			}
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

    set_cursor_position(recv_first_row, recv_first_col);
    printf("Connected to server on %s:%d\n", argv[1], PORT);

    game(stdin, sock_fd);

    close(sock_fd);

    return 0;
}
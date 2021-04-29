#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define PORT 8080
int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int num = 5;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *message1 = "Penambahan berhasil";
    char *message2 = "Pengurangan berhasil";
    char *message3 = "Command tidak sesuai";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    while (1) {
        valread = read( new_socket , buffer, 1024);

        if (strcmp(buffer, "tambah") == 0) {
            num++;
            send(new_socket , message1 , strlen(message1) , 0 );
        } else if (strcmp(buffer, "kurang") == 0) {
             num--;
            send(new_socket , message2 , strlen(message2) , 0 );
        } else if (strcmp(buffer, "cek") == 0) {
            char snum[10];
            sprintf(snum, "%d", num);
            send(new_socket , snum , strlen(snum) , 0 );
        } 
        else {
            printf("%s\n", buffer);
            send(new_socket , message3 , strlen(message3) , 0 );
        }
        
        memset(buffer, 0, sizeof buffer);
    }
    return 0;
}
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>

#define MYPORT 8887
#define BUFFER_SIZE 1024

int main() {
    int client_sock = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(MYPORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(client_sock,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0) {
        perror("connect");
        exit(1);
    }

    char sendBuffer[BUFFER_SIZE];
    char recvBuffer[BUFFER_SIZE];
    while(fgets(sendBuffer,sizeof(sendBuffer),stdin) != NULL) {
        send(client_sock,sendBuffer,strlen(sendBuffer),0);
        if(strcmp(sendBuffer,"exit\n") == 0)
            break;
        recv(client_sock,recvBuffer,sizeof(recvBuffer),0);
        fputs(recvBuffer,stdout);

        memset(sendBuffer,0,sizeof(sendBuffer));
        memset(recvBuffer,0,sizeof(recvBuffer));
    }

    close(client_sock);
    return 0;
}
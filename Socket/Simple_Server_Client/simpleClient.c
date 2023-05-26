#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    int sockfd;
    int len;

    struct sockaddr_un address;

    int result;
    char ch = 'A';

    //creiamo una socket per il client
    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

    //definiamo le generalità della socket
    address.sun_family = AF_UNIX;

    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

    //connettiamo la nostra socket con quella del server
    result = connect(sockfd, (struct sockaddr*)&address, len);

    if(result == -1){
        perror("ops:client 1");
        exit(1);
    }

    //possiamo leggere e scrivere via sockfd
    write(sockfd, &ch, 1);
    read(sockfd, &ch, 1);

    printf("char dal server = %c\n", ch);

    close(sockfd);

}
#ifndef MYSOCKET
#define MYSOCKET

#include<sys/socket.h>
#include<arpa/inet.h> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>
#include "consts.h"

int make_socket(const char ip[],int port){
    int sock;
    struct sockaddr_in address;
    sock = socket(AF_INET,SOCK_STREAM,0);
    if(sock<0){
        return 0;
    }
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    
    int result;
    result = inet_pton(AF_INET,ip,&address.sin_addr);
    if(result<=0){
        printf("\nInvalid Ip address\n");
        return 0;
    }

    result = connect(sock,(struct sockaddr*)&address,sizeof(address));
    if(result<0){
        printf("\nConnection Failed\n");
        return 0;
    }

    return sock;
}


void send_request(const char* request,size_t request_size,char** respons){
    int server_socket = make_socket(IP,PORT);
    if(server_socket == 0){
        printf("\nError while making socket\n");
        strcpy(*respons,"ERROR");
        server_status = 0;
        exit(1);
        return;
    }

    send(server_socket,request,request_size,0);
    char buffer[MAX];
    *respons = calloc(1,1);
    int total_bytes = 0;
    while(1){
        int read_bytes = read(server_socket,buffer,MAX);
        if(read_bytes<=0){
            break;
        }
        total_bytes += read_bytes;
        *respons = realloc(*respons,total_bytes+1);
        strncat(*respons,buffer,read_bytes);
    }
    close(server_socket);
}
#endif
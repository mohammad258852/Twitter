#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<signal.h>
#include"mysocket.h"
#include"process.h"
#include"consts.h"
#include"utility.h"

int server_sock;
int client;

void clearing(int sig){
    shutdown(client,SHUT_RDWR);
    close(client);
    shutdown(server_sock,SHUT_RDWR);
    close(server_sock);
    exit(sig);
}


int main(){
    signal(SIGINT,clearing);
    signal(SIGSEGV,clearing);
    struct sockaddr_in address;
    int address_len = sizeof(address);
    server_sock = socket(AF_INET,SOCK_STREAM,0);
    if(server_sock<0){
        fail("Failed to make socket");
    }
    int true = 1;
    setsockopt(server_sock,SOL_SOCKET,SO_REUSEADDR,&true,sizeof(int));
    logout("Socket made");
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);
    address.sin_addr.s_addr = INADDR_ANY;

    int res = bind(server_sock , (struct sockaddr*)&address,address_len);
    if(res < 0){
        close(server_sock);
        fail("Failed to bind");
    }
    logoutf("Socket binded to port %d",PORT);
    res = listen(server_sock,MAXQUEU);
    if(res!=0)
        fail("Failed to listen");
    logout("Listening...");
    while(1){
        client = accept(server_sock,(struct sockaddr*)&address,&address_len);
        process(client);
        close(client);
    }
    clearing(0);
    return 0;
}
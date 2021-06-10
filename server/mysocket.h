#ifndef MYSOCKET
#define MYSOCKET

#include<sys/socket.h>
#include<arpa/inet.h> 
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <string.h>
#include <stdarg.h>
#include "consts.h"
#include"utility.h"
#include"cJSON.h"

void send_message(int sock,const char* const message,size_t message_size);
void send_messagef(int sock,const char* format,...);
void send_response(int sock,const char* const type,const char* const message);
void send_response_json(int sock,const char* const type,cJSON* message);
void send_responsef(int sock,const char* const type,const char* format,...);
void get_message(int sock,char** result);

void send_message(int sock,const char* const message,size_t message_size){
    send(sock,message,message_size,0);
}

void send_messagef(int sock,const char* format,...){
    va_list ap;
    va_start(ap,format);
    size_t message_size = vsnprintf(NULL,0,format,ap);
    char* message = (char*)malloc(message_size + 1);
    va_end(ap);
    va_start(ap,format);
    vsprintf(message,format,ap);
    send_message(sock,message,message_size);
    free(message);
    va_end(ap);
}

void send_response(int sock,const char* const type,const char* const message){
    size_t len = snprintf(NULL,0,"{\"type\":\"%s\",\"message\":\"%s\"}",type,message);
    char* tmp = malloc(len+1);
    sprintf(tmp,"{\"type\":\"%s\",\"message\":\"%s\"}",type,message);
    logoutf("sending response:%s",tmp);
    send_message(sock,tmp,len);
    free(tmp);
}

void send_response_json(int sock,const char* const type,cJSON* json){
    char* message = cJSON_PrintUnformatted(json);
    size_t len = snprintf(NULL,0,"{\"type\":\"%s\",\"message\":%s}",type,message);
    char* tmp = malloc(len+1);
    sprintf(tmp,"{\"type\":\"%s\",\"message\":%s}",type,message);
    logoutf("sending response:%s",tmp);
    send_message(sock,tmp,len);
    free(tmp);
}

void send_responsef(int sock,const char* const type,const char* format,...){
    va_list ap;
    va_start(ap,format);
    size_t message_size = vsnprintf(NULL,0,format,ap);
    char* message = malloc(message_size + 1);
    va_end(ap);
    va_start(ap,format);
    vsprintf(message,format,ap);
    logoutf("sending response:%s",message);
    send_response(sock,type,message);
    free(message);
    va_end(ap);
}

void get_message(int sock,char** result){
    char buffer[MAXBUF];
    *result = (char*)calloc(1,1);
    int total_bytes = 0;
    while(1){
        int read_bytes = recv(sock,buffer,MAXBUF,0);
        if(read_bytes<=0){
            break;
        }
        int newline = read_bytes;
        for(int i=0;i<read_bytes;i++){
            if(buffer[i]=='\n'){
                newline = i;
                break;
            }
        }
        total_bytes += newline;
        *result = (char*)realloc(*result,total_bytes+1);
        strncat(*result,buffer,newline);
        if(newline != read_bytes)
            break;
    }
}

#endif
#ifndef UTILITYH
#define UTILITYH

#include<stdio.h>
#include<stdlib.h>
#include<stdarg.h>
#include<string.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"


void logout(char* message){
    printf("Log:\t%s\n",message);
}

void logoutf(const char* format,...){
    va_list ap;
    va_start(ap,format);
    printf("Log:\t");
    vprintf(format,ap);
    printf("\n");
    va_end(ap);
}

void errorlogout(char* message){
    fprintf(stderr,ANSI_COLOR_RED"Error:\t%s\n"ANSI_COLOR_RESET,message);
}

void errorlogoutf(const char* format,...){
    va_list ap;
    va_start(ap,format);
    fprintf(stderr,ANSI_COLOR_RED"Erorr:\t");
    vfprintf(stderr,format,ap);
    fprintf(stderr,"\n"ANSI_COLOR_RESET);
    va_end(ap);
}

void fail(char* message){
    fprintf(stderr,ANSI_COLOR_RED"Fail:\t%s\n"ANSI_COLOR_RESET,message);
    exit(-1);
}

void failf(const char* format,...){
    va_list ap;
    va_start(ap,format);
    fprintf(stderr,ANSI_COLOR_RED"Fail:\t");
    vfprintf(stderr,format,ap);
    fprintf(stderr,"\n"ANSI_COLOR_RESET);
    va_end(ap);
    exit(-1);
}

int string_start_with(const char* str,const char* start){
    if(strncmp(str,start,strlen(start)) == 0)
        return 1;
    return 0;
}

int string_end_with(const char* str,const char* end){
    int i = strlen(str)-1;
    int j = strlen(end)-1;
    while(i>=0 && j>=0){
        if(str[i]!=end[j]){
            return 0;
        }
        j--;
        i--;
    }
    if(j<0)
        return 1;
    return 0;
}


#endif
#ifndef PROCESS
#define PROCESS

#include<string.h>
#include<string.h>
#include<ctype.h>
#include"cJSON.h"
#include"mysocket.h"
#include"consts.h"
#include"utility.h"
#include"user.h"

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

void process(int);
int string_start_with(const char* str,const char* start);
void signup(int,const char*);
void login(int ,const char*);
char* make_response(const char* const type,const char* const message,char** result);

void process(int sock){
    char* request;
    get_message(sock,&request);
    logoutf("message recieved: %s",request);
    const char* commands[] = {"signup","login"};
    void (*fun[])(int,const char*) = {signup,login};
    int commands_len = ARRAYSIZE(commands);
    int command_found = 0;
    for(int i=0;i<commands_len;i++){
        if(string_start_with(request,commands[i])){
            fun[i](sock,request);
            command_found = 1;
            break;
        }
    }
    if(!command_found){
        send_response(sock,"Error","Bad request format");
        errorlogout("Command not found");
    }
    free(request);
}

int isvalid_username_char(char t){
    if(t=='\0' || t==',' || t=='\n')
        return 0;
    return isalnum(t);
}

int isvalid_password_char(char t){
    if(t=='\0' || t=='\n')
        return 0;
    return (isalnum(t) || t=='@' || t=='$' || t=='&');
}

void signup(int sock,const char* command){
    char username[MAXUSERNAME];
    char password[MAXPASSWORD];
    sscanf(command,"signup %[^,],%[^\n]",username,password);

    int successful = create_user(username,password);
    if(successful){
        send_response(sock,"Successful","Sign up Successfuly");
        logoutf("user %s signup successfuly",username);
    }
    else{
        send_response(sock,"Error","This username already exist");
        logoutf("user %s exist",username);
    }
}
void login(int sock,const char* command){
    return;
}

#endif
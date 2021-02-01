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
#include"token.h"

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

void process(int);
int string_start_with(const char* str,const char* start);
void signup(int,const char*);
void login(int ,const char*);
void refresh(int ,const char*);
void send_tweet(int,const char*);
void loggout(int,const char*);
void like(int,const char*);

void process(int sock){
    char* request;
    get_message(sock,&request);
    logoutf("message recieved: %s",request);
    const char* commands[] = {"signup","login","send tweet","refresh","logout","like"};
    void (*fun[])(int,const char*) = {signup,login,send_tweet,refresh,loggout,like};
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
    char username[MAXUSERNAME];
    char password[MAXPASSWORD];
    sscanf(command,"login %[^,],%[^\n]",username,password);

    if(!check_username_exist(username)){
        send_response(sock,"Error","username doesn't exist");
        logoutf("usename %s doesn't exist",username);
        return;
    }
    if(check_username_login(username)){
        send_responsef(sock,"Error","user %s is already loged in",username);
        logoutf("user %s is already loged in",username);
        return;
    }
    User user = read_user(username);
    if(strcmp(password,user.password)!=0){
        send_responsef(sock,"Error","Wrong Password",username);
        logoutf("user %s entered wrong password",username);
        return;
    }

    Token* tok = add_user_token(username);
    send_response(sock,"Token",tok->tok);
    logoutf("user %s succussfuly loged in",username);
    return;
}

void send_tweet(int sock,const char* commad){
    char tok[TOKENSIZE+1];
    char content[MAXTEX];
    sscanf(commad,"send tweet %[^,],%[^\n]",tok,content);

    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }

    int id = add_tweet(token->username,content);
    if(id==0){
        send_response(sock,"Error","Failed to make tweet");
        logout("Failed to make tweet");
        return;
    }

    add_tweet_to_user(token->username,id);
}

void refresh(int sock,const char* command){
    char tok[TOKENSIZE+1];
    sscanf(command,"refresh %[^\n]",tok);
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }

    cJSON* tweets = unread_tweets(token->username);
    send_response_json(sock,"List",tweets);
    cJSON_Delete(tweets);
}

void loggout(int sock,const char* command){
    char tok[TOKENSIZE+1];
    sscanf(command,"logout %[^\n]",tok);
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    if(delete_token(token)){
        send_response(sock,"Succussful","Now logged out");
        logoutf("user %s loged out",token->username);
    }
    else{
        send_response(sock,"Error","Something wrong happened");
        logout("Can't delete token");
    }
}

void like(int sock,const char* command){
    char tok[TOKENSIZE+1];
    int id;
    sscanf(command,"like %[^,],%d",tok,&id);
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid Token");
        return;
    }
    if(!tweet_exist(id)){
        send_response(sock,"Error","Invalid tweet id");
        logout("Invalid tweet id");
        return;
    }
    if(!is_user_like_tweet(token->username,id)){
        user_like_tweet(token->username,id);
        send_response(sock,"Successful","Like");
        logoutf("user %s liked tweet %d",token->username,id);
    }
    else{
        user_unlike_tweet(token->username,id);
        send_response(sock,"Successful","Unlike");
        logoutf("user %s unliked tweet %d",token->username,id);
    }
}

#endif
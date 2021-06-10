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
#include"hashtag.h"

#define ARRAYSIZE(a) (sizeof(a)/sizeof(a[0]))

void process(int);
void signup(int,const char*);
void login(int ,const char*);
void refresh(int ,const char*);
void send_tweet(int,const char*);
void loggout(int,const char*);
void like(int,const char*);
void comment(int,const char*);
void search(int,const char*);
void follow(int,const char*);
void unfollow(int,const char*);
void set_bio(int,const char*);
void profile(int,const char*);
void change_password(int,const char*);
void delete_request(int,const char*);
void retweet(int,const char*);
void search_tweet(int,const char*);

void process(int sock){
    char* request;
    get_message(sock,&request);
    logoutf("message recieved: %s",request);
    const char* commands[] = {"signup","login","send tweet",
                                "refresh","logout","like",
                                "comment","search","follow",
                                "unfollow","set bio","profile",
                                "change password","delete","retweet",
                                "tweet search"};
    void (*fun[])(int,const char*) = {signup,login,send_tweet,
                                    refresh,loggout,like,
                                    comment,search,follow,
                                    unfollow,set_bio,profile,
                                    change_password,delete_request,retweet,
                                    search_tweet};
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

int isvalid_bio_char(char t){
    if(t=='\0' || t=='\n')
        return 0;
    return 1;
}

int isvalid_text_char(char t){
    if(t=='\0' || t=='\n')
        return 0;
    return 1;
}

int isvalid_hashtag_char(char t){
    if(t>='a' && t<='z')
        return 1;
    if(t>='A' && t<='Z')
        return 1;
    if(t>='0' && t<='9')
        return 1;
    if(t=='_')
        return 1;
    return 0;
}

int isvalid_username(char* us){
    int i;
    for(i=0;us[i]!='\0';i++){
        if(i>=MAXUSERNAME){
            return 0;
        }
        if(!isvalid_username_char(us[i]))
            return 0;
    }
    if(i==0 || i>MAXUSERNAME)
        return 0;
    return 1;
}

int isvalid_password(char* pw){
    int i;
    for(i=0;pw[i]!='\0';i++){
        if(i>=MAXPASSWORD){
            return 0;
        }
        if(!isvalid_password_char(pw[i]))
            return 0;
    }
    if(i==0 || i>MAXPASSWORD)
        return 0;
    return 1;
}

int isvalid_bio(char* bio){
    int i;
    for(i=0;bio[i]!='\0';i++){
        if(i>=MAXBIO)
            return 0;
        if(!isvalid_bio_char(bio[i]))
            return 0;
    }
    if(i==0 || i>MAXBIO)
        return 0;
    return 1;
}

int isvalid_text(char* text){
    int i;
    for(i=0;text[i]!='\0';i++){
        if(i>=MAXTEX)
            return 0;
        if(!isvalid_text_char(text[i]))
            return 0;
    }
    if(i==0 || i>MAXTEX)
        return 0;
    return 1;
}

int isvalid_hashtag(char* ht){
    int i;
    for(i=0;ht[i]!='\0';i++){
        if(i>=MAXHASHTAG)
            return 0;
        if(!isvalid_hashtag_char(ht[i]))
            return 0;
    }
    if(i==0 || i>MAXHASHTAG)
        return 0;
    return 1;
}

void signup(int sock,const char* command){
    char username[MAXUSERNAME+1];
    char password[MAXPASSWORD+1];
    if(sscanf(command,"signup %[^,],%[^\n]",username,password)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_username(username)){
        send_response(sock,"Error","Bad username format");
        logout("Bad username format");
        return;
    }
    if(!isvalid_password(password)){
        send_response(sock,"Error","Bad password format");
        logout("Bad password format");
        return;
    }

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
    char username[MAXUSERNAME+1];
    char password[MAXPASSWORD+1];
    if(sscanf(command,"login %[^,],%[^\n]",username,password)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_username(username)){
        send_response(sock,"Error","Bad username format");
        logout("Bad username format");
        return;
    }
    if(!isvalid_password(password)){
        send_response(sock,"Error","Bad password format");
        logout("Bad password format");
        return;
    }
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
    free_user(&user);
    Token* tok = add_user_token(username);
    send_response(sock,"Token",tok->tok);
    logoutf("user %s succussfuly loged in",username);
    return;
}

void send_tweet(int sock,const char* commad){
    char tok[TOKENSIZE+1];
    char content[MAXTEX+1];
    if(sscanf(commad,"send tweet %[^,],%[^\n]",tok,content)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_text(content)){
        send_response(sock,"Error","Bad text format");
        logout("Bad text format");
        return;
    }
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
    if(sscanf(command,"refresh %[^\n]",tok)!=1){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
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
    if(sscanf(command,"logout %[^\n]",tok)!=1){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
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
    if(sscanf(command,"like %[^,],%d",tok,&id)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
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

void comment(int sock,const char* command){
    char tok[TOKENSIZE+1];
    int id;
    char comm[MAXTEX+1];
    if(sscanf(command,"comment %[^,],%d,%[^\n]",tok,&id,comm)!=3){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_text(comm)){
        send_response(sock,"Error","Bad text format");
        logout("Bad text format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid Token");
        logout("Invalid Token");
        return;
    }
    if(!tweet_exist(id)){
        send_response(sock,"Error","Invalid tweet id");
        logout("Invalid tweet id");
        return;
    }
    add_comment_to_tweet(id,token->username,comm);
    send_response(sock,"Successful","Comment Submited");
    logoutf("user %s send comment:%s",token->username,comm);
}

void search(int sock,const char* command){
    char tok[TOKENSIZE+1];
    char username[MAXUSERNAME+1];
    if(sscanf(command,"search %[^,],%[^\n]",tok,username)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_username(username)){
        send_response(sock,"Error","Bad username format");
        logout("Bad username format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    cJSON* json = advance_search(username,token->username);
    if(cJSON_GetArraySize(json)<=0){
        send_response(sock,"Error","No user found");
        logoutf("no user match %s",username);
        return;
    }
    send_response_json(sock,"List",json);
    logoutf("send users data to %s",token->username);
    cJSON_Delete(json);
}

void follow(int sock,const char* command){
    char tok[TOKENSIZE+1];
    char username[MAXUSERNAME+1];
    if(sscanf(command,"follow %[^,],%s",tok,username)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_username(username)){
        send_response(sock,"Error","Bad username format");
        logout("Bad username format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    if(!check_username_exist(username)){
        send_response(sock,"Error","No user with this username");
        logoutf("Can't find %s",username);
        return;
    }
    if(is_user_follows_that(token->username,username)){
        send_response(sock,"Error","You already followed this user");
        logoutf("user %s already follows %s",token->username,username);
        return;
    }
    user_follow_that(token->username,username);
    send_responsef(sock,"Successful","now you follow %s",username);
    logoutf("user %s follows %s",token->username,username);
}
void unfollow(int sock,const char* command){
    char tok[TOKENSIZE+1];
    char username[MAXUSERNAME+1];
    if(sscanf(command,"unfollow %[^,],%s",tok,username)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_username(username)){
        send_response(sock,"Error","Bad username format");
        logout("Bad username format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    if(!check_username_exist(username)){
        send_response(sock,"Error","No user with this username");
        logoutf("Can't find %s",username);
        return;
    }
    if(!is_user_follows_that(token->username,username)){
        send_response(sock,"Error","You do not follow this user");
        logoutf("user %s does not follows %s",token->username,username);
        return;
    }
    user_unfollow_that(token->username,username);
    send_responsef(sock,"Successful","now you do not follow %s anymore",username);
    logoutf("user %s unfollows %s",token->username,username);
}

void set_bio(int sock,const char* command){
    char tok[TOKENSIZE+1];
    char bio[MAXBIO+1];
    if(sscanf(command,"set bio %[^,],%[^\n]",tok,bio)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_bio(bio)){
        send_response(sock,"Error","Bad bio format");
        logout("Bad bio format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    User user = read_user(token->username);
    strncpy(user.bio,bio,MAXBIO);
    write_user(&user);
    free_user(&user);
    send_response(sock,"Successful","bio changed successfully");
    logoutf("user %s changed his/her bio to %s",token->username,bio);
}

void profile(int sock,const char* command){
    char tok[TOKENSIZE+1];
    if(sscanf(command,"profile %[^\n]",tok)!=1){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    cJSON* json = make_user_for_client(token->username,token->username);
    send_response_json(sock,"Profile",json);
    logoutf("send %s profile",token->username);
    cJSON_Delete(json);
}

void change_password(int sock,const char* command){
    char tok[TOKENSIZE+1];
    char new_password[MAXPASSWORD+1];
    char cur_password[MAXPASSWORD+1];
    if(sscanf(command,"change password %[^,],%[^,],%[^\n]",tok,cur_password,new_password)!=3){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_password(cur_password) || !isvalid_password(new_password)){
        send_response(sock,"Error","Bad password format");
        logout("Bad password format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    User user = read_user(token->username);
    if(strcmp(user.password,cur_password)!=0){
        send_response(sock,"Error","Wrong password");
        logout("Wrong password");
        return;
    }
    strncpy(user.password,new_password,MAXPASSWORD);
    write_user(&user);
    free_user(&user);
    send_response(sock,"Successful","Password upadted");
    logout("Password updated");
}

void delete_request(int sock,const char* command){
    char tok[TOKENSIZE+1];
    int id;
    if(sscanf(command,"delete %[^,],%d",tok,&id)!=2){
        send_response(sock,"Error","Bad password format");
        logout("Bad password format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    if(!is_user_own_tweet(token->username,id) && !is_user_retweet_tweet(token->username,id)){
        send_response(sock,"Error","This tweet is not yours");
        logoutf("user %s does not own tweet %d",token->username,id);
        return;
    }
    if(is_user_own_tweet(token->username,id)){
        delete_tweet(token->username,id);
        send_response(sock,"Successful","Tweet deleted");
        logoutf("Tweet %d from %s deleted",id,token->username);
        return;
    }
    if(is_user_retweet_tweet(token->username,id)){
        delete_retweet_from_user(token->username,id);
        send_response(sock,"Successful","Retweet deleted");
        logoutf("Retweet %d from %s deleted",id,token->username);
        return;
    }
}

void retweet(int sock,const char* command){
    char tok[TOKENSIZE+1];
    int id;
    if(sscanf(command,"retweet %[^,],%d",tok,&id)!=2){
        send_response(sock,"Error","Bad password format");
        logout("Bad password format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    if(!tweet_exist(id)){
        send_response(sock,"Error","Tweet does not exist");
        logoutf("Tweet %d does not exist",id);
        return;
    }
    if(is_user_own_tweet(token->username,id)){
        send_response(sock,"Error","You can not retweet your own tweet");
        logoutf("user %s owns tweet %d",token->username,id);
        return;
    }
    if(is_user_retweet_tweet(token->username,id)){
        send_response(sock,"Error","You already retweet this tweet");
        logoutf("user %s already retweets tweet %d",token->username,id);
        return;
    }
    add_retweet_to_user(token->username,id);
    send_response(sock,"Successful","Tweet retweeted");
    logoutf("user %s retweets tweet %d",token->username,id);
}

void search_tweet(int sock,const char* command){
    char hashtag[MAXHASHTAG+1];
    char tok[TOKENSIZE+1];
    if(sscanf(command,"tweet search %[^,],%[^\n]",tok,hashtag)!=2){
        send_response(sock,"Error","Bad request format");
        logout("Bad request format");
        return;
    }
    if(!isvalid_hashtag(hashtag)){
        send_response(sock,"Error","Bad hashtag format");
        logout("Bad hashtag format");
        return;
    }
    Token* token = validate_token(tok);
    if(token==NULL){
        send_response(sock,"Error","Invalid token");
        logout("Invalid token");
        return;
    }
    cJSON* json = tweets_with_hashtag(hashtag);
    send_response_json(sock,"List",json);
    logoutf("Tweets with hashtag %s sent",hashtag);
    cJSON_Delete(json);
}

#endif
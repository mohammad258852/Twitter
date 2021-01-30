#ifndef USERH
#define USERH

#include<string.h>
#include"tweet.h"
#include"consts.h"
#include"cJSON.h"
#include"userlist.h"
#include"tweetlist.h"

typedef struct
{
    char username[MAXUSERNAME];
    char password[MAXPASSWORD];
    char bio[MAXBIO];
    UserList* followers;
    UserList* followings;
    TweetList* personalTweets;
} User;

cJSON* user2json(const User* const user);
User json2user(const cJSON* json);
int write_user(const User* user);
User read_user(const char* username);
int check_username_exist(const char* const username);
int create_user(const char* username,const char* password);
int add_tweet_to_user(const char* username,int id);
void free_user(User* user);
cJSON* unread_tweets(const char*);

cJSON* user2json(const User* const user){
    cJSON* user_json = cJSON_CreateObject();
    cJSON_AddItemToObject(user_json,"username",cJSON_CreateString(user->username));
    cJSON_AddItemToObject(user_json,"password",cJSON_CreateString(user->password));
    cJSON_AddItemToObject(user_json,"bio",cJSON_CreateString(user->bio));

    cJSON* followers_json = cJSON_CreateArray();
    for(UserList* i=user->followers;i!=NULL;i = i->next){
        cJSON_AddItemToArray(followers_json,cJSON_CreateString(i->username));
    }
    cJSON_AddItemToObject(user_json,"followers",followers_json);

    cJSON* followings_json = cJSON_CreateArray();
    for(UserList* i=user->followings;i!=NULL;i = i->next){
        cJSON_AddItemToArray(followings_json,cJSON_CreateString(i->username));
    }
    cJSON_AddItemToObject(user_json,"followings",followings_json);

    cJSON* tweets_json = cJSON_CreateArray();
    for(TweetList* i=user->personalTweets;i!=NULL;i = i->next){
        cJSON_AddItemToArray(tweets_json,cJSON_CreateNumber(i->id));
    }
    cJSON_AddItemToObject(user_json,"personalTweets",tweets_json);

    return user_json;
}

User json2user(const cJSON* json){
    User tmp;
    strcpy(tmp.username, cJSON_GetObjectItem(json,"username")->valuestring);
    strcpy(tmp.password, cJSON_GetObjectItem(json,"username")->valuestring);
    strcpy(tmp.bio, cJSON_GetObjectItem(json,"bio")->valuestring);
    
    tmp.followers = make_user_list(cJSON_GetObjectItem(json,"followers"));

    tmp.followings = make_user_list(cJSON_GetObjectItem(json,"followings"));

    tmp.personalTweets = make_tweet_list(cJSON_GetObjectItem(json,"personalTweets"));

    return tmp;
}

int write_user(const User* user){
    char file_path[MAXFILENAMESIZE];
    strcpy(file_path,USERSFILEPATH);
    strcat(file_path,user->username);
    strcat(file_path,".json");
    system("mkdir -p "USERSFILEPATH);
    FILE* file = fopen(file_path,"w");
    if(file==NULL){
        return 0;
    }
    cJSON* user_json = user2json(user);
    fprintf(file,"%s",cJSON_PrintUnformatted(user_json));
    cJSON_Delete(user_json);
    fclose(file);
    return 1;
}

User read_user(const char* username){
    User tmp;
    char file_path[MAXFILENAMESIZE];
    strcpy(file_path,USERSFILEPATH);
    strcat(file_path,username);
    strcat(file_path,".json");
    FILE* file = fopen(file_path,"r");
    if(file==NULL){
        return tmp;
    }

    fseek(file,0,SEEK_END);
    unsigned size = ftell(file);
    fseek(file,0,SEEK_SET);
    char* content = malloc(size +1);
    fgets(content,size+1 , file);
    fclose(file);
    cJSON* json = cJSON_Parse(content);
    tmp = json2user(json);
    cJSON_Delete(json);
    free(content);
    return tmp;
}

int check_username_exist(const char* const username){
    char file_path[MAXFILENAMESIZE];
    strcpy(file_path,USERSFILEPATH);
    strcat(file_path,username);
    strcat(file_path,".json");

    if(access(file_path,F_OK) == 0){
        return 1;
    }
    return 0;
}

//make a user and a file for that user
//returns on success returns 1 otherwise return 0
int create_user(const char* username,const char* password){
    if(check_username_exist(username)){
        return 0;
    }

    User tmp;
    strcpy(tmp.username,username);
    strcpy(tmp.password,password);
    strcpy(tmp.bio,"");
    tmp.followers = NULL;
    tmp.followings = NULL;
    tmp.personalTweets = NULL;

    write_user(&tmp);

    return 1;
}

int add_tweet_to_user(const char* username,int id){
    User user = read_user(username);
    add_tweet_to_list(&user.personalTweets,id);
    write_user(&user);
    free_user(&user);
}

void free_user(User* user){
    free_userlist(user->followers);
    free_userlist(user->followings);
    free_tweetlist(user->personalTweets);
}

cJSON* unread_tweets(const char* username){
    User user = read_user(username);
    int* ids = calloc(1,sizeof(int));
    int total = 0;
    for(UserList* i = user.followings;i!=NULL; i=i->next){
        User following = read_user(i->username);
        for(TweetList* j=following.personalTweets; j!=NULL; j=j->next){
            if(!is_user_read_tweet(username,j->id)){
                user_read_tweet(username,j->id);
                ids = realloc(ids,total+1);
                ids[total] = j->id;
                total++;
            }
        }
    }
    sort_tweet(ids,total);
    cJSON* json = cJSON_CreateArray();
    for(int i=0;i<total;i++){
        cJSON_AddItemToArray(json,read_tweet_json(ids[i]));
    }
    return json;
}

#endif
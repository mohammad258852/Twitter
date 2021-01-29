#ifndef USERH
#define USERH

#include<string.h>
#include"tweet.h"
#include"consts.h"
#include"cJSON.h"

typedef struct
{
    char username[MAXUSERNAME];
    char password[MAXPASSWORD];
    char bio[MAXBIO];
    size_t folowers_number;
    char (*folowers)[MAXUSERNAME];
    size_t folowings_number;
    char (*folowings)[MAXUSERNAME];
    size_t tweets_number;
    int* tweets;
} User;

cJSON* user2json(const User* const user){
    cJSON* user_json = cJSON_CreateObject();
    cJSON_AddItemToObject(user_json,"username",cJSON_CreateString(user->username));
    cJSON_AddItemToObject(user_json,"password",cJSON_CreateString(user->password));
    cJSON_AddItemToObject(user_json,"bio",cJSON_CreateString(user->bio));

    cJSON* followers_json = cJSON_CreateArray();
    for(int i=0;i<user->folowers_number;i++){
        cJSON_AddItemToArray(followers_json,cJSON_CreateString(user->folowers[i]));
    }
    cJSON_AddItemToObject(user_json,"followers",followers_json);

    cJSON* followings_json = cJSON_CreateArray();
    for(int i=0;i<user->folowings_number;i++){
        cJSON_AddItemToArray(followings_json,cJSON_CreateString(user->folowings[i]));
    }
    cJSON_AddItemToObject(user_json,"followings",followings_json);

    cJSON* tweets_json = cJSON_CreateArray();
    for(int i=0;i<user->tweets_number;i++){
        cJSON_AddItemToArray(tweets_json,cJSON_CreateNumber(user->tweets[i]));
    }
    cJSON_AddItemToObject(user_json,"personalTweets",tweets_json);

    return user_json;
}

int write_user_to_file(const User* user){
    char file_path[MAXFILENAMESIZE];
    strcpy(file_path,USERSFILEPATH);
    strcat(file_path,user->username);
    strcat(file_path,".json");
    system("mkdir -p "USERSFILEPATH);
    FILE* file = fopen(file_path,"w");
    if(file==NULL){
        return 1;
    }
    cJSON* user_json = user2json(user);
    fprintf(file,"%s",cJSON_PrintUnformatted(user_json));
    cJSON_Delete(user_json);
    fclose(file);
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
    tmp.folowers_number = 0;
    tmp.folowers = NULL;
    tmp.folowings_number = 0;
    tmp.folowings = NULL;
    tmp.tweets_number = 0;

    write_user_to_file(&tmp);

    return 1;
}


#endif
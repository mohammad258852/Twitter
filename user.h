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
    size_t followers_number;
    char (*followers)[MAXUSERNAME];
    size_t followings_number;
    char (*followings)[MAXUSERNAME];
    size_t tweets_number;
    int* tweets;
} User;

cJSON* user2json(const User* const user){
    cJSON* user_json = cJSON_CreateObject();
    cJSON_AddItemToObject(user_json,"username",cJSON_CreateString(user->username));
    cJSON_AddItemToObject(user_json,"password",cJSON_CreateString(user->password));
    cJSON_AddItemToObject(user_json,"bio",cJSON_CreateString(user->bio));

    cJSON* followers_json = cJSON_CreateArray();
    for(int i=0;i<user->followers_number;i++){
        cJSON_AddItemToArray(followers_json,cJSON_CreateString(user->followers[i]));
    }
    cJSON_AddItemToObject(user_json,"followers",followers_json);

    cJSON* followings_json = cJSON_CreateArray();
    for(int i=0;i<user->followings_number;i++){
        cJSON_AddItemToArray(followings_json,cJSON_CreateString(user->followings[i]));
    }
    cJSON_AddItemToObject(user_json,"followings",followings_json);

    cJSON* tweets_json = cJSON_CreateArray();
    for(int i=0;i<user->tweets_number;i++){
        cJSON_AddItemToArray(tweets_json,cJSON_CreateNumber(user->tweets[i]));
    }
    cJSON_AddItemToObject(user_json,"tweets",tweets_json);

    return user_json;
}

User json2user(const cJSON* json){
    User tmp;
    strcpy(tmp.username, cJSON_GetObjectItem(json,"username")->valuestring);
    strcpy(tmp.password, cJSON_GetObjectItem(json,"username")->valuestring);
    strcpy(tmp.bio, cJSON_GetObjectItem(json,"bio")->valuestring);
    
    tmp.followers_number = cJSON_GetArraySize(cJSON_GetObjectItem(json,"followers"));
    if(tmp.followers_number!=0){
        tmp.followers = calloc((tmp.followers_number),sizeof(char[MAXUSERNAME]));
        int i = 0;
        for(cJSON* followers_json=cJSON_GetObjectItem(json,"followers")->child;
            followers_json!=NULL;
            followers_json=followers_json->next,i++){
                strcpy(tmp.followers[i],followers_json->valuestring);
        }
    }

    tmp.followings_number = cJSON_GetArraySize(cJSON_GetObjectItem(json,"followings"));
    if(tmp.followings_number!=0){
        tmp.followings = calloc((tmp.followings_number),sizeof(char[MAXUSERNAME]));
        int i = 0;
        for(cJSON* following_json=cJSON_GetObjectItem(json,"followings")->child;
            following_json!=NULL;
            following_json=following_json->next,i++){
                strcpy(tmp.followings[i],following_json->valuestring);
        }
    }

    tmp.tweets_number = cJSON_GetArraySize(cJSON_GetObjectItem(json,"tweets"));
    if(tmp.tweets_number!=0){
        tmp.tweets = calloc((tmp.tweets_number),sizeof(int));
        int i = 0;
        for(cJSON* tweet_json=cJSON_GetObjectItem(json,"tweets")->child;
            tweet_json!=NULL;
            tweet_json=tweet_json->next,i++){
                tmp.tweets[i] = tweet_json->valueint;
        }
    }

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
    tmp.followers_number = 0;
    tmp.followers = NULL;
    tmp.followings_number = 0;
    tmp.followings = NULL;
    tmp.tweets_number = 0;

    write_user(&tmp);

    return 1;
}


#endif
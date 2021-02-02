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
cJSON* make_user_for_client(const char*,const char*);
void user_follow_that(const char*,const char*);
void user_unfollow_that(const char*,const char*);
void delete_tweet(const char*,int);
int is_user_own_tweet(const char*,int);

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
    strcpy(tmp.password, cJSON_GetObjectItem(json,"password")->valuestring);
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
    int total = 0;
    for(UserList* i = user.followings;i!=NULL; i=i->next){
        User following = read_user(i->username);
        for(TweetList* j=following.personalTweets; j!=NULL; j=j->next){
            if(!is_user_read_tweet(username,j->id)){
                total++;
            }
        }
    }
    int* ids = malloc(total * sizeof(int));
    int* iter = ids;
    for(UserList* i = user.followings;i!=NULL; i=i->next){
        User following = read_user(i->username);
        for(TweetList* j=following.personalTweets; j!=NULL; j=j->next){
            if(!is_user_read_tweet(username,j->id)){
                user_read_tweet(username,j->id);
                *iter = j->id;
                iter++;
            }
        }
    }
    sort_tweet(ids,total);
    cJSON* json = cJSON_CreateArray();
    for(int i=0;i<total;i++){
        cJSON_AddItemToArray(json,read_tweet_json(ids[i]));
    }
    free(ids);
    return json;
}

int is_user_follows_that(const char* user,const char* that){
    User tmp = read_user(user);
    for(UserList* i = tmp.followings;i!=NULL; i = i->next){
        if(strcmp(that,i->username)==0)
            return 1;
    }
    return 0;
}

cJSON* make_user_for_client(const char* username,const char* client_username){
    User user = read_user(username);
    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json,"username",cJSON_CreateString(user.username));
    cJSON_AddItemToObject(json,"bio",cJSON_CreateString(user.bio));
    cJSON_AddItemToObject(json,"numberOfFollowers",cJSON_CreateNumber(count_userlist(user.followers)));
    cJSON_AddItemToObject(json,"numberOfFollowings",cJSON_CreateNumber(count_userlist(user.followings)));
    if(strcmp(username,client_username)==0 ){
        cJSON_AddItemToObject(json,"followStatus",cJSON_CreateString("Yourself"));
    }
    else{
        if(is_user_follows_that(client_username,username)){
            cJSON_AddItemToObject(json,"followStatus",cJSON_CreateString("Followed"));
        }
        else{
            cJSON_AddItemToObject(json,"followStatus",cJSON_CreateString("NotFollowed"));
        }
    }
    int tweets_number = count_tweetlist(user.personalTweets);
    int* ids = malloc(tweets_number*sizeof(int));
    int* iter = ids;
    for(TweetList* i=user.personalTweets;i!=NULL;i = i->next,iter++){
        *iter = i->id;
    }
    sort_tweet(ids,tweets_number);
    cJSON* tweet_arr = cJSON_CreateArray();
    for(int i=0;i<tweets_number;i++){
        cJSON_AddItemToArray(tweet_arr,read_tweet_json(ids[i]));
    }
    cJSON_AddItemToObject(json,"allTweets",tweet_arr);
    free(ids);
    return json;
}

void user_follow_that(const char* username,const char* thatname){
    User user = read_user(username);
    User that = read_user(thatname);
    add_user_to_list(&user.followings,thatname);
    add_user_to_list(&that.followers,username);
    write_user(&user);
    write_user(&that);
    free_user(&user);
    free_user(&that);
}

void user_unfollow_that(const char* username,const char* thatname){
    User user = read_user(username);
    User that = read_user(thatname);
    delete_user_from_list(&user.followings,thatname);
    delete_user_from_list(&that.followers,username);
    write_user(&user);
    write_user(&that);
    free_user(&user);
    free_user(&that);
}

void delete_tweet(const char* username,int id){
    User user = read_user(username);
    delete_tweet_from_list(&user.personalTweets,id);
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",id);
    remove(path);
    sprintf(path,TWEETLIKEPATH"%d.txt",id);
    remove(path);
    sprintf(path,TWEETREADPATH"%d.txt",id);
    remove(path);
    write_user(&user);
    free_user(&user);
}

int is_user_own_tweet(const char* username,int id){
    User user = read_user(username);
    for(TweetList* i=user.personalTweets;i!=NULL;i = i->next){
        if(i->id == id)
            free_user(&user);
            return 1;
    }
    free_user(&user);
    return 0;
}

#endif
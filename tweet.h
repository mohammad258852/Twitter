#ifndef TWEETH
#define TWEETH

#include<string.h>
#include"cJSON.h"
#include"consts.h"
#include"comment.h"
#include"commentlist.h"

typedef struct 
{
    int id;
    char author[MAXUSERNAME];
    char content[MAXTEX];
    unsigned likes;
    CommentList* comments;
} Tweet;

int add_tweet(const char*,const char*);
int tweet_exist(int);
int write_tweet(const Tweet*);
cJSON* tweet2json(const Tweet*);


int tweet_exist(int id){
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",id);

    if(access(path,F_OK)==0){
        return 1;
    }
    return 0;
}

cJSON* tweet2json(const Tweet* tweet){
    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json,"id",cJSON_CreateNumber(tweet->id));
    cJSON_AddItemToObject(json,"author",cJSON_CreateString(tweet->author));
    cJSON_AddItemToObject(json,"content",cJSON_CreateString(tweet->content));
    cJSON_AddItemToObject(json,"likes",cJSON_CreateNumber(tweet->likes));

    cJSON* comments_json = cJSON_CreateArray();
    for(CommentList* i=tweet->comments; i!=NULL; i=i->next){
        cJSON_AddItemToArray(comments_json,comment2json(&(i->comment)));
    }
    cJSON_AddItemToObject(json,"comments",comments_json);
    return json;
}

int write_tweet(const Tweet* tweet){
    system("mkdir -p "TWEETPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",tweet->id);
    FILE* file = fopen(path,"w");
    if(file==NULL){
        return 0;
    }
    cJSON* tweet_json = tweet2json(tweet);
    fputs(cJSON_PrintUnformatted(tweet_json),file);
    fclose(file);
    cJSON_Delete(tweet_json);
    return 1;
}

int add_tweet(const char* username,const char* content){
    Tweet tmp;
    tmp.id = 1;
    while(tweet_exist(tmp.id)){
        tmp.id++;
    }
    strcpy(tmp.author,username);
    strcpy(tmp.content,content);
    tmp.likes = 0;
    tmp.comments = 0;

    if(write_tweet(&tmp)==0)
        return 0;
    return tmp.id;
}

#endif
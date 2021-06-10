#ifndef HASHTAGH
#define HASHTAGH

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include"consts.h"
#include"idlist.h"
#include"utility.h"
#include"tweet.h"

const char* find_hashtag(const char*,char*);
void extract_hashtags(const char*,int);
void add_id_to_hashtag(const char*,int);
void remove_id_from_hashtag(const char*,int);
void remove_id_from_all_hashtags(int);
cJSON* tweets_with_hashtag(const char*);

//find one hashtag and store it in place.
//returns the next pointer after hashtag
//if no hashtags found return NULL
const char* find_hashtag(const char* str,char* place){
    for(const char* i=str;*i!='\0';i++){
        if(*i=='#'){
            if(sscanf(i,"#%"MAXHASHTAGS"[a-zA-Z0-9_]",place)!=1){
                continue;
            }
            return i+1+strlen(place);
        }
    }
    return NULL;
}

void extract_hashtags(const char* str,int id){
    char tmp[MAXHASHTAG+1];
    while((str=find_hashtag(str,tmp))!=NULL){
        add_id_to_hashtag(tmp,id);
        logoutf("tweet %d contains hashtag %s",id,tmp);
    }
}

void add_id_to_hashtag(const char* hashtag,int id){
    system("mkdir -p "HASHTAGSPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,HASHTAGSPATH"%s.txt",hashtag);
    FILE* file = fopen(path,"a");
    if(file==NULL){
        return;
    }
    fprintf(file,"%d\n",id);
    fclose(file);
}

void remove_id_from_hashtag(const char* hashtag,int id){
    system("mkdir -p "HASHTAGSPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,HASHTAGSPATH"%s.txt",hashtag);
    FILE* file = fopen(path,"r");
    FILE* tmp_file = fopen(HASHTAGSPATH"tmp.txt","w");
    if(file==NULL || tmp_file==NULL){
        return;
    }
    int tmp_id;
    int total = 0;
    while(fscanf(file,"%d",&tmp_id)!=EOF){
        if(tmp_id==id)
            continue;
        fprintf(tmp_file,"%d\n",tmp_id);
        total++;
    }
    fclose(file);
    fclose(tmp_file);
    remove(path);
    rename(HASHTAGSPATH"tmp.txt",path);
    if(total==0)
        remove(path);
}

void remove_id_from_all_hashtags(int id){
    DIR* dir;
    struct dirent* entr;
    if((dir=opendir(HASHTAGSPATH))!=NULL){
        while((entr = readdir(dir))!=NULL){
            if(!string_end_with(entr->d_name,".txt"))
                continue;
            char hashtag[MAXHASHTAG+1];
            strcpy(hashtag,entr->d_name);
            hashtag[strlen(hashtag)-4] = '\0';
            remove_id_from_hashtag(hashtag,id);
        }
    }
}

cJSON* tweets_with_hashtag(const char* hashtag){
    cJSON* arr = cJSON_CreateArray();
    char path[MAXFILENAMESIZE];
    sprintf(path,HASHTAGSPATH"%s.txt",hashtag);
    FILE* file = fopen(path,"r");
    if(file==NULL){
        return arr;
    }
    int total = 0;
    int tmp_id;
    while(fscanf(file,"%d",&tmp_id)!=EOF){
        if(tweet_exist(tmp_id)){
            total++;
        }
    }
    IdList* ids = calloc(total,sizeof(IdList));
    IdList* itr = ids;
    fseek(file,0,SEEK_SET);
    while(fscanf(file,"%d",&tmp_id)!=EOF){
        if(tweet_exist(tmp_id)){
            itr->id = tmp_id;
            strcpy(itr->retweeter,"");
            itr++;
        }
    }
    sort_tweet(ids,total);
    for(int i=0;i<total;i++){
        cJSON* tweet = read_tweet_json(ids[i].id);
        cJSON_AddItemToObject(tweet,"retweeter",cJSON_CreateString(ids[i].retweeter));
        cJSON_AddItemToArray(arr,tweet);
    }
    free(ids);
    return arr;
}

#endif
#ifndef TWEETLISTH
#define TWEETLISTH

#include<stdlib.h>
#include"cJSON.h"

typedef struct __tweetlist{
    int id;
    struct __tweetlist* next;
} TweetList;

TweetList* make_tweet_list(cJSON* json);
void add_tweet_to_list(TweetList** list,int id);
void free_tweetlist(TweetList* list);

TweetList* make_tweet_list(cJSON* json){
    TweetList* first = NULL;
    TweetList** cur = &first;
    TweetList** pre = NULL;
    for(cJSON* i=json->child;i!=NULL;i = i->next){
        *cur = calloc(1,sizeof(TweetList));
        (*cur)->id = i->valueint;
        (*cur)->next = NULL;
        if(pre!=NULL){
            (*pre)->next = *cur;
        }
        pre = cur;
        cur = &((*pre)->next);
    }
    return first;
}

void add_tweet_to_list(TweetList** list,int id){
    if(*list==NULL){
        *list = calloc(1,sizeof(TweetList));
        (*list)->id = id;
        (*list)->next = NULL;
        return;
    }
    while((*list)->next!=NULL){
        list = &((*list)->next);
    }
    (*list)->next = calloc(1,sizeof(TweetList));
    (*list)->next->id = id;
    (*list)->next->next = NULL;
}

void free_tweetlist(TweetList* list){
    while(list!=NULL){
        TweetList* next = list->next;
        free(list);
        list = next;
    }
}

#endif
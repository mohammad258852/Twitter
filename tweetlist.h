#ifndef TWEETLIST
#define TWEETLIST

#include<stdlib.h>
#include"cJSON.h"

typedef struct __tweetlist{
    int id;
    struct __tweetlist* next;
} TweetList;

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

#endif
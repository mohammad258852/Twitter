#ifndef IDLISTH
#define IDLISTH

#include<stdlib.h>
#include"consts.h"
#include"tweet.h"

typedef struct 
{
    int id;
    char retweeter[MAXUSERNAME];
} IdList;

int tweet_cmp(const void* x,const void* y){
    Tweet xx = read_tweet((*(IdList*)x).id);
    Tweet yy = read_tweet((*(IdList*)y).id);
    return (xx.time - yy.time);
    free_tweet(&xx);
    free_tweet(&yy);
}

void sort_tweet(IdList* ids,size_t n){
    qsort(ids,n,sizeof(IdList),tweet_cmp);
}

#endif
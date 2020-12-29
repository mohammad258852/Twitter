#ifndef TWEET
#define TWEET
#include<stdlib.h>
#include"consts.h"
#include"comment.h"
#include"cJSON.h"

typedef 
struct __tweet
{
    int id;
    char *author;
    char *content;
    Comment *comments;
    int likes;
    int comment_number;
}Tweet;

Tweet make_tweet_json(cJSON* json){
    Tweet tmp;
    tmp.id = cJSON_GetObjectItem(json,"id")->valueint;
    tmp.author = cJSON_GetObjectItem(json,"author")->valuestring;
    tmp.content = cJSON_GetObjectItem(json,"content")->valuestring;
    tmp.likes = cJSON_GetObjectItem(json,"likes")->valueint;
    tmp.comment_number = cJSON_GetArraySize(cJSON_GetObjectItem(json,"comments"));
    tmp.comments = (Comment*)calloc(tmp.comment_number,sizeof(Comment));
    int i=0;
    for(cJSON *elm = cJSON_GetObjectItem(json,"comments")->child;i<tmp.comment_number;i++){
        tmp.comments[i] = make_comment_json(elm);
    }
    return tmp;
}

Tweet* make_tweet_array_json(cJSON* json){
    Tweet* tweets = (Tweet*)calloc(cJSON_GetArraySize(json),sizeof(Tweet));
    int i = 0;
    for(cJSON* json_cur_tweet=json->child;
        json_cur_tweet!=NULL;
        json_cur_tweet=json_cur_tweet->next,i++){

        tweets[i] = make_tweet_json(json_cur_tweet);
    }
    return tweets;
}

void print_tweet(Tweet tweet){
    printw("author:%s \t\t id:%d\n",tweet.author,tweet.id);
    printw("%s\n",tweet.content);
    printw("Likes:%d \t Comments:%d\n",tweet.likes,tweet.comment_number);
    printw("\n\n");
}

#endif
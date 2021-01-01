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
    if(tmp.comment_number>0){
        tmp.comments = (Comment*)calloc(tmp.comment_number,sizeof(Comment));
        int i=0;
        for(cJSON *elm = cJSON_GetObjectItem(json,"comments")->child;i<tmp.comment_number&&elm!=NULL;i++,elm=elm->next){
            tmp.comments[i] = make_comment_json(elm);
        }
    }
    else{
        tmp.comments = NULL;
    }
    return tmp;
}

Tweet* make_tweet_array_json(cJSON* json){
    if(cJSON_GetArraySize(json)==0){
        return NULL;
    }
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
    printw("Likes:%d      Comments:%d\n",tweet.likes,tweet.comment_number);
    printw("\n\n");
}

void wprint_tweet(WINDOW* win,Tweet* tweet){
    wclear(win);
    if(tweet==NULL){
        mvwprintw(win,1,4,"No tweet :(");
    }
    else{
        mvwprintw(win,0,0,"author:%s",tweet->author,tweet->id);
        mvwprintw(win,1,0,"%s",tweet->content);
        mvwprintw(win,2,0,"Likes:%d      Comments:%d\n",tweet->likes,tweet->comment_number);
    }
    wrefresh(win);
}

void add_comment(Tweet* tweet,Comment comment){
    Comment* tmp = (Comment*)calloc(tweet->comment_number+1 , sizeof(Comment));
    for(int i=0;i<tweet->comment_number;i++){
        tmp[i] = tweet->comments[i];
    }
    tmp[tweet->comment_number] = comment;
    free(tweet->comments);
    tweet->comments = tmp;
    tweet->comment_number++;
}

void wprint_comment(WINDOW* win,const Tweet* const tweet,const int number){
    wclear(win);
    if(tweet==NULL || tweet->comment_number==0){
        mvwprintw(win,1,3,"No Comments:(");
    }
    else{
        Comment comment = tweet->comments[number];
        mvwprintw(win,0,0,"comment %d",number+1);
        mvwprintw(win,1,0,"author:%s",comment.author);
        mvwprintw(win,2,0,"%s",comment.content);
    }
    wrefresh(win);
}

#endif
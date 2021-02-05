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
    char author[MAX];
    char retweeter[MAX];
    char content[MAXTEXT];
    Comment *comments;
    int likes;
    int comment_number;
}Tweet;

Tweet make_tweet_json(cJSON* json){
    Tweet tmp;
    tmp.id = cJSON_GetObjectItem(json,"id")->valueint;
    strcpy(tmp.author,cJSON_GetObjectItem(json,"author")->valuestring);
    strcpy(tmp.retweeter,cJSON_GetObjectItem(json,"retweeter")->valuestring);
    strcpy(tmp.content,cJSON_GetObjectItem(json,"content")->valuestring);
    tmp.likes = cJSON_GetObjectItem(json,"likes")->valueint;
    tmp.comment_number = cJSON_GetArraySize(cJSON_GetObjectItem(json,"comments"));
    if(tmp.comment_number>0){
        tmp.comments = (Comment*)calloc(tmp.comment_number,sizeof(Comment));
        int i=0;
        for(cJSON *elm = cJSON_GetObjectItem(json,"comments")->child;i<tmp.comment_number&&elm!=NULL;i++,elm=elm->next){
            tmp.comments[i] = make_comment_json(elm->child);
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
        wattron(win,COLOR_PAIR(ERROR_COLOR));
        mvwprintw(win,1,4,"No tweet :(");
        wattroff(win,COLOR_PAIR(ERROR_COLOR));
    }
    else{
        if(strcmp(tweet->retweeter,"")==0){
            wprintw(win,"author:%s\n",tweet->author,tweet->id);
        }
        else{
            wprintw(win,"author:%s",tweet->author);
            wattron(win,COLOR_PAIR(RETWEETER_COLOR));
            wprintw(win,"(%s)\n",tweet->retweeter);
            wattroff(win,COLOR_PAIR(RETWEETER_COLOR));
        }
        wattron(win,COLOR_PAIR(TWEET_COLOR));
        wprintw(win,"%s\n",tweet->content);
        wattroff(win,COLOR_PAIR(TWEET_COLOR));
        wprintw(win,"Likes:%d      Comments:%d",tweet->likes,tweet->comment_number);
    }
    wrefresh(win);
}

void add_comment(Tweet* tweet,Comment comment){
    /*//if author exist just change the comment
    int author_exist = -1;
    for(int i=0;i<tweet->comment_number;i++){
        if(strcmp(comment.author,tweet->comments[i].author)==0){
            author_exist = i;
            break;
        }
    }
    if(author_exist!=-1){
        strcpy(tweet->comments[author_exist].content,comment.content);
        return;
    }*/
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
        wattron(win,COLOR_PAIR(ERROR_COLOR));
        mvwprintw(win,1,3,"No Comments :(");
        wattroff(win,COLOR_PAIR(ERROR_COLOR));
    }
    else{
        Comment comment = tweet->comments[number];
        wprintw(win,"comment %d\n",number+1);
        wprintw(win,"author:%s\n",comment.author);
        wattron(win,COLOR_PAIR(COMMENT_COLOR));
        wprintw(win,"%s\n",comment.content);
        wattroff(win,COLOR_PAIR(COMMENT_COLOR));
    }
    wrefresh(win);
}

void free_tweet(Tweet tweet){
    /*for(int i=0;i<tweet->comment_number;i++){
        free_comment(tweet->comments[i]);
    }*/
    free(tweet.comments);
}

void tweet_copy(Tweet* dest,const Tweet* src){
    dest->id = src->id;
    strcpy(dest->author,src->author);
    strcpy(dest->retweeter,src->retweeter);
    strcpy(dest->content,src->content);
    dest->likes = src->likes;
    dest->comment_number = src->comment_number;
    if(dest->comment_number==0)
        dest->comments = NULL;
    else
        dest->comments = calloc(dest->comment_number,sizeof(Comment));
    for(int i=0;i<src->comment_number;i++){
        dest->comments[i] = src->comments[i];
    }
}

#endif
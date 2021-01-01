#ifndef USERH
#define USERH
#include<string.h>
#include<ncurses.h>
#include"consts.h"
#include"tweet.h"

typedef
struct __user{
    char username[MAX];
    char bio[MAX];
    int followers;
    int followings;
    int following_status;
    int tweets_number;
    Tweet* tweets;
}User;

User make_user_json(cJSON* json){
    User tmp;
    strcpy(tmp.username, cJSON_GetObjectItem(json,"username")->valuestring);
    strcpy(tmp.bio, cJSON_GetObjectItem(json,"bio")->valuestring);
    tmp.followers = cJSON_GetObjectItem(json,"numberOfFollowers")->valueint;
    tmp.followings = cJSON_GetObjectItem(json,"numberOfFollowings")->valueint;
    tmp.following_status = (strcmp(cJSON_GetObjectItem(json,"followStatus")->valuestring,
                            "Followed")==0);
    tmp.tweets_number = cJSON_GetArraySize(cJSON_GetObjectItem(json,"allTweets"));
    tmp.tweets = make_tweet_array_json(cJSON_GetObjectItem(json,"allTweets"));
    return tmp;
}

void wprint_user(WINDOW* win,User user,int follow_status){
    wclear(win);
    mvwprintw(win,0,0,"Username:%s",user.username);
    mvwprintw(win,1,0,"bio:%s",user.bio);
    mvwprintw(win,2,0,"followers:%d    followings:%d",user.followers,user.followings);
    if(follow_status){
        mvwprintw(win,3,0,"%s",(user.following_status==1?"unfollow":"follow"));
    }
    wrefresh(win);
}

#endif
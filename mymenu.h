#ifndef MYMENU
#define MYMENU

#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>
#include<menu.h>
#include<form.h>
#include"consts.h"
#include"mysocket.h"
#include"cJSON.h"
#include"tweet.h"
#include"comment.h"

void start_menu();
void signup_menu();
void login_menu();
void main_menu();
void timeline_menu();
void logout();
void send_tweet();
void press_key_to_continue();
char* itos(int d);
void like_tweet(Tweet* tweet);
void comment_tweet(WINDOW* win,Tweet* tweet);

//TODO
void search_menu();
void tweet_profile_menu();
void personal_area_menu();
void refresh_tweet();
//TODO




void start_menu(){
    const char * const items_name[] = {
        "Sign Up",
        "Log in",
        "Quit"
    };
    int number_choices = ARRAY_SIZE(items_name);
    ITEM** my_items = (ITEM**)calloc(number_choices+1,sizeof(ITEM*));
    for(int i=0;i<number_choices;i++){
        my_items[i] = new_item(items_name[i],"");
    }
    MENU* 	my_menu = new_menu((ITEM **)my_items);


    int repeat = 1;
    while (repeat)
    {
        clear();
        post_menu(my_menu);
        mvprintw(LINES-2,0,"Use Arow key to move and Use Enter to Select");
        refresh();
        int ch;
        int choise = -1;
        while(choise==-1 && (ch=getch())){
            switch (ch)
            {
            case KEY_DOWN:
                menu_driver(my_menu,REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu,REQ_UP_ITEM);
                break;
            case 10:
                choise = item_index(current_item(my_menu));
                break;
            }
        }
        unpost_menu(my_menu);
        switch (choise)
        {
        case 0:
            signup_menu();
            break;
        case 1:
            login_menu();
            break;
        case 2:
        default:
            repeat = 0;
            break;
        }
    }
    for(int i=0;i<number_choices;i++){
        free_item(my_items[i]);
    }
    free_menu(my_menu);
    free(my_items);
}

void signup_menu(){
    char username[MAX];
    char password[MAX];

    printw("Welcome To Sing up Page:\n");
    printw("Enter Your Username:\n");
    echo();
    scanw("%s",username);
    noecho();
    printw("Enter Your Password:\n");
    scanw("%s",password);

    char request[3*MAX];
    char response[MAX];
    sprintf(request,"signup %s,%s\n",username,password);
    send_request(request,strlen(request),response);
    cJSON *json = cJSON_Parse(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;
    char* message = cJSON_GetObjectItem(json,"message")->valuestring;

    if(strcmp(type,"Successful")==0){
        printw("\n Signed up Successfully...\n");
        press_key_to_continue();
        login_menu();
    }
    else{
        printw("Error: %s\n",message);
        press_key_to_continue();
        return;
    }
}

void login_menu(){
    clear();
    printw("Welcome To Login Page:\n");
    printw("Enter Your Username:\n");
    echo();
    scanw("%s",username);
    noecho();
    printw("Enter Your Password:\n");
    scanw("%s",password);


    char request[3*MAX];
    char response[MAX]; 
    sprintf(request,"login %s,%s\n",username,password);
    send_request(request,strlen(request),response);
    cJSON *json = cJSON_Parse(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;
    char* message = cJSON_GetObjectItem(json,"message")->valuestring;

    if(strcmp(type,"Token")==0){
        strcpy(auth,message);
        printw("Logged in Successfully\n");
        press_key_to_continue();
        main_menu();
    }
    else{
        printw("Error: %s\n",message);
        press_key_to_continue();
        return;
    }
}

void main_menu(){
    const char*  const items_name[] = {
        "Timeline",
        "Search",
        "Tweet Profile",
        "Personal area",
        "Log out"
    };

    int number_choices = ARRAY_SIZE(items_name);
    ITEM** my_items = (ITEM**)calloc(number_choices+1,sizeof(ITEM*));
    for(int i=0;i<number_choices;i++){
        my_items[i] = new_item(items_name[i],"");
    }
    MENU* 	my_menu = new_menu((ITEM **)my_items);


    int repeat = 1;
    while (repeat)
    {
        clear();
        post_menu(my_menu);
        mvprintw(LINES-2,0,"Use Arow key to move and Use Enter to Select");
        refresh();
        int ch;
        int choise = -1;
        while(choise==-1 && (ch=getch())){
            switch (ch)
            {
            case KEY_DOWN:
                menu_driver(my_menu,REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu,REQ_UP_ITEM);
                break;
            case 10:
                choise = item_index(current_item(my_menu));
                break;
            }
        }
        unpost_menu(my_menu);
        switch (choise)
        {
        case 0:
            timeline_menu();
            break;
        case 1:
            search_menu();
            break;
        case 2:
            tweet_profile_menu();
            break;
        case 3:
            personal_area_menu();
            break;
        case 4:
        default:
            logout();
            repeat = 0;
            break;
        }
    }
    for(int i=0;i<number_choices;i++){
        free_item(my_items[i]);
    }
    free_menu(my_menu);
    free(my_items);
}

void timeline_menu(){
    const char* const items_name[] = {
        "send tweet",
        "refresh",
        "back"
    };
    int number_choices = ARRAY_SIZE(items_name);
    ITEM** my_items = (ITEM**)calloc(number_choices+1,sizeof(ITEM*));
    for(int i=0;i<number_choices;i++){
        my_items[i] = new_item(items_name[i],"");
    }
    MENU* 	my_menu = new_menu((ITEM **)my_items);


    int repeat = 1;
    while (repeat)
    {
        clear();
        post_menu(my_menu);
        mvprintw(LINES-2,0,"Use Arow key to move and Use Enter to Select");
        refresh();
        int ch;
        int choise = -1;
        while(choise==-1 && (ch=getch())){
            switch (ch)
            {
            case KEY_DOWN:
                menu_driver(my_menu,REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu,REQ_UP_ITEM);
                break;
            case 10:
                choise = item_index(current_item(my_menu));
                break;
            }
        }
        unpost_menu(my_menu);
        switch (choise)
        {
        case 0:
            send_tweet();
            break;
        case 1:
            refresh_tweet();
            break;
        case 2:
        default:
            repeat = 0;
            break;
        }
    }
    for(int i=0;i<number_choices;i++){
        free_item(my_items[i]);
    }
    free_menu(my_menu);
    free(my_items);
}

//TODO
void search_menu(){

}
//TODO
void tweet_profile_menu(){

}
//TODO
void personal_area_menu(){

}

void logout(){
    char request[2*MAX];
    char respons[2*MAX];
    sprintf(request,"logout %s\n",auth);
    send_request(request,strlen(request),respons);

    clear();

    printw("Loged out\n");
    press_key_to_continue();
}

/*
########################
#tweets id             #
########################
#tweet                 #
########################
# comments             #
########################
#help                  #
########################
*/


void send_tweet(){
    char tweet[MAX];
    
    clear();
    printw("Sending a Tweet:\n");
    printw("Type your tweet:\n");
    echo();
    getstr(tweet);
    noecho();

    char request[3*MAX];
    char respons[3*MAX];
    sprintf(request,"send tweet %s,%s\n",auth,tweet);
    send_request(request,strlen(request),respons);
    printw("tweet sent\n");
    press_key_to_continue();
}

void refresh_tweet(){
    char request[2*MAX];
    char response[2*MAX];
    sprintf(request,"refresh %s\n",auth);
    send_request(request,strlen(request),response);
    refresh();
    cJSON *json = cJSON_Parse(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;

    if(strcmp(type,"List")!=0){
        printw("Error accured\n");
        press_key_to_continue();
        return;
    }
    cJSON* json_tweets = cJSON_GetObjectItem(json,"message");
    int tweets_number = cJSON_GetArraySize(json_tweets);
    Tweet* tweets = make_tweet_array_json(json_tweets);
    clear();

    if(tweets_number<1){
        mvprintw(LINES/2 ,4,"NO new tweets :)..");
        move(LINES/2+1,4);
        press_key_to_continue();
        return;
    }

    //calculating windows start x y and heigh,width
    int tweets_id_win_y , tweets_id_win_x;
    tweets_id_win_y = 0;
    tweets_id_win_x = 0;
    int tweets_id_win_h , tweets_id_win_w;
    tweets_id_win_h = 3;
    tweets_id_win_w = COLS;

    int tweet_win_y , tweet_win_x;
    tweet_win_y = tweets_id_win_y + tweets_id_win_h;
    tweet_win_x = 0;
    int tweet_win_h , tweet_win_w;
    tweet_win_h = 5;
    tweet_win_w = COLS;

    int comment_win_y , comment_win_x;
    comment_win_y = tweet_win_y + tweet_win_h;
    comment_win_x = 0;
    int comment_win_h , comment_win_w;
    comment_win_h = 5;
    comment_win_w = COLS;


    mvprintw(LINES-2,0,"I can help you here");
    refresh();
    WINDOW* tweets_id_win = newwin( tweets_id_win_h,tweets_id_win_w,
                                    tweets_id_win_y,tweets_id_win_x);
    keypad(tweets_id_win,TRUE);
    box(tweets_id_win,0,0);
    wrefresh(tweets_id_win);

    WINDOW* tweet_win = newwin( tweet_win_h,tweet_win_w,
                                tweet_win_y,tweet_win_x);
    WINDOW* tweet_subwin = derwin(tweet_win,tweet_win_h-2,tweet_win_w-2,1,1);
    keypad(tweet_win,TRUE);
    box(tweet_win,0,0);
    wrefresh(tweet_win);

    WINDOW* comment_win = newwin(   comment_win_h,comment_win_w,
                                    comment_win_y,comment_win_x);
    WINDOW* comment_subwin = derwin(comment_win,comment_win_h-2,comment_win_w-2,1,1);
    keypad(comment_win,TRUE);
    box(comment_win,0,0);
    wrefresh(comment_win);
    
    ITEM** tweets_id_items = (ITEM**)calloc(tweets_number+1,sizeof(ITEM*));
    char** tweets_id_items_string = (char**)calloc(tweets_number,sizeof(char*));
    for(int i=0;i<tweets_number;i++){
        tweets_id_items_string[i] = itos(tweets[i].id);
        tweets_id_items[i] = new_item(tweets_id_items_string[i],"");
    }
    MENU* tweets_id_menu = new_menu(tweets_id_items);
    menu_opts_off(tweets_id_menu, O_SHOWDESC);
    set_menu_win(tweets_id_menu,tweets_id_win);
    set_menu_sub(tweets_id_menu,derwin(tweets_id_win,tweets_id_win_h-2,tweets_id_win_w-2,1,1));
    set_menu_format(tweets_id_menu,1,COLS/6);
    post_menu(tweets_id_menu);
    wrefresh(tweets_id_win);

    Comment tmp_comment;
    strcpy(tmp_comment.author, "This tweet has no comment!!");
    strcpy(tmp_comment.content,"");

    int current_tweet = 0;
    int current_comment = 0;
    int tweet_change = 0;
    int comment_change = 0;
    int continue_runnig = 1;
    refresh();
    wprint_tweet(tweet_subwin,tweets[current_tweet]);
    wprint_comment( comment_subwin,&tweets[current_tweet],current_comment);
    int ch=1;
    while(continue_runnig && (ch=wgetch(tweets_id_win))){
        switch (ch)
        {
        case KEY_RIGHT:
            menu_driver(tweets_id_menu,REQ_RIGHT_ITEM);
            tweet_change = 1;
            comment_change = 1;
            current_comment = 0;
            break;
        case KEY_LEFT:
            menu_driver(tweets_id_menu,REQ_LEFT_ITEM);
            tweet_change = 1;
            comment_change = 1;
            current_comment = 0;
            break;
        case KEY_UP:
            if(current_comment-1>=0){
                current_comment--;
                comment_change = 1;
            }
            break;
        case KEY_DOWN:
            if(current_comment+1<tweets[current_tweet].comment_number){
                current_comment++;
                comment_change = 1;
            }
            break;
        case 'l':
            like_tweet(tweets+current_tweet);
            tweet_change = 1;
            break;
        case 'c':
            comment_tweet(comment_subwin,tweets+current_tweet);
            comment_change = 1;
            tweet_change = 1;
            break;
        case 'q':
            continue_runnig = 0;
            break;
        }
        if(tweet_change==1){
            tweet_change = 0;
            current_tweet = item_index(current_item(tweets_id_menu));
            wprint_tweet(tweet_subwin,tweets[current_tweet]);
        }
        if(comment_change){
            comment_change = 0;
            wprint_comment( comment_subwin,&tweets[current_tweet],current_comment);
        }
    }
}

void like_tweet(Tweet* tweet){
    char request[2*MAX];
    char response[2*MAX];
    sprintf(request,"like %s,%d\n",auth,tweet->id);
    send_request(request,strlen(request),response);
    tweet->likes++;
}


void comment_tweet(WINDOW* win,Tweet* tweet){
    Comment new_comment;
    wclear(win);
    mvwprintw(win,0,0,"Type Your Comment:");
    wmove(win,1,0);
    echo();
    curs_set(TRUE);
    wgetstr(win,new_comment.content);
    curs_set(FALSE);
    noecho();

    char request[3*MAX];
    char response[3*MAX];
    sprintf(request,"comment %s,%d,%s\n",auth,tweet->id,new_comment.content);
    send_request(request,strlen(request),response);

    strcpy(new_comment.author,username);
    add_comment(tweet,new_comment);
}

void press_key_to_continue(){
    printw("Press any key to continue...");
    getch();
}


int digit_count(int d){
    int s = 0;
    while(d){
        d/=10;
        s++;
    }
    return s;
}
char* itos(int d){
    int digs = digit_count(d);
    char* tmp = calloc(digs+1,sizeof(char));
    sprintf(tmp,"%d",d);
    return tmp;
}

#endif
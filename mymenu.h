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
#include"user.h"

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
void refresh_tweet();
void unfollow(User* user);
void follow(User* user);
void change_password_menu();
void set_bio_menu();
void search_menu();
void tweet_profile_menu();
void personal_area_menu();




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
    attron(COLOR_PAIR(TITLE_COLOR));
    printw("Sing up Page\n");
    attroff(COLOR_PAIR(TITLE_COLOR));
    printw("Enter Your Username:\n");
    echo();
    curs_set(TRUE);
    scanw("%s",username);
    curs_set(FALSE);
    noecho();
    printw("Enter Your Password:\n");
    scanw("%s",password);

    char* response;
    send_requestf(&response,"signup %s,%s\n",username,password);
    cJSON *json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;
    char* message = cJSON_GetObjectItem(json,"message")->valuestring;

    if(strcmp(type,"Successful")==0){
        attron(COLOR_PAIR(SUCCESS_COLOR));
        printw("\n Signed up Successfully...\n");
        attroff(COLOR_PAIR(SUCCESS_COLOR));
        press_key_to_continue();
        login_menu();
    }
    else{
        attron(COLOR_PAIR(ERROR_COLOR));
        printw("Error: %s\n",message);
        attroff(COLOR_PAIR(ERROR_COLOR));
        press_key_to_continue();
    }
    cJSON_Delete(json);
}

void login_menu(){
    clear();
    attron(COLOR_PAIR(TITLE_COLOR));
    printw("Login Page\n");
    attroff(COLOR_PAIR(TITLE_COLOR));
    printw("Enter Your Username:\n");
    echo();
    curs_set(TRUE);
    scanw("%s",username);
    noecho();
    curs_set(FALSE);
    printw("Enter Your Password:\n");
    scanw("%s",password);


    char* response; 
    send_requestf(&response,"login %s,%s\n",username,password);
    cJSON *json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;
    char* message = cJSON_GetObjectItem(json,"message")->valuestring;

    if(strcmp(type,"Token")==0){
        strcpy(auth,message);
        attron(COLOR_PAIR(SUCCESS_COLOR));
        printw("Logged in Successfully\n");
        attroff(COLOR_PAIR(SUCCESS_COLOR));
        press_key_to_continue();
        main_menu();
    }
    else{
        attron(COLOR_PAIR(ERROR_COLOR));
        printw("Error: %s\n",message);
        attroff(COLOR_PAIR(ERROR_COLOR));
        press_key_to_continue();
    }
    cJSON_Delete(json);
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


void search_menu(){
    char username_search[MAX];
    clear();
    attron(COLOR_PAIR(TITLE_COLOR));
    mvprintw(0,0,"Search User");
    attroff(COLOR_PAIR(TITLE_COLOR));
    mvprintw(1,0,"Type the username");
    move(2,0);
    curs_set(TRUE);
    echo();
    scanw("%s",username_search);
    noecho();
    curs_set(FALSE);
    char* response;
    send_requestf(&response,"search %s,%s\n",auth,username_search);
    cJSON* json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"Type")->valuestring;

    if(strcmp(type,"Profile")!=0){
        attron(COLOR_PAIR(ERROR_COLOR));
        mvprintw(LINES/2,4,cJSON_GetObjectItem(json,"message")->valuestring);
        attroff(COLOR_PAIR(ERROR_COLOR));
        move(LINES/2+1,4);
        press_key_to_continue();
        cJSON_Delete(json);
        return;
    }
    clear();
    int help_y = LINES - 3;
    mvprintw(help_y+0,0,"Use <LEFT><RIGHT> to change tweet");
    mvprintw(help_y+1,0,"Use <UP><DOWN> to change comment");
    mvprintw(help_y+2,0,"press u to unfollow,f to follow, and q to quit");
    refresh();
    User user = make_user_json(cJSON_GetObjectItem(json,"message"));
    
    int user_win_x = 0,
        user_win_y = 0;
    int user_win_h = 6,
        user_win_w = COLS;
    WINDOW* user_win = newwin(user_win_h,user_win_w,user_win_y,user_win_x);
    box(user_win,0,0);
    WINDOW* user_subwin = derwin(user_win,user_win_h-2,user_win_w-2,1,1);
    wrefresh(user_win);

    int tweet_win_x = 0,
        tweet_win_y = user_win_y + user_win_h;
    int tweet_win_h = (LINES - user_win_h-3)/2,
        tweet_win_w = COLS;
    WINDOW* tweet_win = newwin(tweet_win_h,tweet_win_w,tweet_win_y,tweet_win_x);
    keypad(tweet_win,TRUE);
    box(tweet_win,0,0);
    WINDOW* tweet_subwin = derwin(tweet_win,tweet_win_h-2,tweet_win_w-2,1,1);
    wrefresh(tweet_win);

    int comment_win_x = 0,
        comment_win_y = tweet_win_y + tweet_win_h;
    int comment_win_h = (LINES - user_win_h-3)/2,
        comment_win_w = COLS;
    WINDOW* comment_win = newwin(comment_win_h,comment_win_w,comment_win_y,comment_win_x);
    keypad(comment_win,TRUE);
    box(comment_win,0,0);
    WINDOW* comment_subwin = derwin(comment_win,comment_win_h-2,comment_win_w-2,1,1);
    wrefresh(comment_win);

    int current_tweet = 0;
    int current_comment = 0;
    int continue_running = 1;
    int tweet_change = 0;
    int user_change = 0;
    int comment_change = 0;
    wprint_user(user_subwin,user,1);
    wprint_tweet(tweet_subwin,user.tweets+current_tweet);
    wprint_comment(comment_subwin,user.tweets+current_tweet,current_comment);
    int ch;
    while(continue_running && (ch=wgetch(tweet_win))){
        switch (ch)
        {
        case KEY_LEFT:
            if(current_tweet-1>=0){
                tweet_change = 1;
                comment_change = 1;
                current_tweet--;
            }
            else{
                beep();
            }
            break;
        case KEY_RIGHT:
            if(current_tweet+1<user.tweets_number){
                tweet_change = 1;
                comment_change = 1;
                current_tweet++;
            }
            else{
                beep();
            }
            break;
        case KEY_UP:
            if(current_comment-1>=0){
                comment_change = 1;
                current_comment--;
            }
            else{
                beep();
            }
            break;
        case KEY_DOWN:
            if(user.tweets+current_tweet!=NULL && current_comment+1<user.tweets[current_tweet].comment_number){
                current_comment++;
                comment_change = 1;
            }
            else{
                beep();
            }
            break;
        case 'f':
            follow(&user);
            user_change = 1;
            break;
        case 'u':
            unfollow(&user);
            user_change = 1;
            break;
        case 'q':
            continue_running = 0;
            break;
        }
        if(user_change){
            user_change = 0;
            wprint_user(user_subwin,user,1);
        }
        if(tweet_change){
            tweet_change = 0;
            current_comment = 0;
            wprint_tweet(tweet_subwin,user.tweets+current_tweet);
        }
        if(comment_change){
            comment_change = 0;
            wprint_comment(comment_subwin,user.tweets+current_tweet,current_comment);
        }
    }
    cJSON_Delete(json);
    delwin(user_subwin);
    delwin(user_win);
    delwin(tweet_subwin);
    delwin(tweet_win);
    delwin(comment_subwin);
    delwin(comment_win);
    free_user(user);
}

void follow(User* user){
    char* response;
    send_requestf(&response,"follow %s,%s\n",auth,user->username);
    free(response);
    user->following_status = 1;
}

void unfollow(User* user){
    char* response;
    send_requestf(&response,"unfollow %s,%s\n",auth,user->username);
    free(response);
    user->following_status = 0;
}
//TODO
void tweet_profile_menu(){
    char* response;
    send_requestf(&response,"profile %s\n",auth);
    cJSON* json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"Type")->valuestring;

    if(strcmp(type,"Profile")!=0){
        attron(COLOR_PAIR(ERROR_COLOR));
        mvprintw(LINES/2,4,"%s",cJSON_GetObjectItem(json,"message"));
        attroff(COLOR_PAIR(ERROR_COLOR));
        press_key_to_continue();
        cJSON_Delete(json);
        return;
    }
    clear();
    int help_y = LINES - 3;
    mvprintw(help_y+0,0,"Use <LEFT><RIGHT> to change tweet");
    mvprintw(help_y+1,0,"Use <UP><DOWN> to change comment");
    mvprintw(help_y+2,0,"press q to quit");
    refresh();
    User user = make_user_json(cJSON_GetObjectItem(json,"message"));
    
    int user_win_x = 0,
        user_win_y = 0;
    int user_win_h = 6,
        user_win_w = COLS;
    WINDOW* user_win = newwin(user_win_h,user_win_w,user_win_y,user_win_x);
    box(user_win,0,0);
    WINDOW* user_subwin = derwin(user_win,user_win_h-2,user_win_w-2,1,1);
    wrefresh(user_win);

    int tweet_win_x = 0,
        tweet_win_y = user_win_y + user_win_h;
    int tweet_win_h = (LINES - user_win_h-3)/2,
        tweet_win_w = COLS;
    WINDOW* tweet_win = newwin(tweet_win_h,tweet_win_w,tweet_win_y,tweet_win_x);
    keypad(tweet_win,TRUE);
    box(tweet_win,0,0);
    WINDOW* tweet_subwin = derwin(tweet_win,tweet_win_h-2,tweet_win_w-2,1,1);
    wrefresh(tweet_win);

    int comment_win_x = 0,
        comment_win_y = tweet_win_y + tweet_win_h;
    int comment_win_h = (LINES - user_win_h-3)/2,
        comment_win_w = COLS;
    WINDOW* comment_win = newwin(comment_win_h,comment_win_w,comment_win_y,comment_win_x);
    keypad(comment_win,TRUE);
    box(comment_win,0,0);
    WINDOW* comment_subwin = derwin(comment_win,comment_win_h-2,comment_win_w-2,1,1);
    wrefresh(comment_win);

    int current_tweet = 0;
    int current_comment = 0;
    int continue_running = 1;
    int tweet_change = 0;
    int user_change = 0;
    int comment_change = 0;
    wprint_user(user_subwin,user,0);
    wprint_tweet(tweet_subwin,user.tweets+current_tweet);
    wprint_comment(comment_subwin,user.tweets+current_tweet,current_comment);
    int ch;
    while(continue_running && (ch=wgetch(tweet_win))){
        switch (ch)
        {
        case KEY_LEFT:
            if(current_tweet-1>=0){
                tweet_change = 1;
                comment_change = 1;
                current_tweet--;
            }
            else{
                beep();
            }
            break;
        case KEY_RIGHT:
            if(current_tweet+1<user.tweets_number){
                tweet_change = 1;
                comment_change = 1;
                current_tweet++;
            }
            else{
                beep();
            }
            break;
        case KEY_UP:
            if(current_comment-1>=0){
                comment_change = 1;
                current_comment--;
            }
            else{
                beep();
            }
            break;
        case KEY_DOWN:
            if(user.tweets+current_tweet!=NULL && current_comment+1<user.tweets[current_tweet].comment_number){
                current_comment++;
                comment_change = 1;
            }
            else{
                beep();
            }
            break;
        case 'q':
            continue_running = 0;
            break;
        }
        if(user_change){
            user_change = 0;
            wprint_user(user_subwin,user,0);
        }
        if(tweet_change){
            tweet_change = 0;
            current_comment = 0;
            wprint_tweet(tweet_subwin,user.tweets+current_tweet);
        }
        if(comment_change){
            comment_change = 0;
            wprint_comment(comment_subwin,user.tweets+current_tweet,current_comment);
        }
    }
    cJSON_Delete(json);
    free_user(user);
    delwin(user_subwin);
    delwin(user_win);
    delwin(tweet_subwin);
    delwin(tweet_win);
    delwin(comment_subwin);
    delwin(comment_win);
}
//TODO
void personal_area_menu(){
    const char* choises[] = {
        "set bio",
        "change password",
        "back"
    };
    int number_choices = ARRAY_SIZE(choises);
    ITEM** my_items = (ITEM**)calloc(number_choices+1,sizeof(ITEM*));
    for(int i=0;i<number_choices;i++){
        my_items[i] = new_item(choises[i],"");
    }
    MENU* my_menu = new_menu(my_items);

    int repeat = 1;
    while(repeat){
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
            set_bio_menu();
            break;
        case 1:
            change_password_menu();
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

void set_bio_menu(){
    char bio[MAX];
    attron(COLOR_PAIR(TITLE_COLOR));
    mvprintw(0,0,"Setting bio");
    attroff(COLOR_PAIR(TITLE_COLOR));
    mvprintw(1,0,"Enter bio:");
    move(2,0);
    curs_set(1);
    echo();
    getstr(bio);
    noecho();
    curs_set(0);

    char* response;
    send_requestf(&response,"set bio %s,%s\n",auth,bio);
    cJSON* json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;
    move(3,0);
    if(strcmp(type,"Error")==0){
        attron(COLOR_PAIR(ERROR_COLOR));
    }
    else{
        attron(COLOR_PAIR(SUCCESS_COLOR));
    }
    printw("%s",cJSON_GetObjectItem(json,"message")->valuestring);
    if(strcmp(type,"Error")==0){
        attroff(COLOR_PAIR(ERROR_COLOR));
    }
    else{
        attroff(COLOR_PAIR(SUCCESS_COLOR));
    }
    move(4,0);
    press_key_to_continue();
    cJSON_Delete(json);
}

void change_password_menu(){
    char new_password[MAX];
    char curr_password[MAX];
    attron(COLOR_PAIR(TITLE_COLOR));
    mvprintw(0,0,"Changing password");
    attroff(COLOR_PAIR(TITLE_COLOR));
    mvprintw(1,0,"Enter current password:");
    move(2,0);
    scanw("%s",curr_password);
    mvprintw(3,0,"Enter new password:");
    move(4,0);
    scanw("%s",new_password);

    char* response;
    send_requestf(&response,"change password %s,%s,%s\n",auth,curr_password,new_password);
    cJSON* json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;
    if(strcmp(type,"Error")==0){
        attron(COLOR_PAIR(ERROR_COLOR));
    }
    else{
        attron(COLOR_PAIR(SUCCESS_COLOR));
    }
    mvprintw(5,0,cJSON_GetObjectItem(json,"message")->valuestring);
    if(strcmp(type,"Error")==0){
        attroff(COLOR_PAIR(ERROR_COLOR));
    }
    else{
        attroff(COLOR_PAIR(SUCCESS_COLOR));
    }
    move(6,0);
    press_key_to_continue();
    cJSON_Delete(json);
}

void logout(){
    char* respons;
    send_requestf(&respons,"logout %s\n",auth);
    free(respons);
    clear();
    attron(COLOR_PAIR(SUCCESS_COLOR));
    printw("Logged out\n");
    attroff(COLOR_PAIR(SUCCESS_COLOR));
    press_key_to_continue();
}


void send_tweet(){
    char tweet[MAX];
    
    clear();
    attron(COLOR_PAIR(TITLE_COLOR));
    printw("Send Tweet\n");
    attroff(COLOR_PAIR(TITLE_COLOR));
    printw("Type your tweet:\n");
    echo();
    curs_set(TRUE);
    getstr(tweet);
    curs_set(FALSE);
    noecho();

    char* respons;
    send_requestf(&respons,"send tweet %s,%s\n",auth,tweet);
    free(respons);
    attron(COLOR_PAIR(SUCCESS_COLOR));
    printw("tweet sent\n");
    attroff(COLOR_PAIR(SUCCESS_COLOR));
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


void refresh_tweet(){
    char* response;
    send_requestf(&response,"refresh %s\n",auth);
    refresh();
    cJSON *json = cJSON_Parse(response);
    free(response);
    char* type = cJSON_GetObjectItem(json,"type")->valuestring;

    if(strcmp(type,"List")!=0){
        attron(COLOR_PAIR(ERROR_COLOR));
        printw("Error accured\n");
        attroff(COLOR_PAIR(ERROR_COLOR));
        press_key_to_continue();
        cJSON_Delete(json);
        return;
    }
    cJSON* json_tweets = cJSON_GetObjectItem(json,"message");
    int tweets_number = cJSON_GetArraySize(json_tweets);
    Tweet* tweets = make_tweet_array_json(json_tweets);
    cJSON_Delete(json);
    clear();

    if(tweets_number<1){
        attron(COLOR_PAIR(ERROR_COLOR));
        mvprintw(LINES/2 ,4,"NO new tweets :(");
        attroff(COLOR_PAIR(ERROR_COLOR));
        move(LINES/2+1,4);
        press_key_to_continue();
        free(tweets);
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
    tweet_win_h = (LINES - tweets_id_win_h - 3)/2;
    tweet_win_w = COLS;

    int comment_win_y , comment_win_x;
    comment_win_y = tweet_win_y + tweet_win_h;
    comment_win_x = 0;
    int comment_win_h , comment_win_w;
    comment_win_h = (LINES - tweets_id_win_h - 3)/2;
    comment_win_w = COLS;


    int help_y = LINES - 3;
    mvprintw(help_y+0,0,"Use <LEFT><RIGHT> to change tweet");
    mvprintw(help_y+1,0,"Use <UP><DOWN> to change comment");
    mvprintw(help_y+2,0,"press l to like,c to comment, and q to quit");
    refresh();
    WINDOW* tweets_id_win = newwin( tweets_id_win_h,tweets_id_win_w,
                                    tweets_id_win_y,tweets_id_win_x);
    WINDOW* tweets_id_subwin = derwin(tweets_id_win,tweets_id_win_h-2,tweets_id_win_w-2,1,1);
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
    set_menu_sub(tweets_id_menu,tweets_id_subwin);
    set_menu_format(tweets_id_menu,1,COLS/6);
    post_menu(tweets_id_menu);
    wrefresh(tweets_id_win);

    int current_tweet = 0;
    int current_comment = 0;
    int tweet_change = 0;
    int comment_change = 0;
    int continue_runnig = 1;
    refresh();
    wprint_tweet(tweet_subwin,tweets+current_tweet);
    wprint_comment( comment_subwin,&tweets[current_tweet],current_comment);
    int ch=1;
    while(continue_runnig && (ch=wgetch(tweets_id_win))){
        switch (ch)
        {
        case KEY_RIGHT:
            if(menu_driver(tweets_id_menu,REQ_RIGHT_ITEM)==E_OK){
                tweet_change = 1;
                comment_change = 1;
                current_comment = 0;
            }
            else{
                beep();
            }
            break;
        case KEY_LEFT:
            if(menu_driver(tweets_id_menu,REQ_LEFT_ITEM)==E_OK){
                tweet_change = 1;
                comment_change = 1;
                current_comment = 0;
            }
            else{
                beep();
            }
            break;
        case KEY_UP:
            if(current_comment-1>=0){
                current_comment--;
                comment_change = 1;
            }
            else{
                beep();
            }
            break;
        case KEY_DOWN:
            if(current_comment+1<tweets[current_tweet].comment_number){
                current_comment++;
                comment_change = 1;
            }
            else{
                beep();
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
            wprint_tweet(tweet_subwin,tweets+current_tweet);
        }
        if(comment_change){
            comment_change = 0;
            wprint_comment( comment_subwin,&tweets[current_tweet],current_comment);
        }
    }
    for(int i=0;i<tweets_number;i++){
        free_tweet(tweets[i]);
    }
    for(int i=0;i<tweets_number;i++){
        free_item(tweets_id_items[i]);
    }
    free_menu(tweets_id_menu);
    free(tweets_id_items);

    free(tweets);

    delwin(tweets_id_subwin);
    delwin(tweets_id_win);
    delwin(tweet_subwin);
    delwin(tweet_win);
    delwin(comment_subwin);
    delwin(comment_win);
}

void like_tweet(Tweet* tweet){
    char* response;
    send_requestf(&response,"like %s,%d\n",auth,tweet->id);
    free(response);
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

    char* response;
    send_requestf(&response,"comment %s,%d,%s\n",auth,tweet->id,new_comment.content);
    free(response);
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
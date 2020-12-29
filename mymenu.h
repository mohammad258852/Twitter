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

void start_menu();
void signup_menu();
void login_menu();
void main_menu();
void timeline_menu();
void logout();
void send_tweet();
void press_key_to_continue();


//TODO
void search_menu();
void tweet_profile_menu();
void personal_area_menu();
void refresh_tweet();
void like_menu();
void comment_menu();
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
    char username[MAX];
    char password[MAX];

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

    for(int i=0;i<tweets_number;i++){
        print_tweet(tweets[i]);
    }
    //TODO
    //comment or like the post

    const char * const items_name[] = {
        "Like",
        "Comment",
        "Back"
    };
    int number_choices = ARRAY_SIZE(items_name);
    ITEM** my_items = (ITEM**)calloc(number_choices+1,sizeof(ITEM*));
    for(int i=0;i<number_choices;i++){
        my_items[i] = new_item(items_name[i],"");
    }
    refresh();
    MENU* 	my_menu = new_menu((ITEM **)my_items);
    WINDOW* my_menu_win = my_menu_win = subwin(stdscr,10, 40, 4, 4);
    keypad(my_menu_win,TRUE);
    box(my_menu_win,0,0);
    wrefresh(my_menu_win);
    set_menu_win(my_menu, my_menu_win);
    set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));
    int repeat = 1;
    while (repeat)
    {
        post_menu(my_menu);
        wrefresh(my_menu_win);
        //mvprintw(LINES-2,0,"Use Arow key to move and Use Enter to Select");
        //refresh();
        int ch;
        int choise = -1;
        while(choise==-1 && (ch=wgetch(my_menu_win))){
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
            like_menu();
            break;
        case 1:
            comment_menu();
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
void like_menu(){

}

//TODO
void comment_menu(){

}

void press_key_to_continue(){
    printw("Press any key to continue...\n");
    getch();
}

#endif
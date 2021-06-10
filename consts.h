#ifndef CONSTS
#define CONSTS

#define MAXUSERNAME 16
#define MAXPASSWORD 16
#define MAXBIO 70
#define MAXTEXT 220
#define MAXBUF 1024
#define MAXHASHTAG 30
#define TOKENSIZE 20
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define IP "127.0.0.2"
#define PORT 12345

char auth[TOKENSIZE+1];
char username[MAXUSERNAME+1];
char password[MAXPASSWORD+1];
int server_status = 1;

enum {
    ERROR_COLOR=1,TITLE_COLOR,SUCCESS_COLOR,TWEET_COLOR,COMMENT_COLOR,RETWEETER_COLOR
};

#endif
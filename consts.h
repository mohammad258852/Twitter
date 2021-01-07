#ifndef CONSTS
#define CONSTS

#define MAX 2024
#define ARRAY_SIZE(a) (sizeof(a)/sizeof(a[0]))

#define IP "127.0.0.2"
#define PORT 12345

char auth[MAX];
char username[MAX];
char password[MAX];
int server_status = 1;

enum {
    ERROR_COLOR=1,TITLE_COLOR,SUCCESS_COLOR
};

#endif
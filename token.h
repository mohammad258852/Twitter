#ifndef TOKENH
#define TOKENH

#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"consts.h"

typedef struct
{
    char tok[TOKENSIZE+1];
    char username[MAXUSERNAME];
}Token;

Token* tokens = NULL;
size_t token_number = 0;

//if valid returns the token otherwise return NULL
Token* validate_token(const char* tok){
    for(int i=0;i<token_number;i++){
        if(strcmp(tok,(tokens+i)->tok) == 0){
            return tokens + i;
        }
    }
    return NULL;
}

char random_char(){
    srand((unsigned int)time(NULL));
    int x = rand()%36;
    if(x < 26)
        return 'a'+x;
    else
        return '0' + (x-26);
}

void random_token(Token* t){
    int valid;
    do{
        for(int i=0;i<TOKENSIZE;i++){
            t->tok[i] = random_char();
        }
        valid = 1;
        for(int i=0;i<token_number;i++){
            if(strcmp(t->tok,(tokens+i)->tok)==0){
                valid = 0;
                break;
            }
        }
    }while(!valid);
}

void add_user_token(const char* const username){
    Token t;
    random_token(&t);
    strcpy(t.username,username);
    tokens = (Token*)realloc(tokens,(token_number+1)*sizeof(Token));
    *(tokens+token_number) = t;
    token_number++;
}

#endif
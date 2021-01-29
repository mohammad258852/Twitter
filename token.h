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

typedef struct __tokenlist{
    Token tok;
    struct __tokenlist* next;
} TokenList;

TokenList* list = NULL;

//if valid returns the token otherwise return NULL
Token* validate_token(const char* tok){
    for(TokenList* cur=list;cur!=NULL; cur = cur->next){
        if(strcmp(tok,cur->tok.tok) == 0){
            return &(cur->tok);
        }
    }
    return NULL;
}

int check_username_login(const char* username){
    for(TokenList* cur=list;cur!=NULL; cur = cur->next){
        if(strcmp(username,cur->tok.username) == 0){
            return 1;
        }
    }
    return 0;
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
        if(validate_token(t->tok)!=NULL){
            valid = 0;
        }
    }while(!valid);
}

Token* add_user_token(const char* const username){
    Token t;
    random_token(&t);
    strcpy(t.username,username);

    if(list==NULL){
        list = calloc(1,sizeof(TokenList));
        list->tok = t;
        list->next = NULL;
        return &(list->tok);
    }
    else{
        TokenList* cur = list;
        while(cur->next!=NULL){
            cur = cur->next;
        }
        cur->next = calloc(1,sizeof(TokenList));
        cur->next->tok = t;
        cur->next->next = NULL;
        return &(cur->next->tok);
    }
}

#endif
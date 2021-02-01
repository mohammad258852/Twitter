#ifndef USERLISTH
#define USERLISTH

#include<string.h>
#include<stdlib.h>
#include"cJSON.h"
#include"consts.h"

typedef struct __userlist{
    char username[MAXUSERNAME];
    struct __userlist* next;
} UserList ;

UserList* make_user_list(cJSON* json);
void free_userlist(UserList* list);
int count_userlist(UserList* list);

UserList* make_user_list(cJSON* json){
    UserList* first = NULL;
    UserList** cur = &first;
    UserList** pre = NULL;
    for(cJSON* i=json->child;i!=NULL;i = i->next){
        *cur = calloc(1,sizeof(UserList));
        strcpy((*cur)->username,i->valuestring);
        (*cur)->next = NULL;
        if(pre!=NULL){
            (*pre)->next = *cur;
        }
        pre = cur;
        cur = &((*pre)->next);
    }
    return first;
}

void free_userlist(UserList* list){
    while(list!=NULL){
        UserList* next = list->next;
        free(list);
        list = next;
    }
}

int count_userlist(UserList* list){
    int n = 0;
    while(list!=NULL){
        n++;
        list = list->next;
    }
    return n;
}

#endif
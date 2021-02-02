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
void add_user_to_list(UserList** list,const char* username);
void delete_user_from_list(UserList** list,const char* username);
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

void add_user_to_list(UserList** list,const char* username){
    if(*list==NULL){
        *list = calloc(1,sizeof(UserList));
        strcpy((*list)->username,username);
        (*list)->next = NULL;
        return;
    }
    while((*list)->next!=NULL){
        list = &((*list)->next);
    }
    (*list)->next = calloc(1,sizeof(UserList));
    strcpy((*list)->next->username,username);
    (*list)->next->next = NULL;
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

void delete_user_from_list(UserList** list,const char* username){
    while((*list)!=NULL){
        if(strcmp((*list)->username,username)==0){
            UserList* tmp = (*list);
            (*list) = tmp->next;
            free(tmp);
            return;
        }
        if((*list)->next==NULL)
            return;
        list = &((*list)->next);
    }
}

#endif
#ifndef COMMENTLISTH
#define COMMENTLISTH

#include"consts.h"
#include"comment.h"

typedef struct __commentlist
{
    Comment comment;
    struct __commentlist* next;
} CommentList;

void add_comment_to_list(CommentList**,const Comment* comment);

void add_comment_to_list(CommentList** list,const Comment* comment){
    if(*list==NULL){
        *list = calloc(1,sizeof(CommentList));
        (*list)->comment = *comment;
        (*list)->next = NULL;
        return;
    }
    while((*list)->next!=NULL){
        list = &((*list)->next);
    }
    (*list)->next = calloc(1,sizeof(CommentList));
    (*list)->next->comment = *comment;
    (*list)->next->next = NULL;
}

#endif
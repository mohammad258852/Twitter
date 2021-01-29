#ifndef COMMENTLISTH
#define COMMENTLISTH

#include"consts.h"
#include"comment.h"

typedef struct __commentlist
{
    Comment comment;
    struct __commentlist* next;
} CommentList;


#endif
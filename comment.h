#ifndef COMMENT
#define COMMENT

#include<stdlib.h>
#include"consts.h"
#include"cJSON.h"

typedef
struct __comment
{
    char author[MAX];
    char content[MAX];
}Comment;

Comment make_comment_json(cJSON* json){
    
}

#endif
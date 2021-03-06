#ifndef COMMENT
#define COMMENT

#include<stdlib.h>
#include"consts.h"
#include"cJSON.h"

typedef
struct __comment
{
    char author[MAXUSERNAME+1];
    char content[MAXTEXT+1];
}Comment;

Comment make_comment_json(cJSON* json){
    Comment tmp;
    strcpy(tmp.author,json->string);
    strcpy(tmp.content,json->valuestring);
    return tmp;
}

#endif
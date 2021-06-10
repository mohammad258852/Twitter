#ifndef COMMENTH
#define COMMENTH

#include"cJSON.h"
#include"consts.h"

typedef struct{
    char author[MAXUSERNAME+1];
    char content[MAXTEX+1];
} Comment;

cJSON* comment2json(const Comment*);
Comment json2comment(cJSON*);

cJSON* comment2json(const Comment* comment){
    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json,comment->author,cJSON_CreateString(comment->content));
    return json;
}

Comment json2comment(cJSON* json){
    Comment comment;
    strcpy(comment.author,json->string);
    strcpy(comment.content,json->valuestring);
    return comment;
}

#endif
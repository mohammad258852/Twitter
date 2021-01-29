#ifndef COMMENTH
#define COMMENTH

#include"cJSON.h"
#include"consts.h"

typedef struct{
    char author[MAXUSERNAME];
    char content[MAXTEX];
} Comment;

cJSON* comment2json(const Comment* comment){
    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json,comment->author,cJSON_CreateString(comment->content));
    return json;
}

#endif
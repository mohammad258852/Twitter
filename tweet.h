#ifndef TWEETH
#define TWEETH

#include<string.h>
#include"cJSON.h"
#include"consts.h"
#include"comment.h"
#include"commentlist.h"

typedef struct 
{
    int id;
    char author[MAXUSERNAME];
    char content[MAXTEX];
    unsigned likes;
    CommentList* comments;
} Tweet;

int add_tweet(const char*,const char*);
int tweet_exist(int);
int write_tweet(const Tweet*);
Tweet read_tweet(int);
cJSON* read_tweet_json(int);
cJSON* tweet2json(const Tweet*);
Tweet json2tweet(cJSON*);
int is_user_read_tweet(const char*,int);
void user_read_tweet(const char*,int);
void delete_tweet_readers(int);
void sort_tweet(int*,size_t);
int is_user_like_tweet(const char*,int);
void user_like_tweet(const char*,int);
void user_unlike_tweet(const char*,int);
void add_comment_to_tweet(int,const char*,const char*);


int tweet_exist(int id){
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",id);

    if(access(path,F_OK)==0){
        return 1;
    }
    return 0;
}

cJSON* tweet2json(const Tweet* tweet){
    cJSON* json = cJSON_CreateObject();
    cJSON_AddItemToObject(json,"id",cJSON_CreateNumber(tweet->id));
    cJSON_AddItemToObject(json,"author",cJSON_CreateString(tweet->author));
    cJSON_AddItemToObject(json,"content",cJSON_CreateString(tweet->content));
    cJSON_AddItemToObject(json,"likes",cJSON_CreateNumber(tweet->likes));

    cJSON* comments_json = cJSON_CreateArray();
    for(CommentList* i=tweet->comments; i!=NULL; i=i->next){
        cJSON_AddItemToArray(comments_json,comment2json(&(i->comment)));
    }
    cJSON_AddItemToObject(json,"comments",comments_json);
    return json;
}

Tweet json2tweet(cJSON* json){
    Tweet tweet;
    tweet.id = cJSON_GetObjectItem(json,"id")->valueint;
    strcpy(tweet.author,cJSON_GetObjectItem(json,"author")->valuestring);
    strcpy(tweet.content,cJSON_GetObjectItem(json,"content")->valuestring);
    tweet.likes = cJSON_GetObjectItem(json,"likes")->valueint;
    tweet.comments = NULL;
    CommentList** cur = &tweet.comments;
    CommentList** prev = NULL;
    for(cJSON* i=cJSON_GetObjectItem(json,"comments")->child;i!=NULL;i=i->next){
        (*cur) = calloc(1,sizeof(CommentList));
        (*cur)->comment = json2comment(i->child);
        (*cur)->next = NULL;
        if(prev != NULL){
            (*prev)->next = *cur;
        }
        prev = cur;
        cur = &((*prev)->next);
    }
    return tweet;
}

int write_tweet(const Tweet* tweet){
    system("mkdir -p "TWEETPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",tweet->id);
    FILE* file = fopen(path,"w");
    if(file==NULL){
        return 0;
    }
    cJSON* tweet_json = tweet2json(tweet);
    fputs(cJSON_PrintUnformatted(tweet_json),file);
    fclose(file);
    cJSON_Delete(tweet_json);
    return 1;
}

Tweet read_tweet(int id){
    Tweet tweet;
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",id);
    FILE* file = fopen(path,"r");
    if(file==NULL){
        return tweet;
    }
    size_t size;
    fseek(file,0,SEEK_END);
    size = ftell(file);
    fseek(file,0,SEEK_SET);
    char* content = malloc(size+1);
    fgets(content,size+1,file);
    fclose(file);
    cJSON* json = cJSON_Parse(content);
    tweet = json2tweet(json);
    free(content);
    cJSON_Delete(json);
    return tweet;
}

cJSON* read_tweet_json(int id){
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETPATH"%d.json",id);
    FILE* file = fopen(path,"r");
    if(file==NULL){
        return NULL;
    }
    size_t size;
    fseek(file,0,SEEK_END);
    size = ftell(file);
    fseek(file,0,SEEK_SET);
    char* content = malloc(size+1);
    fgets(content,size+1,file);
    fclose(file);
    cJSON* json = cJSON_Parse(content);
    free(content);
    return json;
}

int add_tweet(const char* username,const char* content){
    Tweet tmp;
    tmp.id = 1;
    while(tweet_exist(tmp.id)){
        tmp.id++;
    }
    strcpy(tmp.author,username);
    strcpy(tmp.content,content);
    tmp.likes = 0;
    tmp.comments = NULL;

    if(write_tweet(&tmp)==0)
        return 0;
    return tmp.id;
}

int is_user_read_tweet(const char* username,int id){
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETREADPATH"%d.txt",id);
    char user[MAXUSERNAME];
    FILE* file = fopen(path,"r");
    if(file==NULL){
        return 0;
    }
    while(fscanf(file,"%s",user)!=EOF){
        if(strcmp(username,user)==0){
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void user_read_tweet(const char* username,int id){
    system("mkdir -p "TWEETREADPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETREADPATH"%d.txt",id);
    FILE* file = fopen(path,"a");
    if(file==NULL){
        return;
    }
    fprintf(file,"%s\n",username);
    fclose(file);
    return;
}

void delete_tweet_readers(int id){
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETREADPATH"%d.txt",id);
    if(access(path,F_OK)!=0){
        return;
    }
    remove(path);
}

int tweet_cmp(const void* x,const void* y){
    return (*(int*)x - *(int*)y);
}

void sort_tweet(int* ids,size_t n){
    qsort(ids,n,sizeof(int),tweet_cmp);
}

int is_user_like_tweet(const char* username,int id){
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETLIKEPATH"%d.txt",id);
    char user[MAXUSERNAME];
    FILE* file = fopen(path,"r");
    if(file==NULL){
        return 0;
    }
    while(fscanf(file,"%s",user)!=EOF){
        if(strcmp(username,user)==0){
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void user_like_tweet(const char* username,int id){
    system("mkdir -p "TWEETLIKEPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETLIKEPATH"%d.txt",id);
    FILE* file = fopen(path,"a");
    if(file==NULL){
        return;
    }
    fprintf(file,"%s\n",username);
    fclose(file);
    Tweet tweet = read_tweet(id);
    tweet.likes++;
    write_tweet(&tweet);
    delete_tweet_readers(id);
    user_read_tweet(username,id);
    return;
}

void user_unlike_tweet(const char* username,int id){
    system("mkdir -p "TWEETLIKEPATH);
    char path[MAXFILENAMESIZE];
    sprintf(path,TWEETLIKEPATH"%d.txt",id);
    FILE* file = fopen(path,"r");
    FILE* tmp_file = fopen(TWEETLIKEPATH"tmp.txt","w");
    if(file==NULL || tmp_file==NULL){
        return;
    }
    char user[MAXUSERNAME];
    while(fscanf(file,"%s",user)!=EOF){
        if(strcmp(username,user)==0){
            continue;
        }
        fprintf(tmp_file,"%s\n",user);
    }
    fclose(file);
    fclose(tmp_file);
    remove(path);
    rename(TWEETLIKEPATH"tmp.txt",path);
    Tweet tweet = read_tweet(id);
    tweet.likes--;
    write_tweet(&tweet);
    delete_tweet_readers(id);
    user_read_tweet(username,id);
    return;
}
void add_comment_to_tweet(int id,const char* author,const char* content){
    Tweet tweet = read_tweet(id);
    Comment comm;
    strcpy(comm.author,author);
    strcpy(comm.content,content);
    add_comment_to_list(&tweet.comments,&comm);
    write_tweet(&tweet);
    delete_tweet_readers(id);
    user_read_tweet(author,id);
}

#endif
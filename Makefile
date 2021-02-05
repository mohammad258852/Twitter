all:main
	./main

main:main.c consts.h mysocket.h process.h utility.h token.h user.h tweet.h tweetlist.h userlist.h comment.h commentlist.h idlist.h cJSON.o
	gcc cJSON.o main.c -o main

cJSON.o:cJSON.c
	gcc -c cJSON.c -o cJSON.o

clean:
	rm -R "Resource/TweetsRead/"
	rm -R "Resource/TweetsLike/"
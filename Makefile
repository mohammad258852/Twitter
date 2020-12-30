all:main
	./main

main: main.c mymenu.h mysocket.h consts.h cJSON.o tweet.h comment.h
	gcc cJSON.o main.c -o main -lncurses -lmenu

cJSON.o::
	gcc -c cJSON.c

clean:
	rm main

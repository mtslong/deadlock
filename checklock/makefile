#make file demo

CFLAGS = -Wall -g -p -pg -lpthread -finstrument-functions -rdynamic -ldl

.PHONY:all

all:
	cc $(CFLAGS) deadlock.c checklock.c -o deadlock 


.PHONY: clean
clean:
	-rm deadlock 
	-rm trace*.txt

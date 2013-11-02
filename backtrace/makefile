#make file demo

OBJS = backtrace 
#CFLAGS = -Wall -g -p -pg -lpthread -finstrument-functions instrument.c -rdynamic -ldl
CFLAGS = -Wall -g -pg -lpthread -rdynamic -ldl

.PHONY:all

all:backtrace demo_stack

backtrace:backtrace.c
	cc $(CFLAGS) backtrace.c -o backtrace 

demo_stack:demo_stack.c
	cc $(CFLAGS) demo_stack.c -o demo_stack 

.PHONY: clean
clean:
	-rm backtrace
	-rm demo_stack

# Christopher Rung

CC = gcc
FLAGS = -std=gnu99 -Wall -g #-DDEBUG

EXECS = myshell

myshell: myshell.c
	$(CC) $(FLAGS) myshell.c -o $(EXECS)
	
.PHONY: clean
clean:
	/bin/rm -rf *~ myshell

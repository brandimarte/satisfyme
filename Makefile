# ******************************************************* #
#                                                         #
# Makefile for gcc compiler.                              #
#                                                         #
#  *****************************************************  #

CFLAGS = -g -I. -Wall -pedantic -ansi
LDFLAGS = -L.
LDLIBS = -lm 

RM = /bin/rm -f
CC = gcc

#  *****************************************************  #

.PHONY: all clean

#  *****************************************************  #

.c.o:
	$(CC) $(CFLAGS) -c $*.c

.c:
	make $*.o
	$(CC) $(CFLAGS) -o $* $*.o $(LDLIBS) 

#  *****************************************************  #

all: satisfyme

satisfyme: CP.o ksat.o
	$(CC) $(CFLAGS) -o satisfyme CP.o ksat.o $(LDLIBS) 

#  *****************************************************  #

clean:
	$(RM) *~ \#~ .\#* *.o satisfyme core a.out


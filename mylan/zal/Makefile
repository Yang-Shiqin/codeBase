CC = gcc
TARGET = zal
CFLAGS = -c -g -Wall -Wswitch-enum -pedantic 
OBJS = lex.yy.o\
  y.tab.o\
  main.o\
  interface.o\
  create.o\
  execute.o\
  eval.o\
  stack.o\
  string.o\
  util.o\
  native.o\
  heap.o\
  error.o\
  ./MEM/mem.o\

INCLUDES = \

$(TARGET):$(OBJS)
	cd ./MEM; $(MAKE);
	$(CC) $(OBJS) -o $@ -lm
clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~
y.tab.h : zal.y
	yacc -dv zal.y
y.tab.c : error.c zal.y
	yacc -dv zal.y
lex.yy.c : zal.l zal.y y.tab.h
	lex zal.l
y.tab.o: y.tab.c zal_in.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o: lex.yy.c zal_in.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
./MEM/mem.o:
	cd ./MEM; $(MAKE);
############################################################
create.o: create.c MEM.h  zal_in.h ZAL.h 
eval.o: stack.c eval.c MEM.h zal_in.h ZAL.h 
execute.o: execute.c MEM.h  zal_in.h ZAL.h 
interface.o: interface.c MEM.h  zal_in.h ZAL.h
main.o: main.c ZAL.h MEM.h
string.o: string.c MEM.h zal_in.h ZAL.h
util.o: util.c MEM.h  zal_in.h ZAL.h 
native.o: native.c zal_in.h ZAL.h MEM.h
heap.o: heap.c zal_in.h ZAL.h MEM.h
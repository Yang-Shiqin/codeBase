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
  err_msg.o\
  ./MEM/mem.o\
  ./DBG/dbg.o

INCLUDES = \

$(TARGET):$(OBJS)
	cd ./MEM; $(MAKE);
	cd ./DBG; $(MAKE);
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
./DBG/dbg.o:
	cd ./DBG; $(MAKE);
############################################################
create.o: create.c MEM.h  zal_in.h ZAL.h  DBG.h
eval.o: stack.c eval.c MEM.h zal_in.h ZAL.h  DBG.h
execute.o: execute.c MEM.h  zal_in.h ZAL.h  DBG.h
interface.o: interface.c MEM.h  zal_in.h ZAL.h DBG.h
main.o: main.c ZAL.h MEM.h DBG.h
string.o: string.c MEM.h zal_in.h ZAL.h DBG.h
util.o: util.c MEM.h  zal_in.h ZAL.h  DBG.h
native.o: native.c zal_in.h ZAL.h MEM.h DBG.h
heap.o: heap.c zal_in.h ZAL.h MEM.h DBG.h
err_msg.o: err_msg.c zal_in.h ZAL.h MEM.h DBG.h
error.o: error.c err_msg.c zal_in.h ZAL.h MEM.h DBG.h
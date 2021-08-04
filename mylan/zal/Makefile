TARGET = zal
CFLAGS = -c -g -Wall -Wswitch-enum -pedantic 
OBJS = lex.yy.o\
  y.tab.o\
  main.o\
  interface.o\
  create.o\
  execute.o\
  eval.o\
  string.o\
  util.o\
  ./memory/mem.o\

INCLUDES = \

$(TARGET):$(OBJS)
	cd ./memory; $(MAKE);
	$(CC) $(OBJS) -o $@ -lm
clean:
	rm -f *.o lex.yy.c y.tab.c y.tab.h *~
y.tab.h : zal_in.y
	yacc -dv zal_in.y
y.tab.c : zal_in.y
	yacc -dv zal_in.y
lex.yy.c : zal_in.l zal_in.y y.tab.h
	lex zal_in.l
y.tab.o: y.tab.c zal_in.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
lex.yy.o: lex.yy.c zal_in.h MEM.h
	$(CC) -c -g $*.c $(INCLUDES)
.c.o:
	$(CC) $(CFLAGS) $*.c $(INCLUDES)
./memory/mem.o:
	cd ./memory; $(MAKE);
############################################################
create.o: create.c MEM.h  zal_in.h ZAL.h 
eval.o: eval.c MEM.h  zal_in.h ZAL.h 
execute.o: execute.c MEM.h  zal_in.h ZAL.h 
interface.o: interface.c MEM.h  zal_in.h ZAL.h
main.o: main.c ZAL.h MEM.h
string.o: string.c MEM.h zal_in.h ZAL.h
util.o: util.c MEM.h  zal_in.h ZAL.h 

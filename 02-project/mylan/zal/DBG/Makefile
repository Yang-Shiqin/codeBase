TARGET = dbg.o
CFLAGS = -c -g -Wall 
OBJS = debug.o 

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
.c.o:
	$(CC) $(CFLAGS) -I.. $*.c
debug.o: debug.c ../MEM.h debug.h ../DBG.h

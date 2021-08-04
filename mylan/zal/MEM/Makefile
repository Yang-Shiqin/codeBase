TARGET = mem.o
CFLAGS = -c -g -Wall
OBJS = memory.o 

$(TARGET):$(OBJS)
	ld -r -o $@ $(OBJS)
.c.o:
	$(CC) $(CFLAGS) -I.. $*.c
memory.o: memory.c memory.h ../MEM.h

CFLAGS:=-Wall -Werror -g
CC:=gcc
EXE:=mosquittoNet
INCLUDES:=-Iinc
SRCS:=$(wildcard src/*.c)

OBJS:=$(SRCS:.c=.o)

default: $(EXE)
	gcc $(CFLAGS) -o $(EXE) $(OBJS)

<<<<<<< HEAD
clean:
	rm -r *.o
=======
$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(EXE) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm $(wildcard src/*.o) $(EXE)
>>>>>>> feature

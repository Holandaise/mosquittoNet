CFLAGS:=-Wall -g
CC:=gcc
EXE:=mosquittoNet
INCLUDES:=-Iinc
SRCS:=$(wildcard src/*.c)

OBJS:=$(SRCS:.c=.o)

default: $(EXE)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(EXE) $(OBJS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean
clean:
	rm $(wildcard src/*.o) $(EXE)

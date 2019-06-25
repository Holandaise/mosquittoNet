CFLAGS=-Iinc -Wall -Werror -g
SOURCE_DIR=src
PROG=mosquittoNet
OBJS:=$(wildcard $(SOURCE_DIR)/*.c)

default: $(OBJS)
	gcc $(CFLAGS) -o $(PROG) $(OBJS)

*.c:
	gcc $(CFLAGS) $@

clean:
	rm -r *.o

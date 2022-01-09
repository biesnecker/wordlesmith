CC=gcc
ERRFLAGS=-Werror -Wall
CFLAGS=-std=c17 -O3 $(ERRFLAGS)
BINARY_NAME=wordlesmith

OBJS = $(patsubst %.c, %.o, $(wildcard *.c))

HEADERS = $(wildcard *.h)

$(BINARY_NAME): $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
clean:
	find . -name '*.[oa]' -exec rm -f {} ';'
	rm -f $(BINARY_NAME)

CC := gcc
CFLAGS := -Wall -Werror -D_GNU_SOURCE
LDFLAGS := 


OBJECTS := shell.o util.o linkedlist.o stack.o

all: w4118_sh


w4118_sh: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm -f w4118_sh
	rm -f shell.o

.PHONY: clean

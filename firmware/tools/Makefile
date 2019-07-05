
CC = gcc
CFLAGS += -std=gnu99 -I. -O3 -Wall -Wextra -pedantic -Werror

TOOLS = bmp888to12bits

all: $(TOOLS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

bmp888to12bits: bmp888to12bits.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(TOOLS) *.o *.dSYM


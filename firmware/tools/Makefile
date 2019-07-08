
CC = gcc
CFLAGS += -std=gnu99 -I. -O3 -Wall -Wextra -pedantic -Werror
LDFLAGS += -lm

TOOLS = img2h bmp888to12bits

all: $(TOOLS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

img2h: img2h.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bmp888to12bits: bmp888to12bits.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(TOOLS) *.o *.dSYM


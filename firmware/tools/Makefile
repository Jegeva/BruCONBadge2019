
CC = gcc
CFLAGS += -std=gnu99 -I. -O3 -Wall -Wextra -pedantic -Werror
LDFLAGS += -lm
FTCONFIG = freetype-config

TOOLS = img2h font2h bmp888to12bits

all: $(TOOLS)

font2h.o: font2h.c
	$(CC) $(CFLAGS) $(shell $(FTCONFIG) --cflags) -c $< -o $@

font2h: font2h.o
	$(CC) $(CFLAGS) -o $@ $< $(shell $(FTCONFIG) --libs)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

img2h: img2h.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

bmp888to12bits: bmp888to12bits.o
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)

clean:
	rm -rf $(TOOLS) *.o *.dSYM


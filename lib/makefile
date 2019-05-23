CC      = gcc
AR      = ar
CFLAGS  = -Wall -O3 -std=c11 
AFLAGS  = -r -s

INCLUDES = -I bcore -I bclos -I bmath -I badapt

SRCS = $(wildcard bcore/*.c) $(wildcard bclos/*.c)  $(wildcard bmath/*.c)  $(wildcard badapt/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = beth.a

$(TARGET): $(OBJS)
	$(AR) $(AFLAGS) $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJS)

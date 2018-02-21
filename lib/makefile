CC      = gcc
AR      = ar
CFLAGS  = -Wall -O3 -std=c11 
AFLAGS  = -r -s

INCLUDES = -I bcore -I bclos

SRCS = $(wildcard bcore/*.c) $(wildcard bclos/*.c)
OBJS = $(SRCS:.c=.o)
TARGET = beth.a

$(TARGET): $(OBJS)
	$(AR) $(AFLAGS) $@ $^

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJS)

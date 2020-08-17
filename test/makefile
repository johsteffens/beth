TARGET = bin/beth_test

CC      = gcc
CFLAGS  = -Wall -O3 -std=c11 
LDFLAGS = -lbeth -lm -lpthread

MAIN_SRC = src
BETH_LIB = ../out/libbeth.a

LIB_FOLDERS = \
	-L ../out

INCLUDES = \
	-I ../lib/bcore \
	-I ../lib/bmath \
	-I ../lib/bclos \
	-I ../lib/badapt \
	-I ../lib/bhvm \
	-I ../lib/bhpt

SRCS = \
	$(wildcard $(MAIN_SRC)/*.c)

.PHONY: clean
.PHONY: run

$(TARGET): $(SRCS) $(BETH_LIB)
	mkdir -p $(dir $(TARGET) )
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(SRCS) $(LIB_FOLDERS) $(LDFLAGS)

$(BETH_LIB):
	make -C ..

run: $(TARGET) $(SRCS) $(BETH_LIB)
	mkdir -p temp
	$(TARGET)

clean:
	rm $(TARGET)
	rm temp/*


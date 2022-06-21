TARGET = bin/beth_test

CC      = gcc
CFLAGS  = -Wall -O3 -std=c11 

BETH_LIBS = bhpt bhvm badapt bclos bmath bcore 
LIBS      = $(BETH_LIBS) m pthread atomic
LDFLAGS   = $(addprefix -l, $(LIBS))

MAIN_SRC = src

BETH_BIN_DIR = ../bin
BETH_SRC_DIR = ../lib

LIB_FOLDERS = -L $(BETH_BIN_DIR)

BETH_LIBS_FILES = $(foreach lib,$(BETH_LIBS),$(BETH_BIN_DIR)/lib$(lib).a)

INCLUDES = $(addprefix -I , $(wildcard ../lib/*/.))

SRCS = $(wildcard $(MAIN_SRC)/*.c)

.PHONY: clean cleanall run

$(TARGET): $(SRCS) $(BETH_LIBS_FILES)
	mkdir -p $(dir $(TARGET) )
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(SRCS) $(LIB_FOLDERS) $(LDFLAGS)

$(BETH_BIN_DIR)/lib%.a: ../lib/%/.
	make -C $<

run: $(TARGET) $(SRCS) $(BETH_LIBS_FILES)
	mkdir -p temp
	$(TARGET)

clean:
	rm -f $(TARGET)
	rm -rf temp

cleanall:
	make -C . clean
	$(foreach lib,$(BETH_LIBS),make -C $(BETH_SRC_DIR)/$(lib) cleanx;)
	


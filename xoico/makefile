TARGET = bin/xoico

CC       = gcc
MAKE     = make
CFLAGS   = -Wall -O3 -std=c11 

BETH_LIBS = bcore 
LIBS      = $(BETH_LIBS) m pthread atomic
LDFLAGS   = $(addprefix -l, $(LIBS))

MAIN_SRC = src

BETH_DIR = ..
BETH_BIN_DIR = $(BETH_DIR)/bin
BETH_LIB_DIR = $(BETH_DIR)/lib

LIB_FOLDERS = -L $(BETH_BIN_DIR)

BETH_LIBS_FILES = $(foreach lib,$(BETH_LIBS),$(BETH_BIN_DIR)/lib$(lib).a)

INCLUDES = $(foreach lib,$(BETH_LIBS),-I $(BETH_LIB_DIR)/$(lib))

MAIN_SRC = src

INCLUDES = -I $(BETH_LIB_DIR)/bcore
LIB_FOLDER = -L $(BETH_BIN)

SRCS = $(wildcard $(MAIN_SRC)/*.c)

$(TARGET): $(SRCS)
	$(foreach lib,$(BETH_LIBS),$(MAKE) -j12 -l2.5 -C $(BETH_LIB_DIR)/$(lib) pass2;)   
	mkdir -p $(dir $(TARGET) )
	$(CC) -o $@ $(CFLAGS) $(INCLUDES) $(SRCS) $(LIB_FOLDERS) $(LDFLAGS)

.PHONY: clean

clean:
	rm -f $(TARGET)


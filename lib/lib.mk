# Usage: 
#   In folder with source files, create a 'makefile' and include this makefile into 'makefile'
#
# BETH_DIR = <beth root folder>
# BETH_DEPS = <dependencies>   e.g.: bmath bcore ...
# Optional:
# DO_NOT_CLEAN_XO_FILES = 1 # This prevents *.xo.{h,c} form being deleted in clean operation

LIB_NAME = $(notdir $(CURDIR))
BIN_DIR  = $(BETH_DIR)/bin
OBJ_DIR  = $(BIN_DIR)/obj/$(LIB_NAME)
XC_DIR   = $(BETH_DIR)/../xoico

TARGET   = $(BIN_DIR)/lib$(LIB_NAME).a

CC       = gcc
AR       = ar
MAKE     = make
XC       = $(XC_DIR)/bin/xoico
CFLAGS   = -Wall -O3 -std=c11 
AFLAGS   = -r -s

XC_CFG   = $(wildcard $(LIB_NAME)_xoico.cfg)

ifneq ($(XC_CFG),)
XO_STATE = $(LIB_NAME).xo.state
endif

INCLUDES = $(foreach dep,$(BETH_DEPS),-I $(BETH_DIR)/lib/$(dep))

SRCS     = $(wildcard *.c)
OBJS     = $(SRCS:%.c=$(OBJ_DIR)/%.o)

ALL_LIB_NAMES = $(LIB_NAME) $(BETH_DEPS)
ALL_H_FILES = $(foreach dep,$(ALL_LIB_NAMES),$(wildcard $(BETH_DIR)/lib/$(dep)/*.h))
ALL_C_FILES = $(foreach dep,$(ALL_LIB_NAMES),$(wildcard $(BETH_DIR)/lib/$(dep)/*.c))
ALL_X_FILES = $(foreach dep,$(ALL_LIB_NAMES),$(wildcard $(BETH_DIR)/lib/$(dep)/*.x))
ALL_G_FILES = $(foreach dep,$(ALL_LIB_NAMES),$(wildcard $(BETH_DIR)/lib/$(dep)/*.cfg))

.PHONY: clean cleanall pass2 rebuild rebuildall

$(TARGET): $(XC) $(XO_STATE) $(ALL_C_FILES) $(ALL_H_FILES) $(ALL_X_FILES) $(ALL_G_FILES)
	$(foreach dep,$(BETH_DEPS),$(MAKE) -C $(BETH_DIR)/lib/$(dep);)
	$(MAKE) -j12 -l2.5 -C . pass2  # second pass to capture changes by xoico
	
pass2: $(OBJS)
	@mkdir -p $(dir $(TARGET) )
	$(AR) $(AFLAGS) $(TARGET) $(OBJS)

$(XO_STATE): $(XC) $(ALL_H_FILES) $(ALL_X_FILES) $(ALL_G_FILES)
	$(XC) $(XC_CFG)
	touch $(XO_STATE)

$(BIN_DIR)/lib%.a: $(BETH_DIR)/lib/%/.
	$(MAKE) -C $<

$(OBJ_DIR)/%.o: %.c $(ALL_H_FILES)
	@mkdir -p $(dir $(@) )
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(XC):
	$(MAKE) -C $(XC_DIR)	

clean:
	rm -f  $(TARGET)
	rm -rf $(OBJ_DIR)
ifeq ($(DO_NOT_CLEAN_XO_FILES),)
	rm -f *.xo.h
	rm -f *.xo.c
	rm -f *.xo.state
endif

cleanall:
	$(MAKE) clean
	$(foreach dep,$(BETH_DEPS),$(MAKE) -C $(BETH_DIR)/lib/$(dep) clean;)

rebuild:
	$(MAKE) clean
	$(MAKE)

rebuildall:
	$(MAKE) cleanall
	$(MAKE)


CC       = gcc
AR       = ar
MAKE     = make
XOICODIR = ../xoico
XOICO    = $(XOICODIR)/xoico
CFLAGS   = -Wall -O3 -std=c11 
AFLAGS   = -r -s

XOICO_CFGS = \
  lib/lib_xoico.cfg

INCLUDES = \
  -I lib/bcore  \
  -I lib/bclos  \
  -I lib/bmath  \
  -I lib/badapt \
  -I lib/bhvm \
  -I lib/bhpt

SRCS = \
  $(wildcard lib/bcore/*.c)  \
  $(wildcard lib/bclos/*.c)  \
  $(wildcard lib/bmath/*.c)  \
  $(wildcard lib/badapt/*.c) \
  $(wildcard lib/bhvm/*.c) \
  $(wildcard lib/bhpt/*.c)

OBJS = $(SRCS:.c=.o)
TARGET = libbeth.a

.PHONY: clean
.PHONY: run-xoico

$(TARGET): run-xoico $(OBJS)
	$(AR) $(AFLAGS) $@ $(OBJS)

%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

run-xoico: $(XOICO)
	$(XOICO) $(XOICO_CFGS)
	
$(XOICO):
	$(MAKE) -C $(XOICODIR)	

clean:
	rm -f $(TARGET) $(OBJS)

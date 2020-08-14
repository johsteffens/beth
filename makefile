TARGET = out/libbeth.a

CC       = gcc
AR       = ar
MAKE     = make
XOICODIR = ../xoico
XOICO    = $(XOICODIR)/bin/xoico
CFLAGS   = -Wall -O3 -std=c11 
AFLAGS   = -r -s

XOICO_CFGS = \
  lib/lib_xoico.cfg

INCLUDES = \
  -I lib/bcore  \
  -I lib/bclos  \
  -I lib/bmath  \
  -I lib/badapt \
  -I lib/bhvm   \
  -I lib/bhpt

SRCS = \
  $(wildcard lib/bcore/*.c)\
  $(wildcard lib/bclos/*.c)\
  $(wildcard lib/bmath/*.c)\
  $(wildcard lib/badapt/*.c)\
  $(wildcard lib/bhvm/*.c)\
  $(wildcard lib/bhpt/*.c)

HDRS = \
  $(wildcard lib/bcore/*.h)\
  $(wildcard lib/bclos/*.h)\
  $(wildcard lib/bmath/*.h)\
  $(wildcard lib/badapt/*.h)\
  $(wildcard lib/bhvm/*.h)\
  $(wildcard lib/bhpt/*.h)

OBJS = $(SRCS:lib/%.c=out/%.o)

.PHONY: clean
.PHONY: pass2

$(TARGET): $(XOICO)
	$(XOICO) $(XOICO_CFGS)
	$(MAKE) -C . pass2 # second pass to capture changes by xoico

pass2: $(OBJS)
	$(AR) $(AFLAGS) $(TARGET) $(OBJS)

out/%.o: lib/%.c lib/%.h
	@mkdir -p $(dir $(@) )
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(XOICO):
	$(MAKE) -C $(XOICODIR)	

clean:
	rm -f $(TARGET)
	rm -rf out

cleanall:
	rm -f $(TARGET)
	rm -rf out
	$(MAKE) -C $(XOICODIR) clean


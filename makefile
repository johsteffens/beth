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
  $(addprefix -I , $(wildcard lib/*/.))

SRCS = \
  $(wildcard lib/*/*.c)

HDRS = \
  $(wildcard lib/*/*.h)

XSRCS = \
  $(wildcard lib/*/*.x)

OBJS = $(SRCS:lib/%.c=out/%.o)

.PHONY: clean cleanall pass2

$(TARGET): $(XOICO) $(SRCS) $(HDRS) $(XSRCS)
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
	$(MAKE) clean
	$(MAKE) -C $(XOICODIR) clean


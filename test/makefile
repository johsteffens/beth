BETH_DIR  = ..

DEPENDENCIES = \
  $(BETH_DIR)/lib/bcore \
  $(BETH_DIR)/lib/bmath \
  $(BETH_DIR)/lib/bclos \
  $(BETH_DIR)/lib/badapt \
  $(BETH_DIR)/lib/bhvm \
  $(BETH_DIR)/lib/bhpt
  
POSTBUILD = mkdir -p temp;
CLEAN     = rm -rf temp;

include $(BETH_DIR)/mk/app.mk


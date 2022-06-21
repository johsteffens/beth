# Dependency make framework
#
# Usage: 
# In folder with source files, create a makefile as follows:
#
# BETH_DIR  = <beth root folder>
# DEPENDENCIES = <dependencies>   e.g.: ../bmath ../bcore ...
# (Optional) DO_NOT_CLEAN_XO_FILES = 1 # This prevents *.xo.{h,c} from being deleted in clean operation
#
# include $(BETH_DIR)/mk/dep.mk
#

MAKE = make

.PHONY: first clean cleanall rebuild rebuildall

first:
	$(foreach dep,$(DEPENDENCIES),$(MAKE) -C $(dep);)
	
clean:

cleanall:
	$(MAKE) clean
	$(foreach dep,$(DEPENDENCIES),$(MAKE) -C $(dep) clean;)

rebuild:
	$(MAKE) clean
	$(MAKE)

rebuildall:
	$(MAKE) cleanall
	$(MAKE)


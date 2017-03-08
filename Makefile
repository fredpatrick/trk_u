# Makefile -- GNU Makefile for making alll sub directories of trku 
# 
# $Id: Makefile,v 1.3 2003/02/28 21:19:42 fpatrick Exp $
#

include ./MakeVars

all:  obj libs prg 

obj:
	@echo **************Building .o files***************************
	$(IDIRCMD) $(TRKU_DIR)/lib/objs
	$(MAKE) -C ./cpp obj || exit 1;

libs: obj setup
	$(MAKE) -C ./cpp lib || exit 1;

prg: libs
	@echo **************Building executable programs*****************
	$(IDIRCMD) $(TRKU_DIR)/bin
	$(MAKE) -C ./cpp prg || exit 1;

clean: 
	$(MAKE) -C ./cpp clean || exit 1;
	$(MAKE) -C ./lib clean || exit 1;


setup:
	$(IDIRCMD) $(TRKU_DIR)/bin
	echo $(ALL_LIB_DIRS)
	./admin/make_set_env.sh $(ALL_LIB_DIRS) > ./bin/setup_trku.sh

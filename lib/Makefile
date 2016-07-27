# Makefile -- GNU Makefile for making tst libraries - assumes that objectfiles have already been built
#                   libraries
#

include ../MakeVars


lib: libtrk_u$(TRK_U_VERSION).so

libtrk_u$(TRK_U_VERSION).so : $(wildcard ./objs/*.o)
	$(CXX) $(LIBFLAGS) -o $@ $(wildcard ./objs/*.o)

clean: 
	$(RM) libtrk_u$(TRK_U_VERSION).so
	$(RM) ./objs/*.o


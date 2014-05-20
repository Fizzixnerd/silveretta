# Automatically Generated Makefile by EDE.
# For use with: make
#
# DO NOT MODIFY THIS FILE OR YOUR CHANGES MAY BE LOST.
# EDE is the Emacs Development Environment.
# http://cedet.sourceforge.net/ede.shtml
#

top="$(CURDIR)"/
ede_FILES=Project.ede Makefile

include Makefile.cincludes

proj_SOURCES=string-helpers.c val.c mpc.c eval.c read.c print.c list.c error.c variable.c symbol.c
proj_OBJ=string-helpers.o val.o mpc.o eval.o read.o print.o list.o error.o variable.o symbol.o
silveretta_SOURCES=silveretta.c $(proj_SOURCES)
silveretta_OBJ=silveretta.o $(proj_OBJ)
test_SOURCES=test.c $(proj_SOURCES)
test_OBJ=test.o $(proj_OBJ)
CC=gcc
C_COMPILE=$(CC) $(DEFS) $(INCLUDES) $(CPPFLAGS) $(CFLAGS)
C_DEPENDENCIES=-Wp,-MD,.deps/$(*F).P
C_LINK=$(CC) $(CFLAGS) $(LDFLAGS) -L.
VERSION=1.0
DISTDIR=$(top)Silveretta-$(VERSION)
top_builddir = 

DEP_FILES=.deps/silveretta.P .deps/string-helpers.P .deps/val.P .deps/mpc.P .deps/eval.P .deps/read.P .deps/print.P .deps/list.P .deps/error.P

all: silveretta test

DEPS_MAGIC := $(shell mkdir .deps > /dev/null 2>&1 || :)
-include $(DEP_FILES)

%.o: %.c
	@echo '$(C_COMPILE) -c $<'; \
	$(C_COMPILE) $(C_DEPENDENCIES) -o $@ -c $<

silveretta: $(silveretta_OBJ)
	$(C_LINK) -o $@ $^ $(LDDEPS)

test: $(test_OBJ)
	$(C_LINK) -o $@ $^ $(LDDEPS)

tags: 


clean:
	rm -f *.mod *.o *.obj .deps/*.P .lo

.PHONY: dist

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp $(silveretta_SOURCES) $(test_SOURCES) $(ede_FILES) $(DISTDIR)
	tar -cvzf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)

# End of Makefile

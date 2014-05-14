# Automatically Generated Makefile by EDE.
# For use with: make
#
# DO NOT MODIFY THIS FILE OR YOUR CHANGES MAY BE LOST.
# EDE is the Emacs Development Environment.
# http://cedet.sourceforge.net/ede.shtml
#

top="$(CURDIR)"/
ede_FILES=Project.ede Makefile

VERSION=1.0
DISTDIR=$(top)Simple Lisp-$(VERSION)



all: Simple Lisp

.PHONY:Simple Lisp
Simple Lisp:
	$(MAKE) -C src

tags: 
	$(MAKE) -C src/ $(MFLAGS) $@

.PHONY: dist

dist:
	rm -rf $(DISTDIR)
	mkdir $(DISTDIR)
	cp $(ede_FILES) $(DISTDIR)
	$(MAKE) -C src $(MFLAGS) DISTDIR=$(DISTDIR)/src dist
	tar -cvzf $(DISTDIR).tar.gz $(DISTDIR)
	rm -rf $(DISTDIR)

Makefile: Project.ede
	@echo Makefile is out of date!  It needs to be regenerated by EDE.
	@echo If you have not modified Project.ede, you can use 'touch' to update the Makefile time stamp.
	@false



# End of Makefile

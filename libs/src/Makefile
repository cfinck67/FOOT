# $ Id: $
#
# Automatic recursive decender (base directory version)
#
DIRS = $(wildcard ./TA*)

.PHONY: all clean realclean cintclean depclean

all :
	@for dir in $(DIRS); do echo "Build $$dir library"; \
	cd $$dir; $(MAKE); cd ../; echo " ";done
	
#for each in $(DIRS) ; do $(MAKE) $$dir ; done

realclean:
	for dir in $(DIRS) ; do cd $$dir; rm -f *.d; cd ../; done
	for dir in $(DIRS) ; do cd $$dir; rm -f *.o; cd ../; done
	for dir in $(DIRS) ; do cd $$dir; rm -f *.cint.*; cd ../; done
	for dir in $(DIRS) ; do cd $$dir; rm -f *.pcm; cd ../; done

cintclean:
	for dir in $(DIRS) ; do cd $$dir; rm -f *.cint.*; cd ../; done

depclean:
	for dir in $(DIRS) ; do cd $$dir; rm -f *.d; cd ../; done

.DEFAULT :
	@for dir in $(DIRS); do cd $$dir; $(MAKE); cd ../; done
#

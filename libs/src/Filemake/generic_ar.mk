#
# Build an archive
#
# Before including, defined the following variables:
#   APATH    relative directory path of the archive (optional, default: ../lib)
#   ANAME    name of the archive
#
ifndef APATH
APATH = .
endif
#
AFILE = lib$(ANAME).a
#
.PHONY : libs
libs : $(APATH)/$(AFILE)
#
# Build an archive
#
$(APATH)/$(AFILE) : $(OBJ_all)
	if [ ! -d $(APATH) ]; then mkdir $(APATH); fi
	ar -scruv $(APATH)/$(AFILE) $?

#
# Build a sharable library and an archive
#
# Before including, defined the following variables:
#   SOPATH    relative directory path of the library (optional, default: ../lib)
#   SONAME    name of the library
#   SOMAJV    major version number
#   SOMINV    minor version number
#
ifndef SOPATH
SOPATH     = ../lib
endif
#
SOFILE     = lib$(SONAME).so
SOFILEV    = lib$(SONAME).so.$(SOMAJV)
SOFILEVV   = lib$(SONAME).so.$(SOMAJV).$(SOMINV)
AFILE      = lib$(SONAME).a
ARCH       = $(shell root-config --arch)

# for mac os user only
#ifeq ($(ARCH),macosx)
SOFLAGS = -dynamiclib -single_module -undefined dynamic_lookup  
#SOFLAGS = -dynamiclib 
#SOFLAGS  = -shared
#else 
#SOFLAGS  = -shared -Wl,-soname,$(SOFILEV)
#endif

#
.PHONY : libs
libs : $(SOPATH)/$(AFILE) $(SOPATH)/$(SOFILEVV) 
#
# Build the sharable library
#
$(SOPATH)/$(SOFILEVV) : $(OBJ_all)
	@if [ ! -d $(SOPATH) ]; then mkdir $(SOPATH); fi
	$(CXX) $(SOFLAGS) -o $(SOPATH)/$(SOFILEVV) \
		$(OBJ_all) $(LDLIBS)
	(cd $(SOPATH); rm -f $(SOFILE)   $(SOFILEV))
	(cd $(SOPATH); ln -s $(SOFILEVV) $(SOFILEV))
	(cd $(SOPATH); ln -s $(SOFILEV)  $(SOFILE))
#
# Build an archive
#
$(SOPATH)/$(AFILE) : $(OBJ_all)
	if [ ! -d $(SOPATH) ]; then mkdir $(SOPATH); fi
	ar -scruv $(SOPATH)/$(AFILE) $?

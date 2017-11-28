# $ Id: $
#
# Name of the sharable library
#
SOPATH     = ../../lib
SONAME     = TAGbase
SOMAJV     = 1
SOMINV     = 0

ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)

#
# Compile and Link search passes
#
TINCLFLAGS += $(shell root-config --cflags)
INCLFLAGS  = $(filter-out -stdlib=libc++, $(TINCLFLAGS))
LDLIBS    = $(shell root-config --glibs) -lGeom -lGeomPainter -lEve
#
# Files to be included
#
TOBJ_all   = TAGobject.hxx TAGnamed.hxx TAGroot.hxx \
		TAGtimestamp.hxx TAGeventId.hxx \
		TAGrunInfo.hxx \
		TAGdata.hxx TAGdataDsc.hxx TAGpara.hxx TAGparaDsc.hxx \
		TAGaction.hxx TAGactionFile.hxx \
		TAGactTreeReader.hxx TAGactTreeWriter.hxx \
		TAGpadGroup.hxx \
		TAGview.hxx \
		TAGconfigFile.hxx  TAGgeoTrafo.hxx \
		TAGstlUtil.hxx TAGlineFit.hxx \
		TAGclusterDisplay.hxx TAGtrackDisplay.hxx TAGwireDisplay.hxx \
		TAGeventDisplay.hxx
	

CXX_all    = $(TOBJ_all:.hxx=.cxx) 
#
OBJ_all    = $(CXX_all:.cxx=.o) $(TOBJ_all:.hxx=.cint.o) 
DEP_all    = $(CXX_all:.cxx=.d) $(TOBJ_all:.hxx=.cint.d)
CINT_all   = $(TOBJ_all:.hxx=.cint.cxx) $(TOBJ_all:.hxx=.cint.h) 
#
#- generic part ----------------------------------------------------------------
#
SOFILE     = lib$(SONAME).so
SOFILEV    = lib$(SONAME).so.$(SOMAJV)
SOFILEVV   = lib$(SONAME).so.$(SOMAJV).$(SOMINV)
#
include $(ASOFTREF)/make/generic_root_cxx.mk
include $(ASOFTREF)/make/generic_dep.mk
include $(ASOFTREF)/make/generic_so.mk
#
# The magic autodependcy include
#
include $(DEP_all)
#
# cleanup phonies:
#
.PHONY    : clean cleandep realclean
clean     :
	@ rm -f $(OBJ_all)
	@ echo "Object files removed"
#
cleandep  :
	@ rm -f $(DEP_all)
	@ echo "Dependency files removed"
#
cleancint :
	@ rm -f $(CINT_all)
	@ echo "rootcint generated files removed"
#
realclean : clean cleandep cleancint
	@ rm -f $(SOPATH)/lib$(SONAME).*
	@ echo "Library removed"
#

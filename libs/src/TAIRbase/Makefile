# $ Id: $
#
# Name of the sharable library
#
SOPATH     = ../../lib
SONAME     = TAIRbase
SOMAJV     = 1
SOMINV     = 0
#
# Compile and Link search passes
#
TINCLFLAGS += $(shell root-config --cflags)
INCLFLAGS  = $(filter-out -stdlib=libc++, $(TINCLFLAGS))
INCLFLAGS += -I../TAGbase -I../TAGfoot -I../TABMbase 
INCLFLAGS += -I../../../Simulation/TXT2ROOT -I../../../Simulation
INCLFLAGS += -I../../../libs/GenFit/fitters/include -I../../../libs/GenFit/core/include
LDLIBS     = $(shell root-config --glibs) -lGeom -lGeomPainter
#
# Files to be included
#
TOBJ_all   = TAIRdatRaw.hxx  TAIRparMap.hxx \
                  TAIRactNtuMC.hxx TAIRparGeo.hxx  TAIRparTools.hxx

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

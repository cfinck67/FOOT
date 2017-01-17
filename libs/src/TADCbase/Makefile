# $ Id: $
#
# Name of the sharable library
#
SOPATH     = ../../lib
SONAME     = TADCbase
SOMAJV     = 1
SOMINV     = 0
#
# Compile and Link search passes
#
TINCLFLAGS += $(shell root-config --cflags)
INCLFLAGS  = $(filter-out -stdlib=libc++, $(TINCLFLAGS))
INCLFLAGS += -I../TAIRbase -I../TAGbase -I../TAGfoot -I../../../Simulation/TXT2ROOT
INCLFLAGS += -I../../../Simulation
LDLIBS    += $(shell root-config --libs) 
#
# Files to be included
#
TOBJ_all   = TADCdatRaw.hxx  TADCntuRaw.hxx \
	     TADCparMap.hxx TADCparGeo.hxx \
	     TADCactNtuRaw.hxx \
	     TADCactNtuMC.hxx  TADCvieTrackFIRST.hxx \
	     TADCntuTrack.hxx TADCactNtuTrack.hxx \
	     TADCparCon.hxx
#           TADCactDatRaw.hxx 
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

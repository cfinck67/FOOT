# $ Id: $
#
# Name of the sharable library
#
SOPATH     = ../../lib
SONAME     = TAGfoot
SOMAJV     = 1
SOMINV     = 0
#
# Compile and Link search passes
#
TINCLFLAGS += $(shell root-config --cflags)
INCLFLAGS  = $(filter-out -stdlib=libc++, $(TINCLFLAGS))
INCLFLAGS += -I../TAGbase -I../TABMbase -I../TAVTbase -I../TAITbase -I../TATWbase
INCLFLAGS += -I../TAGfoot -I../TAVTbase -I../TAIRbase -I../TAMSDbase -I../TACAbase
INCLFLAGS += -I../TAGmclib -I../../../Simulation/TXT2ROOT -I../../../Simulation
INCLFLAGS += -I../../../libs/GenFit/fitters/include 
INCLFLAGS += -I../../../libs/GenFit/trackReps/include -I../../../libs/GenFit/fields/include 
INCLFLAGS += -I../../../libs/GenFit/measurements/include -I../../../libs/GenFit/include


LDLIBS    = $(shell root-config --libs) 
#
# Files to be included
#
TOBJ_all   =   TATRdatRaw.hxx \
             TATRparGeo.hxx  TATRparMap.hxx \
             TinCan.hxx  KFitter.hxx	FootField.hxx  GlobalTrackRepostory.hxx   GlobalTrackKalman.hxx  MagicSkills.hxx	\
             IronPlate.hxx GlobalPar.hxx	Materials.hxx  ControlPlotsRepository.hxx  ControlPlotInfo.hxx \
             FootBox.hxx  LightSabre.hxx  Booter.hxx

             # TAGvieHorzIR.hxx TAGvieHorzMCIR.hxx    \		// rimosso by Frank
             

#	     TAGvieHorzTracks.hxx TAGvieHorzFIRST.hxx  GlobalTrackCandidate.hxx \
      #        GlobalTrack.hxx TATRactDatRaw.hxx \
      #        MagneticField.hxx TrackableParticle.hxx \
      #        Tracking.hxx TAGactGlbTracking.hxx \
	     # TAGntuGlbTracks.hxx TADIparGeo.hxx \
      #        EventDisplay.hxx GlbEventDisplay.hxx \
      #        ChargeBetheBlock.hxx	
      
CXX_all    = $(TOBJ_all:.hxx=.cxx) 
#
OBJ_all    = $(CXX_all:.cxx=.o) $(TOBJ_all:.hxx=.cint.o) 
DEP_all    = $(CXX_all:.cxx=.d) $(TOBJ_all:.hxx=.cint.d)
CINT_all   = $(TOBJ_all:.hxx=.cint.cxx) $(TOBJ_all:.hxx=.cint.h) 
#
#- generic part ----------------------------------------------------------------
#cd 
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

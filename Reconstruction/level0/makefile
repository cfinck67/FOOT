ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
ARCH       = $(shell root-config --arch)

CXX           = g++

LD            = g++
LDFLAGS       = -g

NGLIBB         = $(ROOTGLIBS) 
NGLIBB        += -lMinuit -lGeom -lGeomPainter -lEve -lRGL -lGed -lEG -lTreePlayer -L$(PWD)/../../libs/GenFit/lib -lgenfit2
NGLIBB        += $(PWD)/libRecoTools.so 
#MC libs
NGLIBB        += $(PWD)/libEvento.so 

#TA* libs
NGLIBB        += $(PWD)/../../libs/lib/libTAGfoot.so $(PWD)/../../libs/lib/libTAVTbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTAGbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTAGmclib.so
NGLIBB        += $(PWD)/../../libs/lib/libTABMbase.so
NGLIBB        += $(PWD)/../../libs/lib/libTAIRbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTAITbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTATWbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTACAbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTAMSDbase.so 

GLIBB          = $(filter-out -lNew, $(NGLIBB))

ELIB        += ./libEvento.so 

TALIB        += -lGeom -lGeomPainter -lEve -L$(PWD)/../../libs/GenFit/lib -lgenfit2
TALIB        += ../../libs/lib/libTAGfoot.so ../../libs/lib/libTAGmclib.so 
TALIB        += ../../libs/lib/libTAGbase.so 
TALIB        += ../../libs/lib/libTABMbase.so 
TALIB        += ../../libs/lib/libTAITbase.so 
TALIB        += ../../libs/lib/libTATWbase.so 
TALIB        += ../../libs/lib/libTACAbase.so 
TALIB        += ../../libs/lib/libTAMSDbase.so 
TALIB        += ../../libs/lib/libTAIRbase.so ../../libs/lib/libTAVTbase.so

ifeq ($(ARCH),macosx64)
#SOFLAGS       = -dynamiclib -single_module -undefined dynamic_lookup  
SOFLAGSE       = -shared $(ROOTGLIBS) 
SOFLAGSV       = -shared $(ROOTGLIBS) 
SOFLAGS       = -dynamiclib -shared $(ROOTGLIBS) $(TALIB) $(ELIB)
else 
SOFLAGSE       = -shared $(ROOTGLIBS) 
SOFLAGSV       = -shared $(ROOTGLIBS) 
SOFLAGS  = -shared $(ROOTGLIBS) 
endif

TAINCL	      = -I../ -I../../libs/src/TAGbase/ -I../../libs/src/TAGmclib/ -I../../libs/src/TAGfoot 
TAINCL	     +=-I../../libs/src/TABMbase -I../../libs/src/TAIRbase 
TAINCL	     +=-I../../libs/src/TAITbase -I../../libs/src/TACAbase -I../../libs/src/TATWbase 
TAINCL	     +=-I../../libs/src/TAVTbase -I../../libs/src/TAMSDbase -I../../Simulation/TXT2ROOT -I../../Simulation 
TAINCL	     +=-I../../libs/GenFit/fitters/include -I../../libs/GenFit/core/include 
TAINCL	     +=-I../../libs/GenFit/trackReps/include -I../../libs/GenFit/fields/include 
TAINCL	     +=-I../../libs/GenFit/measurements/include  -I$(PWD)/../../libs/GenFit/utilities/include 
TAINCL	     +=-I../../libs/GenFit/eventDisplay/include

CXXFLAGS      = -g -Wall -fPIC
CXXFLAGS      += $(ROOTCFLAGS)
CXXFLAGS      += $(TAINCL)

LIBS           = $(ROOTLIBS) 

NTUPLEB = RecoTools.o 
NTUPLEB += RecoToolsDict.o 

NTUPLET = ../../Simulation/TXT2ROOT/Evento.o EventoDict.o

.SUFFIXES: .cc,.C

.cc.o:
	$(CXX) $(CXXFLAGS) -c $<



# ================================================================================
lib:  $(NTUPLEB)
#---------------------------------------------------------------
	$(CXX) $(SOFLAGS) $(NTUPLEB) -o libRecoTools.so

RecoToolsDict.cc:  RecoTools.h
	$(ROOTSYS)/bin/rootcint -f RecoToolsDict.cc -c $(TAINCL) RecoTools.h






# ================================================================================
libEvento:  $(NTUPLET)
#---------------------------------------------------------------
	$(CXX) $(SOFLAGSV) $(NTUPLET) -o libEvento.so

EventoDict.cc: ../../Simulation/TXT2ROOT/Evento.h
	$(ROOTSYS)/bin/rootcint -f EventoDict.cc -c -I../ ../../Simulation/TXT2ROOT/Evento.h

DecodeMC:  RecoL0.o libEvento lib 
# -----------------------------------------------------------------------------
	$(LD) $(LDFLAGS) -o DecodeMC RecoL0.o $(GLIBB)


# ================================================================================
clean:
	rm -f *.o *.so *Dict* DecodeMC



















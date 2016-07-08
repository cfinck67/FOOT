ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
ARCH       = $(shell root-config --arch)

CXX           = g++

LD            = g++
LDFLAGS       = -g

NGLIBB         = $(ROOTGLIBS) 
NGLIBB        += -lMinuit   -lGeom -lGeomPainter -lEve -lRGL -lGed -lEG -lTreePlayer
NGLIBB        += $(PWD)/libRecoTools.so 
#MC libs
NGLIBB        += $(PWD)/libEvento.so 
NGLIBB        += $(PWD)/libSegnale.so 
NGLIBB        += $(PWD)/libGeometry.so 
NGLIBB        += $(PWD)/libTrigger.so
#TA* libs
NGLIBB        += $(PWD)/../../libs/lib/libTAGfoot.so $(PWD)/../../libs/lib/libTAVTbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTAGbase.so 
NGLIBB        += $(PWD)/../../libs/lib/libTAGmclib.so
NGLIBB        += $(PWD)/../../libs/lib/libTABMbase.so
NGLIBB        += $(PWD)/../../libs/lib/libTAIRbase.so 

GLIBB          = $(filter-out -lNew, $(NGLIBB))

ELIB        += ./libEvento.so 
ELIB        += ./libSegnale.so 
ELIB        += ./libGeometry.so 
ELIB        += ./libTrigger.so

TAINCL	       = -I$(PWD)/../ -I$(PWD)/../../libs/src/TAGbase/  -I$(PWD)/../../libs/src/TAGmclib/ -I$(PWD)/../../libs/src/TAVTbase -I$(PWD)/../../libs/src/TAGfoot -I$(PWD)/../../libs/src/TABMbase -I$(PWD)/../../libs/src/TAIRbase  -I$(PWD)/../../Simulation/TXT2ROOT

TALIB        += ../../libs/lib/libTAGfoot.so ../../libs/lib/libTAGmclib.so 
TALIB        += ../../libs/lib/libTAGbase.so 
TALIB        += ../../libs/lib/libTABMbase.so 
TALIB        += ../../libs/lib/libTAIRbase.so ../../libs/lib/libTAVTbase.so 

ifeq ($(ARCH),macosx64)
#SOFLAGS       = -dynamiclib -single_module -undefined dynamic_lookup  
SOFLAGSE       = -shared $(ROOTGLIBS) 
SOFLAGSS       = -shared $(ROOTGLIBS) ./libGeometry.so  
SOFLAGSV       = -shared $(ROOTGLIBS) ./libSegnale.so ./libGeometry.so 
SOFLAGS       = -dynamiclib -shared $(ROOTGLIBS) $(TALIB) $(ELIB)
else 
SOFLAGS  = -shared $(ROOTGLIBS) 
endif

TAINCL	       = -I../ -I../../libs/src/TAGbase/ -I../../libs/src/TAGmclib/ -I../../libs/src/TAGfoot -I../../libs/src/TABMbase -I../../libs/src/TAIRbase -I../../libs/src/TAVTbase -I../../Simulation/TXT2ROOT

CXXFLAGS      = -g -Wall -fPIC
CXXFLAGS      += $(ROOTCFLAGS)
CXXFLAGS      += $(TAINCL)

LIBS           = $(ROOTLIBS) 

NTUPLEB = RecoTools.o 
NTUPLEB += RecoToolsDict.o 

NTUPLET = ../../Simulation/TXT2ROOT/Evento.o EventoDict.o
NTUPLES = ../../Simulation/TXT2ROOT/Segnale.o SegnaleDict.o
NTUPLEG = ../../Simulation/TXT2ROOT/Geometry.o GeometryDict.o
NTUPLEU = ../../Simulation/TXT2ROOT/Trigger.o TriggerDict.o

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

# ================================================================================
libSegnale:  $(NTUPLES)
#---------------------------------------------------------------
	$(CXX) $(SOFLAGSS) $(NTUPLES) -o libSegnale.so

SegnaleDict.cc: ../../Simulation/TXT2ROOT/Segnale.h
	$(ROOTSYS)/bin/rootcint -f SegnaleDict.cc -c -I../ ../../Simulation/TXT2ROOT/Segnale.h

# ================================================================================
libTrigger:  $(NTUPLEU) 
#---------------------------------------------------------------
	$(CXX) $(SOFLAGSE) $(NTUPLEU) -o libTrigger.so

TriggerDict.cc: ../../Simulation/TXT2ROOT/Trigger.h
	$(ROOTSYS)/bin/rootcint -f TriggerDict.cc -c -I../ ../../Simulation/TXT2ROOT/Trigger.h


# ================================================================================
libGeometry:  $(NTUPLEG) 
#---------------------------------------------------------------
	$(CXX) $(SOFLAGSE) $(NTUPLEG) -o libGeometry.so

GeometryDict.cc: ../../Simulation/TXT2ROOT/Geometry.h
	$(ROOTSYS)/bin/rootcint -f GeometryDict.cc -c -I../ ../../Simulation/TXT2ROOT/Geometry.h


DecodeMC:  RecoL0.o libGeometry libSegnale libEvento libTrigger lib 
# -----------------------------------------------------------------------------
	$(LD) $(LDFLAGS) -o DecodeMC RecoL0.o $(GLIBB)


# ================================================================================
clean:
	rm -f *.o *.so *Dict* DecodeMC

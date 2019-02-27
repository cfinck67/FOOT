//header file

#ifndef TCFOrunAction_h
#define TCFOrunAction_h 1

#include "TCGbaseRunAction.hxx"

#include "globals.hh"

#include "Evento.hxx"
#include "TAGroot.hxx"
#include "TString.h"

#include <TStopwatch.h>

class TCFOrunAction : public TCGbaseRunAction
{
public:
   TCFOrunAction();
   ~TCFOrunAction();
   
public:
   Evento*        GetEventMC()     const { return fpEventMC; }
   EVENT_STRUCT*  GetEveStruct()   const { return fpEveStruct; }
   
private:
   Evento*        fpEventMC;  // For data form Simulation
   EVENT_STRUCT*  fpEveStruct;  // For data form Simulation
   
private:
   void           SetContainers();
   void           ClearContainers();
   
};

#endif

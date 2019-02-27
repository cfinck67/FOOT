//TCGbaseEventAction 

#ifndef TCGbaseEventAction_h
#define TCGbaseEventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "TSysEvtHandler.h"

class TAGbaseRunAction;
class Evento;
class TCGmcHit;

class TAGeventInterruptHandler : public TSignalHandler {
public:
   TAGeventInterruptHandler();
   Bool_t          Notify();
};

//------------------------------------------------
class TCGbaseEventAction : public G4UserEventAction
{
public:
   TCGbaseEventAction();
   virtual  ~TCGbaseEventAction();
   
   void          BeginOfEventAction(const G4Event* evt);
   void          EndOfEventAction(const G4Event*);
   virtual void  Collect(const G4Event* /*evt*/) { return;    }
   virtual void  ConstructCollection()           { return;    }

protected:
   virtual Int_t GetEventsNToBeProcessed()        { return -1; }
   virtual void  FillAndClear()                   { return;    }
   
   
protected:
    Int_t              fDebugLevel;
    TSignalHandler*    fEventInterruptHandler;
};

#endif


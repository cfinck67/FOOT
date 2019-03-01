//TCFOeventAction

#ifndef TCFOeventAction_h
#define TCFOeventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"

#include "TSysEvtHandler.h"

class TCFOrunAction;
class TCGbaseGeometryConstructor;
class TCFOgeometryConstructor;
class TAGgeoTrafo;
class Evento;
class TCGmcHit;

class TAGeventInterruptHandler : public TSignalHandler {
    public:
    TAGeventInterruptHandler();
    Bool_t          Notify();
};

// Event action for HIT geometry

//------------------------------------------------
class TCFOeventAction : public G4UserEventAction
{
    public:
    TCFOeventAction(TCFOrunAction* runAction, TCGbaseGeometryConstructor* hitGeomConstructor);
    ~TCFOeventAction();

    public:
    void   BeginOfEventAction(const G4Event* evt);
    void   EndOfEventAction(const G4Event*);
    void   ConstructCollection();
    void   Collect(const G4Event* evt);

    private:
    TCFOrunAction*           fRunAction;           // run action for FOOT
    TCFOgeometryConstructor* fFootGeomConstructor; // geometry for FOOT
    Int_t                    fDebugLevel;
    Int_t                    fEventNumber;         // event number
    Int_t                    fIrCollId;            // Id of SC collection
    Int_t                    fBmCollId;            // Id of BM collection
    Int_t                    fVtxCollId;           // Id of Vtx collection
    Int_t                    fItCollId;            // Id of IT collection
    Int_t                    fMsdCollId;           // Id of MSD collection
    Int_t                    fTwCollId;            // Id of ToF collection
    Int_t                    fCaCollId;            // Id of Calo collection
    TSignalHandler*          fEventInterruptHandler;

    TAGgeoTrafo*             fpGeoTrafo;           // trafo pointer

    private:
    Int_t  GetEventsNToBeProcessed();
    void   FillAndClear();
    void   GetHitPerPlane(const G4Event* evt, G4int idColl);
    void   FillHits(Evento* mcHit, TCGmcHit* hit);
};

#endif



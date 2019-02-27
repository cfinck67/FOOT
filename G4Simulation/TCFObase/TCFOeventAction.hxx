//TCFOeventAction

#ifndef TCFOeventAction_h
#define TCFOeventAction_h 1

#include "TCGbaseEventAction.hxx"

#include "globals.hh"

class TCFOrunAction;
class TCGbaseRunAction;
class TCGbaseGeometryConstructor;
class TCFOgeometryConstructor;
class TAGgeoTrafo;

// Event action for HIT geometry

//------------------------------------------------
class TCFOeventAction : public TCGbaseEventAction
{
public:
   TCFOeventAction(TCGbaseRunAction* runAction, TCGbaseGeometryConstructor* hitGeomConstructor);
   ~TCFOeventAction();
   
public:
   void   ConstructCollection();
   void   Collect(const G4Event* evt);

private:
   TCFOrunAction*           fRunAction;           // run action for FOOT
   TCFOgeometryConstructor* fFootGeomConstructor; // geometry for FOOT
   Int_t                    fIrCollId;            // Id of SC collection
   Int_t                    fBmCollId;            // Id of BM collection
   Int_t                    fVtxCollId;           // Id of Vtx collection
   Int_t                    fItCollId;            // Id of IT collection
   Int_t                    fMsdCollId;           // Id of MSD collection
   Int_t                    fTwCollId;            // Id of ToF collection
   Int_t                    fCaCollId;            // Id of Calo collection
   
   
   TAGgeoTrafo*             fpGeoTrafo;           // trafo pointer
   
private:
   Int_t  GetEventsNToBeProcessed();
   void   FillAndClear();
   
private:
   void   GetHitPerPlane(const G4Event* evt, G4int idColl);
   void   FillHits(Evento* mcHit, TCGmcHit* hit);

};

#endif



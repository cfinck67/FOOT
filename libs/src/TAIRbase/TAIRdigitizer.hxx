#ifndef _TAIRdigitizer_HXX
#define _TAIRdigitizer_HXX

#include "TAGbaseDigitizer.hxx"
#include "TAIRntuHit.hxx"
#include "Evento.h"

class TF1;
class TAIRparGeo;

// --------------------------------------------------------------------------------------
class TAIRdigitizer : public TAGbaseDigitizer {
   
public:
   TAIRdigitizer(EVENT_STRUCT* pNtuHitsMC, TAIRntuHit* pNtuRaw, TAIRparGeo* pParGeo);
   ~TAIRdigitizer();
   
   void           SetFunctions();
   void           SetInitParFunction();
   
   void           Digitize();
   
   Float_t        GetTimeWalk(Float_t edep);
   Double_t       TimeWalk(Double_t* x, Double_t* par);
 
   
   void           SetGain(Float_t g)   { fGain = g; }

private:
   EVENT_STRUCT* fpEvtStr;
   TAIRntuHit*   fpNtuRaw;
   TAIRparGeo*   fpParGeo;
   TF1*          fTimeWalk;
   Float_t       fResSig;
   Float_t       fErrResSig;
   
   Float_t       fTimePar1;
   Float_t       fTimePar2;
   Float_t       fTimeErrPar1;
   Float_t       fTimeErrPar2;
   Float_t       fGain;
   
private:
   Bool_t Process(Int_t sensorId, Int_t mcId, Double_t edep, Double_t time);

   
};
#endif


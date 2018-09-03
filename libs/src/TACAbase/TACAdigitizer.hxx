#ifndef _TACAdigitizer_HXX
#define _TACAdigitizer_HXX

#include "TObject.h"

#include "TACAntuRaw.hxx"

class TF1;
class TACAntuRaw;

// --------------------------------------------------------------------------------------
class TACAdigitizer : public TObject {
   
public:
   TACAdigitizer(TACAntuRaw* p_datraw);
   ~TACAdigitizer();
   
   void           SetFunctions();
   void           SetParFunction();
   
   TACAntuHit*     Process(Int_t sensorId, Double_t edep, Double_t x0, Double_t y0, Double_t time, Double_t zin=0, Double_t zout=0);

   
   Float_t        GetPhotonsN(Float_t X, Float_t Y, Float_t edep);
   Double_t       RecPhotonsN(Double_t* x, Double_t* par);
 
   TF1*           GetFuncBirks() const { return fFuncBirks; }
   
   void           SetGain(Float_t g)   { fGain = g; }

private:
   TACAntuRaw*   fpNtuRaw;
   TF1*          fFuncBirks;
   Float_t       fGain;
   
private:

   
};
#endif


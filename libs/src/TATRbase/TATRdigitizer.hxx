#ifndef _TATRdigitizer_HXX
#define _TATRdigitizer_HXX

#include "TAGbaseDigitizer.hxx"

#include "TATRntuRaw.hxx"

class TF1;

// --------------------------------------------------------------------------------------
class TATRdigitizer : public TAGbaseDigitizer {
   
public:
   TATRdigitizer(TATRntuRaw* p_datraw);
   ~TATRdigitizer();
   
   void           SetFunctions();
   void           SetParFunction();
   
   Bool_t         Process(Double_t edep, Double_t x0, Double_t y0, Double_t zin=0, Double_t zout=0, Double_t time = 0, Int_t sensorId = 0);

   
   Float_t        GetPhotonsN(Float_t X, Float_t Y, Float_t edep);
   Double_t       RecPhotonsN(Double_t* x, Double_t* par);
 
   TF1*           GetFuncBirks() const  { return fFuncBirks;  }
   
   void           SetGain(Float_t g)    { fGain = g;          }
   void           SetResTime(Float_t r) { fResTime = r;       }

   TATRntuHit*    GetCurrentHit()       { return fCurrentHit; }
   
private:
   TATRntuRaw*   fpNtuRaw;
   TF1*          fFuncBirks;
   Float_t       fGain;
   Float_t       fResTime;
   TATRntuHit*   fCurrentHit;

};
#endif


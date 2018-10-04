#ifndef _TAMSDdigitizer_HXX
#define _TAMSDdigitizer_HXX


#include <map>

#include "TAVTbaseDigitizer.hxx"

// --------------------------------------------------------------------------------------
class TAVTbaseParGeo;
class TAMSDdigitizer : public TAVTbaseDigitizer {
   
   // Class to digitize the energy into pixel based on given patterns
public:
   TAMSDdigitizer(TAVTbaseParGeo* parGeo);
   virtual ~TAMSDdigitizer();
   

   void  FillMap(Int_t line, Int_t col, Double_t value);
   Bool_t Process( Double_t edep, Double_t x0, Double_t y0, Double_t zin = 0, Double_t zout = 0, Double_t time = 0, Int_t sensorId = 0);
   
   
private:
   static Float_t  fgChargeGain;      // gain factor for despoted charge
   static Float_t  fgChargeFac;       // sharing factor between strips
   
public:
   static Float_t GetChargeGain()                   { return fgChargeGain;       }
   static void    SetChargeGain(Float_t gain)       { fgChargeGain = gain;       }
   
   static Float_t GetChargeFac()                    { return fgChargeFac;        }
   static void    SetChargeFac(Float_t fac)         { fgChargeFac = fac;         }
   
   
   ClassDef(TAMSDdigitizer,0)
};
        

#endif


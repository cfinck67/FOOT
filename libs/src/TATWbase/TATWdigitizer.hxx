#ifndef _TATWdigitizer_HXX
#define _TATWdigitizer_HXX

#include "TAGbaseDigitizer.hxx"

#include "TATW_Hit.hxx"
#include "TATW_ContainerHit.hxx"

class TF1;


// --------------------------------------------------------------------------------------
class TATWdigitizer : public TAGbaseDigitizer {
   
public:
   TATWdigitizer(TATW_ContainerHit* pNtuRaw);
   ~TATWdigitizer();
   
   void           SetFunctions();
   void           SetInitParFunction();
   
   Bool_t         Process(Double_t edep, Double_t x0, Double_t y0, Double_t zin=0, Double_t zout=0, Double_t time = 0, Int_t sensorId = 0);
   
   Float_t        GetResEnergy(Float_t energy);
   Double_t       ResEnergy(Double_t* x, Double_t* par);
   
   Float_t        GetResToF(Float_t edep);
   Double_t       ResToF(Double_t* x, Double_t* par);
   
   Float_t        GetDeAttLeft(Float_t pos, Float_t edep);
   Double_t       DeAttLeft(Double_t* pos, Double_t* par);

   Float_t        GetDeAttRight(Float_t pos, Float_t edep);
   Double_t       DeAttRight(Double_t* pos, Double_t* par);
   
   Float_t        GetTofLeft(Float_t pos, Float_t time, Float_t edep);
   Float_t        GetTofRight(Float_t pos, Float_t time, Float_t edep);
   
   void           SetGain(Float_t g)   { fGain = g; }
   TATW_Hit*      GetCurrentHit()      { return fCurrentHit; }

   
private:
   TATW_ContainerHit*   fpNtuRaw;
   TATW_Hit*     fCurrentHit;
   TATWparGeo*   fpParGeo;
   
   // deltaE
   TF1*          fDeResE;
   Float_t       fDeResECst;
   Float_t       fDeErrResCst; // not used
   Float_t       fDeResEC;
   Float_t       fDeErrResEC;

   TF1*          fDeAttLeft;
   Float_t       fDeAttCstLeft;
   Float_t       fDeErrAttCstLeft; // not used
   Float_t       fDeAttLambdaLeft;
   Float_t       fDeErrAttLambdaLeft;
   
   TF1*          fDeAttRight;
   Float_t       fDeAttCstRight;
   Float_t       fDeErrAttCstRight; // not used
   Float_t       fDeAttLambdaRight;
   Float_t       fDeErrAttLambdaRight;
   
   Float_t       fDeAttAsym;
   Float_t       fDeAttAsymSmear;

   // TOF
   TF1*          fTofResE;
   Float_t       fTofCstE;
   Float_t       fTofErrCstE;
   Float_t       fTofLambdaE;
   Float_t       fTofErrLambdaE;
   Float_t       fTofk0E;
   Float_t       fTofErrk0E;
   
   Float_t       fTofResP;
   Float_t       fTofErrResP;
   Float_t       fTofPropAlpha; // inverse of light propagation velocity
   Float_t       fTofErrPropAlpha;
   
   // misc
   Float_t       fSlatLength;
   Float_t       fGain;
   
private:
   static const Float_t cm;
   static const Float_t ps;
   static       Float_t fgHfactor; // happy factor for edep
   
   ClassDef(TATWdigitizer,0)

};
#endif


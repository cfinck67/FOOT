#ifndef _TAIRparDiff_HXX
#define _TAIRparDiff_HXX
/*!
 \file
 \brief   Declaration of TAIRparDiff.
 
 \author C.A. Reidel & Ch. Finck
 */
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TString.h"

#include <map>

#include "TAGparaDsc.hxx"
#include "TAGpara.hxx"

//##############################################################################
class TF1;

class TAIRparDiff : public TAGpara {
   
public:
   TAIRparDiff(TAGparaDsc* pGeoMap);
   virtual ~TAIRparDiff();
   
   Double_t  SigmaTheta(Double_t* x, Double_t* par);

   Float_t SigmaThetaCalc(Float_t energy, TString mat, Float_t x);
   Float_t EnergyCalc(Float_t energy, Float_t massNumber, Int_t atomicNumber, Float_t WEPL);
   Float_t PCCalc(Float_t energy, Float_t massNumber);
   Float_t BetaCalc(Float_t energy);
   Float_t WEPLCalc(const TString& material, Float_t thickness);

   Float_t GetRadLength(TString name) { return fListMaterials[name].RadiationLength;   };
   Float_t GetDensity(TString name) { return fListMaterials[name].Density;   };
   
private:
   void SetMaterials();
   
public:
   struct MaterialParameter_t {
      Float_t   RadiationLength; // Radiation length (g/cm2)
      Float_t   Density;         // Density (g/cm3)
   };

private:
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   
   // Parameters for energy loss calculation
   Float_t        fAlpha;
   Float_t        fPfactor;
   
   TF1*           fFuncSigTheta;
   
   map<TString, MaterialParameter_t> fListMaterials;
   
private:
   static const Float_t fgkProtonMass;     // default proton mass
   
   
   ClassDef(TAIRparDiff,1)
};

#endif

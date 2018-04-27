#ifndef _TAVTparCal_HXX
#define _TAVTparCal_HXX
/*!
  \file
  \version $Id: TAVTparCal.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAVTparCal.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TArrayF.h"
#include "TF1.h"

#include "TAGparTools.hxx"

//##############################################################################

class TAVTparCal : public TAGparTools {
      
private:
   TF1*     fLandau[6];
   TF1*     fLandauNorm[6];
   TF1*     fLandauTot;
   TArrayF* fChargeProba;
   TArrayF* fChargeProbaNorm;
   Int_t    fChargeWithMaxProba;
   Float_t  fChargeMaxProba;
   Int_t    fChargeWithMaxProbaNorm;
   Float_t  fChargeMaxProbaNorm;

   struct LandauParameter_t : public  TObject {
	  Int_t   Charge;   // charge
	  Float_t Constant; // constant value
	  Float_t MPV;      // MPV of Landau
	  Float_t Sigma;    // sigma of Landau  
	  Float_t Quench;   // quenching factor of Landau
   };
   
   LandauParameter_t  fLandauParameter[6];

private: 
   static TString fgkDefaultCalName; // default detector charge calibration file
   static Int_t   fgkChargesN;       // number of charge states

private:
   void SetFunction();
   
public:

   TAVTparCal();
    virtual ~TAVTparCal();

   //! Read from file
   Bool_t             FromFile(const TString& name = "");
    
   //! Clear
   virtual void       Clear(Option_t* opt="");
   
   //! Stream output
   virtual void       ToStream(ostream& os = cout, Option_t* option = "") const;
   
   //! Get Lnadau parameter
   LandauParameter_t& GetLandauPar(Int_t idx) {return fLandauParameter[idx];}
   
   //! Get charge probabilities for a given number of pixel
   const TArrayF*     GetChargeProba(Float_t pixelsN);
   
   //! Get charge normalized (integral) probabilities for a given number of pixel
   const TArrayF*     GetChargeProbaNorm(Float_t pixelsN);
   
   //! Get charge with max probability 
   Int_t              GetChargeWithMaxProba()     const { return fChargeWithMaxProba; }
   
   //! Get charge max probability 
   Float_t            GetChargeMaxProba()         const { return fChargeMaxProba; }
   
   //! Get charge with max probability (Normalized)
   Int_t              GetChargeWithMaxProbaNorm() const { return fChargeWithMaxProbaNorm; }
   
   //! Get charge max probability (Normalized)
   Float_t            GetChargeMaxProbaNorm()     const { return fChargeMaxProbaNorm; }
   
   //! Get Pointer to Landau Norm
   TF1*               GetLandauNormFunc(Int_t idx)    const { return fLandauNorm[idx]; }
   
   //! Get Pointer to Landau
   TF1*               GetLandauFunc(Int_t idx)    const { return fLandau[idx]; }

   //! Get Pointer to total Landau (z < 6) 
   TF1*               GetLandauTot()              const { return fLandauTot; }

   //! Quenched Landau function
   Double_t           QLandau(Double_t *x, Double_t *par);
   
   //! Quenched Landau function
   Double_t           QLandauNorm(Double_t *x, Double_t *par);

   
   //! Total quenched Landau function
   Double_t           QLandauTot(Double_t *x, Double_t *par);
   
public:
   static const Char_t* GetDefaultCalName()      { return fgkDefaultCalName.Data(); }

   ClassDef(TAVTparCal,1)
};

#endif

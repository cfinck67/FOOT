/*!
  \file
  \version $Id: TAITparCal.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TAITparCal.
*/

#include <Riostream.h>

#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TCanvas.h"

#include "TAITparMap.hxx"
#include "TAITparCal.hxx"

//##############################################################################

/*!
  \class TAITparCal TAITparCal.hxx "TAITparCal.hxx"
  \brief Charge (raw) calibration for vertex. **
*/

ClassImp(TAITparCal);

TString TAITparCal::fgkDefaultCalName = "./config/TAITdetector.cal";
Int_t   TAITparCal::fgkChargesN       = 6;

//------------------------------------------+-----------------------------------
TAITparCal::TAITparCal()
: TAGparTools(),
  fChargeProba(new TArrayF(6)),
  fChargeProbaNorm(new TArrayF(6)),
  fChargeWithMaxProba(0),
  fChargeMaxProba(0.)
{
  // Standard constructor
}

//------------------------------------------+-----------------------------------
TAITparCal::~TAITparCal()
{
   // Destructor
   delete fChargeProba;
   delete fChargeProbaNorm;
   for (Int_t p = 0; p < fgkChargesN; p++) {
	  delete fLandau[p];
	  delete fLandauNorm[p];
   }
   delete fLandauTot;
}

//------------------------------------------+-----------------------------------
Bool_t TAITparCal::FromFile(const TString& name) 
{   
   // Reading calibration file
   TString nameExp;
   
   if (name.IsNull())
     nameExp = fgkDefaultCalName;
   else 
     nameExp = name;
   
   if (Open(nameExp)) return kTRUE;
   
   Float_t* tmp = new Float_t[4];
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
     
	  // read parameters
	  ReadItem(tmp, 4, ' ');
	  fLandauParameter[p].Charge   = p+1;
	  fLandauParameter[p].Constant = tmp[0];
	  fLandauParameter[p].MPV      = tmp[1];
	  fLandauParameter[p].Sigma    = tmp[2];
	  fLandauParameter[p].Quench   = tmp[3];

	  if(fDebugLevel)
		 cout << endl << " Landau Parameter: " 
		      << Form("%d %f %f %f %f", fLandauParameter[p].Charge, fLandauParameter[p].Constant, 
				 fLandauParameter[p].MPV, fLandauParameter[p].Sigma, fLandauParameter[p].Quench) << endl;
   }	  
   delete[] tmp;
   
   SetFunction();
   
   return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Get Proba
const TArrayF* TAITparCal::GetChargeProba(Float_t pixelsN)
{
   Float_t value[fgkChargesN];
   Float_t fac = 0.;
   Float_t maxProba = 0.;
   
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
	  value[p] = fLandau[p]->Eval(pixelsN);
	  fac += value[p];
   }
   
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
	  Float_t ratio = value[p]/fac*100;
	  fChargeProba->AddAt(ratio, p);
	  if (ratio > maxProba) {
		 maxProba = ratio;
		 fChargeWithMaxProba = p+1;
		 fChargeMaxProba = ratio;
	  }
   }
   
   return fChargeProba;
}

//------------------------------------------+-----------------------------------
//! Get Proba
const TArrayF* TAITparCal::GetChargeProbaNorm(Float_t pixelsN)
{
   Float_t value[fgkChargesN];
   Float_t fac = 0.;
   Float_t maxProba = 0.;
   
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
	  value[p] = fLandauNorm[p]->Eval(pixelsN);
      fac += value[p];
   }
   
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
	  Float_t ratio = value[p]/fac*100;
	  fChargeProbaNorm->AddAt(ratio, p);
	  if (ratio > maxProba) {
		 maxProba = ratio;
		 fChargeWithMaxProbaNorm = p+1;
		 fChargeMaxProbaNorm = ratio;
	  }
   }
   
   return fChargeProbaNorm;
}

//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TAITparCal::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAITparCal::ToStream(ostream& os, Option_t*) const
{
//  os << "TAITparCal " << GetName() << endl;
//  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
//     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}

//------------------------------------------+-----------------------------------
//! Set up quenched Landau parameters
void TAITparCal::SetFunction()
{
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
	  fLandau[p] = new TF1(Form("landau%d", p), this, &TAITparCal::QLandau, 0, 22, 4, "TAITparCal", "QLandau");
	  fLandau[p]->SetParameters(fLandauParameter[p].Constant, fLandauParameter[p].MPV, fLandauParameter[p].Sigma, 
								fLandauParameter[p].Quench);
	  fLandau[p]->SetParNames("Constant", "MPV", "Sigma", "Quench");
   }
   
   Float_t integral0 = 0.;
   Float_t integral1 = 0.;
   for (Int_t p = 0; p < fgkChargesN; p++) { // Loop on each charge
      fLandauNorm[p] = new TF1(Form("landauNorm%d", p), this, &TAITparCal::QLandauNorm, 0, 22, 4, "TAITparCal", "QLandauNorm");
      fLandauNorm[p]->SetParameters(1, fLandauParameter[p].MPV, fLandauParameter[p].Sigma, fLandauParameter[p].Quench);
      fLandauNorm[p]->SetParNames("Constant", "MPV", "Sigma", "Quench");

      integral0 = fLandauNorm[0]->Integral(0,22);
      integral1 = fLandauNorm[p]->Integral(0,22);
      
      Float_t ratio = integral0/integral1;
      fLandauNorm[p]->SetParameter(0, ratio);
   }
   
   fLandauTot = new TF1("landauTot", this, &TAITparCal::QLandauTot, 0, 22, 0, "TAITparCal", "QLandauTot");
}

//------------------------------------------+-----------------------------------
//! Quenched Landau
Double_t TAITparCal::QLandau(Double_t* x, Double_t* par)
{
   Float_t xx = (x[0]-par[1])/par[2];
   Double_t f = 1./TMath::Sqrt(2*TMath::Pi())*par[0]*TMath::Exp(-0.5*(xx + par[3]*TMath::Exp(-xx)));
   return f;   
}

//------------------------------------------+-----------------------------------
//! Quenched Landau
Double_t TAITparCal::QLandauNorm(Double_t* x, Double_t* par)
{
   Float_t xx = (x[0]-par[1])/par[2];
   Double_t f = 1./TMath::Sqrt(2*TMath::Pi())*par[0]*TMath::Exp(-0.5*(xx + par[3]*TMath::Exp(-xx)));
   return f;
}


//------------------------------------------+-----------------------------------
//! Total Quenched Landau
Double_t TAITparCal::QLandauTot(Double_t* x, Double_t* /*par*/)
{
   Float_t xx = x[0];
   Double_t f = 0;
   
   for (Int_t i = 0; i < fgkChargesN-1; ++i) {
	  f += fLandau[i]->Eval(xx);
   }
   
   return f;   
}



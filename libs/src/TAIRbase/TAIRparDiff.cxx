
#include <Riostream.h>

#include "TMath.h"
#include "TString.h"
#include "TF1.h"

#include <map>

#include "TAIRparDiff.hxx"
#include "TAGparGeo.hxx"

//##############################################################################

/*!
 \class TAIRparDiff TAIRparDiff.hxx "TAIRparDiff.hxx"
 \brief  Functions for scattering calculation. **
 */

ClassImp(TAIRparDiff);

const Float_t TAIRparDiff::fgkProtonMass   = 931.;
//_____________________________________________________________________________
//
// Default constructor
TAIRparDiff::TAIRparDiff(TAGparaDsc* pGeoMap)
: TAGpara(),
  fAlpha(0),
  fPfactor(0),
  fpGeoMap(pGeoMap)
{
   SetMaterials();
   
   fFuncSigTheta = new TF1("funcSigTheta", this, &TAIRparDiff::SigmaTheta, 0, 500, 5, "TAIRparDiff", "SigmaTheta");
}

//_____________________________________________________________________________
//
// Destructor
TAIRparDiff::~TAIRparDiff()
{
}

//_____________________________________________________________________________
//
// List of materials parameters
//_____________________________________________________________________________
//
void TAIRparDiff::SetMaterials()
{
   TString name;
   MaterialParameter_t  materialParameter;
   
   name = "Air";
   materialParameter.RadiationLength = 36.2;
   materialParameter.Density = 0.00129;
   fListMaterials[name] = materialParameter;
   
   name = "Si";
   materialParameter.RadiationLength = 21.82;
   materialParameter.Density = 2.33;
   fListMaterials[name] = materialParameter;
   
   name = "Sc.";
   materialParameter.RadiationLength = 43.8;
   materialParameter.Density = 1.032;
   fListMaterials[name] = materialParameter;
   
   name = "H2O";
   materialParameter.RadiationLength = 36.08;
   materialParameter.Density = 1;
   fListMaterials[name] = materialParameter;
   
   name = "Water";
   materialParameter.RadiationLength = 36.08;
   materialParameter.Density = 1;
   fListMaterials[name] = materialParameter;
   
   name = "PMMA";
   materialParameter.RadiationLength = 40.26;
   materialParameter.Density = 0.95;
   fListMaterials[name] = materialParameter;
   
   name = "Ti";
   materialParameter.RadiationLength = 16.17;
   materialParameter.Density = 4.506;
   fListMaterials[name] = materialParameter;
   
   return;
}

//_____________________________________________________________________________
//
// Calculation of the WEPL of the material layer
Float_t TAIRparDiff::WEPLCalc(const TString& mat, Float_t thickness)
{
   
   Float_t factor = 0;
   Float_t waterEq = 0;
   
   TString material(mat);
   
   material.ToUpper();
   
   if (material == "AIR"){
      factor = 0.001045298;
   } else if (material == "SI"){
      factor = 1.833962703;
   } else if (material == "12C"){
      factor = 1.991445766;
   } else if (material == "PMMA"){
      factor = 0.925313636;
   } else if (material == "H2O" || material == "WATER"){
      factor = 1.;
   } else if (material == "TI"){
   	  factor = 3.136600294;
  	} else if (material == "SC."){
   	  factor = 1.017862901;
   } else Warning("WEPLCalc()","Material is not in the list.... Candidates: Air, Si, 12C, PMMA, H2O, Ti, Sc. .");
   
   waterEq = thickness * factor;

   return waterEq;
   
}

//_____________________________________________________________________________
//
// Calculation of the energy loss in the material layer (WEPL in [cm])
Float_t TAIRparDiff::EnergyCalc(Float_t energy, Float_t massNumber, Int_t atomicNumber, Float_t WEPL)
{
   if (energy < 250){
      fAlpha = 0.0022;
      fPfactor = 1.77;
   } else if ((energy >= 250) && (energy < 400)){
      fAlpha = 0.0022;
      fPfactor = 1.76;
   } else {
      fAlpha = 0.0022;
      fPfactor = 1.75;
   }
   
   Float_t range = (massNumber / (atomicNumber*atomicNumber) * fAlpha * pow(energy, fPfactor));
   Float_t path = range - WEPL;
   Float_t dE = pow((path * atomicNumber * atomicNumber / (fAlpha * massNumber)), (1/fPfactor));
   energy = dE;
   if (path < 0) {
      Info("EnergyCalc()","The remaining energy is 0....");
      energy = 0;
   }
   
   return energy;
}

//_____________________________________________________________________________
//
// Calculation of the impact*c [MeV]
Float_t TAIRparDiff::PCCalc(Float_t energy, Float_t massNumber)
{
   Float_t pc = sqrt(energy * energy + 2*energy * fgkProtonMass) * massNumber ;
   
   return pc;
}

//_____________________________________________________________________________
//
// Calculation of beta
Float_t TAIRparDiff::BetaCalc(Float_t energy)
{
   Float_t beta = sqrt(1.0 - (1/(energy/fgkProtonMass +1.0))*(1/(energy/fgkProtonMass +1.0)));
   
   return beta;
}

// --------------------------------------------------------------------------------------
Double_t TAIRparDiff::SigmaTheta(Double_t *x, Double_t *par)
{
   /// Function to modelize the scattering angle of incident ions at a depth x (Highlands)
   /// par[0]=beta of particule
   /// par[1]=momentum MeV/c2
   /// par[2]=z of the beam
   /// par[3]=radiation length of the target material
   /// par[4]=density of target material
   /// x[0] in cm
   
   Double_t radL = x[0]*par[4]/par[3];
   Double_t A    = 14.1*par[2]/(par[0]*par[1]);
   Double_t B    = TMath::Sqrt(radL)*(1+TMath::Log10(radL)/9.);
   
   return A*B*TMath::RadToDeg();
}

// --------------------------------------------------------------------------------------
Float_t TAIRparDiff::SigmaThetaCalc(Float_t energy, TString mat, Float_t x)
{
   TAGparGeo* pGeoMap = (TAGparGeo*) fpGeoMap->Object();
   
   Float_t Abeam = pGeoMap->GetBeamPar().AtomicMass;
   Float_t zbeam = pGeoMap->GetBeamPar().AtomicNumber;
   
   Double_t pc   = PCCalc(energy, Abeam);
   Double_t beta = BetaCalc(energy);
   Double_t radL = GetRadLength(mat);  // [g/cm^2] Radiation length for material
   Double_t rho  = GetDensity(mat);    // [g/cm^3] density of material

   fFuncSigTheta->SetParameter(0, beta);
   fFuncSigTheta->SetParameter(1, pc);
   fFuncSigTheta->SetParameter(2, zbeam);
   fFuncSigTheta->SetParameter(3, radL);
   fFuncSigTheta->SetParameter(4, rho);
   
   return  fFuncSigTheta->Eval(x);
}

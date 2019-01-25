//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************

#include "TRandom3.h"
#include "TF1.h"
#include "TMath.h"
#include "TRandom.h"

#include "TATWdigitizer.hxx"

#include "TAGroot.hxx"

ClassImp(TATWdigitizer)


Float_t TATWdigitizer::fgHfactor = 1.45;

// --------------------------------------------------------------------------------------
TATWdigitizer::TATWdigitizer(TATW_ContainerHit* pNtuRaw)
 : TAGbaseDigitizer(),
   fpNtuRaw(pNtuRaw),
   fDeResECst(680e4),
   fDeErrResCst(1e4),
   fDeResEC(9.9),
   fDeErrResEC(0.4),
   fDeAttCstLeft(10),
   fDeErrAttCstLeft(0.2),
   fDeAttLambdaLeft(12.1),
   fDeErrAttLambdaLeft(0.5),
   fDeAttCstRight(10),
   fDeErrAttCstRight(0.2),
   fDeAttLambdaRight(10.7),
   fDeErrAttLambdaRight(0.3),
   fDeAttAsym(0.09),
   fDeAttAsymSmear(0.),
   fTofCstE(1000),
   fTofErrCstE(0.1),
   fTofLambdaE(2.50),
   fTofErrLambdaE(0.1),
   fTofk0E(140),
   fTofErrk0E(18),
   fTofPropAlpha(280/2.), // velocity of the difference need to divide by 2 (ps/cm)
   fTofErrPropAlpha(2.5), // old 5 ?
   fSlatLength(0),
   fGain(3e4)
{
   SetFunctions();
   SetInitParFunction();
   
   fpParGeo = (TATWparGeo*) gTAGroot->FindParaDsc(TATWparGeo::GetDefParaName(), "TATWparGeo")->Object();

   fSlatLength = fpParGeo->GetBarDimension().Z();
}

// --------------------------------------------------------------------------------------
void  TATWdigitizer::SetFunctions()
{
   fDeResE     = new TF1("ResEnergy", this, &TATWdigitizer::ResEnergy, 0, 10, 2, "TATWdigitizer", "ResEnergy");
   fTofResE    = new TF1("ResDiffTime", this, &TATWdigitizer::ResToF, 0, 2.5, 3, "TATWdigitizer", "ResToF");
   
   fDeAttLeft  = new TF1("DeAttLeft", this, &TATWdigitizer::DeAttLeft,   -fSlatLength/2, fSlatLength/2., 3, "TATWdigitizer", "DeAttLeft");
   fDeAttRight = new TF1("DeAttRight", this, &TATWdigitizer::DeAttRight, -fSlatLength/2, fSlatLength/2., 3, "TATWdigitizer", "DeAttRight");

}

// --------------------------------------------------------------------------------------
void  TATWdigitizer::SetInitParFunction()
{
   //all in cm, ps and MeV
   fDeResE->SetParameters(fDeResECst, fDeResEC); // fig9
   fTofResE->SetParameters(fTofCstE, fTofLambdaE, fTofk0E); // fig7
   
   fDeAttLeft->SetParameters(fDeAttCstLeft, fDeAttLambdaLeft, fSlatLength/2.); // fig8
   fDeAttRight->SetParameters(fDeAttCstRight, fDeAttLambdaRight, fSlatLength/2.); // fig8
}

// --------------------------------------------------------------------------------------
TATWdigitizer::~TATWdigitizer()
{   
   delete fDeResE;
   delete fTofResE;
   delete fDeAttLeft;
   delete fDeAttRight;
}

// --------------------------------------------------------------------------------------
Double_t TATWdigitizer::DeAttLeft(Double_t* x, Double_t* par)
{
   Float_t xx  = x[0];
   Float_t att = par[0]*TMath::Exp(-(par[2]+xx)/par[1]);
   
   return att;
}

// --------------------------------------------------------------------------------------
Double_t TATWdigitizer::DeAttRight(Double_t* x, Double_t* par)
{
   Float_t xx  = x[0];
   Float_t att = par[0]*TMath::Exp(-(par[2]-xx)/par[1]);
   
   return att;
}


// --------------------------------------------------------------------------------------
Double_t TATWdigitizer::ResEnergy(Double_t* x, Double_t* par)
{
   Float_t energy = x[0];
   Float_t res = TMath::Sqrt(par[0]/energy + par[1]*par[1]);
   
   res /= 100*TMath::Sqrt(2.); // %<-
   
   return res;
}

// --------------------------------------------------------------------------------------
Double_t TATWdigitizer::ResToF(Double_t* x, Double_t* par)
{
   Float_t deltaE = x[0];
   Float_t res    = par[0]*TMath::Exp(-deltaE*par[1]) + par[2];

   return res;
}

//___________________________________________________________________________________________
Float_t TATWdigitizer::GetDeAttLeft(Float_t pos, Float_t edep)
{
   Float_t energy = edep*fGain; // share equitable
   energy *= (1-fDeAttAsymSmear);
   
   Float_t lambda = gRandom->Gaus(fDeAttLambdaLeft, fDeErrAttLambdaLeft);
   
   fDeAttLeft->SetParameter(0, energy);
   fDeAttLeft->SetParameter(1, lambda);
   
   return fDeAttLeft->Eval(pos);
}

//___________________________________________________________________________________________
Float_t TATWdigitizer::GetDeAttRight(Float_t pos, Float_t edep)
{
   Float_t energy = edep*fGain; // share equitable
   energy *= (1+fDeAttAsymSmear);
   
   Float_t lambda = gRandom->Gaus(fDeAttLambdaRight, fDeErrAttLambdaRight);
   
   fDeAttRight->SetParameter(0, energy);
   fDeAttRight->SetParameter(1, lambda);
   
   return fDeAttRight->Eval(pos);
}

//___________________________________________________________________________________________
Float_t TATWdigitizer::GetResEnergy(Float_t energy)
{
   Float_t C = gRandom->Gaus(fDeResEC, fDeErrResEC);
   fDeResE->SetParameter(1, C);
   
   return fDeResE->Eval(energy);
}

//___________________________________________________________________________________________
Float_t TATWdigitizer::GetResToF(Float_t edep)
{
   // missing error on parameter (no fit on fig 7)
   return fTofResE->Eval(edep);
}

//___________________________________________________________________________________________
Float_t TATWdigitizer::GetTofLeft(Float_t pos, Float_t time, Float_t edep)
{
   // figure 11 assuming same as right
   Float_t alpha  = gRandom->Gaus(fTofPropAlpha, fTofErrPropAlpha);
   Float_t timeL  = time + (fSlatLength/2. - pos)*alpha;
   Float_t resToF = GetResToF(edep)*TMath::Sqrt(2.); // share same way L/R
   
   timeL += gRandom->Gaus(0, resToF);
   
   return timeL;
}

//___________________________________________________________________________________________
Float_t TATWdigitizer::GetTofRight(Float_t pos, Float_t time, Float_t edep)
{
   // figure 11
   Float_t alpha  = gRandom->Gaus(fTofPropAlpha, fTofErrPropAlpha);
   Float_t timeR  = time + (fSlatLength/2. + pos)*alpha;
   Float_t resToF = GetResToF(edep)*TMath::Sqrt(2.);
   
   timeR += gRandom->Gaus(0, resToF);
   
   return timeR;
}

//___________________________________________________________________________________________
Bool_t TATWdigitizer::Process(Double_t edep, Double_t x0, Double_t y0, Double_t /*zin*/, Double_t /*zout*/, Double_t time, Int_t id )
{
   // energy resolution
   fDeAttAsymSmear = gRandom->Uniform(-fDeAttAsym, +fDeAttAsym); // asymmetry btw left/right ends
   Double_t pos     = 0;

   if (fDebugLevel) {
      printf("asym %4.2f\n", fDeAttAsymSmear);
      printf("edep %f\n", edep);
   }
   
   Int_t view = -1;
   if (id < TATWparGeo::GetLayerOffset()) {
      pos = y0;
      view = 0;
   } else {
      pos = x0;
      view = 1;
      id -= TATWparGeo::GetLayerOffset();
   }
   
   Float_t energyL = GetDeAttLeft(pos, edep);
   Float_t energyR = GetDeAttRight(pos, edep);
   
   Float_t resEnergyL = GetResEnergy(energyL);
   energyL += energyL*gRandom->Gaus(0, resEnergyL);
   
   Float_t resEnergyR = GetResEnergy(energyR);
   energyR += energyR*gRandom->Gaus(0, resEnergyR);

   if (fDebugLevel) {
      printf("pos %.1f\n", pos);
      printf("energy %.1f %.1f\n", energyL, energyR);
      printf("Res %.3f %.3f\n", resEnergyL*100, resEnergyR*100);
   }
   
   // time resolution
   Float_t timeL = GetTofLeft(pos, time, edep);
   Float_t timeR = GetTofRight(pos, time, edep);
   
   if (fDebugLevel) {
      printf("time %.1f\n", time);
      printf("time %.1f %.1f\n", timeL, timeR);
   }
   
   Double_t tof = (timeL+timeR)/2.;
   Double_t energy = (energyL+energyR)/2.;
   
   pos = (timeR-timeL)/fTofPropAlpha;
   
   // no threshold ??
   fCurrentHit = (TATW_Hit*)fpNtuRaw->NewHit(view, id, energy, tof, pos); // save time in ns, Class TATW_Hit not compatible with real data)
   
   return true;
}


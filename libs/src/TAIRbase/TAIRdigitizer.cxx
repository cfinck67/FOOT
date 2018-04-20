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

#include "TAIRntuHit.hxx"
#include "TAIRparGeo.hxx"
#include "TAIRdigitizer.hxx"

// --------------------------------------------------------------------------------------
TAIRdigitizer::TAIRdigitizer(EVENT_STRUCT* pNtuHitsMC, TAIRntuHit* pNtuRaw, TAIRparGeo* pParGeo)
 : TAGbaseDigitizer(),
   fpEvtStr(pNtuHitsMC),
   fpNtuRaw(pNtuRaw),
   fpParGeo(pParGeo),
   fResSig(0.0707), // ns
   fErrResSig(3e-7),
   fTimePar1(-51),
   fTimePar2(11077),
   fTimeErrPar1(3),
   fTimeErrPar2(502),
   fGain(50)
{
   SetFunctions();
}

// --------------------------------------------------------------------------------------
void  TAIRdigitizer::SetFunctions()
{
   // compute birks law for a given Edep
   fTimeWalk = new TF1("TimeWalk", this, &TAIRdigitizer::TimeWalk, 0, 1000, 2, "TAIRdigitizer", "TimeWalk");
}

// --------------------------------------------------------------------------------------
TAIRdigitizer::~TAIRdigitizer()
{   
   delete fTimeWalk;
}

// --------------------------------------------------------------------------------------
Double_t TAIRdigitizer::TimeWalk(Double_t* x, Double_t* par)
{
   Float_t charge = x[0];
   Float_t walk = par[1]/((charge-par[0])*(charge-par[0]));
   
   return walk;
}

//___________________________________________________________________________________________
Float_t TAIRdigitizer::GetTimeWalk(Float_t edep)
{
   Float_t par1 = gRandom->Gaus(fTimePar1, fTimeErrPar1);
   Float_t par2 = gRandom->Gaus(fTimePar2, fTimeErrPar2);
   
   fTimeWalk->SetParameters(par1, par2); // fig 5 JINST7

   return fTimeWalk->Eval(edep);
}

//___________________________________________________________________________________________
Bool_t TAIRdigitizer::Process(Int_t id, Int_t mcId, Double_t edep, Double_t glolTime)
{
   Float_t walk = GetTimeWalk(edep);
   
   Float_t time = glolTime + walk;
   time += gRandom->Gaus(0, fResSig); // not use fErrResSig too small, fig 6 JINST7
   
   if (edep > fgThresholds[2]) {
      TAIRhit* hit = (TAIRhit*)fpNtuRaw->NewHit(id, edep, time);
      hit->AddMcTrackId(mcId);
   }
   
   return true;
}

//-----------------------------------------------------------------------------------------
void TAIRdigitizer::Digitize()
{
   Int_t nHits = fpEvtStr->STCn;
   for (Int_t i  = 0; i < nHits; ++i) {
      Int_t trackId  = fpEvtStr->STCid[i];
      Float_t edep   = fpEvtStr->STCde[i]*fGain;
      Float_t time   = fpEvtStr->STCtim[i];

      Process(i, trackId, edep, time);
   }
}


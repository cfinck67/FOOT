/*!
 \file
 \version $Id: TATWactNtuPoint.cxx $
 \brief   Implementation of TATWactNtuPoint.
 */
#include "TClonesArray.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"

#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

#include "TATWparGeo.hxx"
#include "TATWparCal.hxx"
#include "TATW_ContainerHit.hxx"
#include "TATW_ContainerPoint.hxx"

#include "TATWactNtuPoint.hxx"

/*!
 \class TATWactNtuPoint 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TATWactNtuPoint);

//------------------------------------------+-----------------------------------
//! Default constructor.
TATWactNtuPoint::TATWactNtuPoint(const char* name, 
									 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuPoint, TAGparaDsc* pGeoMap, TAGparaDsc* pCalMap)
 : TAGaction(name, "TATWactNtuCluster - NTuplize cluster"),
   fpNtuRaw(pNtuRaw),
   fpNtuPoint(pNtuPoint),
   fpGeoMap(pGeoMap),
   fpCalMap(pCalMap)
{
   AddDataIn(pNtuRaw,   "TATW_ContainerHit");
   AddDataOut(pNtuPoint, "TATW_ContainerPoint");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TATWactNtuPoint::~TATWactNtuPoint()
{
   
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TATWactNtuPoint::CreateHistogram()
{
   DeleteHistogram();
   
   fpHisDist = new TH1F("twDist", "ToF Wall - Minimal distance between planes", 200, 0., 100);
   AddHistogram(fpHisDist);
  
   fpHisCharge1 = new TH1F("twCharge1", "ToF Wall - Charge layer 1", 1000, 0., 5e6);
   AddHistogram(fpHisCharge1);
   
   fpHisCharge2 = new TH1F("twCharge2", "ToF Wall - Charge layer 2", 1000, 0., 5e6);
   AddHistogram(fpHisCharge2);
   
   fpHisChargeTot = new TH1F("twChargeTot", "ToF Wall - Total charge", 1000, 0., 5e6);
   AddHistogram(fpHisChargeTot);
  
   SetValidHistogram(kTRUE);
}

//______________________________________________________________________________
//
Bool_t TATWactNtuPoint::Action()
{
   Bool_t ok = FindPoints();

   fpNtuPoint->SetBit(kValid);
   
   return true;
}

//______________________________________________________________________________
//  
Bool_t TATWactNtuPoint::FindPoints()
{
   TATW_ContainerHit* pNtuHit      = (TATW_ContainerHit*) fpNtuRaw->Object();
   TATW_ContainerPoint* pNtuPoint  = (TATW_ContainerPoint*) fpNtuPoint->Object();
   TATWparGeo* pGeoMap             = (TATWparGeo*) fpGeoMap->Object();
   TATWparCal* pCalMap             = (TATWparCal*) fpCalMap->Object();

   Float_t minDist = 99999;
   Int_t minId = -1;
   Bool_t best = false;
   
   Int_t layer1 = 0;
   Int_t nHits1 = pNtuHit->GetHitN(layer1);
   if (nHits1 == 0) return false;
   
   Int_t layer2 = 1;
   Int_t nHits2 = pNtuHit->GetHitN(layer2);
   if (nHits2 == 0) return false;

   for (Int_t i = 0; i < nHits1; ++i) {
      minDist = 99999; // should put a given value (2*BarWidth ?)
      
      TATW_Hit* hit1 = pNtuHit->GetHit(layer1, i);
      
      Int_t bar1 = hit1->GetBar();
      Float_t x  = pGeoMap->GetBarPosition(layer1, bar1)[0];
      Float_t y  = hit1->GetPosition();
      
      TVector2 pos1(x, y);
      
      best = false;
      for (Int_t j = 0; j < nHits2; ++j) {
         
         TATW_Hit* hit2 = pNtuHit->GetHit(layer2, j);
         
         Int_t bar2 = hit2->GetBar();
         Float_t y  = pGeoMap->GetBarPosition(layer2, bar2)[1];
         Float_t x  = hit2->GetPosition();
         
         TVector2 pos2(x, y);
         Float_t dist = (pos2 - pos1).Mod();
         
         if (dist < minDist) {
            minDist = dist;
            minId   = j;
            best    = true;
         }
      }
      
      if (best) {
         TATW_Hit* hitmin = pNtuHit->GetHit(1, minId);
         
         Int_t bar2   = hitmin->GetBar();
         Float_t xmin = pGeoMap->GetBarPosition(0, bar1)[0];
         Float_t ymin = pGeoMap->GetBarPosition(1, bar2)[1];
      
         TATW_Point* point = pNtuPoint->NewPoint(xmin, hit1, ymin, hitmin);
         
         Int_t Z = pCalMap->GetChargeZ(point->GetEnergyLoss1());
         point->SetChargeZ(Z);
	 
	 if (ValidHistogram()) {
	   fpHisDist->Fill(minDist);
	   fpHisCharge1->Fill(point->GetEnergyLoss1());
	   fpHisCharge2->Fill(point->GetEnergyLoss2());
	   fpHisChargeTot->Fill(point->GetEnergyLoss());
	 }
      }
   }

   return true;
}






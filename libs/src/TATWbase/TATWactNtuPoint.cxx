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
									 TAGdataDsc* pNtuRaw, TAGdataDsc* pNtuPoint, TAGparaDsc* pGeoMap)
 : TAGaction(name, "TATWactNtuCluster - NTuplize cluster"),
   fpNtuRaw(pNtuRaw),
   fpNtuPoint(pNtuPoint),
   fpGeoMap(pGeoMap)
{
   AddDataIn(pNtuRaw,   "TATW_ContainerHit");
   AddDataOut(pNtuPoint, "TATW_ContainerPoint");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TATWactNtuPoint::~TATWactNtuPoint()
{
   
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

   Float_t minDist = 99999;
   Int_t minId = -1;
   Bool_t best = false;
   
   Int_t layer = 0;
   Int_t nHits1 = pNtuHit->GetHitN(layer);
   if (nHits1 == 0) return false;
   
   layer = 1;
   Int_t nHits2 = pNtuHit->GetHitN(layer);
   if (nHits2 == 0) return false;

   for (Int_t i = 0; i < nHits1; ++i) {
      minDist = 99999; // should put a given value (2*BarWidth ?)
      layer = 0;
      TATW_Hit* hit1 = pNtuHit->GetHit(layer, i);
      
      Int_t bar1 = hit1->GetBar();
      
      Float_t x = pGeoMap->GetBarPosition(layer, bar1)[0];
      Float_t y = hit1->GetPosition();
      
      TVector2 pos1(x, y);
      
      best = false;
      for (Int_t j = 0; j < nHits2; ++j) {
         layer = 1;
         TATW_Hit* hit2 = pNtuHit->GetHit(layer, j);
         
         Int_t bar2 = hit2->GetBar();
         
         Float_t y = pGeoMap->GetBarPosition(layer, bar2)[1];
         Float_t x = hit2->GetPosition();
         

         TVector2 pos2(x, y);
         Float_t dist = (pos2 - pos1).Mod();
         
         if (dist < minDist) {
            minDist = dist;
            minId = j;
            best = true;
         }
      }
      
      if (best) {
         TATW_Hit* hitmin = pNtuHit->GetHit(1, minId);
         Int_t bar2 = hitmin->GetBar();

         Float_t xmin = pGeoMap->GetBarPosition(0, bar1)[0];
         Float_t ymin = pGeoMap->GetBarPosition(1, bar2)[1];
      
         pNtuPoint->NewPoint(xmin, hit1, ymin, hitmin);
      }
   }

   return true;
}






/*!
 \file
 \version $Id: TAVTactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAVTactNtuMC.
 */

#include <map>

#include "TH2F.h"

#include "TAVTparGeo.hxx"
#include "TAVTparMap.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuRaw.hxx"
#include "TAVTdigitizer.hxx"
#include "TAVTactNtuMC.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"


#include "GlobalPar.hxx"

/*!
 \class TAVTactNtuMC"
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAVTactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactNtuMC::TAVTactNtuMC(const char* name,
                           TAGdataDsc* pNtuRaw,
                           TAGparaDsc* pGeoMap,
                           TAGparaDsc* pParMap,
                           EVENT_STRUCT* evStr)
: TAGaction(name, "TAVTactNtuMC - NTuplize Vertex MC data"),
  fpNtuRaw(pNtuRaw),
  fpGeoMap(pGeoMap),
  fpParMap(pParMap),
  fpEvtStr(evStr),
  fDigitizer(new TAVTdigitizer(pGeoMap)),
  fDebugLevel(0)
{
   AddDataOut(pNtuRaw, "TAVTntuRaw");
   AddPara(pGeoMap, "TAVTparGeo");
   AddPara(pParMap, "TAVTparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactNtuMC::~TAVTactNtuMC()
{
   delete fDigitizer;
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactNtuMC::CreateHistogram()
{
   DeleteHistogram();
   
   TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();
   
   for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
      fpHisPixel[i] = new TH1F(Form("vtMcPixel%d", i+1), Form("Vertex - MC # pixels per clusters for sensor %d", i+1), 100, 0., 100.);
      AddHistogram(fpHisPixel[i]);
   }
   
   
   for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
      if (TAVTparConf::IsMapHistOn()) {
         fpHisPixelMap[i]  = new TH2F(Form("vtMcPixelMap%d", i+1) , Form("Vertex - pixel map for sensor %d", i+1),
                                      pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX(),
                                      pGeoMap->GetNPixelY(), 0, pGeoMap->GetNPixelY());
         fpHisPixelMap[i]->SetStats(kFALSE);
         AddHistogram(fpHisPixelMap[i]);
      }
   }
   
   for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
      if (TAVTparConf::IsMapHistOn()) {
         fpHisPosMap[i] =  new TH2F(Form("vtMcPosMap%d", i+1), Form("Vertex - position map for sensor %d", i+1),
                                    pGeoMap->GetNPixelX(), -pGeoMap->GetPitchX()/2*pGeoMap->GetNPixelX(), pGeoMap->GetPitchX()/2*pGeoMap->GetNPixelX(),
                                    pGeoMap->GetNPixelY(), -pGeoMap->GetPitchY()/2*pGeoMap->GetNPixelY(), pGeoMap->GetPitchY()/2*pGeoMap->GetNPixelY());
         fpHisPosMap[i]->SetStats(kFALSE);
         AddHistogram(fpHisPosMap[i]);
      }
      
   }
   
   SetValidHistogram(kTRUE);
   return;
}


//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAVTactNtuMC::Action()
{
   
   TAVTntuRaw* pNtuRaw = (TAVTntuRaw*) fpNtuRaw->Object();
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   
   
   TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   if (fGeoTrafo == 0x0)
      Error("SetGeoTrafoName","No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());
   
   pNtuRaw->Clear();
   
   Int_t nPixelX = pGeoMap->GetNPixelX();
   
   
   if (fDebugLevel)     Info("Action()","Processing n :: %2d hits \n",fpEvtStr->VTXn);
   if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "VTXn   " << fpEvtStr->VTXn<< endl;
   
   
   for (Int_t i = 0; i < fpEvtStr->VTXn; i++) {
      if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;
      
      
      // Generated particle ID
      Int_t genPartID = fpEvtStr->VTXid[i] - 1;
      
      if ( GlobalPar::GetPar()->Debug() > 0 )     {
         cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;
         cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
         cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
      }
      
      Int_t layer = fpEvtStr->VTXilay[i] ;
      
      Int_t sensorId = layer;
      
      
      // set geometry // why ???
      //  pixel->SetVtxGeo(pGeoMap);
      
      
      Double_t eloss = fpEvtStr->VTXde[i];
      Double_t x     = (fpEvtStr->VTXxin[i]+fpEvtStr->VTXxout[i])/2;
      Double_t y     = (fpEvtStr->VTXyin[i]+fpEvtStr->VTXyout[i])/2;
      
      if (!fDigitizer->Process(eloss, x, y)) continue;
      std::map<int, int> map = fDigitizer->GetMap();
      
      // fill pixels from map
      std::map<int,int>::iterator it;
      
      if ( GlobalPar::GetPar()->Debug() > 0 )
         printf("x %.1f y %.1f\n", x, y);
      
      for (it = map.begin(); it != map.end(); ++it) {
         if (map[it->first] == 1) {
            Int_t line = it->first / nPixelX;
            Int_t col  = it->first % nPixelX;
            TAVTntuHitMC* pixel = (TAVTntuHitMC*)pNtuRaw->NewPixel(sensorId, 1., line, col);
            double v = pGeoMap->GetPositionV(line);
            double u = pGeoMap->GetPositionU(col);
            TVector3 pos(v,u,0);
            pixel->SetPosition(pos);
            
            pixel->SetLayer(layer);
            SetMCinfo(pixel, i);
            
            if ( GlobalPar::GetPar()->Debug() > 0 )
               printf("line %d col %d\n", line, col);
            
            if (ValidHistogram()) {
               fpHisPixelMap[sensorId]->Fill(line, col);
               fpHisPosMap[sensorId]->Fill(u, v);
            }
         }
      }
      
      if (ValidHistogram()) {
         Int_t pixelsN = fDigitizer->GetPixelsN();
         fpHisPixel[layer]->Fill(pixelsN);
      }
   }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}

//------------------------------------------+-----------------------------------
void TAVTactNtuMC::SetMCinfo(TAVTntuHitMC* pixel, Int_t hitId)
{
   
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   
   Int_t mcID(-1000);
   
   // Generated particle ID
   Int_t genPartID = fpEvtStr->VTXid[hitId] - 1;
   
   // check true particle ID linked to the hit is in the correct range
   if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
      cout << "TAVTactNtuMC::Action :: ERROR >> wrong generate particle ID: "<< genPartID << " nPart= " << fpEvtStr->TRn << endl;
      exit(0);
   }
   
   // MC tracks info
   mcID = fpEvtStr->VTXid[hitId];
   pixel->SetMCid(mcID);
   
   //Need IDX matching
   TVector3 MCmom(0,0,0);
   TVector3 MCpos(0,0,0);
   
   // global coordinates
   MCpos.SetXYZ((fpEvtStr->VTXxin[hitId]  + fpEvtStr->VTXxout[hitId])/2,  (fpEvtStr->VTXyin[hitId]  + fpEvtStr->VTXyout[hitId])/2,  (fpEvtStr->VTXzin[hitId]  + fpEvtStr->VTXzout[hitId])/2);
   MCmom.SetXYZ((fpEvtStr->VTXpxin[hitId] + fpEvtStr->VTXpxout[hitId])/2, (fpEvtStr->VTXpyin[hitId] + fpEvtStr->VTXpyout[hitId])/2, (fpEvtStr->VTXpzin[hitId] + fpEvtStr->VTXpzout[hitId])/2);
   
   if ( GlobalPar::GetPar()->Debug() > 0 )     {
      cout << "Vertex pixel hit n: " << hitId << ". Col " << pixel->GetPixelColumn() << " row "<< pixel->GetPixelLine() << endl;
      cout << "\tGlobal kinematic: \n\t\tPos:\t";
      MCpos.Print();
      cout << "\t\tMom:\t";
      MCmom.Print();
   }
   
   // change to local
   pGeoMap->Global2Local( &MCpos );
   pGeoMap->Global2Local_RotationOnly( &MCmom );
   
   pixel->SetMCPosition(MCpos);   // set in local coord
   pixel->SetMCMomentum(MCmom);   // set in local coord
   pixel->SetEneLoss(fpEvtStr->VTXde[hitId]);  // VM added 3/11/13
   
   // store generated particle info
   pixel->SetGeneratedParticleInfo( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                                   fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                                   TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                                   TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );
   
}



/*!
  \file
  \version $Id: TAVTactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAVTactNtuMC.
*/

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

}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactNtuMC::CreateHistogram()
{
   DeleteHistogram();
   
   for (Int_t i = 0; i < VTX_NLAY; ++i) {
      fpHisPixel[i] = new TH1F(Form("vtMcPixel%d", i+1), Form("Vertex - MC # pixels per clusters for sensor %d", i+1), 100, 0., 100.);
      AddHistogram(fpHisPixel[i]);
   }

   
 //  TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAVTparConf::IsMapHistOn()) {
 //      fpHisPixelMap[i] 
  // = new TH2F(Form("vtPixelMap%d", i+1)
  //     , Form("Vertex - pixel map for sensor %d", i+1), 
  //     pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu(), 
  //     pGeoMap->GetPixelsNv(), 0, pGeoMap->GetPixelsNv());
 //      fpHisPixelMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPixelMap[i]);
 //    }
 //  }
  
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAVTparConf::IsMapHistOn()) {
 //      fpHisPosMap[i] = 
  // new TH2F(Form("vtPosMap%d", i+1), 
  //   Form("Vertex - position map for sensor %d", i+1), 
  //   pGeoMap->GetPixelsNu(), -pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(), pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(),
  //   pGeoMap->GetPixelsNv(), -pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv(), pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv());
 //      fpHisPosMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPosMap[i]);
 //    }
    
 //    fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
  //           pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu());
 //    AddHistogram(fpHisRateMap[i]);
    
 //    fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
  //      10, 0, 5);
 //    AddHistogram(fpHisRateMapQ[i]);
 //  }
  
   SetValidHistogram(kTRUE);
  return;
}


//------------------------------------------+-----------------------------------
//! Action.
Bool_t TAVTactNtuMC::Action()
{

  TAVTntuRaw* pNtuRaw = (TAVTntuRaw*) fpNtuRaw->Object();
  TAVTparMap* pParMap = (TAVTparMap*) fpParMap->Object();
  TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();


  TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fGeoTrafo == 0x0)
    Error("SetGeoTrafoName","No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());

  pNtuRaw->Clear();

  int mcID(-1000);

    if (fDebugLevel)     Info("Action()","Processing n :: %2d hits \n",fpEvtStr->VTXn);
    if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "VTXn   " << fpEvtStr->VTXn<< endl;


   for (Int_t i = 0; i < fpEvtStr->VTXn; i++) {
    if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;

      Int_t sensorId    = 0;

     //What About a decent post processing?
     //The column refer to Y!!!
     // !!!!!!!!!!!!!!!!!!!!!!!!!!!  in ntuple, the row and col start from 0  !!!!!!!!!!!!!!!!!!!!!!!
     Int_t myTrow, myTcol;
     myTrow = fpEvtStr->VTXirow[i] - 1;
     myTcol = fpEvtStr->VTXicol[i] - 1;
     

     // Generated particle ID 
     Int_t genPartID = fpEvtStr->VTXid[i] - 1;
    
    // check true particle ID linked to the hit is in the correct range
    if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
        cout << "TAVTactNtuMC::Action :: ERROR >> wrong generate particle ID: "<< genPartID << " nPart= " << fpEvtStr->TRn << endl;
        exit(0);
    }


    if ( GlobalPar::GetPar()->Debug() > 0 )     {
        cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;      
        cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
        cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
    }
     
     TAVTntuHitMC* pixel = (TAVTntuHitMC*)pNtuRaw->NewPixel(sensorId, 1., myTrow, myTcol);

     // set geometry // why ???
     pixel->SetVtxGeo(pGeoMap);
    
     Int_t layer = fpEvtStr->VTXilay[i] ;
     pixel->SetLayer(layer);
   
      Double_t eloss = fpEvtStr->VTXde[i];
      Double_t x = (fpEvtStr->VTXxin[i]+fpEvtStr->VTXxout[i])/2;
      Double_t y = (fpEvtStr->VTXyin[i]+fpEvtStr->VTXyout[i])/2;
      
      fDigitizer->Process(eloss, x, y);
      
      if (ValidHistogram()) {
         Int_t pixelsN = fDigitizer->GetPixelsN();
        fpHisPixel[layer]->Fill(pixelsN);
      }
      
      double v = pParMap->GetPositionV(myTrow);
      double u = pParMap->GetPositionU(myTcol);
      TVector3 pos(v,u,0);
      pixel->SetPosition(pos);
      
      
      
      
      // MC tracks info
      mcID = fpEvtStr->VTXid[i];
      pixel->SetMCid(mcID);
      
     //Need IDX matching
     TVector3 MCmom(0,0,0);
     TVector3 MCpos(0,0,0);
      
      // global coordinates
     MCpos.SetXYZ((fpEvtStr->VTXxin[i]+fpEvtStr->VTXxout[i])/2,(fpEvtStr->VTXyin[i]+fpEvtStr->VTXyout[i])/2,(fpEvtStr->VTXzin[i]+fpEvtStr->VTXzout[i])/2);
     MCmom.SetXYZ((fpEvtStr->VTXpxin[i]+fpEvtStr->VTXpxout[i])/2,(fpEvtStr->VTXpyin[i]+fpEvtStr->VTXpyout[i])/2,(fpEvtStr->VTXpzin[i]+fpEvtStr->VTXpzout[i])/2);

      if ( GlobalPar::GetPar()->Debug() > 0 )     {
         cout << "Vertex pixel hit n: " << i << ". Col " << myTcol << " row "<< myTrow << endl;
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
     pixel->SetEneLoss(fpEvtStr->VTXde[i]);  // VM added 3/11/13
     // store generated particle info
    pixel->SetGeneratedParticleInfo ( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                    fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                    TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                    TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );

        
  

   }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}


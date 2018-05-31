/*!
  \file
  \version $Id: TAITactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAITactNtuMC.
*/

#include "TH2F.h"
#include <map>
#include "TF1.h"
#include "TMath.h"
#include "TDirectory.h"


#include "TAITparGeo.hxx"
#include "TAITparMap.hxx"
#include "TAITparConf.hxx"
#include "TAITntuRaw.hxx"
#include "TAITactNtuMC.hxx"

#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

#include "GlobalPar.hxx"
/*!
  \class TAITactNtuMC"
  \brief NTuplizer for vertex raw hits. **
*/

using namespace std;

ClassImp(TAITactNtuMC);

// Bool_t  TAVTactNtuMC::fgPileup          = true;
// Float_t TAVTactNtuMC::fgPoissonPar      = 0.736; // ajust for FIRST
// Int_t   TAVTactNtuMC::fgPileupEventsN   = 10;
// Float_t TAVTactNtuMC::fgSigmaNoiseLevel = -1.;
// Int_t   TAVTactNtuMC::fgMcNoiseId       = -99;



//------------------------------------------+-----------------------------------
//! Default constructor.
TAITactNtuMC::TAITactNtuMC(const char* name,
                           TAGdataDsc* pNtuRaw, 
                           TAGparaDsc* pGeoMap,
                           TAGparaDsc* pParMap,
                           EVENT_STRUCT* evStr)
  : TAGaction(name, "TAITactNtuMC - NTuplize Vertex raw data"),
    fpNtuRaw(pNtuRaw),
    fpGeoMap(pGeoMap),
    fpParMap(pParMap),
    fpEvtStr(evStr),
    // fDigitizer(new TAITdigitizerE(pGeoMap)),
    // fNoisyPixelsN(0),
    fDebugLevel(0)
{
  AddDataOut(pNtuRaw, "TAITntuRaw");
  AddPara(pGeoMap, "TAITparGeo");
  AddPara(pParMap, "TAITparMap");

  // fDigitizer = new TAITdigitizerE(pGeoMap);

  //   fpHisPoisson = (TH1F*)gDirectory->FindObject("vtPoisson");
  //   if (fpHisPoisson == 0x0) {
       
  //       Double_t tot = 0.;
  //       Double_t par = fgPoissonPar;

  //       for (Int_t i = 1; i < 10; ++i) {
  //           tot += TMath::PoissonI(i, par);
  //       }

  //       fpHisPoisson = new TH1F("vtPoisson", "Poisson", 12, -0.5, 11.5);

  //       for (Int_t i = 1; i < 10; ++i) {
  //           Float_t val = TMath::PoissonI(i, par)/tot*100.;
  //           fpHisPoisson->Fill(i, val);
  //       }
  //   }


}





//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAITactNtuMC::CreateHistogram()
{
 //  DeleteHistogram();
 //  TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAITparConf::IsMapHistOn()) {
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
 //    if (TAITparConf::IsMapHistOn()) {
 //      fpHisPosMap[i] = 
    // new TH2F(Form("vtPosMap%d", i+1), 
    //   Form("Vertex - position map for sensor %d", i+1), 
    //   pGeoMap->GetPixelsNu(), -pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(), pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(),
    //   pGeoMap->GetPixelsNv(), -pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv(), pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv());
 //      fpHisPosMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPosMap[i]);
 //    }
    
 //    fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
    //             pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu());
 //    AddHistogram(fpHisRateMap[i]);
    
 //    fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
    //          10, 0, 5);
 //    AddHistogram(fpHisRateMapQ[i]);
 //  }
  
 //  SetValidHistogram(kTRUE);
 //  return;
}


//------------------------------------------+-----------------------------------
//! Action.
bool TAITactNtuMC::Action()   {

    if ( GlobalPar::GetPar()->Debug() > 0 )     
      cout <<endl<< "TAITactNtuMC::Action() start  -->  ITn :  " << fpEvtStr->ITRn << endl;
    
    TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
    TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();     // container of the hits
    pNtuRaw->Clear();

    // static Int_t storedEvents = 0;
    // std::vector<RawMcHit_t> storedEvtInfo;
    // RawMcHit_t mcHit;

    vector<int> blackList;
    // loop over hits
    for (Int_t i = 0; i < fpEvtStr->ITRn; i++) {

        if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "sensorId "<< fpEvtStr->ITRimimo[i] << " + "<< fpEvtStr->ITRiplume[i] << " + "<< fpEvtStr->ITRilay[i] << endl;

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!  in ntuple, the row and col start from 0  !!!!!!!!!!!!!!!!!!!!!!!
        int myTrow, myTcol;
        myTrow = fpEvtStr->ITRirow[i];
        myTcol = fpEvtStr->ITRicol[i];
        int sensorId  = pGeoMap->GetSensorID( fpEvtStr->ITRilay[i], fpEvtStr->ITRiplume[i], fpEvtStr->ITRimimo[i] );


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

        ////////  "DECLUSTER" HACK waiting for clustering   ///////////////////////////////////////
        // remove all the hits adiacent to the current one

        // check if the current event is in the blacklist. if so skip.
        bool skip = false;
        for ( int bl = 0; bl<blackList.size(); bl++ ) {
            if ( blackList.at(bl) == i ) {
                skip = true;
                continue;
            }
        }
        if ( skip )            continue;   // next event
        

        // loop on the remaining hits
        bool decluster = false;
        for ( int j = i+1; j < fpEvtStr->ITRn; j++) {   

            // check if same sensor 
            if ( (fpEvtStr->ITRimimo[i] != fpEvtStr->ITRimimo[j]) || (fpEvtStr->ITRiplume[i] != fpEvtStr->ITRiplume[j]) || (fpEvtStr->ITRilay[i] != fpEvtStr->ITRilay[j]) )
                continue;

            bool decluster_inner = false;
            // search if the j-hit is close to the i-hit. is so decluster_inner TRUE
            for ( int k = -1; k <= 1; k++ ) {
                for ( int h = -1; h <= 1; h++ ) {
                    if   ( myTrow+k == fpEvtStr->ITRirow[j] && myTcol+h == fpEvtStr->ITRicol[j] )   {
                        blackList.push_back( j );
                        decluster_inner = true;
                        break;
                    }
                }
                if ( decluster_inner )    break;
            }

            // 
            if ( decluster_inner ) 
                decluster = true;

        }
        if ( decluster )   {
            blackList.push_back( i );
            // continue;  // next event
       }
        // DECLUSTER end
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////


        // Generated particle ID 
        int genPartID = fpEvtStr->ITRid[i] - 1;
        TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., myTrow, myTcol, "mc_hit", i, genPartID );

        // // Digitization test
        // for ( int io=-10; io<10; io++ ) {
        //     // if (myTrow+io < ) ..........
        //     for ( int jo=-10; jo<10; jo++ ) {
        //         pNtuRaw->NewPixel(sensorId, 1., myTrow+io, myTcol+jo, "mc_cluster", pixel );
        //     }
        // }



        // // used for pileup ...
        // if (fgPileup && storedEvents <= fgPileupEventsN) {
        //     mcHit.id  = sensorId;
        //     mcHit.de  = fpEvtStr->ITRde[i];
        //     mcHit.x   = fpEvtStr->ITRpxin[i];
        //     mcHit.y   = fpEvtStr->ITRpyin[i];
        //     mcHit.zi  = fpEvtStr->ITRzin[i];
        //     mcHit.zo  = fpEvtStr->ITRpxout[i];
        //     storedEvtInfo.push_back(mcHit);
        // }


// ************  fDigitizer  ****************************************************************************************

        // gives all empty
        // if (!fDigitizer->Process(fpEvtStr->ITRde[i], fpEvtStr->ITRpxin[i], fpEvtStr->ITRpyin[i], fpEvtStr->ITRzin[i], fpEvtStr->ITRpxout[i])) continue;     // when false?
        // FillPixels( pixel, sensorId, i );
        
// ****************************************************************************************************

        // fill MC info
        SetMCinfo( pixel, i );


        // if (ValidHistogram()) {
        //     Int_t pixelsN = fDigitizer->GetPixelsN();
        //     fpHisPixel[sensorId]->Fill(pixelsN);
        // }


        // PILEUP

   // if (fgPileup && storedEvents <= fgPileupEventsN) {
   //     fStoredEvents.push_back(storedEvtInfo);
   //     storedEvents++;
   //  }
    
   //  if (fgPileup && storedEvents >= fgPileupEventsN)
   //     GeneratePileup();


   //  if(fDebugLevel) {
   //     std::vector<RawMcHit_t> mcInfo;
   //     if (fgPileup && storedEvents <= fgPileupEventsN) {
   //        for (Int_t i = 0; i < fStoredEvents.size(); ++i) {
   //           printf("Event %d\n", i);
   //           mcInfo = fStoredEvents[i];
   //           for (Int_t j = 0; j < mcInfo.size(); ++j) {
   //              RawMcHit_t hit = mcInfo[j];
   //              printf("id %d de %.4f x %.4f y %.4f\n", hit.id, hit.de, hit.x, hit.y);
   //           }
   //        }
   //     }
   //  }


   }    // END - loop over hits
   blackList.clear();

   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}



//------------------------------------------+-----------------------------------
void TAITactNtuMC::SetMCinfo(TAITntuHit* pixel, int hitId) {

    TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
    int genPartID = fpEvtStr->ITRid[hitId] - 1;

    // check true particle ID linked to the hit is in the correct range
    if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
        cout << "TAITactNtuMC::Action :: ERROR >> wrong generate particle ID: "<< genPartID << " (nPart= " << fpEvtStr->TRn << endl;
        exit(0);
    }

    if ( GlobalPar::GetPar()->Debug() > 0 )     {
        cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << " and mass = " << fpEvtStr->TRmass[genPartID] << endl;
        cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
        cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
    }
     
    // // start filling hit with info!
    // pixel->SetItrGeo(pGeoMap); // first to set!!!!
    // pixel->SetMCid( fpEvtStr->ITRid[hitId] );
    // pixel->SetLayer( fpEvtStr->ITRilay[hitId] );

    // global coordinates
    TVector3 MCpos = TVector3((fpEvtStr->ITRxin[hitId]+fpEvtStr->ITRxout[hitId])/2,(fpEvtStr->ITRyin[hitId]+fpEvtStr->ITRyout[hitId])/2,(fpEvtStr->ITRzin[hitId]+fpEvtStr->ITRzout[hitId])/2);
    TVector3 MCmom = TVector3((fpEvtStr->ITRpxin[hitId]+fpEvtStr->ITRpxout[hitId])/2,(fpEvtStr->ITRpyin[hitId]+fpEvtStr->ITRpyout[hitId])/2,(fpEvtStr->ITRpzin[hitId]+fpEvtStr->ITRpzout[hitId])/2);
    
            
    if ( GlobalPar::GetPar()->Debug() > 0 )     {
        cout << "InnerTracker pixel " << hitId << " col " << pixel->GetPixelColumn() << " row "<< pixel->GetPixelLine() << endl;
        cout << "\tpixel global pos = ";    pixel->GetPixelPosition_footFrame().Print();
        cout << "\tGlobal kinematic: \n\t\tPos:\t"; 
        MCpos.Print();
        cout << "\t\tMom:\t";
        MCmom.Print();
    }
 
    // input in global cartesian coordinates, change to local in pixel
    pixel->SetMCPosition(MCpos);
    pixel->SetMCMomentum(MCmom);
    pixel->SetEneLoss(fpEvtStr->ITRde[hitId]);

    // in global frame
    // pixel->SetHitCoordinate( pGeoMap->GetPosition( fpEvtStr->ITRilay[hitId], fpEvtStr->ITRimimo[hitId], fpEvtStr->ITRiplume[hitId],  myTcol, myTrow ) );

    // set info of the generating particle  --  waiting to resurrect the proper class
    pixel->SetGeneratedParticleInfo ( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                        fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                        TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                        TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID])  );

}








//------------------------------------------+-----------------------------------
void TAITactNtuMC::FillPixels ( TAITntuHit* originatingHit, int sensorId, int hitId ) {
   
   //  TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
   //  TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
 
   //  map<int, int> digiMap = fDigitizer->GetMap();
   //  int nPixelX = fDigitizer->GetNPixelX();
 
   //  // fill pixels from map
   //  for ( map< int, int >::iterator it = digiMap.begin(); it != digiMap.end(); ++it) {

   //     if ( digiMap[it->first] == 1 ) {

   //          int line = it->first / nPixelX;
   //          int col  = it->first % nPixelX;


   //          TAITntuHit* pixel = (TAITntuHit*)pNtuRaw->NewPixel(sensorId, 1., line, col, "mc_cluster", originatingHit);

   //          if ( GlobalPar::GetPar()->Debug() > 0 )
   //              printf("line %d col %d\n", line, col);

   //          // sensor frame
   //          double v = pGeoMap->GetRowCenter_sensorFrame(line);
   //          double u = pGeoMap->GetColumnCenter_sensorFrame(col);

   //          if (ValidHistogram()) {
   //              fpHisPixelMap[sensorId]->Fill(line, col);
   //              fpHisPosMap[sensorId]->Fill(u, v);
   //          }
   //      }
   // }
}





//------------------------------------------+-----------------------------------
void TAITactNtuMC::FillPixels ( int sensorId, int hitId ) {
   
   //  TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
   //  TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
 
   //  map<int, int> digiMap = fDigitizer->GetMap();
   //  int nPixelX = fDigitizer->GetNPixelX();
 
   //  // fill pixels from map
   //  for ( map< int, int >::iterator it = digiMap.begin(); it != digiMap.end(); ++it) {

   //     if ( digiMap[it->first] == 1 ) {

   //          int line = it->first / nPixelX;
   //          int col  = it->first % nPixelX;


   //          TAITntuHit* pixel = (TAITntuHit*)pNtuRaw->NewPixel(sensorId, 1., line, col, "pileup");


   //          if ( GlobalPar::GetPar()->Debug() > 0 )
   //              printf("line %d col %d\n", line, col);

   //          double v = pGeoMap->GetRowCenter_sensorFrame(line);
   //          double u = pGeoMap->GetColumnCenter_sensorFrame(col);

   //          if (ValidHistogram()) {
   //              fpHisPixelMap[sensorId]->Fill(line, col);
   //              fpHisPosMap[sensorId]->Fill(u, v);
   //          }
   //      }
   // }
}










// --------------------------------------------------------------------------------------
void TAITactNtuMC::ComputeNoiseLevel() {

    // // computing number of noise pixels (sigma level) from gaussian
    // TF1* f = new TF1("f", "gaus", -10, 10);
    // f->SetParameters(1,0,1);
    // Float_t fraction = 0;
    
    // if (fgSigmaNoiseLevel > 0) {
    //    fraction = f->Integral(-fgSigmaNoiseLevel, fgSigmaNoiseLevel)/TMath::Sqrt(2*TMath::Pi());
    //    fNoisyPixelsN = TMath::Nint(fDigitizer->GetNPixelX()*fDigitizer->GetNPixelY()*(1.-fraction));
    // }
    
    // if (fDebugLevel)
    //    printf("Number of noise pixels %d\n", fNoisyPixelsN);
    
    // delete f;
}






//___________________________________
void TAITactNtuMC::FillNoise() {

    // TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
    // for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
    //    FillNoise(i);
    // }
}




//___________________________________
void TAITactNtuMC::FillNoise(Int_t sensorId) {

    // TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
    // TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();

    // Int_t pixelsN = gRandom->Uniform(0, fNoisyPixelsN);
    // for (Int_t i = 0; i < pixelsN; ++i) {
    //    Int_t col  = gRandom->Uniform(0,fDigitizer->GetNPixelX());
    //    Int_t line = gRandom->Uniform(0,fDigitizer->GetNPixelY());
    //    // TAITntuHit* pixel = (TAITntuHit*)pNtuRaw->NewPixel(sensorId, 1., line, col);
    //    TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., line, col, "mc_noise" );
    //    double v = pGeoMap->GetRowCenter_sensorFrame(line);
    //    double u = pGeoMap->GetColumnCenter_sensorFrame(col);
    //    TVector3 pos(v,u,0);
    //    // pixel->SetPosition(pos);     //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //    // pixel->SetMCid(fgMcNoiseId);
    // }
}







//------------------------------------------+-----------------------------------
void  TAITactNtuMC::GeneratePileup() {
    
    // Int_t pileupEvents = TMath::Nint(fpHisPoisson->GetRandom())-1;

    
    // // form pileup events number pull out randomly the stored events
    // std::vector<int> rarray;

    // for (Int_t i = 0; i < fgPileupEventsN; ++i) {
    //    if (i > pileupEvents-1)
    //       rarray.push_back(0);
    //    else
    //       rarray.push_back(1);
    // }
    
    // std::random_shuffle (rarray.begin(), rarray.end(), TAITdigitizer::GetRandom);

    // std::vector<RawMcHit_t> mcInfo;

    // for (Int_t p = 0; p < fgPileupEventsN; ++p) {
       
    //    if (rarray[p] == 0) continue;
       
    //    mcInfo = fStoredEvents[p];
       
    //    for (Int_t j = 0; j < mcInfo.size(); ++j) {
    //       RawMcHit_t hit = mcInfo[j];
          
    //       if (!fDigitizer->Process(hit.de, hit.x, hit.y, hit.zi, hit.zo)) continue;
    //       FillPixels( hit.id, -1); // !!!!!!!!!!!!!!!!!!!!!
    //    }
    // }
}




























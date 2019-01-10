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
#include "TAVTparConf.hxx"

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




//------------------------------------------+-----------------------------------
//
TAITactNtuMC::TAITactNtuMC(const char* name, TAGdataDsc* pNtuRaw,  TAGparaDsc* pGeoMap, EVENT_STRUCT* evStr)
 : TAVTactBaseNtuMC(name, pGeoMap, evStr),
   fpNtuRaw(pNtuRaw)
{
	AddDataOut(pNtuRaw, "TAITntuRaw");
	AddPara(pGeoMap, "TAITparGeo");

   CreateDigitizer();
}


//------------------------------------------+-----------------------------------
//! Create histogram
void TAITactNtuMC::CreateDigitizer()
{
   TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
   fDigitizer = new TAITdigitizerE(pGeoMap);
   if (fgSigmaNoiseLevel > 0)
      ComputeNoiseLevel();
}


//------------------------------------------+-----------------------------------
//! Action.
bool TAITactNtuMC::Action()
{
	if ( fDebugLevel> 0 )
	  Info("TAITactNtuMC::Action()", "start  -->  VTn : %d  ", fpEvtStr->ITRn);

	TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();     
	TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
	pNtuRaw->Clear();

	static Int_t storedEvents = 0;
	std::vector<RawMcHit_t> storedEvtInfo;
	RawMcHit_t mcHit;

	// Loop over all MC hits
	for (Int_t i = 0; i < fpEvtStr->ITRn; i++) {
		if ( fDebugLevel> 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;

		// !!  in ntuple, the row and col start from 0  !!!
		Int_t myTrow, myTcol;
		myTrow = fpEvtStr->ITRirow[i];
		myTcol = fpEvtStr->ITRicol[i];
      
		Int_t sensorId = pGeoMap->GetSensorID( fpEvtStr->ITRilay[i], fpEvtStr->ITRiplume[i], fpEvtStr->ITRimimo[i] );
      
		// used for pileup ...
		if (fgPileup && storedEvents <= fgPileupEventsN) {
			mcHit.id  = sensorId;
			mcHit.de  = fpEvtStr->ITRde[i];
			mcHit.x   = fpEvtStr->ITRxin[i];
			mcHit.y   = fpEvtStr->ITRyin[i];
			mcHit.zi  = fpEvtStr->ITRzin[i];
			mcHit.zo  = fpEvtStr->ITRzout[i];
			storedEvtInfo.push_back(mcHit);
		}


		// Digitizing
//      don't change anything ?
//      Int_t genPartID = fpEvtStr->ITRid[i] - 1;
//      if (fpEvtStr->TRcha[genPartID] < 1) continue;
      
      if (ValidHistogram()) {
         fpHisDeTot->Fill(fpEvtStr->ITRde[i]*TAGgeoTrafo::GevToKev());
         fpHisDeSensor[sensorId]->Fill(fpEvtStr->ITRde[i]*TAGgeoTrafo::GevToKev());
      }

      // try to get into sensor frame, does not work yet
      TVector3 posIn(fpEvtStr->ITRxin[i], fpEvtStr->ITRyin[i], fpEvtStr->ITRzin[i]);
      TVector3 posOut(fpEvtStr->ITRxout[i], fpEvtStr->ITRyout[i], fpEvtStr->ITRzout[i]);
      posIn = pGeoMap->Detector2Sensor(sensorId, posIn);
      posOut = pGeoMap->Detector2Sensor(sensorId, posOut);
      
    //  printf("%f %f %f - %f %f %f\n", fpEvtStr->ITRxin[i], fpEvtStr->ITRyin[i], fpEvtStr->ITRzin[i],posIn[0], posIn[1], posIn[2]);

		if (!fDigitizer->Process(fpEvtStr->ITRde[i], posIn[0], posIn[1], posIn[2], posOut[2])) continue;
		FillPixels(sensorId, i);
		
		if (ValidHistogram()) {
         Int_t pixelsN = fDigitizer->GetMap().size();
         fpHisPixel[sensorId]->Fill(pixelsN);
         fpHisPixelTot->Fill(pixelsN);
		}
   }

   // Pileup
   if (fgPileup && storedEvents <= fgPileupEventsN) {
      fStoredEvents.push_back(storedEvtInfo);
      storedEvents++;
   }
	
   if (fgPileup && storedEvents >= fgPileupEventsN)
      GeneratePileup();


   if(fDebugLevel) {
      std::vector<RawMcHit_t> mcInfo;
      if (fgPileup && storedEvents <= fgPileupEventsN) {
         for (Int_t i = 0; i < fStoredEvents.size(); ++i) {
            printf("Event %d\n", i);
            mcInfo = fStoredEvents[i];
            for (Int_t j = 0; j < mcInfo.size(); ++j) {
               RawMcHit_t hit = mcInfo[j];
               printf("id %d de %.4f x %.4f y %.4f\n", hit.id, hit.de, hit.x, hit.y);
            }
         }
      }
   }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}

//------------------------------------------+-----------------------------------
void TAITactNtuMC::FillPixels(Int_t sensorId, Int_t hitId )
{
	TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
	TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
 
	map<int, double> digiMap = fDigitizer->GetMap();
	int nPixelX = fDigitizer->GetNPixelX();
 
	// fill pixels from map
   int count = 0;
	for ( map< int, double >::iterator it = digiMap.begin(); it != digiMap.end(); ++it) {

	   if ( digiMap[it->first] > 0 ) {
         count++;
			int line = it->first / nPixelX;
			int col  = it->first % nPixelX;
         
			TAITntuHit* pixel = (TAITntuHit*)pNtuRaw->NewPixel(sensorId, 1., line, col);

         Int_t genPartID = fpEvtStr->ITRid[hitId] - 1;
         pixel->AddMcTrackId(genPartID, hitId);


         if ( fDebugLevel> 0 )
				printf("line %d col %d\n", line, col);

			double v = pGeoMap->GetPositionV(line);
			double u = pGeoMap->GetPositionU(col);
         TVector3 pos(u,v,0);
         pixel->SetPosition(pos);

			if (ValidHistogram()) {
				fpHisPixelMap[sensorId]->Fill(line, col);
				fpHisPosMap[sensorId]->Fill(u, v);
			}
		}
   }
}

//___________________________________
void TAITactNtuMC::FillNoise()
{
	TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
	for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	   FillNoise(i);
	}
}

//___________________________________
void TAITactNtuMC::FillNoise(Int_t sensorId)
{
	TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();

	Int_t pixelsN = gRandom->Uniform(0, fNoisyPixelsN);
	for (Int_t i = 0; i < pixelsN; ++i) {
	   Int_t col  = gRandom->Uniform(0,fDigitizer->GetNPixelX());
	   Int_t line = gRandom->Uniform(0,fDigitizer->GetNPixelY());
	   TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., line, col);
	   pixel->AddMcTrackId(fgMcNoiseId);
	}
}

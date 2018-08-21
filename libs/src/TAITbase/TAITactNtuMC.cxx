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
#include "TAVTparMap.hxx"
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
}


//------------------------------------------+-----------------------------------
//! Action.
bool TAITactNtuMC::Action()
{
	if ( GlobalPar::GetPar()->Debug() > 0 )
	  Info("TAITactNtuMC::Action()", "start  -->  VTn : %d  ", fpEvtStr->ITRn);

	TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();     
	TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
	pNtuRaw->Clear();

	static Int_t storedEvents = 0;
	std::vector<RawMcHit_t> storedEvtInfo;
	RawMcHit_t mcHit;

	// Loop over all MC hits
	for (Int_t i = 0; i < fpEvtStr->ITRn; i++) {
		if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;

		// !!  in ntuple, the row and col start from 0  !!!
		Int_t myTrow, myTcol;
		myTrow = fpEvtStr->ITRirow[i];
		myTcol = fpEvtStr->ITRicol[i];
      
		Int_t sensorId = pGeoMap->GetSensorID( fpEvtStr->ITRilay[i], myTcol, myTrow );
      // convert id is some reasonnale number that could be an ouput of a DAQ !!!
      sensorId = pGeoMap->GetChipFromSensorID(sensorId)+16*fpEvtStr->ITRilay[i];
	   
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
         fpHisDeTot->Fill(fpEvtStr->ITRde[i]*TAVTbaseDigitizer::GeV2keV());
         fpHisDeSensor[sensorId]->Fill(fpEvtStr->ITRde[i]*TAVTbaseDigitizer::GeV2keV());
      }

      
		if (!fDigitizer->Process(fpEvtStr->ITRde[i], fpEvtStr->ITRxin[i], fpEvtStr->ITRyin[i], fpEvtStr->ITRzin[i], fpEvtStr->ITRzout[i])) continue;
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
 
	map<int, int> digiMap = fDigitizer->GetMap();
	int nPixelX = fDigitizer->GetNPixelX();
 
	// fill pixels from map
   int count = 0;
	for ( map< int, int >::iterator it = digiMap.begin(); it != digiMap.end(); ++it) {

	   if ( digiMap[it->first] == 1 ) {
         count++;
			int line = it->first / nPixelX;
			int col  = it->first % nPixelX;
         
			TAITntuHit* pixel = (TAITntuHit*)pNtuRaw->NewPixel(sensorId, 1., line, col);

         Int_t genPartID = fpEvtStr->ITRid[hitId] - 1;
         pixel->SetMCid(genPartID);
         SetMCinfo(pixel, hitId);


         if ( GlobalPar::GetPar()->Debug() > 0 )
				printf("line %d col %d\n", line, col);

			double v = pGeoMap->GetPositionV(line);
			double u = pGeoMap->GetPositionU(col);


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
	   pixel->SetMCid(fgMcNoiseId);
	}
}

//------------------------------------------+-----------------------------------
void TAITactNtuMC::SetMCinfo(TAITntuHit* pixel, Int_t hitId)
{
   int genPartID = fpEvtStr->ITRid[hitId] - 1;
   
   // check true particle ID linked to the hit is in the correct range
   if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
      Warning("TAITactNtuMC::SetMCinfo()", "wrong generate particle ID: %d nPart = %d", genPartID, fpEvtStr->TRn);
      return;
   }
   
   if ( GlobalPar::GetPar()->Debug() > 0 )     {
      cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;
      cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
      cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
   }
   
   
   // global coordinates
   TVector3 MCpos = TVector3((fpEvtStr->ITRxin[hitId]  + fpEvtStr->ITRxout[hitId])/2,  (fpEvtStr->ITRyin[hitId]  + fpEvtStr->ITRyout[hitId])/2,  (fpEvtStr->ITRzin[hitId]  + fpEvtStr->ITRzout[hitId])/2);
   TVector3 MCmom = TVector3((fpEvtStr->ITRpxin[hitId] + fpEvtStr->ITRpxout[hitId])/2, (fpEvtStr->ITRpyin[hitId] + fpEvtStr->ITRpyout[hitId])/2, (fpEvtStr->ITRpzin[hitId] + fpEvtStr->ITRpzout[hitId])/2);
   
   if ( GlobalPar::GetPar()->Debug() > 0 )     {
      cout << "Vertex pixel hit n: " << hitId << ". Col " << pixel->GetPixelColumn() << " row "<< pixel->GetPixelLine() << endl;
      cout << "\tGlobal kinematic: \n\t\tPos:\t";
      MCpos.Print();
      cout << "\t\tMom:\t";
      MCmom.Print();
   }
   
   
   pixel->SetMCPosition(MCpos);   // set in local coord (transformation in Hit)
   pixel->SetMCMomentum(MCmom);   // set in local coord
   pixel->SetEneLoss(fpEvtStr->ITRde[hitId]);
   
   // store generated particle info
   pixel->SetGeneratedParticleInfo( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                                   fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                                   TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                                   TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );
}

/*!
  \file
  \version $Id: TAVTactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAVTactNtuMC.
*/

#include "TH2F.h"
#include <map>
#include "TF1.h"
#include "TMath.h"
#include "TDirectory.h"


#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTntuRaw.hxx"


#include "TAVTactNtuMC.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

#include "GlobalPar.hxx"

/*!
  \class TAVTactNtuMC"
  \brief NTuplizer for vertex raw hits. **
*/

using namespace std;

ClassImp(TAVTactNtuMC);




//------------------------------------------+-----------------------------------
//
TAVTactNtuMC::TAVTactNtuMC(const char* name, TAGdataDsc* pNtuRaw,  TAGparaDsc* pGeoMap, EVENT_STRUCT* evStr)
 : TAVTactBaseNtuMC(name, pGeoMap, evStr),
   fpNtuRaw(pNtuRaw)
{
	AddDataOut(pNtuRaw, "TAVTntuRaw");
	AddPara(pGeoMap, "TAVTparGeo");

   CreateDigitizer();
   
}


//------------------------------------------+-----------------------------------
//! Create digitizer
void TAVTactNtuMC::CreateDigitizer()
{
   TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();
   fDigitizer = new TAVTdigitizerE(pGeoMap);
}


//------------------------------------------+-----------------------------------
//! Action.
bool TAVTactNtuMC::Action()
{
	if ( fDebugLevel> 0 )     
	  Info("TAVTactNtuMC::Action()", "start  -->  VTn : %d  ", fpEvtStr->VTXn);

	TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();     
	TAVTntuRaw* pNtuRaw = (TAVTntuRaw*) fpNtuRaw->Object();
	pNtuRaw->Clear();

	static Int_t storedEvents = 0;
	std::vector<RawMcHit_t> storedEvtInfo;
	RawMcHit_t mcHit;

	// Loop over all MC hits
	for (Int_t i = 0; i < fpEvtStr->VTXn; i++) {
		if ( fDebugLevel> 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;

		// !!  in ntuple, the row and col start from 0  !!!
		Int_t myTrow, myTcol;
		myTrow = fpEvtStr->VTXirow[i];
		myTcol = fpEvtStr->VTXicol[i];
		Int_t sensorId = pGeoMap->GetSensorID( fpEvtStr->VTXilay[i], myTcol, myTrow );
	   
		// used for pileup ...
		if (fgPileup && storedEvents <= fgPileupEventsN) {
			mcHit.id  = sensorId;
			mcHit.de  = fpEvtStr->VTXde[i];
			mcHit.x   = fpEvtStr->VTXxin[i];
			mcHit.y   = fpEvtStr->VTXyin[i];
			mcHit.zi  = fpEvtStr->VTXzin[i];
			mcHit.zo  = fpEvtStr->VTXzout[i];
			storedEvtInfo.push_back(mcHit);
		}

		// Digitizing
//      don't change anything ?
//      Int_t genPartID = fpEvtStr->VTXid[i] - 1;
//      if (fpEvtStr->TRcha[genPartID] < 1) continue;
      
      if (ValidHistogram()) {
         fpHisDeTot->Fill(fpEvtStr->VTXde[i]*TAVTbaseDigitizer::GeV2keV());
         fpHisDeSensor[sensorId]->Fill(fpEvtStr->VTXde[i]*TAVTbaseDigitizer::GeV2keV());
      }
      
		if (!fDigitizer->Process(fpEvtStr->VTXde[i], fpEvtStr->VTXxin[i], fpEvtStr->VTXyin[i], fpEvtStr->VTXzin[i], fpEvtStr->VTXzout[i])) continue;
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
void TAVTactNtuMC::FillPixels(Int_t sensorId, Int_t hitId )
{
	TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
	TAVTntuRaw* pNtuRaw = (TAVTntuRaw*) fpNtuRaw->Object();
 
	map<int, double> digiMap = fDigitizer->GetMap();
	int nPixelX = fDigitizer->GetNPixelX();
 
	// fill pixels from map
   int count = 0;
	for ( map< int, double >::iterator it = digiMap.begin(); it != digiMap.end(); ++it) {

	   if ( digiMap[it->first] > 0 ) {
         count++;
			int line = it->first / nPixelX;
			int col  = it->first % nPixelX;
         
			TAVTntuHit* pixel = (TAVTntuHit*)pNtuRaw->NewPixel(sensorId, 1., line, col);

         Int_t genPartID = fpEvtStr->VTXid[hitId] - 1;
         pixel->SetMCid(genPartID);
         SetMCinfo(pixel, hitId);


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
void TAVTactNtuMC::FillNoise()
{
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
      FillNoise(i);
   }
}

//___________________________________
void TAVTactNtuMC::FillNoise(Int_t sensorId)
{
	TAVTntuRaw* pNtuRaw = (TAVTntuRaw*) fpNtuRaw->Object();

	Int_t pixelsN = gRandom->Uniform(0, fNoisyPixelsN);
	for (Int_t i = 0; i < pixelsN; ++i) {
	   Int_t col  = gRandom->Uniform(0,fDigitizer->GetNPixelX());
	   Int_t line = gRandom->Uniform(0,fDigitizer->GetNPixelY());
	   TAVTntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., line, col);
	   pixel->SetMCid(fgMcNoiseId);
	}
}

//------------------------------------------+-----------------------------------
void TAVTactNtuMC::SetMCinfo(TAVTntuHit* pixel, Int_t hitId)
{
   int genPartID = fpEvtStr->VTXid[hitId] - 1;
   
   // check true particle ID linked to the hit is in the correct range
   if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
      Warning("TAVTactNtuMC::SetMCinfo()", "wrong generate particle ID: %d nPart = %d", genPartID, fpEvtStr->TRn);
      return;
   }
   
   if ( fDebugLevel> 0 )     {
      cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;
      cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
      cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
   }
   
   
   // global coordinates
   TVector3 MCpos = TVector3((fpEvtStr->VTXxin[hitId]  + fpEvtStr->VTXxout[hitId])/2,  (fpEvtStr->VTXyin[hitId]  + fpEvtStr->VTXyout[hitId])/2,  (fpEvtStr->VTXzin[hitId]  + fpEvtStr->VTXzout[hitId])/2);
   TVector3 MCmom = TVector3((fpEvtStr->VTXpxin[hitId] + fpEvtStr->VTXpxout[hitId])/2, (fpEvtStr->VTXpyin[hitId] + fpEvtStr->VTXpyout[hitId])/2, (fpEvtStr->VTXpzin[hitId] + fpEvtStr->VTXpzout[hitId])/2);
   
   if ( fDebugLevel> 0 )     {
      cout << "Vertex pixel hit n: " << hitId << ". Col " << pixel->GetPixelColumn() << " row "<< pixel->GetPixelLine() << endl;
      cout << "\tGlobal kinematic: \n\t\tPos:\t";
      MCpos.Print();
      cout << "\t\tMom:\t";
      MCmom.Print();
   }
   
   
   pixel->SetMCPosition(MCpos);   // set in local coord (transformation in Hit)
   pixel->SetMCMomentum(MCmom);   // set in local coord
   pixel->SetEneLoss(fpEvtStr->VTXde[hitId]);
   
   // store generated particle info
   pixel->SetGeneratedParticleInfo( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                                   fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                                   TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                                   TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );
}

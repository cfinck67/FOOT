/*!
  \file
  \version $Id: TAMSDactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAMSDactNtuMC.
*/

#include "TH2F.h"
#include <map>
#include "TF1.h"
#include "TMath.h"
#include "TDirectory.h"


#include "TAMSDparGeo.hxx"
#include "TAMSDparConf.hxx"

#include "TAMSDntuRaw.hxx"


#include "TAMSDactNtuMC.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

#include "GlobalPar.hxx"

/*!
  \class TAMSDactNtuMC"
  \brief NTuplizer for vertex raw hits. **
*/

using namespace std;

ClassImp(TAMSDactNtuMC);

//------------------------------------------+-----------------------------------
//
TAMSDactNtuMC::TAMSDactNtuMC(const char* name, TAGdataDsc* pNtuRaw,  TAGparaDsc* pGeoMap, EVENT_STRUCT* evStr)
 : TAVTactBaseNtuMC(name, pGeoMap, evStr),
   fpNtuRaw(pNtuRaw)
{
	AddDataOut(pNtuRaw, "TAMSDntuRaw");
	AddPara(pGeoMap, "TAMSDparGeo");

   CreateDigitizer();
}


//------------------------------------------+-----------------------------------
//! Create histogram
void TAMSDactNtuMC::CreateDigitizer()
{
   TAMSDparGeo* pGeoMap  = (TAMSDparGeo*) fpGeoMap->Object();
   fDigitizer = new TAMSDdigitizer(pGeoMap);
}


//------------------------------------------+-----------------------------------
//! Action.
bool TAMSDactNtuMC::Action()
{
	if ( fDebugLevel> 0 )     
	  Info("TAMSDactNtuMC::Action()", "start  -->  VTn : %d  ", fpEvtStr->MSDn);

	TAMSDntuRaw* pNtuRaw = (TAMSDntuRaw*) fpNtuRaw->Object();
	pNtuRaw->Clear();
   
	// Loop over all MC hits
	for (Int_t i = 0; i < fpEvtStr->MSDn; i++) {
		if ( fDebugLevel> 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;

		Int_t sensorId = fpEvtStr->MSDilay[i];

		// Digitizing
      if (ValidHistogram()) {
         fpHisDeTot->Fill(fpEvtStr->MSDde[i]*TAVTbaseDigitizer::GeV2keV());
         fpHisDeSensor[sensorId]->Fill(fpEvtStr->MSDde[i]*TAVTbaseDigitizer::GeV2keV());
      }
      
		if (!fDigitizer->Process(fpEvtStr->MSDde[i], fpEvtStr->MSDxin[i], fpEvtStr->MSDyin[i], fpEvtStr->MSDzin[i], fpEvtStr->MSDzout[i])) continue;

		FillPixels(sensorId, i);
		
		if (ValidHistogram()) {
         Int_t pixelsN = fDigitizer->GetMap().size();
         fpHisPixel[sensorId]->Fill(pixelsN);
         fpHisPixelTot->Fill(pixelsN);
		}
   }


   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}


//------------------------------------------+-----------------------------------
void TAMSDactNtuMC::FillPixels(Int_t sensorId, Int_t hitId )
{
	TAMSDparGeo* pGeoMap = (TAMSDparGeo*) fpGeoMap->Object();
	TAMSDntuRaw* pNtuRaw = (TAMSDntuRaw*) fpNtuRaw->Object();
 
	map<int, double> digiMap = fDigitizer->GetMap();
	int nPixelX = fDigitizer->GetNPixelX();
 
	// fill pixels from map
   int count = 0;
	for ( map< int, double >::iterator it = digiMap.begin(); it != digiMap.end(); ++it) {

	   if ( digiMap[it->first] > 0 ) {
         count++;
			int line = it->first / nPixelX;
			int col  = it->first % nPixelX;
         
			TAMSDntuHit* pixel = (TAMSDntuHit*)pNtuRaw->NewPixel(sensorId, digiMap[it->first], line, col);

         Int_t genPartID = fpEvtStr->MSDid[hitId] - 1;
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
void TAMSDactNtuMC::FillNoise()
{
   TAMSDparGeo* pGeoMap = (TAMSDparGeo*) fpGeoMap->Object();
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
      FillNoise(i);
   }
}

//___________________________________
void TAMSDactNtuMC::FillNoise(Int_t sensorId)
{
	TAMSDntuRaw* pNtuRaw = (TAMSDntuRaw*) fpNtuRaw->Object();

	Int_t pixelsN = gRandom->Uniform(0, fNoisyPixelsN);
	for (Int_t i = 0; i < pixelsN; ++i) {
	   Int_t col  = gRandom->Uniform(0,fDigitizer->GetNPixelX());
	   Int_t line = gRandom->Uniform(0,fDigitizer->GetNPixelY());
	   TAMSDntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., line, col);
	   pixel->SetMCid(fgMcNoiseId);
	}
}

//------------------------------------------+-----------------------------------
void TAMSDactNtuMC::SetMCinfo(TAMSDntuHit* pixel, Int_t hitId)
{
   int genPartID = fpEvtStr->MSDid[hitId] - 1;
   
   // check true particle ID linked to the hit is in the correct range
   if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
      Warning("TAMSDactNtuMC::SetMCinfo()", "wrong generate particle ID: %d nPart = %d", genPartID, fpEvtStr->TRn);
      return;
   }
   
   if ( fDebugLevel> 0 )     {
      cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;
      cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
      cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
   }
   
   
   // global coordinates
   TVector3 MCpos = TVector3((fpEvtStr->MSDxin[hitId]  + fpEvtStr->MSDxout[hitId])/2,  (fpEvtStr->MSDyin[hitId]  + fpEvtStr->MSDyout[hitId])/2,  (fpEvtStr->MSDzin[hitId]  + fpEvtStr->MSDzout[hitId])/2);
   TVector3 MCmom = TVector3((fpEvtStr->MSDpxin[hitId] + fpEvtStr->MSDpxout[hitId])/2, (fpEvtStr->MSDpyin[hitId] + fpEvtStr->MSDpyout[hitId])/2, (fpEvtStr->MSDpzin[hitId] + fpEvtStr->MSDpzout[hitId])/2);
   
   if ( fDebugLevel> 0 )     {
      cout << "Vertex pixel hit n: " << hitId << ". Col " << pixel->GetPixelColumn() << " row "<< pixel->GetPixelLine() << endl;
      cout << "\tGlobal kinematic: \n\t\tPos:\t";
      MCpos.Print();
      cout << "\t\tMom:\t";
      MCmom.Print();
   }
   
   
   pixel->SetMCPosition(MCpos);   // set in local coord (transformation in Hit)
   pixel->SetMCMomentum(MCmom);   // set in local coord
   pixel->SetEneLoss(fpEvtStr->MSDde[hitId]);
   
   // store generated particle info
   pixel->SetGeneratedParticleInfo( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                                   fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                                   TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                                   TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );
}

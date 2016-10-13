/*!
  \file
  \version $Id: TAITactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAITactNtuMC.
*/

#include "TH2F.h"

#include "TAITparGeo.hxx"
#include "TAITparMap.hxx"
#include "TAITparConf.hxx"
#include "TAITntuRaw.hxx"
#include "TAITactNtuMC.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

/*!
  \class TAITactNtuMC"
  \brief NTuplizer for vertex raw hits. **
*/

ClassImp(TAITactNtuMC);

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
    fDebugLevel(0)
{
  AddDataOut(pNtuRaw, "TAITntuRaw");
  AddPara(pGeoMap, "TAITparGeo");
  AddPara(pParMap, "TAITparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactNtuMC::~TAITactNtuMC()
{

}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAITactNtuMC::CreateHistogram()
{
  DeleteHistogram();
  TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
    if (TAITparConf::IsMapHistOn()) {
      fpHisPixelMap[i] 
	= new TH2F(Form("vtPixelMap%d", i+1)
		   , Form("Vertex - pixel map for sensor %d", i+1), 
		   pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu(), 
		   pGeoMap->GetPixelsNv(), 0, pGeoMap->GetPixelsNv());
      fpHisPixelMap[i]->SetStats(kFALSE);
      AddHistogram(fpHisPixelMap[i]);
    }
  }
  
  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
    if (TAITparConf::IsMapHistOn()) {
      fpHisPosMap[i] = 
	new TH2F(Form("vtPosMap%d", i+1), 
		 Form("Vertex - position map for sensor %d", i+1), 
		 pGeoMap->GetPixelsNu(), -pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(), pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(),
		 pGeoMap->GetPixelsNv(), -pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv(), pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv());
      fpHisPosMap[i]->SetStats(kFALSE);
      AddHistogram(fpHisPosMap[i]);
    }
    
    fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
			       pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu());
    AddHistogram(fpHisRateMap[i]);
    
    fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
				10, 0, 5);
    AddHistogram(fpHisRateMapQ[i]);
  }
  
  SetValidHistogram(kTRUE);
  return;
}


//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAITactNtuMC::Action()
{
  TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
  TAITparMap* pParMap = (TAITparMap*) fpParMap->Object();
  TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();

  bool doXchk = kFALSE;

  TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fGeoTrafo == 0x0)
    Error("SetGeoTrafoName","No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());

  pNtuRaw->Clear();

  Int_t mcID(-1000);

   if (fDebugLevel)
     Info("Action()","Processing n :: %2d hits \n",fpEvtStr->nIT);

   //AS  To be completely rechecked...
   for (Int_t i = 0; i < fpEvtStr->nIT; i++) {

     /*
     // position in global transform in local
     // missing value set to 1. cos binary
     // miSigChips starts at 1 !!!
     Int_t sensorId    = pParMap->GetRealId(fpEvtStr->miSigChip[i]-1);
     //     TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., fpEvtStr->miSigRow[i], fpEvtStr->miSigCol[i]);
     */
     Int_t sensorId    = 0;

     //What About a decent post processing?
     //The column refer to Y!!!
     int myTrow, myTcol;
     myTrow = fpEvtStr->irowIT[i];
     myTcol = fpEvtStr->icolIT[i];
     /*
     myTcol = pParMap->GetPixelsNu()-fpEvtStr->miSigCol[i];
     myTrow = pParMap->GetPixelsNv()-fpEvtStr->miSigRow[i];
     */

     TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., myTrow, myTcol);
     //ID matching for the "trk" block
     //     mcID = fpEvtStr->miSigId[i];
     mcID = fpEvtStr->idIT[i];
     pixel->SetMCid(mcID);

     //Need IDX matching
     TVector3 MCmom(0,0,0); 
     TVector3 MCpos(0,0,0); 

     MCpos.SetXYZ((fpEvtStr->xinIT[i]+fpEvtStr->xoutIT[i])/2,(fpEvtStr->yinIT[i]+fpEvtStr->youtIT[i])/2,(fpEvtStr->zinIT[i]+fpEvtStr->zoutIT[i])/2);
     MCmom.SetXYZ((fpEvtStr->pxinIT[i]+fpEvtStr->pxoutIT[i])/2,(fpEvtStr->pyinIT[i]+fpEvtStr->pyoutIT[i])/2,(fpEvtStr->pzinIT[i]+fpEvtStr->pzoutIT[i])/2);
     pixel->SetMCPosition(MCpos);
     pixel->SetMCMomentum(MCmom);
     //     pixel->SetEneLoss(fpEvtStr->miSigDE[i]);  // VM added 3/11/13
     pixel->SetEneLoss(fpEvtStr->deIT[i]);  // VM added 3/11/13

     /*
     if (fDebugLevel)
       printf("Sensor %d line %d col %d\n", sensorId, fpEvtStr->miSigRow[i], fpEvtStr->miSigCol[i]);
     
     if (fDebugLevel) {
       printf("Recoed Id %d X %f Y %f Z %f\n", sensorId, fpEvtStr->miSigX[i], fpEvtStr->miSigY[i], fpEvtStr->miSigZ[i]);
       printf("Id %d X %f Y %f\n",      sensorId, pParMap->GetPositionU(fpEvtStr->miSigCol[i]), pParMap->GetPositionV(fpEvtStr->miSigRow[i]));
       printf("Id %d X %f Y %f\n",      sensorId, -pParMap->GetPositionV(fpEvtStr->miSigCol[i]), pParMap->GetPositionU(fpEvtStr->miSigRow[i]));
     }
     */
     double v = pParMap->GetPositionV(myTrow);
     double u = pParMap->GetPositionU(myTcol);
     TVector3 pos(v,u,0);
     pixel->SetPosition(pos);

   /*
     if(doXchk) {
       TVector3* senCen = pGeoMap->GetPosition(sensorId);
       TVector3 posi = pixel->GetPosition();
       TVector3 posiCm(posi.X()/10000,posi.Y()/10000,posi.Z()/10000);
       TVector3 posGlb = fGeoTrafo->FromVTLocalToGlobal(posiCm);
       
       if( (fabs(MCpos.X() - (posGlb.X()+senCen->X()/10000))>0.007) || (fabs(MCpos.Y() - (posGlb.Y()+senCen->Y()/10000))>0.007) ) {
	 
	 cout<<"mc:: "<<MCpos.X()<<" "<<MCpos.Y()<<" "<<MCpos.Z()<<" "<<sensorId<<" "<<fpEvtStr->miSigChip[i]<<endl;
	 
	 cout<<"GLB:: "<<posGlb.X()+senCen->X()/10000<<" "<<posGlb.Y()+senCen->Y()/10000<<" "<<posGlb.Z()+senCen->Z()/10000<<endl;
       }
     }

     if (ValidHistogram()) {
       if (TAITparConf::IsMapHistOn()) {
	 fpHisPixelMap[sensorId]->Fill(fpEvtStr->miSigCol[i], fpEvtStr->miSigRow[i]);
	 fpHisPosMap[sensorId]->Fill(pos[0], pos[1]);
       }
       Int_t aColumn = fpEvtStr->miSigCol[i];
       fpHisRateMap[sensorId]->Fill(aColumn);
       
       for (Int_t k = 0; k < 4; ++k) {
	 if (aColumn >= 258*k && aColumn < (k+1)*258)
	   fpHisRateMapQ[sensorId]->Fill(k+1);
       }
     }	 
   */
   }
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}


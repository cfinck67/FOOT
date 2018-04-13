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

#include "GlobalPar.hxx"
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
 //  DeleteHistogram();
 //  TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAITparConf::IsMapHistOn()) {
 //      fpHisPixelMap[i] 
    // = new TH2F(Form("vtPixelMap%d", i+1)
    //     , Form("Vertex - pixel map for sensor %d", i+1), 
    //     pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX(), 
    //     pGeoMap->GetNPixelY(), 0, pGeoMap->GetNPixelY());
 //      fpHisPixelMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPixelMap[i]);
 //    }
 //  }
  
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAITparConf::IsMapHistOn()) {
 //      fpHisPosMap[i] = 
    // new TH2F(Form("vtPosMap%d", i+1), 
    //   Form("Vertex - position map for sensor %d", i+1), 
    //   pGeoMap->GetNPixelX(), -pGeoMap->GetPitchX()/2*pGeoMap->GetNPixelX(), pGeoMap->GetPitchX()/2*pGeoMap->GetNPixelX(),
    //   pGeoMap->GetNPixelY(), -pGeoMap->GetPitchY()/2*pGeoMap->GetNPixelY(), pGeoMap->GetPitchY()/2*pGeoMap->GetNPixelY());
 //      fpHisPosMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPosMap[i]);
 //    }
    
 //    fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
    //             pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX());
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

Bool_t TAITactNtuMC::Action()
{
  TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
  TAITparMap* pParMap = (TAITparMap*) fpParMap->Object();
  TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();

  // bool doXchk = kFALSE;    // era della vecchia ricostruzione

  TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fGeoTrafo == 0x0)
    Error("SetGeoTrafoName","No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());

  pNtuRaw->Clear();

  Int_t mcID(-1000);

   if (fDebugLevel) Info("Action()","Processing n :: %2d hits \n",fpEvtStr->ITRn);
    if ( GlobalPar::GetPar()->Debug() > 0 )         cout<< endl << "ITn   " << fpEvtStr->ITRn<< endl;
   
   //AS  To be completely rechecked...
   for (Int_t i = 0; i < fpEvtStr->ITRn; i++) {

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
     // !!!!!!!!!!!!!!!!!!!!!!!!!!!  in ntuple, the row and col start from 0  !!!!!!!!!!!!!!!!!!!!!!!
     int myTrow, myTcol;
     myTrow = fpEvtStr->ITRirow[i] - 1;
     myTcol = fpEvtStr->ITRicol[i] - 1;
     /*
     myTcol = pParMap->GetNPixelX()-fpEvtStr->miSigCol[i];
     myTrow = pParMap->GetNPixelY()-fpEvtStr->miSigRow[i];
     */


      // Generated particle ID 
     int genPartID = fpEvtStr->ITRid[i] - 1;
    
    // check true particle ID linked to the hit is in the correct range
    if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
        cout << "TADCactNtuMC::Action :: ERROR >> wrong generate particle ID: "<< genPartID << " (nPart= " << fpEvtStr->TRn << endl;
        exit(0);
    }

    // take only hits linked to specific particle
    // if ( fpEvtStr->TRfid[genPartID] != -6 || fpEvtStr->TRcha[genPartID] != 2 )    continue;
    // cout << "ALPHA found! "  << endl;
    // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 3 )    continue;
    // if ( fpEvtStr->TRfid[genPartID] != -2 )    continue;
    // cout << "LITIUM found! "  << endl;
    // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 6 )    continue;
    // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 6 ||
    //               fpEvtStr->TRmass[genPartID] < 10 ||  fpEvtStr->TRmass[genPartID] > 10.5 )    continue;
    // if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "CARBONIUM found! "  << endl;

    if ( GlobalPar::GetPar()->Debug() > 0 )     {
         cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << " and mass = " << fpEvtStr->TRmass[genPartID] << endl;
        // double momentum = sqrt( fpEvtStr->TRipx[genPartID]*fpEvtStr->TRipx[genPartID] +
        //                   fpEvtStr->TRipy[genPartID]*fpEvtStr->TRipy[genPartID] +
        //                   fpEvtStr->TRipz[genPartID]*fpEvtStr->TRipz[genPartID] );
        // cout << "\t\t\tmomentum: " << momentum << endl;
        cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
        cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
    }
     



    // TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., myTrow, myTcol);
    TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., myTrow, myTcol);
    //ID matching for the "trk" block
    //     mcID = fpEvtStr->miSigId[i];
    pixel->SetItrGeo(pGeoMap);
    mcID = fpEvtStr->ITRid[i];
    pixel->SetMCid(mcID);
    pixel->SetLayer( fpEvtStr->ITRilay[i] );

    //Need IDX matching
    TVector3 MCmom(0,0,0); 
    TVector3 MCpos(0,0,0); 

    MCpos.SetXYZ((fpEvtStr->ITRxin[i]+fpEvtStr->ITRxout[i])/2,(fpEvtStr->ITRyin[i]+fpEvtStr->ITRyout[i])/2,(fpEvtStr->ITRzin[i]+fpEvtStr->ITRzout[i])/2);
    MCmom.SetXYZ((fpEvtStr->ITRpxin[i]+fpEvtStr->ITRpxout[i])/2,(fpEvtStr->ITRpyin[i]+fpEvtStr->ITRpyout[i])/2,(fpEvtStr->ITRpzin[i]+fpEvtStr->ITRpzout[i])/2);
     
    if ( GlobalPar::GetPar()->Debug() > 0 )     {
        cout << "InnerTracker pixel " << i << " col " << myTcol << " row "<< myTrow << endl;
        cout << "\tGlobal kinematic: \n\t\tPos:\t"; 
        MCpos.Print();
        cout << "\t\tMom:\t";
        MCmom.Print();
    }
     
     // change to local
     pGeoMap->Global2Local( &MCpos );
     pGeoMap->Global2Local_RotationOnly( &MCmom );

     pixel->SetMCPosition(MCpos);
     pixel->SetMCMomentum(MCmom);
     //     pixel->SetEneLoss(fpEvtStr->miSigDE[i]);  // VM added 3/11/13
     pixel->SetEneLoss(fpEvtStr->ITRde[i]);  // VM added 3/11/13

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

     pixel->SetGeneratedParticleInfo ( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                    fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                    TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                    TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID])  );



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


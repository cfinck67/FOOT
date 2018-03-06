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

Bool_t TAITactNtuMC::Action()   {

    vector<int> blackList;
    if ( GlobalPar::GetPar()->Debug() > 0 )         cout<< endl << "ITn :  " << fpEvtStr->ITRn<< endl;

    // container of the hits
    TAITntuRaw* pNtuRaw = (TAITntuRaw*) fpNtuRaw->Object();
    pNtuRaw->Clear();

    TAITparMap* pParMap = (TAITparMap*) fpParMap->Object(); // deprecated 
    // detector geometry class
    TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();


   // loop over hits
   for (Int_t i = 0; i < fpEvtStr->ITRn; i++) {

        int sensorId  = 0;
        // int sensorId  = 1*fpEvtStr->ITRimimo[i] + 4*fpEvtStr->ITRiplume[i] + 16*fpEvtStr->ITRilay[i];

        if ( GlobalPar::GetPar()->Debug() > 0 ) cout << "sensorId "<< fpEvtStr->ITRimimo[i] << " + "<< fpEvtStr->ITRiplume[i] << " + "<< fpEvtStr->ITRilay[i] << endl;

        //The column refer to Y!!!
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!  in ntuple, the row and col start from 0  !!!!!!!!!!!!!!!!!!!!!!!
        int myTrow, myTcol;
        myTrow = fpEvtStr->ITRirow[i];
        myTcol = fpEvtStr->ITRicol[i];



        ////////  "DECLUSTER" HACK waiting for clustering   ///////////////////////////////////////
        // remove all the hits adiacent to the current one

        // check if the current event is in the blacklist. if so skip.
        for ( int bl = 0; bl<blackList.size(); bl++ ) {
            if ( blackList.at(bl) == i )
            continue;   // next event
        }

        // loop on the remaining hits
        for ( int j = i+1; j < fpEvtStr->ITRn; j++) {   

            // check if same sensor 
            if ( (fpEvtStr->ITRimimo[i] != fpEvtStr->ITRimimo[j]) || (fpEvtStr->ITRiplume[i] != fpEvtStr->ITRiplume[j]) || (fpEvtStr->ITRilay[i] != fpEvtStr->ITRilay[j]) )
                continue;

            bool decluster_inner = false;
            // search if the j-hit is close to the i-hit. is so decluster_inner TRUE
            for ( int k = -1; k <= 1; k++ ) {
                for ( int h = -1; h <= 1; h++ ) {
                    if   ( myTrow == fpEvtStr->ITRirow[j]+k && myTcol == fpEvtStr->ITRicol[j]+h )   {
                        decluster_inner = true;
                        break;
                    }
                }
                if ( decluster_inner )    break;
            }

            // 
            if ( decluster_inner ) {
                blackList.push_back( j );
            }

        }
        // DECLUSTER end
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////


        // Generated particle ID 
        int genPartID = fpEvtStr->ITRid[i] - 1;
    
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
         
        // CREATE HERE THE HIT OBJECT
        TAITntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., myTrow, myTcol);    // sensor id is fake here   ----   fix once a stable geometry
        // start filling hit with info!
        pixel->SetItrGeo(pGeoMap); // first to set!!!!
        pixel->SetMCid( fpEvtStr->ITRid[i] );
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
     
        // input in global cartesian coordinates, change to local in pixel
        pixel->SetMCPosition(MCpos);
        pixel->SetMCMomentum(MCmom);
        pixel->SetEneLoss(fpEvtStr->ITRde[i]);

        double v = pParMap->GetPositionV(myTrow);
        double u = pParMap->GetPositionU(myTcol);
        TVector3 pos(v,u,0);
        pixel->SetPosition(pos);

        // in global frame
        pixel->SetHitCoordinate( pGeoMap->GetPosition( fpEvtStr->ITRilay[i], fpEvtStr->ITRimimo[i], fpEvtStr->ITRiplume[i],  myTcol, myTrow ) );

        // set info of the generating particle  --  waiting to resurrect the proper class
        pixel->SetGeneratedParticleInfo ( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
                            fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
                            TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
                            TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID])  );


   }    // END - loop over hits
   
   fpNtuRaw->SetBit(kValid);
   return kTRUE;
}


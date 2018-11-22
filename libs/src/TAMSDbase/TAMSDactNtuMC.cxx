/*!
  \file
  \version $Id: TAMSDactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TAMSDactNtuMC.
*/

#include "TH2F.h"

#include "TAMSDparGeo.hxx"
// #include "TAMSDparMap.hxx"
#include "TAMSDparConf.hxx"
// #include "TAMSDntuRaw.hxx"
#include "TAMSDactNtuMC.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"

#include "GlobalPar.hxx"

/*!
  \class TAMSDactNtuMC"
  \brief NTuplizer for vertex raw hits. **
*/

ClassImp(TAMSDactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAMSDactNtuMC::TAMSDactNtuMC(const char* name,
						   TAGdataDsc* pNtuRaw, 
						   TAGparaDsc* pGeoMap,
						   TAGparaDsc* pParMap,
						   EVENT_STRUCT* evStr)
  : TAGaction(name, "TAMSDactNtuMC - NTuplize Vertex raw data"),
    fpNtuRaw(pNtuRaw),
    fpGeoMap(pGeoMap),
    fpParMap(pParMap),
    fpEvtStr(evStr),
    fDebugLevel(0)
{
  // AddDataOut(pNtuRaw, "TAMSDntuRaw");
  AddDataOut(pNtuRaw, "MSDcontainerPoint");
  AddPara(pGeoMap, "TAMSDparGeo");
  AddPara(pParMap, "TAMSDparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAMSDactNtuMC::~TAMSDactNtuMC()
{

}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAMSDactNtuMC::CreateHistogram()
{
 //  DeleteHistogram();
 //  TAMSDparGeo* pGeoMap  = (TAMSDparGeo*) fpGeoMap->Object();
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAMSDparConf::IsMapHistOn()) {
 //      fpHisPixelMap[i] 
	// = new TH2F(Form("vtPixelMap%d", i+1)
	// 	   , Form("Vertex - pixel map for sensor %d", i+1), 
	// 	   pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu(), 
	// 	   pGeoMap->GetPixelsNv(), 0, pGeoMap->GetPixelsNv());
 //      fpHisPixelMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPixelMap[i]);
 //    }
 //  }
  
 //  for (Int_t i = 0; i < pGeoMap->GetSensorsN(); ++i) {
 //    if (TAMSDparConf::IsMapHistOn()) {
 //      fpHisPosMap[i] = 
	// new TH2F(Form("vtPosMap%d", i+1), 
	// 	 Form("Vertex - position map for sensor %d", i+1), 
	// 	 pGeoMap->GetPixelsNu(), -pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(), pGeoMap->GetPitchU()/2*pGeoMap->GetPixelsNu(),
	// 	 pGeoMap->GetPixelsNv(), -pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv(), pGeoMap->GetPitchV()/2*pGeoMap->GetPixelsNv());
 //      fpHisPosMap[i]->SetStats(kFALSE);
 //      AddHistogram(fpHisPosMap[i]);
 //    }
    
 //    fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
	// 		       pGeoMap->GetPixelsNu(), 0, pGeoMap->GetPixelsNu());
 //    AddHistogram(fpHisRateMap[i]);
    
 //    fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
	// 			10, 0, 5);
 //    AddHistogram(fpHisRateMapQ[i]);
 //  }
  
 //  SetValidHistogram(kTRUE);
  return;
}


//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAMSDactNtuMC::Action() {

    // TAMSDntuRaw* pNtuRaw = (TAMSDntuRaw*) fpNtuRaw->Object();
    // TAMSD_ContainerHit* containerHit = (TAMSD_ContainerHit*) fpNtuRaw->Object();
    TAMSD_ContainerHit* containerHit = (TAMSD_ContainerHit*) gTAGroot->FindDataDsc("MSDcontainerHit", "TAMSD_ContainerHit")->Object();

    // TAMSDparMap* pParMap = (TAMSDparMap*) fpParMap->Object();
    // TAMSDparGeo* pGeoMap  = (TAMSDparGeo*) fpGeoMap->Object();
    // bool doXchk = kFALSE;    // era della vecchia ricostruzione

    TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
    if (fGeoTrafo == 0x0)
    Error("SetGeoTrafoName","No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());

    containerHit->Clear();

    if (fDebugLevel)     Info("Action()","Processing n :: %2d hits \n",fpEvtStr->MSDn);
    if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "MSDn   " << fpEvtStr->MSDn<< endl;
    // cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx << "  "<< fpEvtStr->TRfy << "  "<< fpEvtStr->TRfz << endl;


    vector<int> blackList;

    // Hit loop
    for (Int_t i = 0; i < fpEvtStr->MSDn; i++) {

        if ( GlobalPar::GetPar()->Debug() > 0 )     cout<< endl << "FLUKA id =   " << fpEvtStr->TRfx[i] << "  "<< fpEvtStr->TRfy[i] << "  "<< fpEvtStr->TRfz[i] << endl;
        /*
        // position in global transform in local
        // missing value set to 1. cos binary
        // miSigChips starts at 1 !!!
        Int_t sensorId    = pParMap->GetRealId(fpEvtStr->miSigChip[i]-1);
        //     TAMSDntuHit* pixel = pNtuRaw->NewPixel(sensorId, 1., fpEvtStr->miSigRow[i], fpEvtStr->miSigCol[i]);
        */

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        int layer = fpEvtStr->MSDilay[i];

        int stripX = fpEvtStr->MSDistripx[i];
        int stripY = fpEvtStr->MSDistripy[i];



        /////////////////////////////////////////////////////////////////////////////////////////////////////////////

        for ( int bl = 0; bl<blackList.size(); bl++ ) {
            if ( blackList.at(bl) == i )         continue;   // next event
        }


        // DECLUSTER
        bool decluster = false;
        for ( int j = i+1; j < fpEvtStr->MSDn; j++) {   // other hit loop

            // same sensor .....
            bool decluster_inner = false;
            for ( int k = -1; k <= 1; k++ ) {
                for ( int h = -1; h <= 1; h++ ) {
                    if   ( stripX == fpEvtStr->MSDistripx[j]+k && stripY == fpEvtStr->MSDistripy[j]+h )   {
                        decluster_inner = true;
                        break;
                    }
                } 
                if ( decluster_inner )    break;
            }   

            if ( decluster_inner ) {
                blackList.push_back( j );
                decluster = true;
            }

        }
        if ( decluster )   {
            blackList.push_back( i );
            continue;  // next event
        }
        
      // DECLUSTER end
/////////////////////////////////////////////////////////////////////////////////////////////////////////////




        // Generated particle ID 
        int genPartID = fpEvtStr->MSDid[i] - 1;
    
        // check true particle ID linked to the hit is in the correct range
        if ( genPartID < 0 || genPartID > fpEvtStr->TRn-1 ) {
            cout << "TAMSDactNtuMC::Action :: ERROR >> wrong generate particle ID: "<< genPartID << " nPart= " << fpEvtStr->TRn << endl;
            exit(0);
        }

        // take only hits linked to specific particle
        // if ( fpEvtStr->TRfid[genPartID] != -6 || fpEvtStr->TRcha[genPartID] != 2 )    continue;
        // cout << "ALPHA found! "  << endl;
        // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 3 )    continue;
        // cout << "LITIUM found! "  << endl;
        // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 6 )    continue;
        // if ( fpEvtStr->TRfid[genPartID] != -2 || fpEvtStr->TRcha[genPartID] != 6 ||
        //               fpEvtStr->TRmass[genPartID] < 10 ||  fpEvtStr->TRmass[genPartID] > 10.5 )    continue;
        // if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "CARBONIUM found! "  << endl;

        if ( GlobalPar::GetPar()->Debug() > 0 )     {
            cout << "Part type: " << fpEvtStr->TRfid[genPartID] << " and charge: " << fpEvtStr->TRcha[genPartID] << endl;
            // double momentum = sqrt( fpEvtStr->TRipx[genPartID]*fpEvtStr->TRipx[genPartID] +
            //                   fpEvtStr->TRipy[genPartID]*fpEvtStr->TRipy[genPartID] +
            //                   fpEvtStr->TRipz[genPartID]*fpEvtStr->TRipz[genPartID] );
            // cout << "\t\t\tmomentum: " << momentum << endl;
            cout << "Generated Position: " << fpEvtStr->TRix[genPartID] <<" "<<fpEvtStr->TRiy[genPartID]<<" "<<fpEvtStr->TRiz[genPartID] << endl;
            cout << "Generated Momentum: " << fpEvtStr->TRipx[genPartID] <<" "<<fpEvtStr->TRipy[genPartID]<<" "<<fpEvtStr->TRipz[genPartID] << endl;
        }

// !!!!!!!!!!!!!!!!!!!!!!!!!!!  in ntuple, the row and col start from 0  !!!!!!!!!!!!!!!!!!!!!!
     
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // Modify chnagig the geometry
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        int sensorId = layer*2;
        // divide the hit in 2 layers, also energy loss is divided
        // one strip for each sensor
        TAMSD_Hit* hitX = containerHit->NewHit( sensorId, stripX, 0.5*fpEvtStr->MSDde[i], fpEvtStr->MSDtim[i], i, genPartID );
        FillAdditonalInfo(hitX, i);
        TAMSD_Hit* hitY = containerHit->NewHit( sensorId+1, stripY,  0.5*fpEvtStr->MSDde[i], fpEvtStr->MSDtim[i], i, genPartID);
        FillAdditonalInfo(hitY, i);

     
    }


    // fill points
    // container of points
    TAMSD_ContainerPoint* containerPoint = (TAMSD_ContainerPoint*) gTAGroot->FindDataDsc("containerPoint", "TAMSD_ContainerPoint")->Object();

    // loop over layers, to consider sensor column and row. 
    // count sensors two by tow
    for (int iLayer=0; iLayer < containerHit->GetNSensors(); iLayer += 2 ) {  

        // built the points from the hits
        for (int iCol=0; iCol < containerHit->GetHitN( iLayer ); iCol++) {  // col loop
            
            // FOR TEST, REMOVE
            // if ( !containerHit->GetHit( 0, iCol )->IsColumn() )       cout <<"TAMSDactNtuMC::Action() Col"<<endl, exit(0);
            TAMSD_Hit* colHit = containerHit->GetHit( iLayer, iCol );
           

            for (int iRow=0; iRow < containerHit->GetHitN( iLayer+1 ); iRow++) {  // row loop 
                
                // FOR TEST, REMOVE
                // if ( !containerHit->GetHit( 1, iRow )->IsRow() )        cout <<"TAMSDactNtuMC::Action() Row"<<endl, exit(0);
                TAMSD_Hit* rowHit = containerHit->GetHit( iLayer+1, iRow );

                // TVector3 rowPos = rowHit->GetHitPosition_detector();
                // TVector3 colPos = colHit->GetHitPosition_detector();
                // TVector3 position ( colHit->GetHitCoordinate_detectorFrame().x(), rowHit->GetHitCoordinate_detectorFrame().y(), 
                //                     ( colHit->GetHitCoordinate_detectorFrame().z() + rowHit->GetHitCoordinate_detectorFrame().z() )/2 );

                // float pointX = 
                // float pointX = 
                // float pointZ = rowHit->GetHitPosition_detector().z() ;

                // containerPoint->NewPoint( iCol, colHit, iRow, rowHit );

                // TAMSD_Point* point = containerPoint->NewPoint( iCol, colHit, iRow, rowHit );

                // one point for each layer (half sensor number)
                int pointLayer = trunc( iLayer/2 );
                TAMSD_Point* point = containerPoint->NewPoint( pointLayer, iRow, rowHit, iCol, colHit );
                
                // point->SetColumnBar( iRow, rowHit );    // set pos and mom inside
                // point->SetRowBar( iCol, colHit );       // set pos and mom inside
                // point->FindTrueGhost(...);
                // point->SetMCPosition( MCpos );
                // point->SetMCMomentum( MCmom );

                if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "Point "<<iCol<<" "<<iRow<<" -> Col: "<<point->GetColumn()<<" row: "<<point->GetRow()<<endl;

                // ControlPlotsRepository::GetControlObject( "MSDcontrol" )->SetTW_HitPoint( "MSD___Point", point->GetColumn(), point->GetRow() );
            

            }
        }

    }



    containerHit->SetBit(kValid);
    containerPoint->SetBit(kValid);
    fpNtuRaw->SetBit(kValid);
    return kTRUE;
}






//------------------------------------------+-----------------------------------
void TAMSDactNtuMC::FillAdditonalInfo( TAMSD_Hit* hit, int i ) {

    TAMSDparGeo* pGeoMap  = (TAMSDparGeo*) fpGeoMap->Object();

    //Need IDX matching
    TVector3 MCmom(0,0,0); 
    TVector3 MCpos(0,0,0); 

    // global coordinates
    MCpos.SetXYZ((fpEvtStr->MSDxin[i]+fpEvtStr->MSDxout[i])/2,(fpEvtStr->MSDyin[i]+fpEvtStr->MSDyout[i])/2,(fpEvtStr->MSDzin[i]+fpEvtStr->MSDzout[i])/2);
    MCmom.SetXYZ((fpEvtStr->MSDpxin[i]+fpEvtStr->MSDpxout[i])/2,(fpEvtStr->MSDpyin[i]+fpEvtStr->MSDpyout[i])/2,(fpEvtStr->MSDpzin[i]+fpEvtStr->MSDpzout[i])/2);
     
    if ( GlobalPar::GetPar()->Debug() > 0 )     {
        // cout << "MSD pixel " << i << " lay " << fpEvtStr->MSDilay[i] << " col " << myTstrip << " row "<< myTview << endl;
        cout << "\tGlobal kinematic: \n\t\tPos:\t"; 
        MCpos.Print();
        cout << "\t\tMom:\t";
        MCmom.Print();
    }

// change to local
     pGeoMap->Global2Local( &MCpos );
     pGeoMap->Global2Local_RotationOnly( &MCmom );
     

     hit->SetMCPosition(MCpos);
     hit->SetMCMomentum(MCmom);
     // hit->SetEneLoss(fpEvtStr->MSDde[i]);  // VM added 3/11/13
     // store generated particle info
    // pixel->SetGeneratedParticleInfo ( genPartID, fpEvtStr->TRfid[genPartID], fpEvtStr->TRcha[genPartID],
    //                 fpEvtStr->TRbar[genPartID], fpEvtStr->TRmass[genPartID],
    //                 TVector3(fpEvtStr->TRix[genPartID], fpEvtStr->TRiy[genPartID], fpEvtStr->TRiz[genPartID]),
    //                 TVector3(fpEvtStr->TRipx[genPartID], fpEvtStr->TRipy[genPartID], fpEvtStr->TRipz[genPartID]) );



     
}



































/*!
  \file
  \version $Id: TATWactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TATWactNtuMC.
*/
#include "TAGgeoTrafo.hxx"

#include "TATWdatRaw.hxx"
#include "TATWactNtuMC.hxx"

/*!
  \class TATWactNtuMC TATWactNtuMC.hxx "TATWactNtuMC.hxx"
  \brief NTuplizer for BM raw hits. **
*/

ClassImp(TATWactNtuMC);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWactNtuMC::TATWactNtuMC(const char* name,
			 TAGdataDsc* p_datraw, 
			 EVENT_STRUCT* evStr)
  : TAGaction(name, "TATWactNtuMC - NTuplize ToF raw data"),
    m_hitContainer(p_datraw),
    m_eventStruct(evStr)
{
  AddDataOut(p_datraw, "TATW_ContainerHit");
}


//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TATWactNtuMC::CreateHistogram()
{
   
   DeleteHistogram();
   
   fpHisHitCol = new TH1F("twHitCol", "ToF Wall - Column hits", 22, 0., 22);
   AddHistogram(fpHisHitCol);
   
   fpHisHitLine = new TH1F("twHitLine", "ToF Wall - Line hits", 22, 0., 22);
   AddHistogram(fpHisHitLine);

   fpHisHitMap = new TH2F("twHitMap", "ToF Wall - Hit Map", 22, 0., 22, 22, 0, 22);
   AddHistogram(fpHisHitMap);

   fpHisDeTot = new TH1F("twDeTot", "ToF Wall - Total energy loss", 1000, 0., 200);
   AddHistogram(fpHisDeTot);

   fpHisDeTotMc = new TH1F("twMcDeTot", "ToF wall - MC Total energy loss", 1000, 0., 200);
   AddHistogram(fpHisDeTotMc);
   
   fpHisTimeTot = new TH1F("twTimeTot", "ToF Wall - Total time", 1000, 0., 200);
   AddHistogram(fpHisTimeTot);
   
   fpHisTimeTotMc = new TH1F("twMcTimeTot", "ToF wall - MC Total time", 1000, 0., 200);
   AddHistogram(fpHisTimeTotMc);
   
   SetValidHistogram(kTRUE);
}


//------------------------------------------+-----------------------------------
//! Action.

bool TATWactNtuMC::Action() {

    if ( fDebugLevel> 0 )     cout << "TATWactNtuMC::Action() start" << endl;


    TATW_ContainerHit* containerHit = (TATW_ContainerHit*) gTAGroot->FindDataDsc("containerHit", "TATW_ContainerHit")->Object(); // extremly dangerous !
    TATWparGeo* geoMap = (TATWparGeo*) gTAGroot->FindParaDsc("twGeo", "TATWparGeo")->Object();
    // int nhits(0);
    // if (!containerHit->m_listOfHits) containerHit->SetupClones();

    //The number of hits inside the Start Counter is stn
    if ( fDebugLevel> 0 )     cout << "Processing n Scint " << m_eventStruct->SCNn << endl;

    // fill the container of hits, divided by layer, i.e. the column at 0 and row at 1
    for (int i=0; i < m_eventStruct->SCNn; i++) { 
    
       Float_t edep  = m_eventStruct->SCNde[i]*TAGgeoTrafo::GevToMev();

        //First two numbers make sense only for data (typ, channel)
        // TATW_Hit *mytmp = new((*(containerHit->hir))[i]) 
        // TATWrawHit(0,0,m_eventStruct->SCNde[i],m_eventStruct->SCNtim[i]);

        // layer, bar, de, time, ntupID, parentID
        int view = ( m_eventStruct->SCNiview[i] == -1 ? 0 : 1 );    // in ntuple layers are -1 and 1
        //int view = ( m_eventStruct->SCNiview[i] == -1 ? 1 : 0 );    // in ntuple layers are -1 and 1
       
       if ( fDebugLevel> 0 )
          printf("%d %d\n", view,  m_eventStruct->SCNibar[i]);
       
        TATW_Hit* hit = containerHit->NewHit( view, m_eventStruct->SCNibar[i], m_eventStruct->SCNde[i], 
                                                m_eventStruct->SCNtim[i] );
       
        hit->AddMcTrackId(m_eventStruct->SCNid[i]-1, i);
       
       if (ValidHistogram()) {
          fpHisDeTotMc->Fill(edep);
          fpHisDeTot->Fill(hit->GetEnergyLoss()*TAGgeoTrafo::GevToMev());
          
          fpHisTimeTotMc->Fill(m_eventStruct->SCNtim[i]*TAGgeoTrafo::SecToNs() );
          fpHisTimeTot->Fill(hit->GetTime()*TAGgeoTrafo::SecToNs());

          
          if (hit->IsColumn())
             fpHisHitCol->Fill(hit->GetBar());
          else
             fpHisHitLine->Fill(hit->GetBar());
       }
    }



    // container of points, name of descriptor is hard coded
    TATW_ContainerPoint* containerPoint = (TATW_ContainerPoint*) gTAGroot->FindDataDsc("containerPoint", "TATW_ContainerPoint")->Object(); // extremly dangerous no check !!

    if ( fDebugLevel> 0 )     cout << "N Col: "<<containerHit->GetHitN( 0 )<<endl;
    if ( fDebugLevel> 0 )     cout << "N Row: "<<containerHit->GetHitN( 1 )<<endl;

    // built the points from the hits
    for (int iCol=0; iCol < containerHit->GetHitN( 0 ); iCol++) {  // col loop
        
        // FOR TEST, REMOVE
        if ( !containerHit->GetHit( 0, iCol )->IsColumn() )       cout <<"TATWactNtuMC::Action() cazzoCol"<<endl, exit(0);
        TATW_Hit* colHit = containerHit->GetHit( 0, iCol );
       

        for (int iRow=0; iRow < containerHit->GetHitN( 1 ); iRow++) {  // row loop 
            
            // FOR TEST, REMOVE
            if ( !containerHit->GetHit( 1, iRow )->IsRow() )        cout <<"TATWactNtuMC::Action() cazzoRow"<<endl, exit(0);
            TATW_Hit* rowHit = containerHit->GetHit( 1, iRow );

            // TVector3 rowPos = rowHit->GetHitPosition_detector();
            // TVector3 colPos = colHit->GetHitPosition_detector();
            // TVector3 position ( colHit->GetHitCoordinate_detectorFrame().x(), rowHit->GetHitCoordinate_detectorFrame().y(), 
            //                     ( colHit->GetHitCoordinate_detectorFrame().z() + rowHit->GetHitCoordinate_detectorFrame().z() )/2 );

            // float pointX = 
            // float pointX = 
            // float pointZ = rowHit->GetHitPosition_detector().z() ;

            // containerPoint->NewPoint( iCol, colHit, iRow, rowHit );

            // TATW_Point* point = containerPoint->NewPoint( iCol, colHit, iRow, rowHit );
            TATW_Point* point = containerPoint->NewPoint( iRow, rowHit, iCol, colHit );
            
            // point->SetColumnBar( iRow, rowHit );    // set pos and mom inside
            // point->SetRowBar( iCol, colHit );       // set pos and mom inside
            // point->FindTrueGhost(...);
            // point->SetMCPosition( MCpos );
            // point->SetMCMomentum( MCmom );

            if ( fDebugLevel> 0 )     cout << "Point "<<iCol<<" "<<iRow<<" -> Col: "<<point->GetColumn()<<" row: "<<point->GetRow()<<endl;

           // ControlPlotsRepository::GetControlObject( "TWcontrol" )->SetTW_HitPoint( "TW___Point", point->GetColumn(), point->GetRow() );
           if (ValidHistogram())
              fpHisHitMap->Fill(point->GetColumn(), point->GetRow());

        }

    }

    // containerHit->nirhit  = nhits;

    // m_hitContainer->SetBit(kValid);
    return true;
}




































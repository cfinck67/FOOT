/*!
  \file
  \version $Id: TATWactNtuMC.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
  \brief   Implementation of TATWactNtuMC.
*/

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
  Info("Action()"," Creating the Beam Monitor MC tuplizer action\n");
  AddDataOut(p_datraw, "TATWdatRaw");
}





//------------------------------------------+-----------------------------------
//! Action.

bool TATWactNtuMC::Action() {

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "TATWactNtuMC::Action() start" << endl;


    // TATW_ContainerHit* containerHit = (TATW_ContainerHit*) m_hitContainer->Object();
    TATW_ContainerHit* containerHit = (TATW_ContainerHit*) gTAGroot->FindDataDsc("containerHit", "TATW_ContainerHit")->Object();
    TATWparGeo* geoMap = (TATWparGeo*) gTAGroot->FindParaDsc("twGeo", "TATWparGeo")->Object();
    // int nhits(0);
    // if (!containerHit->m_listOfHits) containerHit->SetupClones();

    //The number of hits inside the Start Counter is stn
    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "Processing n Scint " << m_eventStruct->SCNn << endl;

    // fill the container of hits, divided by layer, i.e. the column at 0 and row at 1
    for (int i=0; i < m_eventStruct->SCNn; i++) { 
    
        //First two numbers make sense only for data (typ, channel)
        // TATW_Hit *mytmp = new((*(containerHit->hir))[i]) 
        // TATWrawHit(0,0,m_eventStruct->SCNde[i],m_eventStruct->SCNtim[i]);

        // layer, bar, de, time, ntupID, parentID
        int view = ( m_eventStruct->SCNiview[i] == -1 ? 0 : 1 );    // in ntuple layers are -1 and 1
        // int view = ( m_eventStruct->SCNiview[i] == -1 ? 1 : 0 );    // in ntuple layers are -1 and 1
        
        TATW_Hit* hit = containerHit->NewHit( view, m_eventStruct->SCNibar[i], m_eventStruct->SCNde[i], 
                                                m_eventStruct->SCNtim[i], i, m_eventStruct->SCNid[i]-1 );
        
        TVector3 MCpos = TVector3(  (m_eventStruct->SCNxin[i]  + m_eventStruct->SCNxout[i])/2,  
                                    (m_eventStruct->SCNyin[i]  + m_eventStruct->SCNyout[i])/2,  
                                    (m_eventStruct->SCNzin[i]  + m_eventStruct->SCNzout[i])/2 );
        TVector3 MCmom = TVector3(  (m_eventStruct->SCNpxin[i] + m_eventStruct->SCNpxout[i])/2, 
                                    (m_eventStruct->SCNpyin[i] + m_eventStruct->SCNpyout[i])/2, 
                                    (m_eventStruct->SCNpzin[i] + m_eventStruct->SCNpzout[i])/2 );    
        
        geoMap->Global2Local( &MCpos );
        geoMap->Global2Local_RotationOnly( &MCmom );

        hit->SetMCPosition( MCpos );
        hit->SetMCMomentum( MCmom );

        if ( GlobalPar::GetPar()->Debug() > 0 )    {
            cout << "Layer: "<<hit->GetLayer()<<" IsColumn: "<<hit->IsColumn()<<" Bar: "<<hit->GetBar()<< endl;
            cout<<"  GenPart: "<<hit->GetGenPartID()<< " Coord: "<<hit->GetHitCoordinate_footFrame()<<" Z: "<<hit->GetHitZ_footFrame()<<endl;
        }

        ControlPlotsRepository::GetControlObject( "TWcontrol" )->SetTW_HitView( "TW___Hit", hit->IsColumn(), hit->GetBar() );
        

    }



    // container of points
    TATW_ContainerPoint* containerPoint = (TATW_ContainerPoint*) gTAGroot->FindDataDsc("containerPoint", "TATW_ContainerPoint")->Object();

    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "N Col: "<<containerHit->GetHitN( 0 )<<endl;
    if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "N Row: "<<containerHit->GetHitN( 1 )<<endl;

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

            if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "Point "<<iCol<<" "<<iRow<<" -> Col: "<<point->GetColumn()<<" row: "<<point->GetRow()<<endl;

            ControlPlotsRepository::GetControlObject( "TWcontrol" )->SetTW_HitPoint( "TW___Point", point->GetColumn(), point->GetRow() );
        

        }

    }

    // containerHit->nirhit  = nhits;

    // m_hitContainer->SetBit(kValid);
    return true;
}




































#include "TString.h"
#include "TClonesArray.h"

#include "TACA_Hit.hxx"


ClassImp(TACA_Hit) // Description of Single Detector TATW_Hit 



//______________________________________________________________________________
//  build a hit from a rawHit
TACA_Hit::TACA_Hit( TACArawHit* hit )
: TObject(),
  // fSensorNumber(aSensorNumber),
  m_indexMC(-1)
{
   // constructor of a TATW_Hit from a base hit
   
  //     m_layer(aView),
  // m_bar(aBar),
      //  = hit->Time();
      //  = hit->Charge();
      //  = hit->ChID();
 
    Initialise();
    
}





//______________________________________________________________________________
// Build the hit in a BGO from its ID, enLoss and time
TACA_Hit::TACA_Hit ( int id_BGO , double enLoss , double time )
: TObject(),
  m_BGO(id_BGO),
  m_enLoss(enLoss),
  m_time(time)
{
    Initialise();

    // SetGenPartID( parentID );
}





//______________________________________________________________________________
//  
void TACA_Hit::Initialise() {

    m_coordinate = (0,0,0);
  
    m_posMC.SetXYZ(0, 0, 0);
    m_momMC.SetXYZ(0, 0, 0);

    m_genPartIndex = -1;
    m_genPartPointer = NULL;

    // take the detector geometry
    m_geometry = (TACAparGeo*) gTAGroot->FindParaDsc("caGeo", "TACAparGeo")->Object();

    // set center position
    if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "TACA_Hit::Initialise()  :: BGO ID : = " << m_BGO << endl;
    // m_coordinate = m_geometry->GetCoordiante_detectorFrame( m_BGO );

}



/*

//______________________________________________________________________________
void TATW_Hit::SetGenPartID( int agenPartID ) { 
    m_genPartIndex = agenPartID; 
    
    // find the pointer in the list
    if( !GlobalPar::GetPar()->IncludeEvent() )  return;

    TAGntuMCeve* ntup = (TAGntuMCeve*) gTAGroot->FindDataDsc("myn_mceve", "TAGntuMCeve")->Object();
    if ( m_genPartIndex < 0 || m_genPartIndex >= ntup->GetHitN() )  
      cout << "ERROR >> TATW_Hit::SetGenPartID  -->  No truth particle found with index "<< m_genPartIndex<<endl,  exit(0);
    m_genPartPointer = ntup->Hit( m_genPartIndex );

    // for (int i = 0; i < ntup->GetHitN(); i++) {   // over all sensors
    //     if ( ntup->Hit( i )->FlukaID() == m_genPartIndex ) {
    //         m_genPartPointer = ntup->Hit( i );
    //         // ntup->Hit( i )->AddVTXhit( this );  // x Alberto to implement <3
    //         return;
    //     }
    // }
    // cout << "ERROR >> TATW_Hit::SetGenPartID  -->  No truth particle found with index "<< m_genPartIndex<<endl;
    // exit(0);
}


*/

//______________________________________________________________________________
TVector3 TATW_Hit::GetMCPosition_sensorFrame() {
    TVector3 glob = m_posMC;
    m_geometry->Detector2Sensor_frame( m_BGOx, m_BGOy, &glob ); 
    return glob; 
}


//______________________________________________________________________________
TVector3 TATW_Hit::GetMCPosition_footFrame() { 
    TVector3 glob = m_posMC;
    m_geometry->Local2Global( &glob ); 
    return glob; 
};


//______________________________________________________________________________
TVector3 TATW_Hit::GetMCMomentum_footFrame() { 
    TVector3 globP = m_momMC;
    m_geometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
};   






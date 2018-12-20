



#include "TString.h"
#include "TClonesArray.h"

#include "TATW_Hit.hxx"


// layer 0 -->  column, asse X
// layer 1 -->  row, asse Y


ClassImp(TATW_Hit) // Description of Single Detector TATW_Hit 



//______________________________________________________________________________
//  build a hit from a rawHit
TATW_Hit::TATW_Hit( TATWrawHit* hit )
: TObject(),
  // fSensorNumber(aSensorNumber),
  m_indexMC(-1)
{
   // constructor of a TATW_Hit from a base hit
   
  //     m_layer(aView),
  // m_bar(aBar),
      //   = hit->Time();
      // = hit->Charge();
      //  = hit->ChID();
 
    Initialise();
    
}





//______________________________________________________________________________
// Build the hit from its sensor, line and column// constructor of a Pixel with column and line 
TATW_Hit::TATW_Hit ( int aView, int aBar, double aDe, double aTime, int aID, int parentID )
: TObject(),
  m_layer(aView),
  m_bar(aBar),
  m_indexMC(aID),
  m_de(aDe),
  m_time(aTime)
{
    Initialise();

    SetGenPartID( parentID );
}




//______________________________________________________________________________
//  
void TATW_Hit::Initialise() {

    m_coordinate = 0;
    m_z = 0;

    m_posMC.SetXYZ(0, 0, 0);
    m_momMC.SetXYZ(0, 0, 0);

    m_genPartIndex = -1;
    m_genPartPointer = NULL;

    // take the detector geometry
   m_geometry = (TATWparGeo*) gTAGroot->FindParaDsc(TATWparGeo::GetDefParaName(), "TATWparGeo")->Object();

   // set center position
    if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "TATW_Hit::Initialise()  ::  layer = " << m_layer << " bar = " << m_bar << endl;

   // transformation handle TATWparGeo not Hit itself
//    m_coordinate = m_geometry->GetCoordiante_detectorFrame( m_layer, m_bar );
//    m_z = m_geometry->GetZ_detectorFrame(  m_layer, m_bar  );

}





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



// transformation handle TATWparGeo not Hit itself

////______________________________________________________________________________
//TVector3 TATW_Hit::GetMCPosition_sensorFrame() {
//    TVector3 glob = m_posMC;
//    m_geometry->Detector2Sensor_frame( m_layer, m_bar, &glob ); 
//    return glob; 
//}
//
//
////______________________________________________________________________________
//TVector3 TATW_Hit::GetMCPosition_footFrame() { 
//    TVector3 glob = m_posMC;
//    m_geometry->Local2Global( &glob ); 
//    return glob; 
//};
//
//
////______________________________________________________________________________
//TVector3 TATW_Hit::GetMCMomentum_footFrame() { 
//    TVector3 globP = m_momMC;
//    m_geometry->Local2Global_RotationOnly( &globP ); 
//    return globP; 
//};























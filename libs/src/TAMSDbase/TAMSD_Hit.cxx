



#include "TString.h"
#include "TClonesArray.h"

#include "TAMSD_Hit.hxx"

// layer 0 -->  column, asse X
// layer 1 -->  row, asse Y


ClassImp(TAMSD_Hit) // Description of Single Detector TAMSD_Hit 



//______________________________________________________________________________
//  build a hit from a rawHit
TAMSD_Hit::TAMSD_Hit( TAMSDrawHit* hit )
: TObject(),
  // fSensorNumber(aSensorNumber),
  m_indexMC(-1)
{
   // constructor of a TAMSD_Hit from a base hit
 
    Initialise();
    
}





//______________________________________________________________________________
// Build the hit from its sensor, line and column// constructor of a Pixel with column and line 
TAMSD_Hit::TAMSD_Hit ( int aSensor, int aStrip, double aDe, double aTime, int aID, int parentID )
: TObject(),
  m_sensor(aSensor),
  m_strip(aStrip),
  m_indexMC(aID),
  m_de(aDe),
  m_time(aTime)
{

    Initialise();

    SetGenPartID( parentID );
}




//______________________________________________________________________________
//  
void TAMSD_Hit::Initialise() {

    m_coordinate = 0;
    m_z = 0;

    m_posMC.SetXYZ(0, 0, 0);
    m_momMC.SetXYZ(0, 0, 0);

    m_genPartIndex = -1;
    m_genPartPointer = NULL;

    m_layer = trunc( m_sensor/2 );
    m_view = m_sensor%2;

    // take the detector geometry
    m_geometry = (TAMSDparGeo*) gTAGroot->FindParaDsc("msdGeo", "TAMSDparGeo")->Object();

    // set center position
    if ( GlobalPar::GetPar()->Debug() > 1 )   cout << "TAMSD_Hit::Initialise()  ::  layer = " << m_layer << " strip = " << m_strip << endl;
    m_coordinate = m_geometry->GetStripCoordiante_detectorFrame( m_layer, m_view, m_strip );
    m_z = m_geometry->GetStripZ_detectorFrame(  m_layer, m_view, m_strip  );

}





//______________________________________________________________________________
void TAMSD_Hit::SetGenPartID( int agenPartID ) { 
    m_genPartIndex = agenPartID; 
    
    // find the pointer in the list
    if( !GlobalPar::GetPar()->IncludeEvent() )  return;

    TAGntuMCeve* ntup = (TAGntuMCeve*) gTAGroot->FindDataDsc("myn_mceve", "TAGntuMCeve")->Object();
    if ( m_genPartIndex < 0 || m_genPartIndex >= ntup->GetHitN() )  
      cout << "ERROR >> TAMSD_Hit::SetGenPartID  -->  No truth particle found with index "<< m_genPartIndex<<endl,  exit(0);
    m_genPartPointer = ntup->Hit( m_genPartIndex );

    // for (int i = 0; i < ntup->GetHitN(); i++) {   // over all sensors
    //     if ( ntup->Hit( i )->FlukaID() == m_genPartIndex ) {
    //         m_genPartPointer = ntup->Hit( i );
    //         // ntup->Hit( i )->AddVTXhit( this );  // x Alberto to implement <3
    //         return;
    //     }
    // }
    // cout << "ERROR >> TAMSD_Hit::SetGenPartID  -->  No truth particle found with index "<< m_genPartIndex<<endl;
    // exit(0);
}




//______________________________________________________________________________
TVector3 TAMSD_Hit::GetMCPosition_sensorFrame() {
    TVector3 glob = m_posMC;
    m_geometry->Detector2Sensor_frame( m_layer, m_view, m_strip, &glob ); 
    return glob; 
}


//______________________________________________________________________________
TVector3 TAMSD_Hit::GetMCPosition_footFrame() { 
    TVector3 glob = m_posMC;
    m_geometry->Local2Global( &glob ); 
    return glob; 
};


//______________________________________________________________________________
TVector3 TAMSD_Hit::GetMCMomentum_footFrame() { 
    TVector3 globP = m_momMC;
    m_geometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
};























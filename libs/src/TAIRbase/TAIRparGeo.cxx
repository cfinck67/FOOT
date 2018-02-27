#include <Riostream.h>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TEveGeoShapeExtract.h"
#include "TEveTrans.h"
#include "TGeoManager.h"
#include "TGeoMatrix.h"
#include "TList.h"
#include "TMath.h"
#include "TObjArray.h"
#include "TObjString.h"
#include "TROOT.h"
#include "TSystem.h"

#include "TAGgeoTrafo.hxx"

#include "TAIRparMap.hxx"
#include "TAIRparGeo.hxx"

#include "foot_geo.h"
#include "GlobalPar.hxx"




TAIRparGeo::TAIRparGeo() {
 
};

/*

//  copy constructor
TAIRparGeo::TAIRparGeo( TAIRparGeo* original ) :

  m_rotation(original->m_rotation),
  m_origin(original->m_origin),  // current position
  m_center(original->m_center),  // current position
  m_dimension(original->m_dimension),

  m_nSensors_X(original->m_nSensors_X),
  m_nSensors_Y(original->m_nSensors_Y),
  m_nSensors_Z(original->m_nSensors_Z),
  m_NSensors (original->m_NSensors),

  m_materialOrder(original->m_materialOrder),

  m_materialThick(original->m_materialThick),
  m_materialType(original->m_materialType),

  m_siliconSensorThick_Lz(original->m_siliconSensorThick_Lz),
  m_layerDistance(original->m_layerDistance),

  m_nPixel_X(original->m_nPixel_X),
  m_nPixel_Y(original->m_nPixel_Y)         {

  SensorMatrix m_sensorMatrix = original->m_sensorMatrix;

}

//_____________________________________________________________________________

void TAIRparGeo::InitGeo()  {
}



//_____________________________________________________________________________
TVector3 TAIRparGeo::GetPosition( int layer, int col, int row )  {
  // TVector3 sensorCoord = GetSensorCoortdinates( int layer, int col, int row );
  // TVector3 pos = m_sensorMatrix[sensorCoord.z()][sensorCoord.x()][sensorCoord.y()]->GetPosition();
  TVector3 pos = m_sensorMatrix[layer][0][0]->GetPosition( col, row );
  Local2Global(&pos);
  return pos;
}



//_____________________________________________________________________________
void TAIRparGeo::Global2Local( TVector3* glob ) {
  glob->Transform( GetRotationToLocal() );
  *glob = *glob - m_center;
}

//_____________________________________________________________________________
void TAIRparGeo::Global2Local_TranslationOnly( TVector3* glob ) {
  *glob = *glob - m_center;
}

//_____________________________________________________________________________
void TAIRparGeo::Global2Local_RotationOnly( TVector3* glob ) {
  glob->Transform( GetRotationToLocal() );
}

//_____________________________________________________________________________
void TAIRparGeo::Local2Global( TVector3* loc ) {
  loc->Transform( GetRotationToGlobal() );
  *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAIRparGeo::Local2Global_TranslationOnly( TVector3* loc ) {
  *loc = *loc + m_center;
}

//_____________________________________________________________________________
void TAIRparGeo::Local2Global_RotationOnly( TVector3* loc ) {
  loc->Transform( GetRotationToGlobal() );
}



//_____________________________________________________________________________
TGeoVolume* TAIRparGeo::GetVolume() {

  if ( !GlobalPar::GetPar()->geoROOT() ) 
    cout << "ERROR << TAIRparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

}

*/

//_____________________________________________________________________________
void TAIRparGeo::PrintBodies( string geoFileName ) {
  
  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TAITparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  // ofstream geofile;
  // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );

  // geofile << "* ***Vertex" << endl;
  cout << "* BODIES ***Start Counter" << endl;

  stringstream ss;
  double zero = 0.;

  ss << setiosflags(ios::fixed) << setprecision(6);
  ss << "RCC stc     "  << STC_X << " " << STC_Y << " " << STC_Z
     << " " << zero << " " << zero << " " << STC_THICK << " " << STC_RAD << endl;
  
  cout << ss.str() << endl;
  // geofile.close();
  
}



//_____________________________________________________________________________
void TAIRparGeo::PrintRegions( string geoFileName ) {
  
  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TAITparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  // ofstream geofile;
  // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );

  // geofile << "* ***Vertex" << endl;
  cout << "* REGIONS ***Start Counter" << endl;

  cout << "STC       5 stc" << endl;

  // geofile.close();

}



//------------------------------------------+-----------------------------------
//! Clear geometry info.
void TAIRparGeo::Clear(Option_t*)
{
  return;
}

//------------------------------------------+-----------------------------------
//! ostream insertion.
void TAIRparGeo::ToStream(ostream& os, Option_t*) const
{
  //  os << "TAIRparGeo " << GetName() << endl;
  //  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
  //     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}


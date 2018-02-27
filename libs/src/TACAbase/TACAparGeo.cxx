#include <iostream>
#include <fstream>
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

#include "TACAparMap.hxx"
#include "TACAparGeo.hxx"

#include "foot_geo.h"
#include "GlobalPar.hxx"

//##############################################################################


//_____________________________________________________________________________
TACAparGeo::TACAparGeo() {

  m_nCry=-1;

};



//_____________________________________________________________________________
void TACAparGeo::InitMaterial() {

}



//_____________________________________________________________________________
void TACAparGeo::InitGeo()  {
  
  if ( GlobalPar::GetPar()->Debug() > 0 )     cout << "\n\nTACAparGeo::InitGeo" << endl<< endl;
 
}


/*
//_____________________________________________________________________________
TVector3 TACAparGeo::GetPosition( int col, int row )  {
}



//_____________________________________________________________________________
void TACAparGeo::Global2Local( TVector3* glob ) {
glob->Transform( GetRotationToLocal() );
*glob = *glob - m_center;
}



//_____________________________________________________________________________
void TACAparGeo::Global2Local_RotationOnly( TVector3* glob ) {
glob->Transform( GetRotationToLocal() );
}



//_____________________________________________________________________________
void TACAparGeo::Local2Global( TVector3* loc ) {
loc->Transform( GetRotationToGlobal() );
*loc = *loc + m_center;
}



//_____________________________________________________________________________
void TACAparGeo::Local2Global_RotationOnly( TVector3* loc ) {
loc->Transform( GetRotationToGlobal() );
}



//_____________________________________________________________________________
TGeoVolume* TACAparGeo::GetVolume() {

if ( !GlobalPar::GetPar()->geoROOT() ) 
cout << "ERROR << TACAparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

return m_universe;
}

*/

//_____________________________________________________________________________
void TACAparGeo::PrintBodies( string geoFileName ){

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TACAparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
    

  // ofstream geofile;
  // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );
    
  // geofile << "* ***Calorimeter" << endl;
  cout << "* BODIES ***Calorimeter" << endl;

  stringstream ss;
  char bodyname[20];

  double z = CAL_Z;
  for(int i=0;i<CAL_NROW;i++){
    double y = CAL_Y - CAL_HEIGHT/2 + i * CAL_CRY_HEIGHT+ CAL_CRY_HEIGHT/2;
    for (int j=0;j<CAL_NCOL;j++){
      double x = CAL_X - CAL_WIDTH/2 + j * CAL_CRY_WIDTH + CAL_CRY_WIDTH/2;
      if( sqrt(x*x+y*y) <= CAL_WIDTH/2 - 0.7){
	m_nCry++;
	sprintf(bodyname,"cal%d     ",m_nCry);
	double xmin = x - CAL_CRY_WIDTH/2.;
	double xmax = x + CAL_CRY_WIDTH/2.;
	double ymin = y - CAL_CRY_HEIGHT/2.;
	double ymax = y + CAL_CRY_HEIGHT/2.;
	double zmin = z - CAL_CRY_THICK/2.;
	double zmax = z + CAL_CRY_THICK/2.;

	ss << setiosflags(ios::fixed) << setprecision(6);
	ss << "RPP " << bodyname << xmin << " " << xmax << " "
	   << ymin << " " << ymax << " " 
	   << zmin << " " << zmax << endl;

      }
    }
  }
  cout << ss.str() << endl;

  // geofile.close();
}



//_____________________________________________________________________________
void TACAparGeo::PrintRegions( string geoFileName ){

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TACAparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  // ofstream geofile;
  // geofile.open( geoFileName.c_str(), std::ofstream::out | std::ofstream::app );
      
  // geofile << "* ***Inner Tracker" << endl;
  cout << "* REGION ***Calorimeter" << endl;

  char stringa[100];
  for (int i=0; i<=m_nCry; i++){
    sprintf(stringa,"CAL%03d       5 cal%d",i,i);
    cout << stringa << endl;
  }  
  // geofile.close();

}


//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TACAparGeo::Clear(Option_t*)
{
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TACAparGeo::ToStream(ostream& os, Option_t*) const
{
  //  os << "TACAparGeo " << GetName() << endl;
  //  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
  //     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}


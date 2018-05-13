#include "LightSabre.hxx"


LightSabre::LightSabre() { 

  // row along Y
  // col along X

  vector<string> checkTic = { "coordinates", "material"};
  for ( unsigned int i=0; i<checkTic.size(); i++ ) {
    m_readyCheck[ checkTic.at(i) ] = false;
  }

  m_origin = TVector3( 0, 0, 0 );

};




// TVector3 LightSabre::GetPosition( int layer, int bar ) { 

//   if ( layer < 0 || layer > m_nLayer || bar < 0 || bar > m_nBar ) {
//     cout << "ERROR!!  ::  LightSabre::GetPosition  -->  layer("<<layer<<") or bar("<<bar<<") out of boundaries ("<<m_nBar<<"x"<<m_nLayer<<")"<<endl;
//     exit(0);
//   }

//   // evaluate position of the bar
//   // double tmpX = - m_halfLength.X() + (bar+0.5)*m_barLength_X;
//   // double tmpY = - m_halfLength.Y() + (bar+0.5)*m_barLength_Y;
//   double tmpX = m_center.X();
//   double tmpY = m_center.Y();
//   double tmpZ = m_center.Z();

//   TVector3 pos ( tmpX, tmpY, tmpZ );

//   // Local2Global(&pos);
//   return pos;
// };



void LightSabre::SetBar( TVector3 acenter, TVector3 adimension,
			 // int nlayer, int nbar,
			 double bar_StepX, double bar_StepY, double bar_StepZ,
			 TVector3 aeuler
			 ) {

  m_center = acenter;
  m_dimension = adimension;
  m_halfLength = 0.5 * m_dimension;
  // m_nLayer = nlayer;
  // m_nBar = nbar;
  // double deg2rad = (TMath::Pi())/2.;
  m_tilt_eulerAngle = aeuler;

  m_barDistance_X = bar_StepX;
  m_barDistance_Y = bar_StepY;
  m_barDistance_Z = bar_StepZ;

  // define rotation matrix - local to global
  m_rotation = new TRotation();
  m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );

  if ( !m_readyCheck["material"] ) {
    cout << "LightSabre::SetSensor :: ERROR >> Set the bar material BEFORE the coordinates!" << endl;
    exit(0);
  }

  // box == region
  m_name = m_regionName;

  // check if the material of the box is defined
  if ( !gGeoManager->GetMedium( m_materialName.c_str() ) ) 
    cout << "ERROR >> FootBox::FootBox  :  Material " << m_materialName << " not defined in gGeoManager for the region " << m_regionName, exit(0);

  // if the volume of the box is NOT defined  -->  define it
  if ( !gGeoManager->GetVolume( m_materialRegionName.c_str() ) ) {
    TGeoVolume *rootVolume = gGeoManager->MakeBox( m_materialRegionName.c_str(), gGeoManager->GetMedium( m_materialName.c_str() ),
						   m_dimension.X()/2, m_dimension.Y()/2, m_dimension.Z()/2); 
    rootVolume->SetLineColor(kOrange);

   }


  m_readyCheck["coordinates"] = true;

}




// void LightSabre::SetBar( double x, double y, double z, 
// 			 double length_dx, double length_dy, double length_dz,
// 			 int nlayer, int nbar,
// 			 // double bar_Lx, double bar_Ly, double bar_Lz,
// 			 double bar_StepX, double bar_StepY, double bar_StepZ,
// 			 double euler_dx, double euler_dy, double euler_dz
// 			 ) {

//   SetBar( TVector3( x, y, z ), TVector3( length_dx, length_dy, length_dz ),
// 	  nlayer, nbar,
// 	  // bar_Lx,  bar_Ly,  bar_Lz, 
// 	  bar_StepX,  bar_StepY,  bar_StepZ,
// 	  TVector3( euler_dx, euler_dy, euler_dz ) );

// }




//_____________________________________________________________________________
TVector3 LightSabre::GetPosition() { 
    TVector3 pos (m_center);
    Local2Global(&pos);
    return pos;
}


//_____________________________________________________________________________
void LightSabre::Global2Local( TVector3* glob ) {
  glob->Transform( GetRotationToLocal() );
  *glob = *glob - m_center;
}

//_____________________________________________________________________________
void LightSabre::Global2Local_TranslationOnly( TVector3* glob ) {
  *glob = *glob - m_center;
} 

//_____________________________________________________________________________
void LightSabre::Global2Local_RotationOnly( TVector3* glob ) {
  glob->Transform( GetRotationToLocal() );
} 

//_____________________________________________________________________________
void LightSabre::Local2Global( TVector3* loc ) {
  loc->Transform( GetRotationToGlobal() );
  *loc = *loc + m_center;
}

//_____________________________________________________________________________
void LightSabre::Local2Global_TranslationOnly( TVector3* loc ) {
  *loc = *loc + m_center;
}

//_____________________________________________________________________________
void LightSabre::Local2Global_RotationOnly( TVector3* loc ) {
  loc->Transform( GetRotationToGlobal() );
} 



void LightSabre::SetMaterial( string materialName, string materialRegionName, string bodyName, string regionName, int volumeID ) {

  // m_pixelMaterialName = materialName;	// not used
  m_materialName = materialName;
  m_materialRegionName = materialRegionName;
  m_bodyName = bodyName;
  m_regionName = regionName;
  m_volumeID = volumeID;

  m_readyCheck["material"] = true;	

}




// void LightSabre::SetMaterial( string barMat ) {
// void SetMaterial( string pixMat, map<string, double>  materials ) {
// m_materialMap = materials;		// global ?
// m_barMaterialName = barMat;
// m_readyCheck["material"] = true;

// check the map
// SetGeometry();

// }



















#include "IronPlate.hxx"


IronPlate::IronPlate() { 

	// row along Y
	// col along X
	
	vector<string> checkTic = { "coordinates", "material"};
	for ( unsigned int i=0; i<checkTic.size(); i++ ) {
		m_readyCheck[ checkTic.at(i) ] = false;
	}

	m_origin = TVector3( 0, 0, 0 );

};



TVector3 IronPlate::GetPosition_local( int col, int row ) { 
		
	if ( col < 0 || col > m_nColPixel || row < 0 || row > m_nRowPixel )	{
		cout << "ERROR!!  ::  IronPlate::GetPosition  -->  column("<<col<<") or row("<<row<<") out of boundaries ("<<m_nColPixel<<"x"<<m_nRowPixel<<")"<<endl;
		exit(0);
	}

	// evaluate position of the pixel
	double tmpX = - m_halfLength.X() + (col+1)*m_pixelDistance_X + ( (float)col+0.5 )*m_pixelLength_X;
	double tmpY = - m_halfLength.Y() + (row+1)*m_pixelDistance_Y + ( (float)row+0.5 )*m_pixelLength_Y;
	TVector3 pos ( tmpX, tmpY, m_pixelDistance_Z );

	return pos;
};




TVector3 IronPlate::GetPosition( int col, int row ) { 
		
	TVector3 pos ( GetPosition_local(col, row) );
	Local2Global(&pos);
	return pos;
};




void IronPlate::SetSensor( TVector3 acenter,	TVector3 alength,
				int ncol, int nrow, 
				double px_Lx, double px_Ly, double px_thick, 
				double px_StepX, double px_StepY, double px_Zshift,
				TVector3 aeuler
				  ) {
	
	// row along Y		// col along X
	m_center = acenter;
	m_dimension = alength;
	m_halfLength = 0.5 * m_dimension;
	m_nColPixel = ncol;
	m_nRowPixel = nrow;
	m_tilt_eulerAngle = aeuler;

	m_pixelLength_X = px_Lx;
	m_pixelLength_Y = px_Ly;
	m_pixelLength_Z = px_thick;

	m_pixelDistance_X = px_StepX;
	m_pixelDistance_Y = px_StepY;
	m_pixelDistance_Z = px_Zshift;

	// define rotation matrix - local to global
	m_rotation = new TRotation();
    m_rotation->SetYEulerAngles( m_tilt_eulerAngle.x(), m_tilt_eulerAngle.y(), m_tilt_eulerAngle.z() );

    if ( !m_readyCheck["material"] ) {
    	cout << "IronPlate::SetSensor :: ERROR >> Set the pixel material BEFORE the coordinates!" << endl;
    	exit(0);
    }

    // box == region
	m_name = m_regionName;

	// check if the material of the box is defined
	if ( !gGeoManager )	
		cout << "ERROR >> FootBox::FootBox  ::  Material " << m_materialName << " not defined gGeoManager " << endl, exit(0);
	if ( !gGeoManager->GetMedium( m_materialName.c_str() ) ) 
		cout << "ERROR >> FootBox::FootBox  ::  Material " << m_materialName << " not defined in gGeoManager for the region " << m_regionName << endl, exit(0);

	// if the volume of the box is NOT defined  -->  define it
	if ( !gGeoManager->GetVolume( m_materialRegionName.c_str() ) ) {
		TGeoVolume *rootVolume = gGeoManager->MakeBox( m_materialRegionName.c_str() ,gGeoManager->GetMedium( m_materialName.c_str() ),
                                                m_dimension.x()/2, m_dimension.y()/2, m_dimension.z()/2); 
        rootVolume->SetLineColor(kOrange); 

	}

	m_readyCheck["coordinates"] = true;

}




void IronPlate::SetSensor( double x, double y, double z, 
			double length_dx, double length_dy, double length_dz,
			int ncol, int nrow, double px_Lx, double px_Ly, double px_Zshift, 
			double px_StepX, double px_StepY, double px_thick,
			double euler_dx, double euler_dy, double euler_dz
			  ) {

	SetSensor( TVector3( x, y, z ), TVector3( length_dx, length_dy, length_dz ),
					 ncol, nrow, px_Lx,  px_Ly,  px_Zshift, 
					 px_StepX,  px_StepY,  px_thick,
					 TVector3( euler_dx, euler_dy, euler_dz ) );

}



//_____________________________________________________________________________
void IronPlate::Global2Local( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
    *glob = *glob - m_center;
}

//_____________________________________________________________________________
void IronPlate::Global2Local_TranslationOnly( TVector3* glob ) {
    *glob = *glob - m_center;
} 

//_____________________________________________________________________________
void IronPlate::Global2Local_RotationOnly( TVector3* glob ) {
    glob->Transform( GetRotationToLocal() );
} 

//_____________________________________________________________________________
void IronPlate::Local2Global( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void IronPlate::Local2Global_TranslationOnly( TVector3* loc ) {
    *loc = *loc + m_center;
}

//_____________________________________________________________________________
void IronPlate::Local2Global_RotationOnly( TVector3* loc ) {
    loc->Transform( GetRotationToGlobal() );
} 





void IronPlate::SetMaterial( string materialName, string materialRegionName, string bodyName, string regionName, int volumeID ) {

	m_pixelMaterialName = materialName;	// not used
	m_materialName = materialName;
	m_materialRegionName = materialRegionName;
	m_bodyName = bodyName;
	m_regionName = regionName;
	m_volumeID = volumeID;

	m_readyCheck["material"] = true;

	

}
















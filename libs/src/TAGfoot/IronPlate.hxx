#ifndef IRONPLATE_H
#define IRONPLATE_H

#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>  
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"

#include "TVector3.h"
#include "TRotation.h"

using namespace std;


//	IronPlate is an helper class to stor and manage geometrcal info of a single sensor of pixel.
//	Store the info using the SetSensor() procedure.
//	GetPosition( int col, int row ) Returns pixel position in the parent subdetector reference 
// 		frame in cartesian coord. given the column and row of the corresponding pixel.

//	Material is currently managed by the parent subdetector part. 

// row along Y
// col along X


class IronPlate{

public:

	IronPlate();
	~IronPlate() {};

	TVector3 GetCenter() { return m_center; };
	TVector3 GetDimension() { return m_dimension; };
	TVector3 GetEuler() { return m_tilt_eulerAngle; };
	
	int GetNCol() { return m_nColPixel; };
	int GetNRow() { return m_nRowPixel; };

	TVector3 GetPosition( int col, int row );

	string GetMaterialName() { return m_materialName; };
	string GetMaterialRegionName() { return m_materialRegionName; };

	string GetBodyName() { return m_bodyName; };
	string GetRegionName() { return m_regionName; };


	TVector3 GetMinCoord() { 
		TVector3 tmpCenter = TVector3(m_origin);
		Local2Global_TranslationOnly(&tmpCenter);
		return tmpCenter - m_halfLength; 
	};

	TVector3 GetMaxCoord() { 
		TVector3 tmpCenter = TVector3(m_origin);
		Local2Global_TranslationOnly(&tmpCenter);
		return tmpCenter + m_halfLength; 
	};

	void AddNodeToUniverse( TGeoVolume* universe ) {
        universe->AddNode( gGeoManager->GetVolume( m_materialRegionName.c_str() ), m_volumeID , 
        					new TGeoCombiTrans( m_origin.x(), m_origin.y(), m_origin.z(), new TGeoRotation("null,",0,0,0)) );
	}


	TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };


	// put private
	void Global2Local( TVector3* glob );
	void Global2Local_TranslationOnly( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );

    void Local2Global( TVector3* loc );
    void Local2Global_TranslationOnly( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );


	void SetMaterial( string materialName, string materialRegion, 
				string bodyName, string regionName, int volumeID );



	void SetSensor( TVector3 acenter,	TVector3 alength,
				int ncol, int nrow, 
				double px_Lx, double px_Ly, double px_thick, 
				double px_StepX, double px_StepY, double px_Zshift,
				TVector3 aeuler = TVector3( 0,0,0 )
				  ) ;
	void SetSensor( double x, double y, double z, 
				double length_dx, double length_dy, double length_dz,
				int ncol, int nrow, double px_Lx, double px_Ly, double px_Zshift, 
				double px_StepX, double px_StepY, double px_thick,
				double euler_dx = 0, double euler_dy = 0, double euler_dz = 0
				  ) ;



	bool Check() {
		bool tic = true;
		for (map<string, bool>::iterator it=m_readyCheck.begin(); it!=m_readyCheck.end(); it++ )
			tic += (*it).second;
		if (!tic)
			cout << "ERROR" << endl;
		return tic;
	}





private:

	// TVector3 GetCenter() { return m_center; };


	map<string, bool> m_readyCheck;		
	map<string, double> m_materialMap;
	
	TRotation* m_rotation;

	TVector3 m_origin;
	TVector3 m_center;
	TVector3 m_dimension;
	TVector3 m_halfLength;
	TVector3 m_tilt_eulerAngle;

	double m_pixelLength_X;
	double m_pixelLength_Y;
	double m_pixelLength_Z;

	double m_pixelDistance_X;
	double m_pixelDistance_Y;
	double m_pixelDistance_Z;

	int m_nColPixel;
	int m_nRowPixel;

	string m_name;
	string m_pixelMaterialName; // not used
	string m_materialName;		// the name of the material found in the .inp file
	string m_materialRegionName;	// the name of the variable corresponding to the material name in the foot_geo.h file

	// corrispondenza biunivoca
	string m_bodyName;			// name of the body
	string m_regionName;		// name of the region

	int m_id;
	int m_volumeID;

};


#endif


















































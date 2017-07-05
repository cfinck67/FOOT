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
	TVector3 GetLength() { return m_length; };
	TVector3 GetEuler() { return m_tilt_eulerAngle; };
	int GetNCol() { return m_nColPixel; };
	int GetNRow() { return m_nRowPixel; };
	// string GetMaterialName() {return m_MaterialName;};

	TRotation GetRotationToGlobal() { return *m_rotation; };
    TRotation GetRotationToLocal() { return m_rotation->Inverse(); };


	TVector3 GetPosition( int col, int row );

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

	// put private
	void Global2Local( TVector3* glob );
	void Global2Local_TranslationOnly( TVector3* glob );
    void Global2Local_RotationOnly( TVector3* glob );

    void Local2Global( TVector3* loc );
    void Local2Global_TranslationOnly( TVector3* loc );
    void Local2Global_RotationOnly( TVector3* loc );


	// void GetCoord_ROOT( TVector3& center, TVector3& length ) { 
	// 	center = m_center;
	// 	length = m_halfLength;
	// };
	// void GetCoord_FLUKA( TVector3& pos, TVector3& length ) { 
	// 	pos = m_center - m_halfLength;
	// 	length = m_length;
	// };
	// void GetPosition_FLUKA( TVector3& pos ) { 
	// 	pos = m_center - m_halfLength;
	// };
	// void GetLenght_FLUKA( TVector3& length ) { 
	// 	length = m_length;
	// };


	void SetMaterial( string pixMat );
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

	

	bool AddVolumeTo( TGeoVolume* volume ) {

		// /***********************************************************************************/
		// TGeoMaterial * siliconMat = new TGeoMaterial( "siliconMat", 28.0855, 14., 2.329);
	 //    siliconMat->SetRadLen(1.);             //calc automatically, need this for elemental mats.
	 //    TGeoMedium * silicon = new TGeoMedium( "silicon", 1, siliconMat );
	 //    **********************************************************************************

		// TGeoVolume *redBullCan = gGeoManager->MakeTube("redBullCan", 
		// 				silicon, 0, m_radius, m_length.Mag()/2 ); 

  //       redBullCan->SetLineColor(kRed);

  //       TGeoTranslation *m_transform = new TGeoTranslation(m_center.X(), m_center.Y(), m_center.Z());

  //       volume->AddNode( redBullCan, 1, m_transform );

        return 0;
	}



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
	TVector3 m_length;
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
	string m_pixelMaterialName;

	int m_id;

};


#endif


















































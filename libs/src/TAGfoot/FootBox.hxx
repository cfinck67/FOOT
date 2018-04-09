#ifndef FOOTBOX_HXX
#define FOOTBOX_HXX

#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

#include "TString.h"
#include "TVector3.h"
#include <TGeoManager.h>


using namespace std;

class TGeoVolume;

class FootBox {

//---------------------------------------------------------------------
//     Everything is in the detector coordinate reference frame !!!
//---------------------------------------------------------------------

public:

	FootBox() {};    // wrong!!!!!!!!!!!!!!!!!!!!!
	FootBox( 	TVector3 origin, TVector3 dimension, 
				string materialName, string materialRegionName, string bodyName, string regionName, 
				int volumeID );
	~FootBox() {};

	
	void SetBox( TVector3 origin, TVector3 dimension, 
				string materialName, string materialRegionName, string bodyName, string regionName, 
				int volumeID );

	// void AddNodeToUniverse( TGeoVolume* universe ) {

	// 	// cout << m_materialRegionName <<endl;
	// 	if ( !gGeoManager->GetVolume( m_materialRegionName.c_str() ) ) {
	// 		cout << "ERROR >> FootBox::AddNodeToUniverse  -->  volume not defined: "<< m_materialRegionName << endl;
	// 	}

 //        universe->AddNode( gGeoManager->GetVolume( m_materialRegionName.c_str() ), m_volumeID , 
 //        					new TGeoCombiTrans( m_origin.x(), m_origin.y(), m_origin.z(), new TGeoRotation("null,",0,0,0)) );
	// }

	TVector3 GetPosition() { return m_origin; };
	TVector3 GetDimension() { return m_dimension; };

	int GetNodeID() { return m_volumeID; };

	string GetMaterialName() { return m_materialName; };
	string GetMaterialRegionName() { return m_materialRegionName; };

	string GetBodyName() { return m_bodyName; };
	string GetRegionName() { return m_regionName; };

	TVector3 GetMinCoord() { 
		// TVector3 tmpCenter = TVector3(m_origin);
		// Local2Global_TranslationOnly(&tmpCenter);
		return m_origin - (m_dimension * 0.5); 
	};

	TVector3 GetMaxCoord() { 
		// TVector3 tmpCenter = TVector3(m_origin);
		// Local2Global_TranslationOnly(&tmpCenter);
		return m_origin + (m_dimension * 0.5); 
	};


protected:

	string m_name;				// name of the region  ==  m_regionName
	int m_volumeID;

	TVector3 m_origin;
	TVector3 m_dimension;

	string m_materialName;		// the name of the material found in the .inp file
	string m_materialRegionName;	// the name of the variable corresponding to the material name in the foot_geo.h file

	// corrispondenza biunivoca
	string m_bodyName;			// name of the body
	string m_regionName;		// name of the region

	vector<string> m_bodiesToBeSubtracted;

};


#endif




































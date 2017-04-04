#ifndef TINCAN_H
#define TINCAN_H

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


using namespace std;


class TinCan{


public:

	TinCan() { 
		
		vector<string> checkTic = {"x", "y", "z", "lx", "ly", "lz", "r", "material"};
		for ( unsigned int i=0; i<checkTic.size(); i++ ) {
			m_readyCheck[ checkTic.at(i) ] = false;
		}
		m_readyCheck["material"] = true;		// provvisorio!!!!!
		m_MaterialName = "Copper";				// provvisorio!!!!!
	};
	~TinCan() {};




	bool AddVolumeTo( TGeoVolume* volume ) {

		/***********************************************************************************/
		TGeoMaterial * siliconMat = new TGeoMaterial( "siliconMat", 28.0855, 14., 2.329);
	    siliconMat->SetRadLen(1.);             //calc automatically, need this for elemental mats.
	    TGeoMedium * silicon = new TGeoMedium( "silicon", 1, siliconMat );
	    /************************************************************************************/

		TGeoVolume *redBullCan = gGeoManager->MakeTube("redBullCan", 
						silicon, 0, m_radius, m_length.Mag()/2 ); 

        redBullCan->SetLineColor(kRed);

        TGeoTranslation *m_transform = new TGeoTranslation(m_center.X(), m_center.Y(), m_center.Z());

        volume->AddNode( redBullCan, 1, m_transform );

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


	TVector3 GetCenter() { return m_center; };
	TVector3 GetLength() { return m_length; };
	double GetRadius() { return m_radius; };
	string GetMaterialName() {return m_MaterialName;};

	void GetCoord_ROOT( TVector3& center, TVector3& length ) { 
		center = m_center;
		length = m_halfLength;
		// cout << "\t\t\t\t\t\t TinCan  ";
		// center.Print(); 
		// length.Print();
	};
	void GetCoord_FLUKA( TVector3& pos, TVector3& length ) { 
		pos = m_center - m_halfLength;
		length = m_length;
	};
	void GetPosition_FLUKA( TVector3& pos ) { 
		pos = m_center - m_halfLength;
	};
	void GetLenght_FLUKA( TVector3& length ) { 
		length = m_length;
	};
	bool IsSense() { return m_isSense; };



	void SetPosX( double x ) {
		m_center.SetX(x);
		m_readyCheck["x"] = true;
	}
	void SetPosY( double x ) {
		m_center.SetY(x);
		m_readyCheck["y"] = true;
	}
	void SetPosZ( double x ) {
		m_center.SetZ(x);
		m_readyCheck["z"] = true;
	}
	void SetLengthX( double x ) {
		m_length.SetX(x);
		m_halfLength.SetX(x/2);
		m_readyCheck["lx"] = true;
	}
	void SetLengthY( double x ) {
		m_length.SetY(x);
		m_halfLength.SetY(x/2);
		m_readyCheck["ly"] = true;
	}
	void SetLengthZ( double x ) {
		m_length.SetZ(x);
		m_halfLength.SetZ(x/2);
		m_readyCheck["lx"] = true;
	}
	void SetRadius( double r ) {
		m_radius = r;
		m_readyCheck["r"] = true;
	}
	void SetSense( bool asense ) { m_isSense = asense; };
		// provvisorio!!!!!
	void SetMaterial( string mat ) {
		m_MaterialName = mat;
		m_readyCheck["material"] = true;
	}



private:

	map<string, bool> m_readyCheck;		

	TVector3 m_center;
	TVector3 m_length;
	TVector3 m_halfLength;

	string m_name;
	string m_MaterialName;

	double m_radius;

	int m_id;
	bool m_isSense;

};



#endif






























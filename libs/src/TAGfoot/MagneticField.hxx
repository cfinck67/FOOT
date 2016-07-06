#ifndef CLASS_MagneticField
#define CLASS_MagneticField


#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h> 
#include <vector>
#include <iostream>
#include <iomanip>

//#include <cmath>
//#include <cstring>
//#include <algorithm>


#include <TROOT.h>
#include "TRandom3.h"
#include "TVector3.h"
#include "TObject.h"
#include "TMath.h"
#include "TString.h"

//#include <TTree.h>


using namespace std;

class MagneticField: public TObject {
  
public:
  MagneticField(Double_t Current, TVector3 BoundingBox,
		TVector3 PositionOffset, TVector3 Orientation, TString dir);
  
  MagneticField(Double_t Current, TString dir);
  
  ~MagneticField();
  
  TVector3 GetField(TVector3 Position);//TODO: what units?
  
  void SetCurrent(Double_t current);
	
  Double_t GetCurrent();
  
  Bool_t IsInsideBox(TVector3 Point, TVector3 BoxPosition, 
		     TVector3 BoxDimension);
  
private:

	/* Range of supported ALADIN currents */
	static const Int_t kAladinCurrentMin = 0; /* [A] */
	static const Int_t kAladinCurrentMax = 2500; /* [A] */

	Double_t fCurrent;//UNITS: A
	Double_t fScaleWoodSaxon;//UNITS:1
	TVector3 fBoundingBox;//UNITS: cm //half of the side lengths
	TVector3 fPositionOffset;//UNITS: cm
	TVector3 fOrientation;//UNITS: cm

private:

	//for simple field map
	typedef struct {
		TVector3 Position;//UNITS: cm
		TVector3 Field;//UNITS: Tesla
	} StrucSimpleMap;

	static const Int_t fSimpleMapXN=33;
	static const Int_t fSimpleMapYN=13;
	static const Int_t fSimpleMapZN=89;
	TVector3 fSimpleMapRange;//UNITS: cm
	TVector3 fSimpleMapMin;//UNITS: cm
	TVector3 fSimpleMapMax;//UNITS: cm
	StrucSimpleMap fSimpleMap[fSimpleMapXN][fSimpleMapYN][fSimpleMapZN];


	//stuff for reading magnetic field (from C. Sfienti, translated from a FORTRAN code)

	/* Special coordinate system for the two mapper boxes.           */
	/* pos[0] = xx, pos[1] = xy, pos[2] = xz, pos[3] = y, pos[4] = z */
	typedef struct {
		double pos[5];
		double field[3];
		int status;//is set to 1 if we have a field from the current box, is 0 if not
	} TTCoordFieldMap;

	//done in  xmap, ymap, zmap
	//xx xy xz projections on coord system of the field box, y,z coord 
	//y-axis remains the same, other axis change: x->z z->x

	/* stores the fieldmap delivered by the two measurement boxes. */
	typedef struct {
		int li;
		int lj;
		int lk;
		float lbx[90][19][21];
		float lby[90][19][21];
		float lbz[90][19][21];
		int lstatus; //left box
		int ri;
		int rj;
		int rk;
		float rbx[85][19][17];
		float rby[85][19][17];
		float rbz[85][19][17];
		int rstatus; //right box
	} TTFieldMap;

	/* used for transformation between measurement boxes and magnet */
	typedef struct {
		/* they were all float */
		double box1[3][2];
		double box2[3][2];
		double mag1[3][2];
		double mag2[3][2];
		double off1[3];
		double off2[3];
		double alpha1;
		double alpha2;
		int status; //is 1 if we initialized
	} TTFieldTrafo;

	  TTFieldMap theFieldMap;
	  TTFieldTrafo theFieldTrafo;


	  /* Left box indizes */
	  Int_t   LBI ;
	  Int_t   LBJ ;
	  Int_t   LBK ;
	  /* Right box indizes */
	  Int_t   RBI ;
	  Int_t   RBJ ;
	  Int_t   RBK ;
	  

private:
	  //simple field map
	typedef struct {
		TVector3 BoundingBox;
		TVector3 Step;
		TVector3 Position[3][4][5];
		TVector3 Field[3][4][5];
		Int_t Status;//is set to 1 if initialized, is 0 if not
	} MagneticFieldMap;

	MagneticFieldMap fFieldMapSimple;


private:

	  void InitMagnetic();
         TString m_dir;
	  //Wood-Saxon field
	  TVector3 GetFieldWoodSaxon(TVector3 Position);

	  //field map from file complicated field maps
	  void   InitFieldMap();
	  void   InitFieldTrafo();

	  TVector3 GetFieldMap(TVector3 Position);
	  void   GetFieldMap1(TTCoordFieldMap *mp1);
	  void   GetFieldMap2(TTCoordFieldMap *mp2);

	  Int_t FillFieldMap();

	  //field map from file simple field maps
	  void InitFieldMapSimple();
	  TVector3 GetFieldMapSimple(TVector3 Position);

public:
	  //getters and setters
	  TVector3 GetBoundingBox() const{
		  return fBoundingBox;
	  };

	  TVector3 GetPositionOffset() const{
		  return fPositionOffset;
	  };
	  
	  TVector3 GetOrientation() const{
		  return fOrientation;
	  };
	  
	  Double_t GetCurrent() const{
		  return fCurrent;
	  };
	  

//protected:
//	ClassDef(MagneticField,1);
	 
};


#endif

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************


#ifndef ROOT_GEOMETRY
#define ROOT_GEOMETRY
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "TObject.h"
#include "TVector3.h"
#include "TString.h"

#include "gsi_geo.h"

using namespace std;


typedef struct {

  Int_t Z;
  Int_t A;
  Int_t Type;//FLUKA particle typ

  Double_t T;//kinetic energy [GeV]
  Double_t MomSpr;// [GeV]
  //-1= not set, 0=Gaussian, 1=, 2=...
//  Int_t    MomShape;

  TVector3 Avg;
  TVector3 Sig;

  //-1= not set, 0=Gaussian, 1=Annular, 2=...
  Int_t Shape;

} BEAM_PARAM;


typedef struct {

/* U view (wires along x) */
  Double_t  U_x[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  U_y[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  U_z[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  U_cx[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  U_cy[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  U_cz[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  U_rad[BMN_WIRELAYER_N][BMN_LAYER_N];

/* V view (wires along y) */

  Double_t  V_x[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  V_y[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  V_z[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  V_cx[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  V_cy[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  V_cz[BMN_WIRELAYER_N][BMN_LAYER_N];
  Double_t  V_rad[BMN_WIRELAYER_N][BMN_LAYER_N];

  Int_t idsense[BMN_SENSE_N];
  
  /*  number of layers, wires for each layer and total sense wires */
  Int_t nlayer;
  Int_t nwirelayer;
  Int_t nsense;

  
  Double_t CenterDch[3];//centre position of drift chamber
  Double_t SideDch[3];//box of drift chamber (full length)  /* Dimensioni dei lati della camera */
  Double_t DeltaDch[3];    /* displacement del primo filo rispetto al lato camera */
   
} GEO_BMON; //new BMON!


typedef struct {

  Double_t DIST_TGT_MI;//distance target (end) and mimosa (middle of first plane)
  TVector3 POS;//middle of mimosa|middle of mimosa|set position where stand-alone mimosa simulation starts (start of first plane without thickness of first MIMOSA chip)

  Double_t DIST_PLANES;//distance between mimosa planes (middle to middle)

  Int_t    MIMO_N;//number of MIMOSA chips
  TVector3 MIMO_DIM;//dimension of MIMOSA chips
  TVector3 MIMO_SHIFT[MI_MIMO_N];

  TVector3 SENS_DIM;//dimension of sensitive planes
  Int_t    SENS_BIN[3]; //number of bins

  TVector3 PAD_DIM;//dimension of pads
  
  Double_t MIMO_POS_Z[MI_MIMO_N];
  Double_t DIST_TGT_MIMO[MI_MIMO_N];


} GEO_MIMO;

typedef struct {

  TVector3 POS;//middle of target|middle of target|beginning of target
  TVector3 DIM;//dimensions of target

} GEO_TARG;



typedef struct {

  /*
//old BMON!
// vista U (filo lungo x) 
  Double_t  U_x[NWIRELAYER][NLAYER]; 
  Double_t  U_y[NWIRELAYER][NLAYER]; 
  Double_t  U_z[NWIRELAYER][NLAYER];
  Double_t  U_cx[NWIRELAYER][NLAYER]; 
  Double_t  U_cy[NWIRELAYER][NLAYER]; 
  Double_t  U_cz[NWIRELAYER][NLAYER];
  Double_t  U_rad[NWIRELAYER][NLAYER];
  
// vista V (filo lungo y) 

  Double_t  V_x[NWIRELAYER][NLAYER]; 
  Double_t  V_y[NWIRELAYER][NLAYER]; 
  Double_t  V_z[NWIRELAYER][NLAYER]; 
  Double_t  V_cx[NWIRELAYER][NLAYER]; 
  Double_t  V_cy[NWIRELAYER][NLAYER]; 
  Double_t  V_cz[NWIRELAYER][NLAYER]; 
  Double_t  V_rad[NWIRELAYER][NLAYER];
  
  Int_t idsense[NSENSE];

  Double_t UWS[3];
  Double_t UWC[3];
  Double_t UWD[3];
  Double_t VWS[3];
  Double_t VWC[3];
  Double_t VWD[3];
*/ 
  BEAM_PARAM Beam;

  GEO_BMON Bmon;
  
  GEO_MIMO Mimo;
  
  GEO_TARG Targ;
  
} GEOMETRY_STRUCT;


class Geometry : public TObject {
  
 public:
  
  Geometry();
  virtual ~Geometry();
  
  void InitGeoFromFile(string cfgname);

  Int_t InitBeam();

  Int_t InitBmonNew();//new BMON
//  Int_t InitBmon();
  
  Int_t InitTarg();

  Int_t InitMimo();

  GEOMETRY_STRUCT GetStruct();

  //read association of FLUKA region names and correspoding region numbers
  Int_t InitRegions(TString FileName);

  
  Int_t InitMagneticField(Double_t shiftX, Double_t shiftY, Double_t shiftZ, Double_t angleY, Double_t current);
  Int_t InsertInFile(TString fFileInS,TString fFileOutS,TString fTagStart,TString fTagStop,TString fInsertText);

  /* regions */
  // take bodies and subtract them in groups of 8
  Int_t Reg8sub(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,Int_t fVerbosity,TString* fFLUKALine);
  // take bodies and subtract them in groups of 8 (variable letter number)
  Int_t Reg8sub(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,Int_t fVerbosity,TString* fFLUKALine,Int_t fLetterNb);

  
  // take bodies and add them in groups of 8 (by union)
  Int_t Reg8add(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,Int_t fVerbosity,TString* fFLUKALine);

  // take changing bodies and add them in groups of 8 (by union) with fExpression
  Int_t Reg8addExp(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,TString fExpression,Int_t fVerbosity,TString* fFLUKALine);


  /* media */
  // assign a material
  Int_t AssignMaterial(TString fRegion,TString fMaterial,Int_t fVerbosity,TString* fFLUKALine);
  // assign a material from ... to ... steps of ...
  Int_t AssignMaterial(TString fRegionS, TString fRegionE,Int_t fStep, TString fMaterial,Int_t fVerbosity,TString* fFLUKALine);
  // assign a material, with option for magnetic field 
  Int_t AssignMaterial(TString Region,TString Material,Bool_t Magnetic,Int_t Verbosity,TString* FLUKALine);

  /* volumes */
  // create box with position in centre, and half length given
  Int_t VolBox(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fWidX,Double_t fWidY,Double_t fWidZ);
  // create box with position in centre, and half length given (limited precision: 0.1um)
  Int_t VolBoxLP(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fWidX,Double_t fWidY,Double_t fWidZ);
  
  // create box with angle Y, position in centre in X,Y and Z plus Rad in z-direction (when AngleZ=0), and half length given, 
  Int_t VolBox_Y(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fWidX,Double_t fWidY,Double_t fWidZ, Double_t fAngY, Double_t fRad);

  // create box with angle Y, position in centre in X,Y and at beginning of Z plus Rad in z-direction (when AngleZ=0), and half length given, 
  Int_t VolBox_Y_bZ(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fWidX,Double_t fWidY,Double_t fWidZ, Double_t fAngY, Double_t fRad);
   
  // create cylinder with angle Y and Theta, position in centre, and radius and height given
  Int_t VolCyl_YTheta(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fRadius,Double_t fHeight, Double_t fAngY, Double_t fAngTheta);
  
  // create cylinder with angle Y and Theta, position in the beginning of Z, and radius and height given
  Int_t VolCyl_YTheta_bZ(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fRadius,Double_t fHeight, Double_t fAngY, Double_t fAngTheta);
  
  // create plane with angle Y (direction x-z plane) and theta (azimutal from Y), angles in degree
  //for Y and theta=0 the plane is in x-y and the vector is pointing to z positive
  Int_t VolPla(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ, Double_t fAngY,Double_t fAngTheta);

  // create plane with angle Y (direction x-z plane) and theta (azimutal from Y), angles in degree
  //for Y and theta=0 the plane is in y-z and the vector is pointing to x positive
  Int_t VolPla_X(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ, Double_t fAngY,Double_t fAngTheta);
  
  // create trapezoid with angle Y (direction in x-z plane) and theta (azimutal from Y), angles in degree
  // x1,x2 and y1,y2 are the minimum and maximum half lengths, z1 is the half length in z, z0 is the distance to the rotation origin, position in centre in X,Y and Z
  //for Y and theta=0 the the direction vector is pointing to z positive
  //shift by a distance fRad, in direction defined by the angles
  //half length: fLenX1,fLenX2, fLenY1,fLenY2,fLenZ1
  Int_t VolTrap_YTheta(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ, Double_t fAngY,Double_t fAngTheta,Double_t fRad,Double_t fLenX1,Double_t fLenX2,Double_t fLenY1,Double_t fLenY2,Double_t fLenZ1);

  // create trapezoid with angle Y (direction in x-z plane) and theta (azimutal from Y), angles in degree
  // x1,x2 and y1,y2 are the minimum and maximum half lengths, z1 is the half length in z, z0 is the distance to the rotation origin, position in centre in X,Y and at beginning of Z
  //for Y and theta=0 the the direction vector is pointing to z positive
  //shift by a distance fRad, in direction defined by the angles
  //half length: fLenX1,fLenX2, fLenY1,fLenY2,fLenZ1
  Int_t VolTrap_YTheta_bZ(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ, Double_t fAngY,Double_t fAngTheta,Double_t fRad,Double_t fLenX1,Double_t fLenX2,Double_t fLenY1,Double_t fLenY2,Double_t fLenZ1);
  
  // create EVENTBIN with position in centre, and half length given
  //WATCHOUT: number precision goes to 0.1um
  Int_t ScoEVENTBIN(TString fScoName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ,Double_t fWidX,Double_t fWidY,Double_t fWidZ,Int_t fNbX,Int_t fNbY,Int_t fNbZ,Int_t fType,TString fScoring,Int_t fUnit);

  // create genertic FLUKA card: TITLE WHAT(1-6) SDUM
  Int_t GenericCard(TString fTitle,Int_t fVerbosity,TString* fFLUKALine,TString fWHAT1,TString fWHAT2,TString fWHAT3,TString fWHAT4,TString fWHAT5,TString fWHAT6,TString fSDUM);
 
  /* FIRST geometry parametrizations */
  //TOF Wall parameterization for slabs, fNbOfSlats= number of slabs in each down and up part, as PLANE
  Int_t TW_SlatParam(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX,Double_t fPosY,Double_t fPosZ, Double_t fAngY,Double_t fXprime,Double_t fYprime,Double_t fZprime,Double_t fStartX,Double_t fSlatWidth,Int_t fNbOfSlats,Int_t fBS[100]);
  //TOF Wall parameterization for slabs, fNbOfSlats= number of slabs in each down and up part, as BOX
  Int_t TW_SlatParamBOX(TString fVolName,Int_t fVerbosity,TString* fFLUKALine
	,Double_t fPosX,Double_t fPosY,Double_t fPosZ
	,Double_t fAngY
	,Double_t fXprime,Double_t fYprime,Double_t fZprime
	,Double_t fStartX
	,Double_t fSlatX,Double_t fSlatY,Double_t fSlatZ
	,Double_t fSlatSpaceX
	,Double_t fModuleSpaceX
	,Int_t fNbOfSlats
	,Int_t fBS[100]);

  //Onion ring parameterization
  Int_t TW_RingParam(TString fVolName,Int_t fVerbosity,TString* fFLUKALine
	,Double_t fPosX,Double_t fPosY,Double_t fPosZ
	,Double_t fAngY
//	,Double_t fAngTheta
	,Double_t fXprime,Double_t fYprime,Double_t fZprime
	,Double_t fdR
	,Double_t fDepth
//	,Double_t fThicknessSpace // distance between rings
	,Int_t fNbOfRings);


  //given a MIMOSA pad in a chip return the position in FLUKA coordinates
  TVector3 MI_Coord(Double_t Column, Double_t Row, Int_t Chip);

  //given a pad in a chip return the index for reconstruction (C. Finck)
  Int_t MI_IndexRec(Double_t Column, Double_t Row);
  
  //given a pad in a chip return the position in coordinates for reconstruction (C. Finck)
  TVector3 MI_CoordRec(Double_t Column, Double_t Row, Int_t Chip);

  
  //convert Double_t to Int_t without crashing - hell knows why (deadly FORTRAN-C++ mix?).
  virtual Int_t castd2i(Double_t number);

    //Interaction point
    TVector3 GetIP();
    void     SetIP(TVector3 ip);

    // Start Counter
    TVector3 GetSCCenter();
    TVector3 GetSCAngles();
    void     SetSCCenter(double x_c, double y_c, double z_c);
    void     SetSCCenter(TVector3 cen);
    void     SetSCAngles(double x_d, double y_d, double z_d);
    void     SetSCAngles(TVector3 ang);

    // Beam Monitor
    TVector3 GetBMCenter();
    TVector3 GetBMAngles();
    void     SetBMCenter(double x_c, double y_c, double z_c);
    void     SetBMCenter(TVector3 cen);
    void     SetBMAngles(double x_d, double y_d, double z_d);
    void     SetBMAngles(TVector3 ang);

    // Vertex
    TVector3 GetVTCenter();
    TVector3 GetVTAngles();
    void     SetVTCenter(double x_c, double y_c, double z_c);
    void     SetVTCenter(TVector3 cen);
    void     SetVTAngles(double x_d, double y_d, double z_d);
    void     SetVTAngles(TVector3 ang);

    // Kentros
    TVector3 GetKECenter();
    TVector3 GetKEAngles();
    void     SetKECenter(double x_c, double y_c, double z_c);
    void     SetKECenter(TVector3 cen);
    void     SetKEAngles(double x_d, double y_d, double z_d);
    void     SetKEAngles(TVector3 ang);

    // Alading
    TVector3 GetALCenter();
    TVector3 GetALAngles();
    void     SetALCenter(double x_c, double y_c, double z_c);
    void     SetALCenter(TVector3 cen);
    void     SetALAngles(double x_d, double y_d, double z_d);
    void     SetALAngles(TVector3 ang);

    // Music
    TVector3 GetMUCenter();
    TVector3 GetMUAngles();
    void     SetMUCenter(double x_c, double y_c, double z_c);
    void     SetMUCenter(TVector3 cen);
    void     SetMUAngles(double x_d, double y_d, double z_d);
    void     SetMUAngles(TVector3 ang);

    // New Reference point
    TVector3 GetNUCenter();
    TVector3 GetNUAngles();
    void     SetNUCenter(double x_c, double y_c, double z_c);
    void     SetNUCenter(TVector3 cen);
    void     SetNUAngles(double x_d, double y_d, double z_d);
    void     SetNUAngles(TVector3 ang);

    // tof Wall
    TVector3 GetTWRCenter();
    TVector3 GetTWRAngles();
    void     SetTWRCenter(double x_c, double y_c, double z_c);
    void     SetTWRCenter(TVector3 cen);
    void     SetTWRAngles(double x_d, double y_d, double z_d);
    void     SetTWRAngles(TVector3 ang);

    TVector3 GetTWFCenter();
    TVector3 GetTWFAngles();
    void     SetTWFCenter(double x_c, double y_c, double z_c);
    void     SetTWFCenter(TVector3 cen);
    void     SetTWFAngles(double x_d, double y_d, double z_d);
    void     SetTWFAngles(TVector3 ang);

    // Veto
    TVector3 GetVECenter();
    TVector3 GetVEAngles();
    void     SetVECenter(double x_c, double y_c, double z_c);
    void     SetVECenter(TVector3 cen);
    void     SetVEAngles(double x_d, double y_d, double z_d);
    void     SetVEAngles(TVector3 ang);

    void InitVtxPlanes();
    Double_t VtPlanesZ[4];
    inline Double_t GetVTPlanesZ(int pl) { return VtPlanesZ[pl]; };

    //container size
    Double_t MI_THICK;
    inline Double_t GetVTThick() { return MI_THICK; };

	
 private:
  /* coord della base del filo e del vettore che unisce le due basi*/

  GEOMETRY_STRUCT geo;
  Bool_t          fRegionFlag;
  std::vector<TString> fRegionName;
  std::vector<Int_t>   fRegionNumber;
  
  //exit window thickness (entrance not needed anymore and it set to vacuum)
  
  //global coordinates
  TVector3 m_ip;
  
  //Those are the positions and the angles in the FIRST ref. frame
  TVector3 m_SC_center, m_SC_angles;
  TVector3 m_BM_center, m_BM_angles;
  TVector3 m_TG_center, m_TG_angles;
  TVector3 m_VT_center, m_VT_angles;
  TVector3 m_KE_center, m_KE_angles;
  TVector3 m_AL_center, m_AL_angles;
  TVector3 m_MU_center, m_MU_angles;
  TVector3 m_NU_center, m_NU_angles;
  TVector3 m_TWR_center, m_TWR_angles;
  TVector3 m_TWF_center, m_TWF_angles;
  TVector3 m_VE_center, m_VE_angles;
  
 public:
 /*-----------------------------------------------------------------*/
  std::vector<TString> GetRegionNames() const{
  
    if (fRegionFlag==0){
      cerr<<"\nERROR: Geometry::GetRegionNames(): Regions not initialized"<<endl;
      exit(-1);
    }
    
  return fRegionName;
  };

/*-----------------------------------------------------------------*/
  std::vector<Int_t> GetRegionNumbers() const{
    if (fRegionFlag==0){
      cerr<<"\nERROR: Geometry::GetRegionNumbers(): Regions not initialized"<<endl;
      exit(-1);
    }
  return fRegionNumber;
  };
   
  
 protected:
  
  ClassDef(Geometry,1);
  
};

#endif


///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

#ifndef ROOT_GEOMETRY
#define ROOT_GEOMETRY
#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>

#include "TObject.h"
#include "TVector3.h"
#include "TString.h"

#include "foot_geo.h"

using namespace std;


typedef struct {

  // U view (wires along x)
  Double_t  U_x[BMN_NWIRELAY][BMN_NLAY];//base x coord
  Double_t  U_y[BMN_NWIRELAY][BMN_NLAY];//base y coord
  Double_t  U_z[BMN_NWIRELAY][BMN_NLAY];//base z coord
  Double_t  U_cx[BMN_NWIRELAY][BMN_NLAY];//height along x
  Double_t  U_cy[BMN_NWIRELAY][BMN_NLAY];//height along y
  Double_t  U_cz[BMN_NWIRELAY][BMN_NLAY];//height along z
  Double_t  U_rad[BMN_NWIRELAY][BMN_NLAY];//radius of cylinder  
  // U view (cells along x)
  Double_t  Uc_xm[BMN_NSENSELAY][BMN_NLAY];//x min
  Double_t  Uc_ym[BMN_NSENSELAY][BMN_NLAY];//y min
  Double_t  Uc_zm[BMN_NSENSELAY][BMN_NLAY];//z min
  Double_t  Uc_xM[BMN_NSENSELAY][BMN_NLAY];//z max
  Double_t  Uc_yM[BMN_NSENSELAY][BMN_NLAY];//z max
  Double_t  Uc_zM[BMN_NSENSELAY][BMN_NLAY];//z max
  
  // V view (wires along y) 
  Double_t  V_x[BMN_NWIRELAY][BMN_NLAY];
  Double_t  V_y[BMN_NWIRELAY][BMN_NLAY];
  Double_t  V_z[BMN_NWIRELAY][BMN_NLAY];
  Double_t  V_cx[BMN_NWIRELAY][BMN_NLAY];
  Double_t  V_cy[BMN_NWIRELAY][BMN_NLAY];
  Double_t  V_cz[BMN_NWIRELAY][BMN_NLAY];
  Double_t  V_rad[BMN_NWIRELAY][BMN_NLAY]; 
  // V view (cells along y) 
  Double_t  Vc_xm[BMN_NSENSELAY][BMN_NLAY]; 
  Double_t  Vc_ym[BMN_NSENSELAY][BMN_NLAY]; 
  Double_t  Vc_zm[BMN_NSENSELAY][BMN_NLAY]; 
  Double_t  Vc_xM[BMN_NSENSELAY][BMN_NLAY]; 
  Double_t  Vc_yM[BMN_NSENSELAY][BMN_NLAY]; 
  Double_t  Vc_zM[BMN_NSENSELAY][BMN_NLAY];
  
  Int_t idsense[BMN_NSENSELAY];
  
  //  number of layers, wires for each layer and total sense wires
  Int_t nlayer;
  Int_t nwirelayer;
  Int_t nsense;
  
  Double_t CenterBmn[3];//centre position of beam monitor
  Double_t SideBmn[3];//lateral dimension of beam monitor
  Double_t DeltaBmn[3];// displacement del primo filo rispetto al lato camera 
   
} GEO_BMON;

/*
typedef struct {

  // U view (wires along x)  (dimensions: [DCH_NWIRELAY][DCH_NLAY])
  Double_t  U_x[DCH_NWIRELAY][DCH_NLAY];
  Double_t  U_y[DCH_NWIRELAY][DCH_NLAY];
  Double_t  U_z[DCH_NWIRELAY][DCH_NLAY];
  Double_t  U_cx[DCH_NWIRELAY][DCH_NLAY];
  Double_t  U_cy[DCH_NWIRELAY][DCH_NLAY];
  Double_t  U_cz[DCH_NWIRELAY][DCH_NLAY];
  Double_t  U_rad[DCH_NWIRELAY][DCH_NLAY];
  // U view (cells along x) (dimensions: [DCH_NSENSELAY][DCH_NLAY])
  Double_t  Uc_xm[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Uc_ym[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Uc_zm[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Uc_xM[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Uc_yM[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Uc_zM[DCH_NSENSELAY][DCH_NLAY]; 
  
  // V view (wires along y) 
  Double_t  V_x[DCH_NWIRELAY][DCH_NLAY];
  Double_t  V_y[DCH_NWIRELAY][DCH_NLAY];
  Double_t  V_z[DCH_NWIRELAY][DCH_NLAY];
  Double_t  V_cx[DCH_NWIRELAY][DCH_NLAY];
  Double_t  V_cy[DCH_NWIRELAY][DCH_NLAY];
  Double_t  V_cz[DCH_NWIRELAY][DCH_NLAY];
  Double_t  V_rad[DCH_NWIRELAY][DCH_NLAY]; 
  // V view (cells along y) 
  Double_t  Vc_xm[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Vc_ym[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Vc_zm[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Vc_xM[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Vc_yM[DCH_NSENSELAY][DCH_NLAY]; 
  Double_t  Vc_zM[DCH_NSENSELAY][DCH_NLAY];
  
  Int_t idsense[DCH_NSENSELAY];
  
  //  number of layers, wires for each layer and total sense wires
  Int_t nlayer;
  Int_t nwirelayer;
  Int_t nsense;
  
  Double_t CenterDch[3];//centre position of drift chamber
  Double_t SideDch[3];//lateral dimension of beam monitor
  Double_t DeltaDch[3];// displacement del primo filo rispetto al lato camera 

  } GEO_DCH; */

typedef struct {

  GEO_BMON Bmon;
  
  //GEO_DCH Dch;
  
} GEOMETRY_STRUCT;


class Geometry : public TObject {
  
 public:
  
  Geometry();
  virtual ~Geometry();

  Int_t InitBmon();
  
  //  Int_t InitDch();

  GEOMETRY_STRUCT GetStruct();

  Int_t InsertInFile(TString fFileInS,TString fFileOutS,TString fTagStart,TString fTagStop
		     ,TString fInsertText);  

  /* regions */
  
  // take bodies and subtract them in groups of 8
  Int_t Reg8sub(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,Int_t fVerbosity
		,TString* fFLUKALine);
  // take bodies and subtract them in groups of 8 (variable letter number)
  Int_t Reg8sub(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,Int_t fVerbosity
		,TString* fFLUKALine,Int_t fLetterNb);  
  // take changing bodies and add them in groups of 8 (by union) with fExpression
  Int_t Reg8addExp(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,TString fExpression
		   ,Int_t fVerbosity,TString* fFLUKALine);
   // take changing bodies and add them in groups of 8 (by union) with variable fExpression
  Int_t Reg8addVarExp(TString fVolPreName,Int_t fVolNbStart,Int_t fVolNbEnd,TString fExpression
		      ,Int_t fVerbosity,TString* fFLUKALine);  

  /* media */
  
  // assign a material
  Int_t AssignMaterial(TString fRegion,TString fMaterial,Int_t fVerbosity,TString* fFLUKALine);
  // assign a material from ... to ... steps of ...
  Int_t AssignMaterial(TString fRegionS,TString fRegionE,Int_t fStep,TString fMaterial
		       ,Int_t fVerbosity,TString* fFLUKALine);
  // assign a material, with option for magnetic field 
  Int_t AssignMaterial(TString Region,TString Material,Bool_t Magnetic,Int_t Verbosity
		       ,TString* FLUKALine);
  // assign a material, with option for magnetic field from ... to ... steps of ...
  Int_t AssignMaterial(TString fRegionS,TString fRegionE,Int_t fStep,TString Material
		       ,Bool_t Magnetic,Int_t Verbosity,TString* FLUKALine);
  
  /* volumes */
  
  // create box with position in centre, and half length given
  Int_t VolBox(TString fVolName,Int_t fVerbosity,TString* fFLUKALine,Double_t fPosX
	       ,Double_t fPosY,Double_t fPosZ,Double_t fWidX,Double_t fWidY,Double_t fWidZ);

  // create genertic FLUKA card: TITLE WHAT(1-6) SDUM
  Int_t GenericCard(TString fTitle,Int_t fVerbosity,TString* fFLUKALine,TString fWHAT1,TString fWHAT2
		    ,TString fWHAT3,TString fWHAT4,TString fWHAT5,TString fWHAT6,TString fSDUM);


 private:
  /* coord della base del filo e del vettore che unisce le due basi*/

  GEOMETRY_STRUCT geo;
  Bool_t          fRegionFlag;
  std::vector<TString> fRegionName;
  std::vector<Int_t>   fRegionNumber;
  
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

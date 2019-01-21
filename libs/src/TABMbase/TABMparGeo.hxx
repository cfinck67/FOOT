#ifndef _TABMparGeo_HXX
#define _TABMparGeo_HXX
/*!
  \file
  \version $Id: TABMparGeo.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TABMparGeo.
*/
/*------------------------------------------+---------------------------------*/

#include "TEveGeoShapeExtract.h"
#include "TString.h"
#include "TVector3.h"
#include "TGeoVolume.h"

#include "TAGpara.hxx"
#include "foot_geo.h"

#include "GlobalPar.hxx"
#include "FootField.hxx"

#include <TRotation.h>
#include <FieldManager.h>

//##############################################################################


class TGeoVolume;
class TABMparGeo : public TAGpara {
public:

  TABMparGeo();
  virtual         ~TABMparGeo();

  void InitGeo();
  void CreateLocalBMGeo();//create an output file LocalBM.root with the geometry of the BM
  void CoutWirePosDir();
  void SetA0Wvers(Int_t cellid, TVector3 &A0, TVector3 &Wvers);//for a given cellid, set A0 and Wvers
  
  //fluka's stuff
  string PrintBodies();
  string PrintRegions();
  string PrintAssignMaterial();
  string PrintParameters();
  
  //transformers
  Int_t RotateBmon();//to rotate the x_pos...cx_pos coordinate with GOLDSTEIN CONVENTION, 
  void RotateNewBmon(Double_t phi, Double_t theta, Double_t psi, Bool_t reverse);//to rotate the x_pos...cx_pos coordinate, input in deg!
  Int_t ShiftBmonLG(Bool_t global2local); //if global2local is false: it shifts (not rotate!) the local coordinate of the bm wires (xpos,ypos,zpos) in the global coordinate, otherwise if global2local is true
  void ShiftBmon(TVector3 shift); //shift the BM wire position (xpos, ypos, zpos) of a TVector3 shift
  void SetRotation(Double_t xrot, Double_t yrot, Double_t zrot);// set m_rotation
  TRotation GetRotationToGlobal()                   {return *m_rotation;};
  TRotation GetRotationToLocal()                    {return m_rotation->Inverse();};
  void Global2Local(TVector3* glob)                 {*glob=*glob-m_center;glob->Transform(m_rotation->Inverse()); return;};  
  void Global2Local(TVector3& glob)                 {glob=glob-m_center;glob.Transform(m_rotation->Inverse()); return;};  
  void Global2Local_TranslationOnly(TVector3* glob) {*glob = *glob - m_center;return;};
  void Global2Local_RotationOnly(TVector3* glob)    {glob->Transform(m_rotation->Inverse());return;}
  void Local2Global(TVector3* loc)                  {*loc = *loc + m_center;loc->Transform(*m_rotation);return;};    
  void Local2Global_TranslationOnly(TVector3* loc)  {*loc = *loc + m_center;return;};
  void Local2Global_RotationOnly(TVector3* loc)     {loc->Transform(*m_rotation);return;};
    
  //other methods
  TVector3 ProjectFromPversR0(TVector3 Pvers, TVector3 R0, Double_t z);  
  TVector3 ProjectFromPversR0(Double_t PversXZ, Double_t PversYZ, Double_t R0X, Double_t R0Y, Double_t z);
    
  //inline getters
  Int_t    GetID(Int_t cell);//Id sense as function of cell
  Double_t GetX(Int_t w, Int_t p, Int_t v); //X,Y,Z as a function of wire, plane, view
  Double_t GetY(Int_t w, Int_t p, Int_t v);
  Double_t GetZ(Int_t w, Int_t p, Int_t v);
  Double_t GetCX(Int_t w, Int_t p, Int_t v);
  Double_t GetCY(Int_t w, Int_t p, Int_t v);
  Double_t GetCZ(Int_t w, Int_t p, Int_t v);
  
  //other getters:
  TGeoVolume*     GetVolume();
  void GetCellInfo(Int_t view, Int_t plane, Int_t cellID, Double_t& h_x, Double_t& h_y, Double_t& h_z, Double_t& h_cx, Double_t& h_cy, Double_t& h_cz); //used in TABMvieTrackFOOT
  //get a number from 0 to 35 to identify any cell (ivew=0 or 1)
  Int_t GetBMNcell(Int_t ilay, Int_t iview, Int_t icell){return icell+iview*3+ilay*6;};
  //for a given cellid, it sets the ilay (0-5), view (0 or 1) and icell (0-2) 
  Bool_t GetBMNlvc(const Int_t cellid, Int_t& ilay, Int_t& iview, Int_t& icell);
  Int_t CellId2cell(Int_t cellid){return cellid%3;}; 
  Int_t CellId2lay(Int_t cellid){return (Int_t)(cellid/6);}; 
  Int_t CellId2view(Int_t cellid){return (((Int_t)(cellid/3))%2==0) ? 0:1;}; 
  //get a number from 0 to 11 to identify real wire plane (iview=0 or 1)
  Int_t GetWirePlane(Int_t ilay, Int_t iview){return iview + ilay*2;};
  TVector3    GetCenter(){return m_center;};
  TVector3    GetSide(){return bm_SideDch;};
  TVector3    GetDelta(){return bm_DeltaDch;};
  TVector3    GetMylar1(){return bm_mylar1;};
  TVector3    GetMylar2(){return bm_mylar2;};
  TVector3    GetTarget(){return bm_target;};

  virtual void    Clear(Option_t* opt="");

  virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;
          
  TGeoVolume*     AddBM(const char *bmName = "BM");//used in TAIR and TACA eventDisplay.cxx to draw a box representing the BM
   
  TEveGeoShapeExtract* AddExtractBM(const char *bmName = "BM");

  ClassDef(TABMparGeo,1)


  private:

  int m_debug;
  //~ int m_nCell[2];//number of cells (or sense wires) per layer
  //~ int m_nFieldW[2];//number of field wires per layer
    

  Int_t bm_idsense[3]; //sense wire index (used in TABMvieTrackFOOT)
  TVector3  m_origin;  // current position in local coord. (not used...)
  TVector3  m_center;  // current position in global coord.
  TRotation* m_rotation; //rotation to the global coord, WARNING: animuthal z angle not considered...

  TVector3  bm_mylar1;  // mylar1 center position in local coord.
  TVector3  bm_mylar2;  // mylar2 center position in local coord.
  TVector3  bm_target;  // target center position in local coord.
  
  TVector3 bm_SideDch;      /* Chamber side dimensions */
  TVector3 bm_DeltaDch;     /* displacement of 1st wire wrt chmb side */

  //x,y,z center positions of the wires
  Double_t x_pos[BMN_NWIRELAY][BMN_NLAY][2];
  Double_t y_pos[BMN_NWIRELAY][BMN_NLAY][2];
  Double_t z_pos[BMN_NWIRELAY][BMN_NLAY][2];

  //lenght of the wires... maybe useless
  Double_t cx_pos[BMN_NWIRELAY][BMN_NLAY][2];
  Double_t cy_pos[BMN_NWIRELAY][BMN_NLAY][2];
  Double_t cz_pos[BMN_NWIRELAY][BMN_NLAY][2];

  TGeoVolume* m_universe;

};

#include "TABMparGeo.icc"

#endif

/*!
  \file
  \version $Id: TATRparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TATRparGeo.
*/

#include <string.h>

#include <fstream>

#include "TSystem.h"
#include "TString.h"
#include "TGeoManager.h"

#include "TATRparGeo.hxx"
#include "TAGgeoTrafo.hxx"

#include "foot_geo.h"

//##############################################################################

/*!
  \class TATRparGeo TATRparGeo.hxx "TATRparGeo.hxx"
  \brief Map and Geometry parameters for the beam monitor. **
*/

ClassImp(TATRparGeo);

//------------------------------------------+-----------------------------------
//! Default constructor.
TATRparGeo::TATRparGeo() {

  NWIRELAYERNEW  = 21;
  NLAYERNEW = 6;
  NSENSENEW = 3;
  
  RSENSE = 0.003;
  RFIELD = 0.008;
 
  bm_step = 0.5;
  bm_cellwide = 0.8;
  bm_dplane = 0.3;
 
  /* parametri geometrici per la nuova camera a drift di monitor del beam */
  BMHEIGHTNEW = 11.2;
  BMWIDTHNEW  = 11.2;
  BMLENGHTNEW = 21.0;
 
  /*  cordinates of the Beam Monitor center */
  XMONNEW = 0.0; 
  YMONNEW = 0.0; 
  ZMONNEW = 0.0; // 51 cm + half leght away from origin 
 
  /* Euler angles that defines the BEAM monitor orientation ( degrees)*/
  EULER1MONNEW  =  0.;
  EULER2MONNEW  =  0.;
  EULER3MONNEW  =  0.;
 
  /*  shift dei fili rispetto ai lati della camera */
  DELTAZNEW  =  2.8; 
  DELTAYNEW  =  2.8;
  DELTAXNEW  =  2.8;

  TVector3 cen, del, sid, ang, dir; 
  cen.SetXYZ(XMONNEW,YMONNEW,ZMONNEW);  SetCenter(cen);
  del.SetXYZ(DELTAXNEW,DELTAYNEW,DELTAZNEW);  SetDelta(del);
  sid.SetXYZ(BMWIDTHNEW,BMHEIGHTNEW,BMLENGHTNEW); SetSide(sid);
  ang.SetXYZ(EULER1MONNEW,EULER2MONNEW,EULER3MONNEW); SetAngles(ang);

   DefineMaterial();
}

//_____________________________________________________________________________
void TATRparGeo::DefineMaterial()
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
      new TGeoManager( TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   TGeoElementTable* table = gGeoManager->GetElementTable();
   
   // create material
   TGeoMaterial* mat = 0x0;;
   TGeoMedium*   med = 0x0;
   TGeoMixture*  mix = 0x0;;
   
   // EJ-212 Scintillator material from eljen technology
   const Char_t* matName = SCN_MEDIUM.Data();
   if ( (mat = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject(matName)) == 0x0 ) {
      
      TGeoElement* matC = table->GetElement(6);
      TGeoElement* matH = table->GetElement(1);
      
      mix =new TGeoMixture(matName,2, 1.023);
      mix->AddElement(matC, 9);
      mix->AddElement(matH, 10);
   }
   if ( (med = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject(matName)) == 0x0 )
      med = new TGeoMedium(matName,1,mat);
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRparGeo::~TATRparGeo()
{}

void TATRparGeo::GetCellInfo(Int_t view, Int_t plane, Int_t cellID, Double_t& h_x, Double_t& h_y, Double_t& h_z, Double_t& h_cx, Double_t& h_cy, Double_t& h_cz) {
  
  /* Set Chamber center positioning */
  int my_ID = GetID(cellID);
  int myview = 0; //U view is default (side view)
  if(view == -1) myview = 1; //V view (top view)
  h_x = x_pos[my_ID][plane][myview];
  h_y = y_pos[my_ID][plane][myview];
  h_z = z_pos[my_ID][plane][myview];

  h_cx =  cx_pos[my_ID][plane][myview];
  h_cy =  cy_pos[my_ID][plane][myview];
  h_cz =  cz_pos[my_ID][plane][myview];

  return;
}

void TATRparGeo::SetCenter(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_CenterDch = h_vec;   

 return;
}

TVector3 TATRparGeo::GetCenter() {
  return bm_CenterDch;
}

void TATRparGeo::SetDirection(TVector3 dir) {

  bm_Direction = dir;
  return;
}

TVector3 TATRparGeo::GetDirection() {

  return bm_Direction;
}

void TATRparGeo::SetAngles(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_AnglesDch = h_vec;   
 return;
}

TVector3 TATRparGeo::GetAngles() {

  /* Get Chamber center positioning */
 return bm_AnglesDch;
}

void TATRparGeo::SetSide(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_SideDch = h_vec;   

 return;
}

TVector3 TATRparGeo::GetSide() {

 return bm_SideDch;
}

void TATRparGeo::SetDelta(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_DeltaDch = h_vec;   

 return;
}

TVector3 TATRparGeo::GetDelta() {

  /* Get Chamber center positioning */
  return bm_DeltaDch;
}

void TATRparGeo::InitGeo(){

   double aa[NWIRELAYERNEW], bb[NWIRELAYERNEW];
   
   bm_idsense[0]= 8;
   bm_idsense[1]= 10;
   bm_idsense[2]= 12;

   for(int nn=0;nn<7;nn++) {
     bb[nn] = nn*bm_cellwide;
     bb[nn+7] = nn*bm_cellwide;
     bb[nn+14] = nn*bm_cellwide;

     aa[nn] = 0. ;
     aa[nn+7] = bm_step;
     aa[nn+14] = 2*bm_step;
   }

   /* 
      generazione parametri volumi di FLUKA :  ogni filo e' definito 
      da un cilindro: 2 basi nel mylar e direzione
      parallela all'asse x ( vista U) e y (vista V) definito da:
      
      1) vettore posizione del punto centrale della base 
      2) vettore che da una base arriva all'altra base
      3) raggio
   */

   for(int il=0; il<NLAYERNEW;il++){
     for (int kk =0; kk<NWIRELAYERNEW;kk++){       

       /*  U wires -> along x */
       x_pos[kk][il][0] = bm_CenterDch[0] - bm_SideDch[0]/2;
       z_pos[kk][il][0] = bm_CenterDch[2] - bm_SideDch[2]/2 + bm_DeltaDch[2] +
	 aa[kk] + (2*il+1)*(2*bm_step + bm_dplane) ;
       /* in principles should be like that but we mounted the chamber upside down*/
       //       if( (il==0) || (il==2) || (il==4)){ 
       if( (il==1) || (il==3) || (il==5)){ 
	 y_pos[kk][il][0] = bm_CenterDch[1] - bm_SideDch[1]/2 + bm_DeltaDch[1] +
	   bb[kk];
       }
       else{ 
	 y_pos[kk][il][0] = bm_CenterDch[1] - bm_SideDch[1]/2 + bm_DeltaDch[1] +
	   bb[kk] + bm_cellwide ;
       }
       cx_pos[kk][il][0] = bm_SideDch[0];
       cy_pos[kk][il][0] = 0.;
       cz_pos[kk][il][0] = 0.;
       
       /* V wires -> along y*/
       y_pos[kk][il][1] = bm_CenterDch[1] - bm_SideDch[1]/2;
       z_pos[kk][il][1] = bm_CenterDch[2] - bm_SideDch[2]/2 + bm_DeltaDch[2] +
	 aa[kk] + 2*il*(2*bm_step + bm_dplane) ;
       /* see above !!!!! upside down....*/
       // if( (il==0) || (il==2) || (il==4) ){
       if( (il==1) || (il==3) || (il==5) ){
	 x_pos[kk][il][1] = bm_CenterDch[0] - bm_SideDch[0]/2 + bm_DeltaDch[0] +
	   + bb[kk]  ;
       }
       else{
	 x_pos[kk][il][1] = bm_CenterDch[0] - bm_SideDch[0]/2 + bm_DeltaDch[0] +
	   + bb[kk] + bm_cellwide ;
       }
       cx_pos[kk][il][1] = 0.;
       cy_pos[kk][il][1] = bm_SideDch[1];
       cz_pos[kk][il][1] = 0.;

       bm_senserad = RSENSE;
       bm_fieldrad = RFIELD;

     }
   }

  return;
}

//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TATRparGeo::Clear(Option_t*)
{
  for(int i=0; i<3; i++) {
    bm_idsense[i] = 0;
  }
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TATRparGeo::ToStream(ostream& os, Option_t*) const
{
  os << "TATRparGeo " << GetName() << endl;
  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}


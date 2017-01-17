/*!
  \file
  \version $Id: TABMparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TABMparGeo.
*/

#include <string.h>
#include <math.h>

#include <fstream>
#include "TEveTrans.h"
#include "TGeoBBox.h"
#include "TSystem.h"
#include "TString.h"
#include "TVectorD.h"
#include "TMatrixD.h"
#include "TList.h"
#include "TGeoBBox.h"
#include "TColor.h"
#include "TGeoManager.h"
#include "TROOT.h"


#include "TAGgeoTrafo.hxx" 
#include "TABMparGeo.hxx"

//##############################################################################

/*!
  \class TABMparGeo TABMparGeo.hxx "TABMparGeo.hxx"
  \brief Map and Geometry parameters for the beam monitor. **
*/

ClassImp(TABMparGeo);

//------------------------------------------+-----------------------------------
//! Default constructor.
TABMparGeo::TABMparGeo() {

  NWIRELAYERNEW  = 21;
  NLAYERNEW = 6;
  NSENSENEW = 3;
  
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
 
  /*  shift dei fili rispetto ai lati della camera (-0.8 in x,y if you change senseid)*/
  DELTAZNEW  =  2.85; 
  DELTAYNEW  =  2.80;
  DELTAXNEW  =  2.80;

  TVector3 cen, del, sid, ang, dir; 
  cen.SetXYZ(XMONNEW,YMONNEW,ZMONNEW);  SetCenter(cen);
  del.SetXYZ(DELTAXNEW,DELTAYNEW,DELTAZNEW);  SetDelta(del);
  sid.SetXYZ(BMWIDTHNEW,BMHEIGHTNEW,BMLENGHTNEW); SetSide(sid);
  ang.SetXYZ(EULER1MONNEW,EULER2MONNEW,EULER3MONNEW); SetAngles(ang);

}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMparGeo::~TABMparGeo()
{}

void TABMparGeo::GetCellInfo(Int_t view, Int_t plane, Int_t cellID, Double_t& h_x, Double_t& h_y, Double_t& h_z, Double_t& h_cx, Double_t& h_cy, Double_t& h_cz) {
  
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

void TABMparGeo::SetCenter(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_CenterDch = h_vec;   

 return;
}

TVector3 TABMparGeo::GetCenter() {
  return bm_CenterDch;
}

void TABMparGeo::SetDirection(TVector3 dir) {

  bm_Direction = dir;
  return;
}

TVector3 TABMparGeo::GetDirection() {

  return bm_Direction;
}

void TABMparGeo::SetAngles(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_AnglesDch = h_vec;   
 return;
}

TVector3 TABMparGeo::GetAngles() {

  /* Get Chamber center positioning */
 return bm_AnglesDch;
}

void TABMparGeo::SetSide(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_SideDch = h_vec;   

 return;
}

TVector3 TABMparGeo::GetSide() {

 return bm_SideDch;
}

void TABMparGeo::SetDelta(TVector3 h_vec) {

  /* Set Chamber center positioning */
  bm_DeltaDch = h_vec;   

 return;
}

TVector3 TABMparGeo::GetDelta() {

  /* Get Chamber center positioning */
  return bm_DeltaDch;
}

void TABMparGeo::InitGeo(){

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

       /*  U wires -> along x, SIDE view */
       x_pos[kk][il][0] = - bm_SideDch[0]/2;
       z_pos[kk][il][0] = - bm_SideDch[2]/2 + bm_DeltaDch[2] +
	 aa[kk] + (2*il)*(2*bm_step + bm_dplane) ;

       if( (il==0) || (il==2) || (il==4) ){ 
	 //       if( (il==1) || (il==3) || (il==5) ){ 
	 y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] +
	   bb[kk] + bm_cellwide ;
	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" Y,Z::" <<kk<<" "<<il<<" "<<y_pos[kk][il][0]<< " "<<z_pos[kk][il][0]<<endl;
       }
       else{ 
	 y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] +
	   bb[kk];
	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" Y,Z::" <<kk<<" "<<il<<" "<<y_pos[kk][il][0]<< " "<<z_pos[kk][il][0]<<endl;
       }
       cx_pos[kk][il][0] = bm_SideDch[0];
       cy_pos[kk][il][0] = 0.;
       cz_pos[kk][il][0] = 0.;
       
       /* V wires -> along y, TOP view*/
       y_pos[kk][il][1] = - bm_SideDch[1]/2;
       z_pos[kk][il][1] = - bm_SideDch[2]/2 + bm_DeltaDch[2] +
	 aa[kk] + (2*il+1)*(2*bm_step + bm_dplane) ;

       if( (il==0) || (il==2) || (il==4) ){
	 //       if( (il==1) || (il==3) || (il==5) ){
	 x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] +
	   + bb[kk]  ;

	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" X,Z::" <<kk<<" "<<il<<" "<<x_pos[kk][il][1]<< " "<<z_pos[kk][il][1]<<endl;
       }
       else{
	 x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] +
	   + bb[kk] + bm_cellwide ;
	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" X,Z::" <<kk<<" "<<il<<" "<<x_pos[kk][il][1]<< " "<<z_pos[kk][il][1]<<endl;
       }

       cx_pos[kk][il][1] = 0.;
       cy_pos[kk][il][1] = bm_SideDch[1];
       cz_pos[kk][il][1] = 0.;

     }
   }

  return;
}


int TABMparGeo::ShiftBmon(){

  TVector3 cen = GetCenter();

  for(int il=0; il<NLAYERNEW;il++){
    for (int kk =0; kk<NWIRELAYERNEW;kk++){

      x_pos[kk][il][0]  += cen.X();
      y_pos[kk][il][0]  += cen.Y();
      z_pos[kk][il][0]  += cen.Z();

      x_pos[kk][il][1]  += cen.X();
      y_pos[kk][il][1]  += cen.Y();
      z_pos[kk][il][1]  += cen.Z();
    }
  }
  return 0 ;
}


 /*-----------------------------------------------------------------*/
int TABMparGeo::RotateBmon(){

  /*
    Rotate the chamber by means of the Euler angles (in degrees) 
    WE FOLLOW THE GOLDSTEIN CONVENTION for the Euler angles
  */

  const int nrows = 3, ncols = 3;
  const double epsilon = 1.0e-14;
  const int DBG = 0;

  TVectorD PosWireIn,PosWireOut,DirWireIn,DirWireOut;
  TMatrixD EulerRot(nrows,ncols);
  PosWireIn.ResizeTo(nrows); PosWireIn.Zero();
  PosWireOut.ResizeTo(nrows); PosWireOut.Zero();
  DirWireIn.ResizeTo(nrows); DirWireIn.Zero();
  DirWireOut.ResizeTo(nrows); DirWireOut.Zero();
  
  double phi = GetAngles().X()*acos(-1)/180;
  double theta = GetAngles().Y()*acos(-1)/180;
  double psi = GetAngles().Z()*acos(-1)/180;

  /* definizione della matrice di rotazione tramite angoli di Eulero */

  EulerRot[0][0] = cos(phi)*cos(psi) - sin(phi)*cos(theta)*sin(psi);
  EulerRot[0][1] = sin(phi)*cos(psi) + cos(phi)*cos(theta)*sin(psi);
  EulerRot[0][2] = sin(theta)*sin(psi) ;
  EulerRot[1][0] = -cos(phi)*sin(psi) - sin(phi)*cos(theta)*cos(psi);
  EulerRot[1][1] = -sin(phi)*sin(psi) + cos(phi)*cos(theta)*cos(psi);
  EulerRot[1][2] = sin(theta)*cos(psi) ;

  EulerRot[2][0] = sin(phi)*sin(theta);
  EulerRot[2][1] = -cos(phi)*sin(theta);
  EulerRot[2][2] = cos(theta);

  /*  clean up very tiny matrix elements due to roundings */

  for (int nc =0; nc<ncols;nc++){       
    for(int nr=0; nr<nrows;nr++){
      if(fabs(EulerRot(nr,nc)) < epsilon) EulerRot(nr,nc) = 0 ;
    }
  }
  
  if(DBG>0) {
    cout<<" Beam Monitor Rotation: phi = "<<phi<<"  theta= "<<
      theta<<"  psi= "<<psi<<endl<<endl;
    cout<<"  Rotation Matrix= "<<endl<<endl;
    cout<<"  "<<EulerRot(0,0)<<"  "<<EulerRot(0,1)<<"  "<<EulerRot(0,2)<<endl;
    cout<<"  "<<EulerRot(1,0)<<"  "<<EulerRot(1,1)<<"  "<<EulerRot(1,2)<<endl;
    cout<<"  "<<EulerRot(2,0)<<"  "<<EulerRot(2,1)<<"  "<<EulerRot(2,2)<<endl;
  }
  /*   rotate the U, SIDE view */
  for(int il=0; il<NLAYERNEW;il++){
    for (int iw =0; iw<NWIRELAYERNEW;iw++){       
      
      PosWireIn(0) = x_pos[iw][il][0];
      PosWireIn(1) = y_pos[iw][il][0];
      PosWireIn(2) = z_pos[iw][il][0];
      DirWireIn(0) = cx_pos[iw][il][0];
      DirWireIn(1) = cy_pos[iw][il][0];
      DirWireIn(2) = cz_pos[iw][il][0];
  
      PosWireOut = EulerRot*PosWireIn;
      DirWireOut = EulerRot*DirWireIn;

      x_pos[iw][il][0] = PosWireOut(0);
      y_pos[iw][il][0] = PosWireOut(1);
      z_pos[iw][il][0] = PosWireOut(2);
      cx_pos[iw][il][0] = DirWireOut(0);
      cy_pos[iw][il][0] = DirWireOut(1);
      cz_pos[iw][il][0] = DirWireOut(2);
     }
   }

  /*   rotate the V, TOP view */
  for(int il=0; il<NLAYERNEW;il++){
    for (int iw =0; iw<NWIRELAYERNEW;iw++){       
      
      PosWireIn(0) = x_pos[iw][il][1];
      PosWireIn(1) = y_pos[iw][il][1];
      PosWireIn(2) = z_pos[iw][il][1];
      DirWireIn(0) = cx_pos[iw][il][1];
      DirWireIn(1) = cy_pos[iw][il][1];
      DirWireIn(2) = cz_pos[iw][il][1];
  
      PosWireOut = EulerRot*PosWireIn;
      DirWireOut = EulerRot*DirWireIn;

      x_pos[iw][il][1] = PosWireOut(0);
      y_pos[iw][il][1] = PosWireOut(1);
      z_pos[iw][il][1] = PosWireOut(2);
      cx_pos[iw][il][1] = DirWireOut(0);
      cy_pos[iw][il][1] = DirWireOut(1);
      cz_pos[iw][il][1] = DirWireOut(2);

     }
   }

  return 0;
}


//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TABMparGeo::Clear(Option_t*)
{
  for(int i=0; i<3; i++) {
    bm_idsense[i] = 0;
  }
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TABMparGeo::ToStream(ostream& os, Option_t*) const
{
  os << "TABMparGeo " << GetName() << endl;
  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}

/*
TGeoManager* TABMparGeo::GetGeoManager() {

  
}
*/

//_____________________________________________________________________________
TGeoVolume* TABMparGeo::AddBM(const char *bmName )
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
	  new TGeoManager(TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create module
   TGeoMaterial* matBM;
   TGeoMedium*   medBM;
   
   if ( (matBM = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
	  matBM = new TGeoMaterial("Vacuum", 0., 0., 0.);;
   if ( (medBM = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
	  medBM = new TGeoMedium("Vacuum", 1, matBM);
   
   TGeoVolume* tube = gGeoManager->MakeBox(bmName, medBM, TAGgeoTrafo::CmToMu()*GetWidth()/2., TAGgeoTrafo::CmToMu()*GetHeigth()/2., 
										                  TAGgeoTrafo::CmToMu()*GetLength()/2.);
   tube->SetVisibility(true);
   tube->SetLineColor(17);
   tube->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return tube;
}

//_____________________________________________________________________________
TEveGeoShapeExtract* TABMparGeo::AddExtractBM(const char *bmName )
{
   TGeoBBox* tube = new TGeoBBox(bmName, TAGgeoTrafo::CmToMu()*GetWidth()/2., TAGgeoTrafo::CmToMu()*GetHeigth()/2., 
										   TAGgeoTrafo::CmToMu()*GetLength()/2.);
   TColor* color = gROOT->GetColor(17);
   Float_t rgba[4];
   color->GetRGB(rgba[0], rgba[1], rgba[2]);
   rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;
   
   TEveGeoShapeExtract* tubeExtract = new TEveGeoShapeExtract(bmName);
   tubeExtract->SetShape(tube);
   tubeExtract->SetRGBA(rgba);
   
   return tubeExtract;
}



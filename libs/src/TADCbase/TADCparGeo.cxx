/*!
  \file
  \version $Id: adapted from TABMparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TADCparGeo.
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
#include "TADCparGeo.hxx"

//##############################################################################

/*!
  \class TADCparGeo TADCparGeo.hxx "TADCparGeo.hxx"
  \brief Map and Geometry parameters for the drift chamber. **
*/

ClassImp(TADCparGeo);

//------------------------------------------+-----------------------------------
//! Default constructor.
TADCparGeo::TADCparGeo() {

  NWIRELAYERNEW  = 21;
  NLAYERNEW = 6;
  NSENSENEW = 3;
  
  RSENSE = 0.003;
  RFIELD = 0.008;
 
  dc_step = 0.5;
  dc_cellwide = 0.8;
  dc_dplane = 0.3;
 
  /* parametri geometrici per la nuova camera a drift */
  DCHEIGHTNEW = 11.2;
  DCWIDTHNEW  = 11.2;
  DCLENGHTNEW = 21.0;
 
  /*  cordinates of the drift chamber center */
  XDCNEW = 0.0; 
  YDCNEW = 0.0; 
  ZDCNEW = 0.0; // 51 cm + half leght away from origin 
 
  /* Euler angles that defines the drift chamber orientation ( degrees)*/
  EULER1DCNEW  =  0.;
  EULER2DCNEW  =  0.;
  EULER3DCNEW  =  0.;
 
  /*  shift dei fili rispetto ai lati della camera (-0.8 in x,y if you change senseid)*/
  DELTAZNEW  =  2.85; 
  DELTAYNEW  =  2.80;
  DELTAXNEW  =  2.80;

  TVector3 cen, del, sid, ang, dir; 
  cen.SetXYZ(XDCNEW,YDCNEW,ZDCNEW);  SetCenter(cen);
  del.SetXYZ(DELTAXNEW,DELTAYNEW,DELTAZNEW);  SetDelta(del);
  sid.SetXYZ(DCWIDTHNEW,DCHEIGHTNEW,DCLENGHTNEW); SetSide(sid);
  ang.SetXYZ(EULER1DCNEW,EULER2DCNEW,EULER3DCNEW); SetAngles(ang);

}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCparGeo::~TADCparGeo()
{}

void TADCparGeo::GetCellInfo(Int_t view, Int_t plane, Int_t cellID, Double_t& h_x, Double_t& h_y, Double_t& h_z, Double_t& h_cx, Double_t& h_cy, Double_t& h_cz) {
  
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

void TADCparGeo::SetCenter(TVector3 h_vec) {

  /* Set Chamber center positioning */
  dc_CenterDch = h_vec;   

 return;
}

TVector3 TADCparGeo::GetCenter() {
  return dc_CenterDch;
}

void TADCparGeo::SetDirection(TVector3 dir) {

  dc_Direction = dir;
  return;
}

TVector3 TADCparGeo::GetDirection() {

  return dc_Direction;
}

void TADCparGeo::SetAngles(TVector3 h_vec) {

  /* Set Chamber center positioning */
  dc_AnglesDch = h_vec;   
 return;
}

TVector3 TADCparGeo::GetAngles() {

  /* Get Chamber center positioning */
 return dc_AnglesDch;
}

void TADCparGeo::SetSide(TVector3 h_vec) {

  /* Set Chamber center positioning */
  dc_SideDch = h_vec;   

 return;
}

TVector3 TADCparGeo::GetSide() {

 return dc_SideDch;
}

void TADCparGeo::SetDelta(TVector3 h_vec) {

  /* Set Chamber center positioning */
  dc_DeltaDch = h_vec;   

 return;
}

TVector3 TADCparGeo::GetDelta() {

  /* Get Chamber center positioning */
  return dc_DeltaDch;
}

void TADCparGeo::InitGeo(){

   double aa[NWIRELAYERNEW], bb[NWIRELAYERNEW];
   
   dc_idsense[0]= 8;
   dc_idsense[1]= 10;
   dc_idsense[2]= 12;

   for(int nn=0;nn<7;nn++) {
     bb[nn] = nn*dc_cellwide;
     bb[nn+7] = nn*dc_cellwide;
     bb[nn+14] = nn*dc_cellwide;

     aa[nn] = 0. ;
     aa[nn+7] = dc_step;
     aa[nn+14] = 2*dc_step;
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
       x_pos[kk][il][0] = - dc_SideDch[0]/2;
       z_pos[kk][il][0] = - dc_SideDch[2]/2 + dc_DeltaDch[2] +
	 aa[kk] + (2*il)*(2*dc_step + dc_dplane) ;

       if( (il==0) || (il==2) || (il==4) ){ 
	 //       if( (il==1) || (il==3) || (il==5) ){ 
	 y_pos[kk][il][0] = - dc_SideDch[1]/2 + dc_DeltaDch[1] +
	   bb[kk] + dc_cellwide ;
	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" Y,Z::" <<kk<<" "<<il<<" "<<y_pos[kk][il][0]<< " "<<z_pos[kk][il][0]<<endl;
       }
       else{ 
	 y_pos[kk][il][0] = - dc_SideDch[1]/2 + dc_DeltaDch[1] +
	   bb[kk];
	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" Y,Z::" <<kk<<" "<<il<<" "<<y_pos[kk][il][0]<< " "<<z_pos[kk][il][0]<<endl;
       }
       cx_pos[kk][il][0] = dc_SideDch[0];
       cy_pos[kk][il][0] = 0.;
       cz_pos[kk][il][0] = 0.;
       
       /* V wires -> along y, TOP view*/
       y_pos[kk][il][1] = - dc_SideDch[1]/2;
       z_pos[kk][il][1] = - dc_SideDch[2]/2 + dc_DeltaDch[2] +
	 aa[kk] + (2*il+1)*(2*dc_step + dc_dplane) ;

       if( (il==0) || (il==2) || (il==4) ){
	 //       if( (il==1) || (il==3) || (il==5) ){
	 x_pos[kk][il][1] = - dc_SideDch[0]/2 + dc_DeltaDch[0] +
	   + bb[kk]  ;

	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" X,Z::" <<kk<<" "<<il<<" "<<x_pos[kk][il][1]<< " "<<z_pos[kk][il][1]<<endl;
       }
       else{
	 x_pos[kk][il][1] = - dc_SideDch[0]/2 + dc_DeltaDch[0] +
	   + bb[kk] + dc_cellwide ;
	 //	 if(kk == 8 || kk == 10 || kk == 12)cout<<" X,Z::" <<kk<<" "<<il<<" "<<x_pos[kk][il][1]<< " "<<z_pos[kk][il][1]<<endl;
       }

       cx_pos[kk][il][1] = 0.;
       cy_pos[kk][il][1] = dc_SideDch[1];
       cz_pos[kk][il][1] = 0.;

       dc_senserad = RSENSE;
       dc_fieldrad = RFIELD;

     }
   }

  return;
}


int TADCparGeo::ShiftDcon(){

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
int TADCparGeo::RotateDcon(){

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
    cout<<" Drift Chamber Rotation: phi = "<<phi<<"  theta= "<<
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

void TADCparGeo::Clear(Option_t*)
{
  for(int i=0; i<3; i++) {
    dc_idsense[i] = 0;
  }
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TADCparGeo::ToStream(ostream& os, Option_t*) const
{
  os << "TADCparGeo " << GetName() << endl;
  os << "p 8p   ref_x   ref_y   ref_z   hor_x   hor_y   hor_z"
     << "   ver_x   ver_y   ver_z  width" << endl;

  return;
}

//_____________________________________________________________________________
TGeoVolume* TADCparGeo::AddDC(const char *dcName )
{
   if ( gGeoManager == 0x0 ) { // a new Geo Manager is created if needed
	  new TGeoManager(TAGgeoTrafo::GetDefaultGeomName(), TAGgeoTrafo::GetDefaultGeomTitle());
   }
   
   // create module
   TGeoMaterial* matDC;
   TGeoMedium*   medDC;
   
   if ( (matDC = (TGeoMaterial *)gGeoManager->GetListOfMaterials()->FindObject("Vacuum")) == 0x0 )
	  matDC = new TGeoMaterial("Vacuum", 0., 0., 0.);;
   if ( (medDC = (TGeoMedium *)gGeoManager->GetListOfMedia()->FindObject("Vacuum")) == 0x0 )
	  medDC = new TGeoMedium("Vacuum", 1, matDC);
   
   TGeoVolume* tube = gGeoManager->MakeBox(dcName, medDC, TAGgeoTrafo::CmToMu()*GetWidth()/2., TAGgeoTrafo::CmToMu()*GetHeigth()/2., 
										                  TAGgeoTrafo::CmToMu()*GetLength()/2.);
   tube->SetVisibility(true);
   tube->SetLineColor(17);
   tube->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
   return tube;
}

//_____________________________________________________________________________
TEveGeoShapeExtract* TADCparGeo::AddExtractDC(const char *dcName )
{
   TGeoBBox* tube = new TGeoBBox(dcName, TAGgeoTrafo::CmToMu()*GetWidth()/2., TAGgeoTrafo::CmToMu()*GetHeigth()/2., 
										   TAGgeoTrafo::CmToMu()*GetLength()/2.);
   TColor* color = gROOT->GetColor(17);
   Float_t rgba[4];
   color->GetRGB(rgba[0], rgba[1], rgba[2]);
   rgba[3] = TAGgeoTrafo::GetDefaultTransp()/100.;
   
   TEveGeoShapeExtract* tubeExtract = new TEveGeoShapeExtract(dcName);
   tubeExtract->SetShape(tube);
   tubeExtract->SetRGBA(rgba);
   
   return tubeExtract;
}

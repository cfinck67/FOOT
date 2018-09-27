/*!
  \file
  \version $Id: TABMparGeo.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TABMparGeo.
*/

#include <string.h>
#include <math.h>

#include <fstream>
#include <sstream>
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
#include "TFile.h"


//#include "Geometry.h" 
#include "TAGgeoTrafo.hxx" 
#include "TABMparGeo.hxx"

using namespace std;

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
  ZMONNEW = -14.; // 51 cm + half leght away from origin //modificato da yun per mettere a mano posizione per ora... poi da fare in modo che legga da file esterno FOOT_geo.map!!!!!!
 
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
  int iCell[2], iField[2];
   
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
	iCell[0]++;
	//       if( (il==1) || (il==3) || (il==5) ){ 
	y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] +
	  bb[kk] + bm_cellwide ;
	//	 if(kk == 8 || kk == 10 || kk == 12)cout<<" Y,Z::" <<kk<<" "<<il<<" "<<y_pos[kk][il][0]<< " "<<z_pos[kk][il][0]<<endl;
      }
      else{
	iField[0]++;
	y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] +
	  bb[kk];
	//	 if(kk == 8 || kk == 10 || kk == 12)cout<<" Y,Z::" <<kk<<" "<<il<<" "<<y_pos[kk][il][0]<< " "<<z_pos[kk][il][0]<<endl;
      }
      cx_pos[kk][il][0] = bm_SideDch[0];
      cy_pos[kk][il][0] = 0.;
      cz_pos[kk][il][0] = 0.;
       
      /* V wires -> along y, TOP view*/
      y_pos[kk][il][1] = - bm_SideDch[1]/2;
      z_pos[kk][il][1] = - bm_SideDch[2]/2 + bm_DeltaDch[2] + aa[kk] + (2*il+1)*(2*bm_step + bm_dplane) ;

      if( (il==0) || (il==2) || (il==4) ){
	iCell[1]++;
	//       if( (il==1) || (il==3) || (il==5) ){
	x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk]  ;

	//	 if(kk == 8 || kk == 10 || kk == 12)cout<<" X,Z::" <<kk<<" "<<il<<" "<<x_pos[kk][il][1]<< " "<<z_pos[kk][il][1]<<endl;
      }
      else{
	iField[1]++;
	x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk] + bm_cellwide ;
	//	 if(kk == 8 || kk == 10 || kk == 12)cout<<" X,Z::" <<kk<<" "<<il<<" "<<x_pos[kk][il][1]<< " "<<z_pos[kk][il][1]<<endl;
      }

      cx_pos[kk][il][1] = 0.;
      cy_pos[kk][il][1] = bm_SideDch[1];
      cz_pos[kk][il][1] = 0.;

    }
  }

  m_nCell[0] = iCell[0];
  m_nCell[1] = iCell[1];
  m_nFieldW[0] = iCell[0];
  m_nFieldW[1] = iCell[1];
  
  //to create LocalBM.root file for genfit tracking:
  // CreateLocalBMGeo();

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

void TABMparGeo::CreateLocalBMGeo() 
{
   
  new TGeoManager("bm_geo", "geometria beam monitor");
  
  // create material   
  TGeoMaterial *matAr = new TGeoMaterial("Argon", 39.948, 18., 0.001662);//densità viene da flair, 
  TGeoMaterial *matC = new TGeoMaterial("Carbon", 12.0107, 6., 2.26);
  TGeoMaterial *matO = new TGeoMaterial("Oxygen", 16., 8., 0.0013315);
  TGeoMaterial *matAl = new TGeoMaterial("Aluminium", 26.981539, 13., 2.6989);
  TGeoMaterial *matW = new TGeoMaterial("Tungsten", 183.84, 74., 19.3);// poi magari mettere la copertura in oro
  TGeoMaterial *vacuum = new TGeoMaterial("vacuum",0,0,0);//a,z,rho

  TGeoMixture *ArCO2 = new TGeoMixture("ArCO2",3);
  ArCO2->AddElement(matAr ,1./4.);
  ArCO2->AddElement(matC ,1./4.);
  ArCO2->AddElement(matO ,2./4.);
  //   ArCO2->AddElement(matCO2 ,20.);
 
  ArCO2->SetDensity(0.001677136); //da flair


  //create medium
  TGeoMedium *vacuum_med = new TGeoMedium("vacuum_med",0, vacuum);
  TGeoMedium *gas_med = new TGeoMedium("gas_med",1, ArCO2);
  TGeoMedium *c_wire_med = new TGeoMedium("catod wire",2, matAl);
  TGeoMedium *a_wire_med = new TGeoMedium("anod wire",3, matW);
   
   
  //create gas boxe
  //~ TGeoVolume *top = bm_geo->MakeBox("top",vacuum_med,1000,1000,1000); //top è scatola che conterrà tutto (dimensioni in cm)
  TGeoVolume *top = gGeoManager->MakeBox("top",vacuum_med,1000,1000,1000); //top è scatola che conterrà tutto (dimensioni in cm)
  //~ TGeoVolume* top = gGeoManager->MakeBox("top", gas_med, BMHEIGHTNEW/2., BMWIDTHNEW/2., BMLENGHTNEW/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.

  //qua provo a togliere box di area_gas e mettere solo top al suo posto...
  //~ bm_geo->SetTopVolume(top); //setta il mio mondo a top
  //~ bm_geo->SetTopVisible(0);  //per vedere confini del mondo metterci 1 invece che 0   
  gGeoManager->SetTopVolume(top);
  gGeoManager->SetTopVisible(0);
  //~ TGeoVolume* area_gas = bm_geo->MakeBox("gas_vol", gas_med, BMHEIGHTNEW/2., BMWIDTHNEW/2., BMLENGHTNEW/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.
  TGeoVolume* area_gas = gGeoManager->MakeBox("gas_vol", gas_med, BMHEIGHTNEW/2., BMWIDTHNEW/2., BMLENGHTNEW/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.
  //area_gas->SetLineColor(8);
   
   
   
  //create wires  
  //TGeoVolume* x_cell = bm_geo->MakeBox("x_cell", gas_med, BMHEIGHTNEW/2., bm_cellwide, bm_step); //cella di gas forse ne faccio a meno
  //~ TGeoVolume* x_cell = gGeoManager->MakeBox("x_cell", gas_med, BMHEIGHTNEW/2., bm_cellwide, bm_step); //cella di gas forse ne faccio a meno
  //TGeoVolume *c_x_wire = bm_geo->MakeTubs("c_x_wire", c_wire_med, 0, 0.0045, BMHEIGHTNEW/2.,0.,0.); //filo catodo lungo z 
  TGeoVolume *c_x_wire = gGeoManager->MakeTubs("c_x_wire", c_wire_med, 0, 0.0045, BMHEIGHTNEW/2.,0.,0.); //filo catodo lungo z 
  c_x_wire->SetLineColor(kBlue);
  TGeoVolume *c_y_wire = gGeoManager->MakeTubs("c_y_wire", c_wire_med, 0, 0.0045, BMHEIGHTNEW/2.,0.,0.); 
  //TGeoVolume *c_y_wire = bm_geo->MakeTubs("c_y_wire", c_wire_med, 0, 0.0045, BMHEIGHTNEW/2.,0.,0.); 
  c_y_wire->SetLineColor(kRed);
  TGeoVolume *a_x_wire = gGeoManager->MakeTubs("a_x_wire", a_wire_med, 0, 0.0015, BMHEIGHTNEW/2.,0.,0.); //BISOGNERÀ POI SETTARE MEGLIO IL TUTTO
  //TGeoVolume *a_x_wire = bm_geo->MakeTubs("a_x_wire", a_wire_med, 0, 0.0015, BMHEIGHTNEW/2.,0.,0.); //BISOGNERÀ POI SETTARE MEGLIO IL TUTTO
  a_x_wire->SetLineColor(kYellow);
  TGeoVolume *a_y_wire = gGeoManager->MakeTubs("a_y_wire", a_wire_med, 0, 0.0015, BMHEIGHTNEW/2.,0.,0.);  
  //TGeoVolume *a_y_wire = bm_geo->MakeTubs("a_y_wire", a_wire_med, 0, 0.0015, BMHEIGHTNEW/2.,0.,0.);  
  a_y_wire->SetLineColor(kGreen);   

  //set wires
  Int_t c=0;

  for(Int_t il=0; il<NLAYERNEW;il++){
    for (Int_t kk =0; kk<NWIRELAYERNEW;kk++){   
      //fili || a x:
      if(kk==bm_idsense[0] || kk==bm_idsense[1] || kk==bm_idsense[2]){//se è filo di sense
	area_gas->AddNode(a_x_wire, c++ , new TGeoCombiTrans(0.,y_pos[kk][il][0],z_pos[kk][il][0], new TGeoRotation("a_x_wire,",90.,90.,0.)));    
	//~ top->AddNode(a_x_wire, c++ , new TGeoCombiTrans(0.,y_pos[kk][il][0],z_pos[kk][il][0], new TGeoRotation("a_x_wire,",90.,90.,0.)));    
	area_gas->AddNode(a_y_wire, c++ , new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("a_y_wire,",0.,90.,0.)));
	//~ top->AddNode(a_y_wire, c++ , new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("a_y_wire,",0.,90.,0.)));
      }
      else{// filo di catodo
	area_gas->AddNode(c_x_wire, c++ , new TGeoCombiTrans(0.,y_pos[kk][il][0],z_pos[kk][il][0], new TGeoRotation("c_x_wire,",90.,90.,0.)));  
	//~ top->AddNode(c_x_wire, c++ , new TGeoCombiTrans(0.,y_pos[kk][il][0],z_pos[kk][il][0], new TGeoRotation("c_x_wire,",90.,90.,0.)));  
	area_gas->AddNode(c_y_wire, c++ , new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("c_y_wire,",0.,90.,0.)));
	//~ top->AddNode(c_y_wire, c++ , new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("c_y_wire,",0.,90.,0.)));
      }
    }
  }
   
   
   
  top->AddNode(area_gas,c, gGeoIdentity);
  //   bm_geo->CloseGeometry();
  gGeoManager->CloseGeometry();
  TFile *outfile = TFile::Open("LocalBM_pro.root","RECREATE");
  //   bm_geo->Write();

  gGeoManager->Write();
  outfile->Close();
  cout<<"LocalBM.root created"<<endl;
  return ;
}


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




//_____________________________________________________________________________
string TABMparGeo::PrintBodies(){

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TABMparGeo::PrintBodies()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);
    

  stringstream outstr;
  outstr << "* ***Beam Monitor" << endl;

  int iSense[2]={-1,-1}, iField[2]={-1,-1};
  // stringstream ss;
  char bodyname[20];
  double xmin, xmax, ymin, ymax, zmin, zmax;
  
  double shift = 0.00001;
  
  outstr << setiosflags(ios::fixed) << setprecision(6);

  outstr << "RPP BmnShiOu    "
	 << BMN_X-BMN_WIDTH/2.-BMN_SHI_THICK << "   "
	 << BMN_X+BMN_WIDTH/2.+BMN_SHI_THICK << " "
	 << BMN_Y-BMN_HEIGHT/2.-BMN_SHI_THICK << " "
	 << BMN_Y+BMN_HEIGHT/2.+BMN_SHI_THICK << " "
	 << BMN_Z-BMN_SHI_LENGTH/2. << " "
	 << BMN_Z+BMN_SHI_LENGTH/2. << endl;
  
  outstr << "RPP BmnShiIn    "
	 << BMN_X-BMN_WIDTH/2. << "   "
	 << BMN_X+BMN_WIDTH/2. << " "
	 << BMN_Y-BMN_HEIGHT/2. << " "
	 << BMN_Y+BMN_HEIGHT/2. << " "
	 << BMN_Z-BMN_SHI_LENGTH/2. << " "
	 << BMN_Z+BMN_SHI_LENGTH/2. << endl;
  
  outstr << "XYP BmnMyl0     " << BMN_Z-BMN_LENGTH/2.-BMN_MYL_THICK << endl;
  outstr << "XYP BmnMyl1     " << BMN_Z-BMN_LENGTH/2. << endl;
  outstr << "XYP BmnMyl2     " << BMN_Z+BMN_LENGTH/2. << endl;
  outstr << "XYP BmnMyl3     " << BMN_Z+BMN_LENGTH/2.+BMN_MYL_THICK << endl;
  
	
  // Cells
  // prima lungo x, poi lungo y
  int cella=0;
  for (int il=0;il<NLAYERNEW;il++){ // loop on layers
    for (int ic =0; ic<NWIRELAYERNEW;ic++){  // loop on cells
      if ( (ic==bm_idsense[0]) ||(ic==bm_idsense[1]) ||
      	   (ic==bm_idsense[2]) ){
	for (int iv =0; iv<2;iv++){      // loop on views
	  if ( iv == 0 ){
	    xmin = - BMWIDTHNEW/2. + shift;
	    xmax = + BMWIDTHNEW/2. - shift;
	    ymin = y_pos[ic][il][iv] - bm_cellwide + BMN_RFIELD + shift;
	    ymax = y_pos[ic][il][iv] + bm_cellwide - BMN_RFIELD -shift;
	  }else{
	    xmin = x_pos[ic][il][iv] - bm_cellwide + BMN_RFIELD + shift;
	    xmax = x_pos[ic][il][iv] + bm_cellwide - BMN_RFIELD - shift;
	    ymin = - BMWIDTHNEW/2. + shift;
	    ymax = + BMWIDTHNEW/2. - shift;
	  }
	  zmin = z_pos[ic][il][iv] - bm_step + BMN_RFIELD + shift;
	  zmax = z_pos[ic][il][iv] + bm_step - BMN_RFIELD -shift;
	  outstr << "RPP BmnC" << iv << cella << "   "
		 << xmin << " " << xmax << " " << ymin << " " << ymax
		 << " " << zmin << " " << zmax << endl;
	}
	cella++;
      }
    }
  }
  
  // Wires  
  for (int il=0;il<NLAYERNEW;il++){
    for (int iw =0; iw<NWIRELAYERNEW;iw++){ 
      for (int iv =0; iv<2;iv++){
	// int iv=1;
	if ( (iw==bm_idsense[0]) ||(iw==bm_idsense[1]) ||
	     (iw==bm_idsense[2]) ){	
	  iSense[iv]++;	
	  outstr << "RCC BmnS" << iv << iSense[iv] << "   "
		 << x_pos[iw][il][iv] << " " << y_pos[iw][il][iv] << " "
		 << z_pos[iw][il][iv] << " "
		 << cx_pos[iw][il][iv] << " " << cy_pos[iw][il][iv] << " "
		 << cz_pos[iw][il][iv] << " " << BMN_RSENSE << endl;
	} else {
	  iField[iv]++;     // loop on views    		
	  outstr << "RCC BmnF" << iv << iField[iv] << "   "
		 << x_pos[iw][il][iv] << " " << y_pos[iw][il][iv] << " "
		 << z_pos[iw][il][iv] << " "
		 << cx_pos[iw][il][iv] << " " << cy_pos[iw][il][iv] << " "
		 << cz_pos[iw][il][iv] << " " << BMN_RFIELD << endl;
	}
      }
    }
  }
  
  return outstr.str();
}


//_____________________________________________________________________________
string TABMparGeo::PrintRegions(){

  if ( !GlobalPar::GetPar()->geoFLUKA() ) 
    cout << "ERROR << TABMparGeo::PrintRegions()  -->  Calling this function without enabling the corrct parameter in the param file.\n", exit(0);

  stringstream outstr;
  outstr << "* ***Beam Monitor" << endl;

  int iCell[2]={-1,-1}, iSense[2]={-1,-1}, iField[2]={-1,-1};
  char stringa[100];
  
  outstr << "BMN_SHI      5 BmnShiOu -BmnShiIn" << endl;
  outstr << "BMN_MYL0     5 BmnShiIn -BmnMyl0 +BmnMyl1" << endl;
  outstr << "BMN_MYL1     5 BmnShiIn -BmnMyl2 +BmnMyl3" << endl;

  for (int iv =0; iv<2;iv++){      // loop on views
    for (int il=0;il<NLAYERNEW;il++){ // loop on layers
      for (int ic =0; ic<NSENSENEW;ic++){  // loop on cells
  iCell[iv]++;
  outstr << "BMN_C" << iv << setw(2) << setfill('0') << iCell[iv]
     << "     5 BmnC" << iv << iCell[iv]
     << " -BmnS" << iv << iCell[iv] << endl;
      }
    }
  }
  
  outstr << "BMN_FWI      5";
  int count=0;
  for (int iv = 0; iv<2; iv++){
    for (int il=0;il<NLAYERNEW;il++){ // loop on layers
      for (int jj = 0; jj < (NWIRELAYERNEW-NSENSENEW); jj++) {
	if ((( count  % 4 ) == 0) && count>0 )
	  outstr << "\n              ";
	outstr << " | " << "BmnShiIn + BmnF" << iv  << (NWIRELAYERNEW-NSENSENEW)*il+jj;
	count++;
      }
    }
  }
  outstr << endl;
  
  outstr << "BMN_SWI      5";
  count =0;
  for (int iv =0; iv<2;iv++){      // loop on views
    for (int il=0;il<NLAYERNEW;il++){ // loop on layers
      for (int ic =0; ic<NSENSENEW;ic++){  // loop on cells
  if ((( count  % 4 ) == 0) && count>0 )
    outstr << "\n              ";
  outstr << " | " << "BmnC" << iv << NSENSENEW*il+ic << " + BmnS" << iv  << NSENSENEW*il+ic;
  count++;
      }
    }
  }

  outstr << endl;
  outstr << "BMN_GAS      5 BmnShiIn -BmnMyl1 +BmnMyl2";
  count =0;
  for (int iv =0; iv<2;iv++){      // loop on views
    for (int il=0;il<NLAYERNEW;il++){ // loop on layers
      for (int ic =0; ic<(NWIRELAYERNEW-NSENSENEW);ic++){  // loop on field wires
  if ((( count  % 6 ) == 0) )
    outstr << "\n              ";
  outstr << " -BmnF" << iv << (NWIRELAYERNEW-NSENSENEW)*il+ic;
  count++;
      }
      for (int ic =0; ic<NSENSENEW;ic++){  // loop on cells
  if ((( count  % 6 ) == 0) )
    outstr << "\n              ";
  outstr << " -BmnC" << iv << NSENSENEW*il+ic;
  count++;
      }
    }
  }
  
  outstr << endl; 

  return outstr.str();
  
  /*
  // loop in order of the material alfabeth
  for ( map<string, vector<string> >::iterator itMat = m_regionPrintOut.begin(); itMat != m_regionPrintOut.end(); itMat++ ) {
  // loop over all body of the same material
  for ( vector<string>::iterator itRegion = (*itMat).second.begin(); itRegion != (*itMat).second.end(); itRegion++ ) {
  // geofile << (*itRegion);
  cout << (*itRegion);
  if (m_debug > 0)    cout << (*itRegion);
  }        
  }
  */
}





//_____________________________________________________________________________
string TABMparGeo::PrintAssignMaterial() {

    if ( !GlobalPar::GetPar()->geoFLUKA() ) 
        cout << "ERROR << TABMparGeo::PrintAssignMaterial()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);


    // loop in order of the material alfabeth
    stringstream outstr; 
    // for ( map<string, vector<string> >::iterator itMat = m_regionName.begin(); itMat != m_regionName.end(); itMat++ ) {

    //     // check dimension greater than 0
    //     if ( (*itMat).second.size() == 0 ) {
    //         cout << "ERROR << TAITparGeo::PrintAssignMaterial  ::  "<<endl, exit(0);
    //     }

    //     // take the first region
    //     string firstReg = (*itMat).second.at(0);
    //     // take the last region
    //     string lastReg = "";
    //     if ( (*itMat).second.size() != 1 ) 
    //         lastReg = (*itMat).second.at( (*itMat).second.size()-1 );

    //     // build output string 
    //     outstr  << setw(10) << setfill( ' ' ) << std::left << "ASSIGNMA" 
    //             << setw(10) << setfill( ' ' ) << std::right << (*itMat).first 
    //             << setw(10) << setfill( ' ' ) << std::right << firstReg 
    //             << setw(10) << setfill( ' ' ) << std::right << lastReg;
                       
        
    //     // multiple region condition 
    //     if ( (*itMat).second.size() != 1 ) {
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << 1 ;
    //     }
    //     else {
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << " ";
    //     }


    //     // region in the magnetic filed condition
    //     bool isMag = true;
    //     for (int i=0; i<(*itMat).second.size(); i++) {
    //         if ( m_magneticRegion[ (*itMat).second.at(i) ] == 0 ) {
    //             isMag = false;
    //             break;
    //         }
    //     }
    //     if ( isMag )
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << 1 ;
    //     else 
    //         outstr << setw(10) << setfill( ' ' ) << std::right  << " " ;
        
    //     outstr << endl;

    //     // DEBUG
    //     if (m_debug > 0)    cout << outstr.str();

    // }



    outstr << "ASSIGNMA    ALUMINUM   BMN_SHI" << endl;
    outstr << "ASSIGNMA       Mylar  BMN_MYL0" << endl;
    outstr << "ASSIGNMA       Mylar  BMN_MYL1" << endl;
    outstr << "ASSIGNMA      Ar-CO2  BMN_C000  BMN_C017         1" << endl;
    outstr << "ASSIGNMA      Ar-CO2  BMN_C100  BMN_C117         1" << endl;
    outstr << "ASSIGNMA      Ar-CO2   BMN_GAS" << endl;
    outstr << "ASSIGNMA    ALUMINUM   BMN_FWI" << endl;
    outstr << "ASSIGNMA    TUNGSTEN   BMN_SWI" << endl;


    return outstr.str();
}





//_____________________________________________________________________________
string TABMparGeo::PrintParameters() {
  
  stringstream outstr;
  outstr << setiosflags(ios::fixed) << setprecision(5);


  outstr << "c     BEAM MONITOR PARAMETERS " << endl;
  outstr << endl;    
  
  map<string, int> intp;
  intp["nlayBMN"] = NLAYERNEW;
  intp["ncellBMN"] = NSENSENEW;
  for (auto i : intp){
    outstr << "      integer " << i.first << endl;
    outstr << "      parameter (" << i.first << " = " << i.second << ")" << endl;
  }
  outstr << endl;    
  
  return outstr.str();

}



















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
#include "foot_geo.h"
#include "TAGgeoTrafo.hxx" 
#include "TABMparGeo.hxx"
#include "GlobalPar.hxx"

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
   
  m_debug = GlobalPar::GetPar()->Debug();

}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMparGeo::~TABMparGeo()
{
delete m_rotation;  
}

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

Bool_t TABMparGeo::GetBMNlvc(const Int_t cellid, Int_t& ilay, Int_t& iview, Int_t& icell){
  if(cellid>35 || cellid<0){
    cout<<"ERROR in TABMparGeo::GetBMNcell, cellid is wrong: cellid="<<cellid<<endl;
    return kFALSE;
  }
  icell=cellid%3;
  iview=(((int)(cellid/3))%2==0) ? 1:-1; 
  ilay=(int)(cellid/6);
  return kTRUE;
}

void TABMparGeo::InitGeo(){
  if ( GlobalPar::GetPar()->Debug() > 0 )     
    cout << "\n\nTABMparGeo::InitGeo" << endl<< endl;

  m_origin=TVector3(0., 0., 0.);
  m_center = TVector3( BMN_X, BMN_Y, BMN_Z );
  m_rotation = new TRotation();  //è davvero necessario???
  //WARNING!!!!! other detector use YEulerAngles!!! chiedere a Matteo quale usare di default
  //WARNING!!!!! rotations aren't implemented in FLUKA geometry yet
  m_rotation->SetXEulerAngles(0.,0.,0.);//XEulerAngles to adopt the Goldstein convention
  bm_mylar1=TVector3(0.,0.,(BMN_LENGTH+BMN_MYL_THICK)/2.);
  bm_mylar2=TVector3(0.,0.,-(BMN_LENGTH+BMN_MYL_THICK)/2.);
  bm_target=TVector3(TG_X,TG_Y,-BMN_Z+TG_Z+TG_THICK/2.);
  bm_DeltaDch=TVector3(BMN_DELTAX,BMN_DELTAY,BMN_DELTAZ);
  bm_SideDch=TVector3(BMN_WIDTH,BMN_HEIGHT,BMN_LENGTH);
  bm_idsense[0]=8;
  bm_idsense[1]=10;
  bm_idsense[2]=12;


  double aa[BMN_NWIRELAY], bb[BMN_NWIRELAY];
   

  for(int nn=0;nn<7;nn++) {
    bb[nn] = nn*BMN_PASSO;
    bb[nn+7] = nn*BMN_PASSO;
    bb[nn+14] = nn*BMN_PASSO;

    aa[nn] = 0. ;
    aa[nn+7] = BMN_STEP;
    aa[nn+14] = 2*BMN_STEP;
  }

  /* 
     generazione parametri volumi di FLUKA :  ogni filo e' definito 
     da un cilindro: 2 basi nel mylar e direzione
     parallela all'asse x ( vista U) e y (vista V) definito da:
      
     1) vettore posizione del punto centrale della base 
     2) vettore che da una base arriva all'altra base
     3) raggio
  */

  for(int il=0; il<BMN_NLAY;il++){
    for (int kk =0; kk<BMN_NWIRELAY;kk++){       
      /*  U wires -> along x, SIDE view */
      x_pos[kk][il][0] = - bm_SideDch[0]/2;
      z_pos[kk][il][0] = - bm_SideDch[2]/2 + bm_DeltaDch[2] +	aa[kk] + (2*il)*(2*BMN_STEP + BMN_LAYDIST);
      if( (il==0) || (il==2) || (il==4) ){
        y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] + bb[kk] + BMN_PASSO;
      }
      else{
        y_pos[kk][il][0] = - bm_SideDch[1]/2 + bm_DeltaDch[1] + bb[kk];
      }
      cx_pos[kk][il][0] = bm_SideDch[0];
      cy_pos[kk][il][0] = 0.;
      cz_pos[kk][il][0] = 0.;

      /* V wires -> along y, TOP view*/
      y_pos[kk][il][1] = - bm_SideDch[1]/2;
      z_pos[kk][il][1] = - bm_SideDch[2]/2 + bm_DeltaDch[2] + aa[kk] + (2*il+1)*(2*BMN_STEP + BMN_LAYDIST);
      if( (il==0) || (il==2) || (il==4) ){
        x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk];
      }
      else{
        x_pos[kk][il][1] = - bm_SideDch[0]/2 + bm_DeltaDch[0] + bb[kk] + BMN_PASSO;
      }
      cx_pos[kk][il][1] = 0.;
      cy_pos[kk][il][1] = bm_SideDch[1];
      cz_pos[kk][il][1] = 0.;

    }
  }
  
  //Create root geometry:
  //create the universe volume
  if (GlobalPar::GetPar()->geoROOT()) {
      m_universe = gGeoManager->MakeBox("BMNuniverse",gGeoManager->GetMedium(BMN_GAS_MEDIUM),(BMN_WIDTH+BMN_SHI_THICK)/2.,(BMN_HEIGHT+BMN_SHI_THICK)/2.,BMN_SHI_LENGTH/2.); //top è scatola che conterrà tutto (dimensioni in cm)
      gGeoManager->SetTopVisible(1);
  }
  if(m_debug) cout<<"Build BM ROOT geometry"<<endl;   
  //create BM's wire volume
  TGeoVolume *c_x_wire = gGeoManager->MakeTubs("c_x_wire", gGeoManager->GetMedium(BMN_FWIRE_MEDIUM), 0, BMN_RFIELD,BMN_HEIGHT/2.,0.,0.); //cathod along x 
  c_x_wire->SetLineColor(kYellow);
  TGeoVolume *a_x_wire = gGeoManager->MakeTubs("a_x_wire", gGeoManager->GetMedium(BMN_SWIRE_MEDIUM), 0, BMN_RSENSE, BMN_HEIGHT/2.,0.,0.); //anod along x
  a_x_wire->SetLineColor(kBlue);
  TGeoVolume *mylar = gGeoManager->MakeBox("mylar",gGeoManager->GetMedium(BMN_MYL_MEDIUM),BMN_WIDTH/2.,BMN_HEIGHT/2., BMN_MYL_THICK/2.);
  mylar->SetLineColor(kMagenta);
  TGeoVolume *shield_x = gGeoManager->MakeBox("shield_x",gGeoManager->GetMedium(BMN_SHI_MEDIUM),BMN_WIDTH/2.,BMN_SHI_THICK/2., BMN_SHI_LENGTH/2.);
  shield_x->SetLineColor(kGray);
  TGeoVolume *shield_y = gGeoManager->MakeBox("shield_y",gGeoManager->GetMedium(BMN_SHI_MEDIUM),BMN_SHI_THICK/2.,BMN_HEIGHT/2.+BMN_SHI_THICK, BMN_SHI_LENGTH/2.);//shield_y is longer than shield_x
  shield_y->SetLineColor(kGray);


  Int_t c=0;    
  if (GlobalPar::GetPar()->geoROOT()) {  
    for(Int_t il=0; il<BMN_NLAY;il++){
      for (Int_t kk =0; kk<BMN_NWIRELAY;kk++){   
        if(kk==bm_idsense[0] || kk==bm_idsense[1] || kk==bm_idsense[2]){//sense wire
          m_universe->AddNode(a_x_wire, c++ ,new TGeoCombiTrans(0.,y_pos[kk][il][0],z_pos[kk][il][0], new TGeoRotation("yrot",90.,90.,0.)));  
          m_universe->AddNode(a_x_wire, c++ ,new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("xrot",0.,90.,0.)));  
          //~ m_universe->AddNode(a_y_wire, c++ ,new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("a_y_wire,",0.,90.,0.)));  
        }else{//cathod wires
          m_universe->AddNode(c_x_wire, c++ ,new TGeoCombiTrans(0.,y_pos[kk][il][0],z_pos[kk][il][0], new TGeoRotation("yrot",90.,90.,0.)));
          m_universe->AddNode(c_x_wire, c++ ,new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("xrot",0.,90.,0.)));
          //~ m_universe->AddNode(c_y_wire, c++ ,new TGeoCombiTrans(x_pos[kk][il][1],0.,z_pos[kk][il][1], new TGeoRotation("c_y_wire,",0.,90.,0.)));  
        }
      }
    }
  }
  m_universe->AddNode(mylar, c++, new TGeoCombiTrans(0.,0.,-BMN_LENGTH/2.+ BMN_MYL_THICK/2., new TGeoRotation("null",0.,0.,0.)));
  m_universe->AddNode(mylar, c++, new TGeoCombiTrans(0.,0.,+BMN_LENGTH/2.- BMN_MYL_THICK/2., new TGeoRotation("null",0.,0.,0.)));
  m_universe->AddNode(shield_x, c++, new TGeoCombiTrans(0.,(BMN_HEIGHT+BMN_SHI_THICK)/2.,0., new TGeoRotation("null",0.,0.,0.)));
  m_universe->AddNode(shield_x, c++, new TGeoCombiTrans(0.,-(BMN_HEIGHT+BMN_SHI_THICK)/2.,0., new TGeoRotation("null",0.,0.,0.)));
  m_universe->AddNode(shield_y, c++, new TGeoCombiTrans((BMN_WIDTH+BMN_SHI_THICK)/2.,0.,0., new TGeoRotation("null",0.,0.,0.)));
  m_universe->AddNode(shield_y, c++, new TGeoCombiTrans(-(BMN_WIDTH+BMN_SHI_THICK)/2.,0.,0., new TGeoRotation("null",0.,0.,0.)));

  return;
}

int TABMparGeo::ShiftBmon(bool global2local){

  TVector3 cen = GetCenter();
  if(global2local)
    cen=-cen;

  for(int il=0; il<BMN_NLAY;il++){
    for (int kk =0; kk<BMN_NWIRELAY;kk++){

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
  
  double phi = m_rotation->GetXPhi();
  double theta = m_rotation->GetXPsi();
  double psi = m_rotation->GetXTheta();

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
  for(int il=0; il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NWIRELAY;iw++){       
      
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
  for(int il=0; il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NWIRELAY;iw++){       
      
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
  //~ TGeoVolume* top = gGeoManager->MakeBox("top", gas_med, BMN_HEIGHT/2., BMN_WIDTH/2., BMN_LENGTH/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.

  //qua provo a togliere box di area_gas e mettere solo top al suo posto...
  //~ bm_geo->SetTopVolume(top); //setta il mio mondo a top
  //~ bm_geo->SetTopVisible(0);  //per vedere confini del mondo metterci 1 invece che 0   
  gGeoManager->SetTopVolume(top);
  gGeoManager->SetTopVisible(0);
  //~ TGeoVolume* area_gas = bm_geo->MakeBox("gas_vol", gas_med, BMN_HEIGHT/2., BMN_WIDTH/2., BMN_LENGTH/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.
  TGeoVolume* area_gas = gGeoManager->MakeBox("gas_vol", gas_med, BMN_HEIGHT/2., BMN_WIDTH/2., BMN_LENGTH/2.); //scatola di gas poi si dovrà mettere le rotazioni e le traslazioni, per ora centrato nell'origine.
  //area_gas->SetLineColor(8);
   
   
   
  //create wires  
  //TGeoVolume* x_cell = bm_geo->MakeBox("x_cell", gas_med, BMN_HEIGHT/2., BMN_PASSO, BMN_STEP); //cella di gas forse ne faccio a meno
  //~ TGeoVolume* x_cell = gGeoManager->MakeBox("x_cell", gas_med, BMN_HEIGHT/2., BMN_PASSO, BMN_STEP); //cella di gas forse ne faccio a meno
  //TGeoVolume *c_x_wire = bm_geo->MakeTubs("c_x_wire", c_wire_med, 0, 0.0045, BMN_HEIGHT/2.,0.,0.); //filo catodo lungo z 
  TGeoVolume *c_x_wire = gGeoManager->MakeTubs("c_x_wire", c_wire_med, 0, 0.0045, BMN_HEIGHT/2.,0.,0.); //filo catodo lungo z 
  c_x_wire->SetLineColor(kBlue);
  TGeoVolume *c_y_wire = gGeoManager->MakeTubs("c_y_wire", c_wire_med, 0, 0.0045, BMN_HEIGHT/2.,0.,0.); 
  //TGeoVolume *c_y_wire = bm_geo->MakeTubs("c_y_wire", c_wire_med, 0, 0.0045, BMN_HEIGHT/2.,0.,0.); 
  c_y_wire->SetLineColor(kRed);
  TGeoVolume *a_x_wire = gGeoManager->MakeTubs("a_x_wire", a_wire_med, 0, 0.0015, BMN_HEIGHT/2.,0.,0.); //BISOGNERÀ POI SETTARE MEGLIO IL TUTTO
  //TGeoVolume *a_x_wire = bm_geo->MakeTubs("a_x_wire", a_wire_med, 0, 0.0015, BMN_HEIGHT/2.,0.,0.); //BISOGNERÀ POI SETTARE MEGLIO IL TUTTO
  a_x_wire->SetLineColor(kYellow);
  TGeoVolume *a_y_wire = gGeoManager->MakeTubs("a_y_wire", a_wire_med, 0, 0.0015, BMN_HEIGHT/2.,0.,0.);  
  //TGeoVolume *a_y_wire = bm_geo->MakeTubs("a_y_wire", a_wire_med, 0, 0.0015, BMN_HEIGHT/2.,0.,0.);  
  a_y_wire->SetLineColor(kGreen);   

  //set wires
  Int_t c=0;

  for(Int_t il=0; il<BMN_NLAY;il++){
    for (Int_t kk =0; kk<BMN_NWIRELAY;kk++){   
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
  TFile *outfile = TFile::Open("LocalBM.root","RECREATE");
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
   
  TGeoVolume* tube = gGeoManager->MakeBox(bmName, medBM, TAGgeoTrafo::CmToMu()*BMN_WIDTH/2., TAGgeoTrafo::CmToMu()*BMN_HEIGHT/2., TAGgeoTrafo::CmToMu()*BMN_LENGTH/2.);
  tube->SetVisibility(true);
  tube->SetLineColor(17);
  tube->SetTransparency(TAGgeoTrafo::GetDefaultTransp());
   
  return tube;
}

//_____________________________________________________________________________
TEveGeoShapeExtract* TABMparGeo::AddExtractBM(const char *bmName )
{
  TGeoBBox* tube = new TGeoBBox(bmName, TAGgeoTrafo::CmToMu()*BMN_WIDTH/2., TAGgeoTrafo::CmToMu()*BMN_HEIGHT/2., 
				TAGgeoTrafo::CmToMu()*BMN_LENGTH/2.);
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
  
  double shift = 0.00001;//to avoid overlap of bodies: the wires(RCC) are outside the cells(RPP)
  
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
  for (int il=0;il<BMN_NLAY;il++){ // loop on layers
    for (int ic =0; ic<BMN_NWIRELAY;ic++){  // loop on cells
      if ( (ic==bm_idsense[0]) ||(ic==bm_idsense[1]) || (ic==bm_idsense[2]) ){
        for (int iv =0; iv<2;iv++){      // loop on views
          if ( iv == 0 ){
            xmin = - BMN_WIDTH/2. + shift + BMN_X;
            xmax = + BMN_WIDTH/2. - shift + BMN_X;
            ymin = y_pos[ic][il][iv] - BMN_PASSO + BMN_RFIELD + shift + BMN_Y;
            ymax = y_pos[ic][il][iv] + BMN_PASSO - BMN_RFIELD -shift + BMN_Y;
          }else{
            xmin = x_pos[ic][il][iv] - BMN_PASSO + BMN_RFIELD + shift + BMN_X;
            xmax = x_pos[ic][il][iv] + BMN_PASSO - BMN_RFIELD - shift + BMN_X;
            ymin = - BMN_WIDTH/2. + shift + BMN_Y;
            ymax = + BMN_WIDTH/2. - shift + BMN_Y;
          }
          zmin = z_pos[ic][il][iv] - BMN_STEP + BMN_RFIELD + shift + BMN_Z;
          zmax = z_pos[ic][il][iv] + BMN_STEP - BMN_RFIELD -shift + BMN_Z;
          outstr << "RPP BmnC" << iv << cella << "   "
           << xmin << " " << xmax << " " << ymin << " " << ymax
           << " " << zmin << " " << zmax << endl;
        }
        cella++;
      }
    }
  }
  
  // Wires  
  for (int il=0;il<BMN_NLAY;il++){
    for (int iw =0; iw<BMN_NWIRELAY;iw++){ 
      for (int iv =0; iv<2;iv++){
        // int iv=1;
        if ( (iw==bm_idsense[0]) ||(iw==bm_idsense[1]) || (iw==bm_idsense[2]) ){	
          iSense[iv]++;	
          outstr << "RCC BmnS" << iv << iSense[iv] << "   "
           << x_pos[iw][il][iv] + BMN_X << " " << y_pos[iw][il][iv] + BMN_Y << " "
           << z_pos[iw][il][iv]  + BMN_Z<< " "
           << cx_pos[iw][il][iv] << " " << cy_pos[iw][il][iv] << " "
           << cz_pos[iw][il][iv] << " " << BMN_RSENSE << endl;
        } else {
          iField[iv]++;     // loop on views    		
          outstr << "RCC BmnF" << iv << iField[iv] << "   "
           << x_pos[iw][il][iv]  + BMN_X << " " << y_pos[iw][il][iv] + BMN_Y << " "
           << z_pos[iw][il][iv] + BMN_Z << " "
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
    for (int il=0;il<BMN_NLAY;il++){ // loop on layers
      for (int ic =0; ic<BMN_NSENSELAY;ic++){  // loop on cells
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
    for (int il=0;il<BMN_NLAY;il++){ // loop on layers
      for (int jj = 0; jj < (BMN_NWIRELAY-BMN_NSENSELAY); jj++) {
	if ((( count  % 4 ) == 0) && count>0 )
	  outstr << "\n              ";
	outstr << " | " << "BmnShiIn + BmnF" << iv  << (BMN_NWIRELAY-BMN_NSENSELAY)*il+jj;
	count++;
      }
    }
  }
  outstr << endl;
  
  outstr << "BMN_SWI      5";
  count =0;
  for (int iv =0; iv<2;iv++){      // loop on views
    for (int il=0;il<BMN_NLAY;il++){ // loop on layers
      for (int ic =0; ic<BMN_NSENSELAY;ic++){  // loop on cells
  if ((( count  % 4 ) == 0) && count>0 )
    outstr << "\n              ";
  outstr << " | " << "BmnC" << iv << BMN_NSENSELAY*il+ic << " + BmnS" << iv  << BMN_NSENSELAY*il+ic;
  count++;
      }
    }
  }

  outstr << endl;
  outstr << "BMN_GAS      5 BmnShiIn -BmnMyl1 +BmnMyl2";
  count =0;
  for (int iv =0; iv<2;iv++){      // loop on views
    for (int il=0;il<BMN_NLAY;il++){ // loop on layers
      for (int ic =0; ic<(BMN_NWIRELAY-BMN_NSENSELAY);ic++){  // loop on field wires
  if ((( count  % 6 ) == 0) )
    outstr << "\n              ";
  outstr << " -BmnF" << iv << (BMN_NWIRELAY-BMN_NSENSELAY)*il+ic;
  count++;
      }
      for (int ic =0; ic<BMN_NSENSELAY;ic++){  // loop on cells
  if ((( count  % 6 ) == 0) )
    outstr << "\n              ";
  outstr << " -BmnC" << iv << BMN_NSENSELAY*il+ic;
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
  intp["nlayBMN"] = BMN_NLAY;
  intp["ncellBMN"] = BMN_NSENSELAY;
  for (auto i : intp){
    outstr << "      integer " << i.first << endl;
    outstr << "      parameter (" << i.first << " = " << i.second << ")" << endl;
  }
  outstr << endl;    
  
  return outstr.str();

}


TGeoVolume* TABMparGeo::GetVolume() {

    if ( !GlobalPar::GetPar()->geoROOT() ) 
        cout << "ERROR << TABMparGeo::GetVolume()  -->  Calling this function without enabling the correct parameter in the param file.\n", exit(0);

    return m_universe;
}

















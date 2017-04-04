/*!
  \file
  \version $Id: adapted from TABMntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TADCntuRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
#include <algorithm>

#include "TString.h"

#include "TADCntuRaw.hxx"

using namespace std;

/*!
  \class TADCntuRaw TADCntuRaw.hxx "TADCntuRaw.hxx"
  \brief Mapping and Geometry parameters for Tof wall. **
*/

ClassImp(TADCntuHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TADCntuHit::~TADCntuHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCntuHit::TADCntuHit()
  : iddc(0),    iview(0),    ilayer(0),    icell(0), itrkass(0), ichi2(0),
    xcadc(0.),    ycadc(0.),    zcadc(0.),
    pxcadc(0.),   pycadc(0.),   pzcadc(0.),
    rdrift(0.),    tdrift(0.),    timdc(0.)
{
  sigma = 0;
  A0.SetXYZ(0,0,0);
  Wvers.SetXYZ(0,0,0);
  rho = 0;
  pca.SetXYZ(0,0,0);
}

TADCntuHit::TADCntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm) {

  iddc = id;  iview = iv;  ilayer = il;      icell = ic;  ichi2 = 0;
  xcadc = x;   ycadc = y;  zcadc = z;
  pxcadc = px;   pycadc = py;   pzcadc = pz;
  rdrift = r;   tdrift = t;    timdc = tm;
  sigma = 0;
  A0.SetXYZ(0,0,0);
  Wvers.SetXYZ(0,0,0);
  rho = 0;
  pca.SetXYZ(0,0,0);
}

void TADCntuHit::SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t tm) {
  
  iddc = id;  iview = iv;  ilayer = il;      icell = ic;  ichi2 = 0;
  xcadc = x;   ycadc = y;  zcadc = z;
  pxcadc = px;   pycadc = py;   pzcadc = pz;
  rdrift = r;   tdrift = t;    timdc = tm;

  return;
}

void TADCntuHit::SetAW(TADCparGeo* f_dcgeo) {
  // int idfilo = f_dcgeo->GetID(Cell());
  /*  vista U */
  Int_t tmp_fview = 0;
  if(View()<0) tmp_fview = 1;

  //Temporary FIX
  // if(idfilo <0 || idfilo>2) idfilo = 2;
 
  // Cell()  = sense id
  // Plane() = layer

  // cout << "TADCntuHit::SetAW >> cell  " <<Cell() << "  layer  " <<  Plane() << endl;
// f_dcgeo->Print();
  
  A0 = TVector3( f_dcgeo->GetWirePosition_FLUKA( Cell(), Plane(), View() ) );
  Wvers = TVector3( f_dcgeo->GetWireLenght_FLUKA( Cell(), Plane(), View() ) );

  
  Double_t modulo = Wvers.Mag();
  if(modulo!=0.) {
    Wvers = (1./modulo)*Wvers;
  }
  else{
    cout<<"Track::AddWire-> ERROR Wvers modulo: "<<modulo<<endl;
    return;
  }
  
  return;
}



void TADCntuHit::SetRdrift(TVector3 pos, TVector3 dir) {

  Double_t tp = 0., tf= 0.; 
  TVector3 D0, R0, Pvers;

  R0.SetXYZ(pos.X(),pos.Y(),pos.Z());//set position
  
  Pvers=dir;
  Pvers.SetMag(1.);

  // Double_t pmodulo = dir.Mag(); //sqrt(fpxp*fpxp +fpyp*fpyp +fpzp*fpzp );
  // if (pmodulo!=0.){
  //   Pvers.SetXYZ(dir.X()/pmodulo,dir.Y()/pmodulo,dir.Z()/pmodulo);//set directions
  // }else{
  //   printf("WARNING: FindPca(): momentum is 0 and thus pmodulo=0 is set to 1\n");
  //   Pvers.SetXYZ(0.,0.,1.);
  // }

  D0 = R0 - GetA0();//distance between position of reference point of current wire and current particle position

  Double_t prosca = Pvers*GetWvers() ;//scalar product of directions
  Double_t D0W = D0*GetWvers();//distance projected on wire
  Double_t D0P = D0*Pvers;//distance projected on particle direction

  if(prosca!= 1.) {//if the don't fly parallel
    tp = (D0W*prosca - D0P)/(1.-prosca*prosca);
    tf = (-D0P*prosca + D0W)/(1.-prosca*prosca);
    rdrift = sqrt( abs(D0.Mag2() + tp*tp + tf*tf + 2.*tp*D0P -2.*tf*D0W -2.*prosca*tf*tp ));
       pca = R0 + tp*Pvers;
  } else { //if they go parallel
    rdrift = sqrt(abs( D0.Mag2() - D0W*D0W)); 
    // cout << "TADCntuHit::SetRdrift :: WARNING >> particle fly // to the wire " << endl;
    cout << "TADCntuHit::SetRdrift :: ERROR >> particle fly // to the wire " << endl;
    exit(0);
  }

  // hack Frank
  // rdrift = 0.4;
  // rdrift = 1;

  return;
}



//##############################################################################

ClassImp(TADCntuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCntuRaw::TADCntuRaw() :
  nhit(0), h(0) {
}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCntuRaw::~TADCntuRaw() {
  delete h;
}


//------------------------------------------+-----------------------------------
//!Frank Add all the measurement points of the current event to the input vector.
void TADCntuRaw::FillWithMeasurementPoints( vector<AbsMeasurement*> & hit_vect, TADCparGeo* dc_geo ) {
  
    int det_type = 1;
    // int det_type = 7;
    // int det_type = 8;
    vector<string> viewTranslator = { "SIDE", "TOP" };

    cout << "TADCntuRaw::FillWithMeasurementPoints" << endl;

    for (int i = 0; i < nhit; i++) {
        TADCntuHit* p_hit = Hit(i);
        int view = ( p_hit->View() == 1 ? 0 : 1 );

        // cout << "DC test = cell:" << p_hit->Cell() <<" layer:"<< p_hit->Plane() <<" view:"<< viewTranslator[view] << endl;

        TVector3 wire_e1, wire_e2;        
        dc_geo->GetWireEndPoints( p_hit->Cell(), p_hit->Plane(), viewTranslator[view], wire_e1, wire_e2 );

        // cout << "Hit " << i << " : cell: " << p_hit->Cell() 
        //       << " plane: " << p_hit->Plane() << " view: " << viewTranslator[view] 
        //       << " end1 ";
        //       wire_e1.Print();
        //       cout << " end2 "; 
        //       wire_e2.Print();
              

        // cout << "DIST; " << p_hit->Dist() << "  " << p_hit->GetSigma() << endl;
        // // DIST; 0  0.0262182

        // det_type -> useless, never called apparently
        // p_hit->Dist()  -> drift radius, misleading name => change!!!!!
        WireMeasurementNew* hit = new WireMeasurementNew ( p_hit->Dist(), 
                    p_hit->GetSigma(), wire_e1, wire_e2, det_type, i, nullptr);

        hit_vect.push_back(hit);
    }
}


//------------------------------------------+-----------------------------------
//!Frank Add all the measurement points of the current event to the input vector.
void TADCntuRaw::FillWithMeasurementPoints_Test( vector<AbsMeasurement*> & hit_vect, TADCparGeo* dc_geo ) {

    int det_type = 1;
    // int det_type = 7;
    // int det_type = 8;
    vector<string> viewTranslator = { "SIDE", "TOP" };

    cout<< endl << "TADCntuRaw::FillWithMeasurementPoints_Test" << endl<< endl;

    int NHits = 10;
    vector<int> v_cell = { 2, 4, 2, 4, 3, 3, 3, 2, 4, 1 };
    vector<int> v_plane = { 0, 0, 1, 1, 2, 2, 3, 3, 4, 4 };


    for (int i = 0; i < NHits; i++) {
        
        int view = ( i%2 == 0 ? 0 : 1 );

        // cout << "Geo test = " << dc_geo->GetWidth() << endl;

        TVector3 wire_e1, wire_e2;        
        dc_geo->GetWireEndPoints( v_cell[i], v_plane[i], viewTranslator[view], wire_e1, wire_e2 );

        // cout << "Hit " << i << " : cell: " << v_cell[i] 
        //       << " plane: " << v_plane[i] << " view: " << viewTranslator[view] 
        //       << " end1 ";
        //       wire_e1.Print();
        //       cout << " end2 "; 
        //       wire_e2.Print();
        //       cout << endl;

        // det_type -> useless, never called apparently
        // p_hit->Dist()  -> drift radius, misleading name => change!!!!!
        WireMeasurementNew* hit = new WireMeasurementNew ( 1,
                    0.262182, wire_e1, wire_e2, det_type, i, nullptr);

        // hit->SetTrueP();

        hit_vect.push_back(hit);
    }
}



//------------------------------------------+-----------------------------------
//!Frank Add all the measurement points of the current event to the input vector.
void TADCntuRaw::FillWithMeasurementTypes( vector<genfit::eMeasurementType> & hit_vect ) {

    eMeasurementType type = WirePoint; 
    // eMeasurementType type = eMeasurementType(8); 

    // enum eMeasurementType { Pixel = 0,
    //     Spacepoint,
    //     ProlateSpacepoint,
    //     StripU,
    //     StripV,
    //     StripUV,
    // Wire,
    // WirePoint,
    // nMeasurementTypes
    //     };

    for (int i = 0; i < nhit; i++) {
        hit_vect.push_back( type );
    }
}


//------------------------------------------+-----------------------------------
//! Setup clones.

void TADCntuRaw::SetupClones()
{
  if (!h) h = new TClonesArray("TADCntuHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TADCntuRaw::Clear(Option_t*)
{
  nhit = 0;
  if (h) h->Clear();
  return;
}

/*------------------------------------------+---------------------------------*/
//! helper to format one converter value

static void print_value(ostream& os, Int_t i_val)
{
  char c_ran = (i_val & 0x1000) ? 'h' : 'l';
  os << Form("%4d %s %d", i_val & 0x0fff, "d", 4) << " " << c_ran;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TADCntuRaw::ToStream(ostream& os, Option_t* option) const
{
  os << "TADCntuRaw " << GetName()
     << Form("  nhit=%3d", nhit)
     << endl;
  
  os << "slat stat    adct    adcb    tdct    tdcb" << endl;
  for (Int_t i = 0; i < nhit; i++) {
    const TADCntuHit*  hit = Hit(i);
    os << Form("%4d", hit->Cell());
    os << "  "; print_value(os, hit->Plane());
    os << endl;
  }
  return;
}


#include "TABMntuHit.hxx"

using namespace std;

ClassImp(TABMntuHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuHit::~TABMntuHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuHit::TABMntuHit()
  : idmon(0),    iview(0),    ilayer(0),    icell(0), ichi2(999),
    xcamon(0.),    ycamon(0.),    zcamon(0.),
    pxcamon(0.),   pycamon(0.),   pzcamon(0.),
    rdrift(0.),    tdrift(0.)
{
  sigma = 0;
  A0.SetXYZ(0,0,0);
  Wvers.SetXYZ(0,0,0);
  rho = 0;
  pca.SetXYZ(0,0,0);
  realRdrift=100.;
  isSelected=false;
  isCrossTalk=false;  
}

TABMntuHit::TABMntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x, Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t s) {

  idmon = id;  
  iview = iv;  ilayer = il;   icell = ic;  
  xcamon = x;  ycamon = y;    zcamon = z;
  pxcamon = px;   pycamon = py;   pzcamon = pz;
  rdrift = r;   tdrift = t; 
  sigma = 0; ichi2 = 999;
  A0.SetXYZ(0,0,0);
  Wvers.SetXYZ(0,0,0);
  rho = 0;
  pca.SetXYZ(0,0,0);
  realRdrift=100.;
  isSelected=false;
  isCrossTalk=false;
  sigma=s;
}

//~ void TABMntuHit::SetData(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t x,    Double_t y, Double_t z, Double_t px, Double_t py, Double_t pz, Double_t r, Double_t t, Double_t s) {
  
  //~ idmon = id;  
  //~ iview = iv;  ilayer = il;  icell = ic; 
  //~ xcamon = x;   ycamon = y;  zcamon = z;
  //~ pxcamon = px;   pycamon = py;   pzcamon = pz;
  //~ rdrift = r;   tdrift = t;
  //~ ichi2 = 0;  

  //~ return;
//~ }

void TABMntuHit::SetAW(TABMparGeo* f_bmgeo) {

  int idfilo = f_bmgeo->GetID(Cell());
  /*  vista U */
  Int_t tmp_fview = (View()<0)? 1 : 0;            

 // if(idfilo <0 || idfilo>2) idfilo = 2; //yun: nosense questa cosa...

  A0.SetXYZ(f_bmgeo->GetX(idfilo,Plane(),tmp_fview),
	    f_bmgeo->GetY(idfilo,Plane(),tmp_fview),
	    f_bmgeo->GetZ(idfilo,Plane(),tmp_fview));
  Wvers.SetXYZ(f_bmgeo->GetCX(idfilo,Plane(),tmp_fview),
	       f_bmgeo->GetCY(idfilo,Plane(),tmp_fview),
	       f_bmgeo->GetCZ(idfilo,Plane(),tmp_fview));
  
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

void TABMntuHit::SmearRdrift(Int_t smear_type){
TRandom3 *rand = new TRandom3();
Double_t a, smeared;

if(smear_type==0)
  return;

if(smear_type==1){ //gaussian truncated to 1 sigma
  do{smeared=rand->Gaus(rdrift,sigma);}while(smeared<0 || smeared>0.943 || fabs(smeared-rdrift)>sigma);
  }

if(smear_type==2){ //gaussian truncated to 2 sigma
  do{smeared=rand->Gaus(rdrift,sigma);}while(smeared<0 || smeared>0.943 || fabs(smeared-rdrift)>2.*sigma);
  }

if(smear_type==3){ //gaussian truncated to 3 sigma
  do{smeared=rand->Gaus(rdrift,sigma);}while(smeared<0 || smeared>0.943 || fabs(smeared-rdrift)>3.*sigma);
  }
  
if(smear_type==4){ //flat smearing
  a=sigma*sqrt(12.)/2.;
  do{smeared=rdrift+rand->Uniform(-a,a);}while(smeared<0 || smeared>0.943);
  }  

if(smear_type==5){ //gaussian not truncated
  do{smeared=rand->Gaus(rdrift,sigma);}while(smeared<0 || smeared>0.943);
  }

delete rand;
rdrift=smeared;
return;  
} 



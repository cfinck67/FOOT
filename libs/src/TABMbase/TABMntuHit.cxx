#include "TRandom3.h"


#include "TABMntuHit.hxx"

using namespace std;

ClassImp(TABMntuHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TABMntuHit::~TABMntuHit()
{

}

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMntuHit::TABMntuHit()
: idmon(0),    iview(0),    ilayer(0),    icell(0), ichi2(999),
  rdrift(0.),    tdrift(0.)
{
  sigma = 0;
  A0.SetXYZ(0,0,0);
  Wvers.SetXYZ(0,0,0);
  rho = 0;
  pca.SetXYZ(0,0,0);
  realRdrift=100.;
  residual=100.;
  isSelected=false;
  isFake=false;  
}

TABMntuHit::TABMntuHit(Int_t id, Int_t iv, Int_t il, Int_t ic, Double_t r, Double_t t, Double_t s) {

  idmon = id;  
  iview = iv;  ilayer = il;   icell = ic;  
  rdrift = r;   tdrift = t; 
  ichi2 = 999;
  A0.SetXYZ(0,0,0);
  Wvers.SetXYZ(0,0,0);
  rho = 0;
  pca.SetXYZ(0,0,0);
  realRdrift=100.;
  residual=100.;
  isSelected=false;
  isFake=false;
  sigma=s;
}

void TABMntuHit::Clear(Option_t* /*option*/)
{
   fMCindex.Set(0);
   fMcTrackId.Set(0);
}

void TABMntuHit:: AddMcTrackId(Int_t trackId, Int_t mcId)
{
   fMCindex.Set(fMCindex.GetSize()+1);
   fMCindex[fMCindex.GetSize()-1]   = mcId;
   
   fMcTrackId.Set(fMcTrackId.GetSize()+1);
   fMcTrackId[fMcTrackId.GetSize()-1] = trackId;
}


void TABMntuHit::SetAW(TABMparGeo* f_bmgeo) {

  Int_t idfilo = f_bmgeo->GetSenseId(Cell());
  /*  vista U */
  Int_t tmp_fview = (View()<0)? 1 : 0;            

 // if(idfilo <0 || idfilo>2) idfilo = 2; //yun: nosense questa cosa...

  A0.SetXYZ(f_bmgeo->GetWireX(idfilo,Plane(),tmp_fview), f_bmgeo->GetWireY(idfilo,Plane(),tmp_fview), f_bmgeo->GetWireZ(idfilo,Plane(),tmp_fview));
  Wvers.SetXYZ(f_bmgeo->GetWireCX(idfilo,Plane(),tmp_fview), f_bmgeo->GetWireCY(idfilo,Plane(),tmp_fview), f_bmgeo->GetWireCZ(idfilo,Plane(),tmp_fview));
  
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
   Double_t smeared;
   
   if(smear_type==0)
      return;
   
   if(smear_type==1){ //gaussian truncated to 1 sigma
      do{smeared=gRandom->Gaus(rdrift,sigma);}while(fabs(smeared-rdrift)>sigma);
   }
   
   if(smear_type==2){ //gaussian truncated to 2 sigma
      do{smeared=gRandom->Gaus(rdrift,sigma);}while(fabs(smeared-rdrift)>2.*sigma);
   }
   
   if(smear_type==3){ //gaussian truncated to 3 sigma
      do{smeared=gRandom->Gaus(rdrift,sigma);}while(fabs(smeared-rdrift)>3.*sigma);
   }
   
   if(smear_type==4) //gaussian not truncated
      smeared=gRandom->Gaus(rdrift,sigma);
   
   
   if(smear_type==5) //flat smearing
      smeared=rdrift+gRandom->Uniform(-sigma*sqrt(12.)/2.,sigma*sqrt(12.)/2.);
   
   
   if (smeared<0)
      smeared=0.;
   rdrift=smeared;
   return;  
} 



/*!
  \file
  \version $Id: adapted from TABMntuTrack.cxx,v 1.9 2003/07/07 18:42:33 mueller Exp $
  \brief   Implementation of TADCntuTrack.
*/

#include "math.h"
#include "TString.h"
#include "TMath.h"

#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"
#include "TADCntuTrack.hxx"

/*!
  \class TADCntuTrackTr TADCntuTrack.hxx "TADCntuTrack.hxx"
  \brief Drift Chamber track description - track object **
*/

ClassImp(TADCntuTrackTr);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCntuTrackTr::TADCntuTrackTr()
  : nwire(0), nass(0),
    x0(0.), y0(0.), ux(0.), uy(0.)
{

  trackrho.Zero();
  z0 = 0;
  uz = 1.;
  R0.SetXYZ(x0,y0,z0);
  Pvers.SetXYZ(ux,uy,uz);
  trackpar.ResizeTo(NUMPAR);
  trackpar.Zero();
  trackpar(0) = x0;
  trackpar(1) = y0;
  trackpar(2) = ux;
  trackpar(3) = uy;
  tr_chi2 = 10000;

}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCntuTrackTr::~TADCntuTrackTr()
{}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::Set(Double_t fx0, Double_t fy0, Double_t fux, 
		 Double_t fuy, Double_t fuz)
{
  Double_t pmodulo = sqrt( fux*fux + fuz*fuz + fuy*fuy);
  ux = fux/pmodulo;
  uy = fuy/pmodulo;
  uz = fuz/pmodulo;
  Pvers.SetXYZ(ux,uy,uz);

  x0 = fx0;
  y0 = fy0;
  z0 = 0;
  R0.SetXYZ(x0,y0,z0);
  trackpar(0) = x0;
  trackpar(1) = y0;
  trackpar(2) = ux;
  trackpar(3) = uy;
  return 1 ;
}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::Set(Double_t fx0, Double_t fy0, Double_t fux, Double_t fuy)
{
  ux = fux;
  uy = fuy;
  Double_t transv = ux*ux + uy*uy;
  if(transv<1){
    uz = sqrt(1-transv);
  }
  else{
    Error("Action()","TADCntuTrackTr::Set-> ERROR: pvers>1  px*px+py*py= %lf %lf %lf ",ux,uy,transv);
    return -1;
  }
  Pvers.SetXYZ(ux,uy,uz);

  x0 = fx0;
  y0 = fy0;
  z0 = 0;
  R0.SetXYZ(x0,y0,z0);

  trackpar(0) = x0;
  trackpar(1) = y0;
  trackpar(2) = ux;
  trackpar(3) = uy;

  return 1 ;
}


/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::Set(TVectorD ftrackpar)
{
  ux = ftrackpar(2);
  uy = ftrackpar(3);
  Double_t transv = ux*ux + uy*uy;
  if(transv<1){
    uz = sqrt(1-transv);
  }
  else{
    Error("Action()","TADCntuTrackTr::Set(vec)-> ERROR: pvers>1  px*px+py*py= %lf %lf %lf ",ux,uy,transv);

    return -1;
  }
  Pvers.SetXYZ(ux,uy,uz);

  x0 = ftrackpar(0);
  y0 = ftrackpar(1);
  z0 = 0;
  R0.SetXYZ(x0,y0,z0);
  trackpar = ftrackpar;
  return 1 ;
}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::SetR0(Double_t fx0, Double_t fy0)
{
  x0 = fx0;
  y0 = fy0;
  z0 = 0;
  R0.SetXYZ(x0,y0,z0);
  trackpar(0) = x0;
  trackpar(1) = y0;
  return 1 ;
}
/*-----------------------------------------------------------------*/
void  TADCntuTrackTr::GetPvers(Double_t &fux, Double_t &fuy) {
  
  fux = ux;
  fuy = uy;
  return;
}

Int_t TADCntuTrackTr::SetPvers(Double_t fux, Double_t fuy)
{
  ux = fux;
  uy = fuy;
  Double_t transv = fux*fux + fuy*fuy;
  if(transv<1){
    uz = sqrt(1-transv);
  }
  else{
    Error("Action()","TADCntuTrackTr::SetPvers-> ERROR: pvers>1  px*px+py*py= %lf %lf %lf ",fux,fuy,transv);

    return -1;
  }
  Pvers.SetXYZ(ux,uy,uz);
  trackpar(2) = ux;
  trackpar(3) = uy;

  return 1 ;
}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::SetPvers(Double_t fux, Double_t fuy, Double_t fuz)
{
  Double_t pmodulo = sqrt( fux*fux + fuz*fuz + fuy*fuy);
  ux = fux/pmodulo;
  uy = fuy/pmodulo;
  uz = fuz/pmodulo;
  Pvers.SetXYZ(ux,uy,uz);
  trackpar(2) = ux;
  trackpar(3) = uy;

  return 1 ;
}


void TADCntuTrackTr::Calibrate(TF1* mypol, TF1* mypol2) {


  TAGgeoTrafo *fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fGeoTrafo == 0x0)
    Error("SetGeoTrafoName", Form("No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data()));

  double tmp_ux, tmp_uy, tmp_uz;
  tmp_ux = ux;
  tmp_uy = uy;
  tmp_uz = uz;

  //Angle from the DC itself
  double angleDC_x =atan(tmp_ux/tmp_uz);
  double angleDC_y =atan(tmp_uy/tmp_uz);

  //Global angles
  TVector3 versDC (ux,uy,uz);
  TVector3 versGlbDC = fGeoTrafo->VecFromDCLocalToGlobal(versDC);

  double angleDC_x_GL = versGlbDC.X()/versGlbDC.Z();
  double angleDC_y_GL = versGlbDC.Y()/versGlbDC.Z();
  

  //Calibration curve (pol5)
  double angleCal_x_GL;
  double angleCal_y_GL;

  string fn = mypol->GetName();

  //  cout<<"Glb Angle used in calib:: "<<angleDC_x_GL<<" "<<angleDC_y_GL<<" "<<endl;

  if(!strcmp(fn.data(),"mypol")) {
    //Data
    if(angleDC_y_GL<-0.003) { 
      angleCal_y_GL = -0.0002;
    } else if(angleDC_y_GL>0.003) {
      angleCal_y_GL = 0.001;
    } else {
      angleCal_y_GL=   mypol2->Eval(angleDC_y_GL);
    }

    if(angleDC_x_GL<-0.003) { 
      angleCal_x_GL = 0.0004;
    } else if(angleDC_x_GL>0.003) {
      angleCal_x_GL = 0.0006;
    } else {
      angleCal_x_GL=   mypol->Eval(angleDC_x_GL); 
    }
  } else {
    //MC
    angleCal_y_GL=   mypol2->Eval(angleDC_y_GL);
    angleCal_x_GL=   mypol->Eval(angleDC_x_GL); 
  }

  //Getting back from Global to Local

  //Calibrated versors
  double new_ux_GL = tan(angleCal_x_GL)*tmp_uz;
  double new_uy_GL = tan(angleCal_y_GL)*tmp_uz;

  //Renormalize
  double modulo = sqrt(pow(new_ux_GL,2)+pow(new_uy_GL,2)+pow(tmp_uz,2));

  tmp_ux = new_ux_GL/modulo;
  tmp_uy = new_uy_GL/modulo;
  tmp_uz = tmp_uz/modulo;   

  //  cout<<"GLB angle:: "<<tmp_ux<<" "<<tmp_uy<<" "<<tmp_uz<<" "<<endl;

  //Global angles
  TVector3 versCalDC (tmp_ux,tmp_uy,tmp_uz);
  TVector3 versCalDC_Local = fGeoTrafo->VecFromGlobalToDCLocal(versCalDC);

  //  cout<<"Local angle:: "<<versCalDC_Local.X()<<" "<<versCalDC_Local.Y()<<" "<<versCalDC_Local.Z()<<" "<<endl;

  ux = versCalDC_Local.X();
  uy = versCalDC_Local.Y();
  uz = versCalDC_Local.Z();

  Pvers.SetXYZ(versCalDC_Local.X(),versCalDC_Local.Y(),versCalDC_Local.Z());
  trackpar(2) = versCalDC_Local.X();
  trackpar(3) = versCalDC_Local.Y();

  return;

}



TVector3 TADCntuTrackTr::PointAtLocalZ(double zloc) {
  
  TVector3 loc_poi;

  double x_cen = GetX0();
  double y_cen = GetY0();
  double pversx = GetUx();
  double pversy = GetUy();

  double ptra2 = pversx*pversx + pversy*pversy;
  if(ptra2>1.){
    ptra2 = 1.;
    cout <<"warning : wrong output from the fit"<<endl;
  }
  double pversz = sqrt(1. - ptra2);
  /* 
     use parametric equation for track : 
     tf = parameter at specified z in DC ref frame
  */
  double t_i = (zloc)/pversz;
  //  cout<<" "<<angleDC<<" "<<angleVtx_x<<" "<<facscalex<<" "<<endl;
  double xch_i = x_cen + pversx*t_i ;
  double ych_i = y_cen + pversy*t_i ;

  loc_poi.SetXYZ(xch_i,ych_i,zloc);
  
  return loc_poi;

}


/*-----------------------------------------------------------------*/

void TADCntuTrackTr::Dump() const
{ 
  cout<<endl<<"------------ Dump Track Class ---------"<<endl;
  cout<<"nwire= "<<nwire<<" x0= "<<x0<<" y0= "<<y0<<
    " z0= "<<z0<<" Pvers= "<<ux<<" "<<uy<<" "<<uz<<endl;
}

/*-----------------------------------------------------------------*/

void TADCntuTrackTr::Clean()
{
  /*  
      reset the Track values to default 
  */
  trackpar.Zero();
  trackrho.Zero();
  nwire = 0;
  nass = 0;
  x0 = 0.;
  y0 = 0.;
  z0 = 0;
  ux = 0.;
  uy = 0.;
  uz = 1.;
  R0.SetXYZ(x0,y0,z0);
  Pvers.SetXYZ(ux,uy,uz);
}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::AddHit(TADCntuHit *h, TADCparGeo *f_dcgeo) {
  
  h->SetAW(f_dcgeo);
  h->SetTrkAss(1);
  nwire ++;
  trackrho.ResizeTo(nwire);

  return 1;
}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::ComputeDataWire(TADCntuHit *wr, Int_t fwire){

  if(fwire>=nwire){return -1;}
  Double_t tp = 0., tf= 0.; 
  Double_t prosca, D0W, D0P;
  TVector3 D0;
  
  D0 = R0 - wr->GetA0();    

  prosca = Pvers*wr->GetWvers() ;

  D0W = D0*wr->GetWvers();
  D0P = D0*Pvers;
  if(prosca!= 1.) {
    tp = (D0W*prosca - D0P)/(1-prosca*prosca);
    tf = (-D0P*prosca + D0W)/(1-prosca*prosca);
    wr->SetRho(sqrt( D0.Mag2() + tp*tp + tf*tf + 2*tp*D0P -2*tf*D0W 
		     -2*prosca*tf*tp));
    wr->SetPca(R0 + tp*Pvers);
    wr->SetPcaWire(wr->GetA0() + tf*wr->GetWvers());
    trackrho(fwire) = wr->GetRho();
  }
  else{
    wr->SetRho(sqrt(D0.Mag2() - D0W*D0W));
    trackrho(fwire) = wr->GetRho();
  }

  return 1 ;

}

/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::EstimateTrackPar(TADCntuRaw *hitp, TADCparGeo *f_dcgeo)
{
  Int_t nwire_U=0, nwire_V=0;
  Int_t piano, cella, idwire;
  Double_t raggio=0.;
  Double_t Ycentro_U[MAXNWIRE_VIEW],Zcentro_U[MAXNWIRE_VIEW],R_U[MAXNWIRE_VIEW];
  Double_t Xcentro_V[MAXNWIRE_VIEW],Zcentro_V[MAXNWIRE_VIEW],R_V[MAXNWIRE_VIEW];
  double min_sum_scarto, sum_scarto, scarto, distanza, dist_max, dist_curr;
  Int_t ii_max, jj_max;
  double p_U(0), q_U(0), p_V(0), q_V(0);
  bool dbgflg = false;
  
  TGDC_STRUCT tan_max;
  int mynwire = hitp->nhit;
  for (int ifilo=0;ifilo<mynwire;ifilo++){
    TADCntuHit *wr = hitp->Hit(ifilo);
    if(!wr->TrkAss()) continue;

    piano = wr->Plane();
    cella = wr->Cell();
    raggio = wr->Dist();

    idwire = f_dcgeo->GetID(cella);
    
    if(wr->View()>0){    /*   U view */
      nwire_U++;
      Ycentro_U[nwire_U-1]= f_dcgeo->GetY(idwire,piano,0); //trackgeo.U_y[idwire][piano];
      Zcentro_U[nwire_U-1]= f_dcgeo->GetZ(idwire,piano,0); //trackgeo.U_z[idwire][piano];
      R_U[nwire_U-1]= raggio;
    }
    else{    /*   V view */
      nwire_V++;
      Xcentro_V[nwire_V-1]= f_dcgeo->GetX(idwire,piano,1); //trackgeo.V_x[idwire][piano];
      Zcentro_V[nwire_V-1]= f_dcgeo->GetZ(idwire,piano,1); //trackgeo.V_z[idwire][piano];
      R_V[nwire_V-1]= raggio;
    }
  }
  if( (nwire_U<=2) || (nwire_V<=2) ){
    return -1; /* not enough wires fired to estimate track parameters */
  }
  
  /*  estimate track line in the U view ( namely y-z plane) */

  /* start to find the 4 tangent of the 2 farer hits */
  dist_max=0; 
  ii_max=-1;
  jj_max=-1;
  for (int ii = 1; ii<nwire_U;ii++){
    for(int jj=0;jj<ii;jj++ ){
      dist_curr = sqrt( pow((Ycentro_U[ii]-Ycentro_U[jj]),2.) + 
			pow((Zcentro_U[ii]-Zcentro_U[jj]),2.0) );
      if(dbgflg)  {
	cout <<"ii,jj= "<<ii<<" "<<jj<<" yc[ii],zc[ii]= "<<Ycentro_U[ii]<<" "<<
	  Zcentro_U[ii]<<" yc[jj],zc[jj]= "<<Ycentro_U[jj]<<" "<<Zcentro_U[jj]<<endl;
      }
      if(dist_curr>dist_max) {
	dist_max = dist_curr;
	ii_max = ii;
	jj_max = jj;
	tan_max = Circles2Tangents(Ycentro_U[ii], Zcentro_U[ii], R_U[ii],
				   Ycentro_U[jj], Zcentro_U[jj], R_U[jj]);
      }
    }
  }
  if(dbgflg) { 
    cout<<"celle piu' distanti= "<<ii_max<<" "<<jj_max<<endl;
    for(int hh=0;hh<4;hh++){
      cout<<" m= "<<tan_max.mm[hh]<<" q= "<<tan_max.qq[hh]<<endl;
    }
    cout<<"scelta tangente"<<endl;
  }
  min_sum_scarto = 10000000.;
  for(int hh=0;hh<4;hh++){
    sum_scarto = 0.;
    if(dbgflg)  cout<<"tangente # "<<hh<<endl;
    for (int ii = 0; ii<nwire_U;ii++){
      if((ii!=ii_max)&&(ii!=jj_max)){
	distanza = fabs(Zcentro_U[ii]-tan_max.mm[hh]*Ycentro_U[ii] - tan_max.qq[hh])/
	  sqrt(1. + tan_max.mm[hh]*tan_max.mm[hh]);
	scarto = pow((distanza - R_U[ii]),2.);
	sum_scarto +=scarto;
	if(dbgflg) {
	  cout<<"piano= "<<ii<<" distanza="<<distanza<<" scarto= "<<scarto<<
	    " sum_scarto= "<<sum_scarto<<endl;
	}
      }
    }
    if(sum_scarto<min_sum_scarto){
      min_sum_scarto = sum_scarto;
      p_U = tan_max.mm[hh];
      q_U = tan_max.qq[hh];
    }
  }

  /*  estimate track line in the V view ( namely x-z plane) */
  //  cout<<" VISTA V"<<endl;
  
  dist_max=0;
  ii_max=-1;
  jj_max=-1;
  /* start to find the 4 tangent of the 2 farest hits */
  for (int ii = 1; ii<nwire_V;ii++){
    for(int jj=0;jj<ii;jj++ ){
      dist_curr = sqrt( pow((Xcentro_V[ii]-Xcentro_V[jj]),2.) + 
			pow((Zcentro_V[ii]-Zcentro_V[jj]),2.0) );
      if(dbgflg)  {
	cout <<"ii,jj= "<<ii<<" "<<jj<<" xc[ii],zc[ii]= "<<Xcentro_V[ii]<<" "<<
	  Zcentro_V[ii]<<" xc[jj],zc[jj]= "<<Xcentro_V[jj]<<" "<<Zcentro_V[jj]<<endl;
      }
      if(dist_curr>dist_max) {
	dist_max = dist_curr;
	ii_max = ii;
	jj_max = jj;
	tan_max = Circles2Tangents(Xcentro_V[ii], Zcentro_V[ii], R_V[ii],
				   Xcentro_V[jj], Zcentro_V[jj], R_V[jj]);
      }
    }
  }
  if(dbgflg) {
    cout<<"celle piu' distanti= "<<ii_max<<" "<<jj_max<<endl;
    for(int hh=0;hh<4;hh++){
      cout<<" m= "<<tan_max.mm[hh]<<" q= "<<tan_max.qq[hh]<<endl;
    }
    cout<<"scelta tangente"<<endl;
  }
  min_sum_scarto = 10000000.;
  for(int hh=0;hh<4;hh++){
    sum_scarto = 0.;
    if(dbgflg)   cout<<"tangente # "<<hh<<endl;
    for (int ii = 0; ii<nwire_V;ii++){
      if((ii!=ii_max)&&(ii!=jj_max)){
	distanza = fabs(Zcentro_V[ii]-tan_max.mm[hh]*Xcentro_V[ii] - tan_max.qq[hh])/
	  sqrt(1. + tan_max.mm[hh]*tan_max.mm[hh]);
	scarto = pow((distanza - R_V[ii]),2.);
	sum_scarto +=scarto;
	if(dbgflg) {
	  cout<<"piano= "<<ii<<" distanza="<<distanza<<" scarto= "<<scarto<<
	    " sum_scarto= "<<sum_scarto<<endl;
	}
      }
    }
    if(sum_scarto<min_sum_scarto){
      min_sum_scarto = sum_scarto;
      p_V = tan_max.mm[hh];
      q_V = tan_max.qq[hh];
    }
  }

  /*  calcolo dei parametri iniziali della traccia x0,y0,px0,py0*/
  double pz0(0), px0, py0, x0, y0;
  TVector3 CenterDch;
  CenterDch = f_dcgeo->GetCenter();

  if(p_U && p_V) {  pz0=1./sqrt(1.0 + 1.0/(p_U*p_U)+ 1.0/(p_V*p_V)); }
  else if(!p_U && p_V) {  pz0=1./sqrt(1.0 + 1.0/(p_V*p_V)); }
  else if(p_U && !p_V) {  pz0=1./sqrt(1.0 + 1.0/(p_U*p_U)); }
  if(p_V) {
    px0 = pz0/p_V;
    x0 = ((CenterDch[2])-q_V)/p_V;
  } else {
    px0 = 0;
    x0 = -0.4;
  }
  if(p_U) {
    py0 = pz0/p_U;
    y0 = ((CenterDch[2])-q_U)/p_U;
  } else {
    py0 = 0;
    y0 = -0.4;
  }
  if(fabs(py0)>1 || fabs(px0)>1) { py0 = px0 = 0;}

  int status = Set(x0,y0,px0,py0);
  if(status<0) {
    Error("Action()","EstimateTrackPat:: %lf %lf %lf %lf",px0,py0,p_V,p_U);
  }
  if(dbgflg){
    cout<<"vista U: p_U= "<<p_U<<" q_U= "<<q_U<<"  vista V: p_V= "<<p_V<<
      " q_V= "<<q_V<<endl;
    cout<<"px, py, pz= "<<px0<<" "<<py0<<" "<<pz0<<" x0, y0= "<<x0<<" "<<y0<<endl;
  }

  return status;
}


TGDC_STRUCT TADCntuTrackTr::Circles2Tangents(double xc1, double yc1, double r1,
			     double xc2, double yc2, double r2)
{
  /*  Given 2 circles in the x,y plane return the 4 tangents*/
  TGDC_STRUCT tan_par;
  for (int ii=0;ii<4;ii++){ /* clean up */
    tan_par.mm[ii] =0. ;
    tan_par.qq[ii] =0. ;
  }
  /*    cout<<"sono in Circles2Tangets"<<endl;
  cout<<"xc1= "<<xc1<<" yc1= "<<yc1<<" r1= "<<r1<<"  xc2= "<<xc2<<" yc1= "<<
  yc2<<" r2= "<<r2<<endl; */

  double aa,bb,cc, dete, qpiu1, qmeno1, qpiu2, qmeno2;
  cc = (r1*r1 -2*r1*r2 +r2*r2 - yc2*yc2 + 2*yc1*yc2 -yc1*yc1);
  bb = 2*(yc1-yc2)*(xc1-xc2);
  aa = (r1*r1 -2*r1*r2 +r2*r2 -xc1*xc1 +2*xc1*xc2 - xc2*xc2);
  dete = bb*bb -4*aa*cc;
  if(dete>0) {
    tan_par.mm[0] = (-bb + sqrt(dete))/2/aa;
    tan_par.mm[1] = (-bb - sqrt(dete))/2/aa;
  }
  cc = (r1*r1 +2*r1*r2 +r2*r2 - yc2*yc2 + 2*yc1*yc2 -yc1*yc1);
  bb = 2*(yc1-yc2)*(xc1-xc2);
  aa = (r1*r1 +2*r1*r2 +r2*r2 -xc1*xc1 +2*xc1*xc2 - xc2*xc2);
  dete = bb*bb -4*aa*cc;
  if(dete>0) {
    tan_par.mm[2] = (-bb + sqrt(dete))/2/aa;
    tan_par.mm[3] = (-bb - sqrt(dete))/2/aa;
  }
  for(int hh=0;hh<4;hh++){
    qpiu1 = yc1 -tan_par.mm[hh]*xc1 + r1*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    qmeno1 = yc1 -tan_par.mm[hh]*xc1 - r1*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    qpiu2 = yc2 -tan_par.mm[hh]*xc2 + r2*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);
    qmeno2 = yc2 -tan_par.mm[hh]*xc2 - r2*sqrt(1+ tan_par.mm[hh]*tan_par.mm[hh]);

    if( (fabs(qpiu1-qpiu2)<.001) || (fabs(qpiu1-qmeno2)<0.001) ){ 
      tan_par.qq[hh] = qpiu1; 
    }
    else if( (fabs(qmeno1-qmeno2)<0.001)||(fabs(qmeno1-qpiu2)<0.001)){ 
      tan_par.qq[hh] = qmeno1; 
    }
    /*     cout<<"p= "<<tan_par.mm[hh]<<" qpiu1= "<<qpiu1<<" qmeno1= "<<qmeno1<<
	   " qpiu2= "<<qpiu2<<" qmeno2= "<<qmeno2<<" q= "<<tan_par.qq[hh]<<endl;*/
  }
  
  return tan_par;
}



/*-----------------------------------------------------------------*/

Int_t TADCntuTrackTr::ComputeDataAll(TADCntuRaw *hitp){

  Double_t tp = 0., tf= 0.; 
  Double_t prosca, D0W, D0P;
  TVector3 D0;
  int usedH(0);
  int mynwire = hitp->nhit;
  for(int ii=0;ii<mynwire;ii++){

    TADCntuHit* wr = hitp->Hit(ii);
    if(!wr->TrkAss()) continue;
    D0 = R0 - wr->GetA0();    
    prosca = Pvers*wr->GetWvers() ;
    D0W = D0*wr->GetWvers();
    D0P = D0*Pvers;
    /*
    cout<<ii<<endl;
    D0.Print();
    */
    if(prosca!= 1.) {
      tp = (D0W*prosca - D0P)/(1-prosca*prosca);
      tf = (-D0P*prosca + D0W)/(1-prosca*prosca);
      wr->SetRho(sqrt( D0.Mag2() + tp*tp + tf*tf + 2*tp*D0P -2*tf*D0W 
		       -2*prosca*tf*tp));
      wr->SetPca(R0 + tp*Pvers);
      wr->SetPcaWire(wr->GetA0() + tf*wr->GetWvers());
      trackrho(usedH) = wr->GetRho();
    }
    else{
      wr->SetRho(sqrt(D0.Mag2() - D0W*D0W));
      trackrho(usedH) = wr->GetRho();
    }
    usedH++;
  }
  return 1 ;
}

void   TADCntuTrackTr::SetChi2H(TVectorD dy, TVectorD au, TADCntuRaw *hitp) {

  int mynwire = hitp->nhit;
  int idx(0);
  double tchi2(0);
  for(int ii=0;ii<mynwire;ii++){
    
    TADCntuHit* wr = hitp->Hit(ii);
    if(wr->TrkAss()) {
      tchi2 = dy(idx)*au(idx);
      idx++;
      wr->SetChi2(tchi2);
    }
  }
  return;
}

//##############################################################################

/*!
  \class TADCntuTrack TADCntuTrack.hxx "TADCntuTrack.hxx"
  \brief PC track description - container (Qad level). **
*/

ClassImp(TADCntuTrack);

//------------------------------------------+-----------------------------------
//! Default constructor.

TADCntuTrack::TADCntuTrack()
  : ntrk(0),
    t(0)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCntuTrack::~TADCntuTrack()
{
  delete t;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TADCntuTrack::SetupClones()
{
  if (!t) t = new TClonesArray("TADCntuTrackTr");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear.

void TADCntuTrack::Clear(Option_t*)
{
  TAGdata::Clear();

  ntrk   = 0;
  t->Delete();

  return;
}


/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TADCntuTrack::ToStream(ostream& os, Option_t* option) const
{
  os << "TADCntuTrack" << Form("  ntrk=%3d", ntrk) << endl;

  if (ntrk == 0) return;
  os << "ind nh "
     << "    x0     ux   y0     uy"<< endl;  
                  
  for (Int_t i = 0; i < ntrk; i++) {
    const TADCntuTrackTr* p_trk = Track(i);
    os << Form("%3d", i)
       << Form(" %2d", p_trk->nwire);
    os << Form(" %5.0f %6.3f", p_trk->x0, p_trk->ux)
       << Form(" %4.0f %6.3f", p_trk->y0, p_trk->uy);
    
    os << endl;
  }

  return;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TADCntuTrack::Chi2Fit(TADCntuRaw *hitp, TADCntuTrackTr* p_trk, int n_ite)
{

  int mynwire(0);

  Int_t nmeas, ntass;
  Double_t chi2;
  TMatrixD AA, VV;
  TVectorD alpha, Dy, Eta;

  alpha.ResizeTo(4);
  Eta.ResizeTo(4);
  
  nmeas = hitp->nhit;
  ntass = p_trk->nass;

  Dy.ResizeTo(ntass);
  alpha(0) = p_trk->GetX0();
  alpha(1) = p_trk->GetY0();
  alpha(2) = p_trk->GetUx();
  alpha(3) = p_trk->GetUy();
  AA.ResizeTo(ntass,4);
  VV.ResizeTo(ntass,ntass);

  Dy = ComputeDy(hitp,p_trk);
  VV = ComputeVV(hitp,p_trk);
  chi2 = ComputeChiQua(Dy,VV);
  p_trk->SetChi2(chi2/(double)ntass);
  //  cout<<" Before iteration "<<endl;

  //  Dy.Print(); AA.Print(); alpha.Print(); VV.Print(); Eta.Print();

  for (int hh=0; hh<n_ite;hh++){
    Info("Action()"," Iteration number %d\n",hh);
    Dy = ComputeDy(hitp, p_trk);
    AA = ComputeAA(hitp, p_trk, alpha);
    Eta = Mini(ntass, AA, VV, Dy);
    Update(Dy, alpha, Eta, VV, p_trk, hitp);
    Info("Action()","A:: %lf, %lf, %lf %lf\n",alpha(0),alpha(1),
	 alpha(2),alpha(3));

  }
  
  return;
}

/*-----------------------------------------------------------------*/

TVectorD TADCntuTrack::ComputeDy(TADCntuRaw *hitp, TADCntuTrackTr* p_trk)
{
  TVectorD Dy; 
  int nmeas = hitp->nhit;
  int nass = p_trk->nass;
  int u_fil(0);
  Dy.ResizeTo(nass);
  for (int ifilo=0;ifilo<nmeas;ifilo++){
    TADCntuHit *wr = hitp->Hit(ifilo);
    if(!wr->TrkAss()) continue;
    Dy(u_fil) =  wr->Dist() - wr->GetRho(); 
    if(wr->Dist()<0 || wr->GetRho()<0) {
      Warning("Action()","TADCntuTrack::Negative Distances!!! %lf %lf",wr->Dist(),wr->GetRho());
    }
    u_fil++;
  }
  /*
  cout<<"Printing Dy vector="<<endl;
  Dy.Print();
  */
  return Dy ;
}

TMatrixD TADCntuTrack::ComputeAA(TADCntuRaw *hitp, TADCntuTrackTr *p_trk, TVectorD alpha)
/*  numerical computation of the Df/Dalpha derivatives */
{

  double eps = 0.0001;
  TMatrixD AA;
    
  int nmeas = hitp->nhit;
  int ntass = p_trk->nass;
  AA.ResizeTo(ntass,4);
  
  TVectorD distanze_moved(ntass);
  TVectorD parametri_moved(4);
  TVectorD distanze0(ntass);
  distanze0 = p_trk->GetTrackRho();

  for(int ii = 0; ii<4;ii++){
    parametri_moved = alpha;
    if(parametri_moved(ii)!=0.){
      parametri_moved(ii) *= (1.+eps);
    }
      else{
	parametri_moved(ii) = eps;
      }
    p_trk->Set(parametri_moved);
    
    //    cout<<"print out of alpha with incremented parameter # "<<ii<<endl;
    //    parametri_moved.Print();
    
    p_trk->ComputeDataAll(hitp);
    distanze_moved = p_trk->GetTrackRho();
    for(int kk = 0;kk<ntass;kk++){
      
      AA(kk,ii) = (distanze_moved(kk)-distanze0(kk))/
	(parametri_moved(ii)-alpha(ii));
    }
  }

  p_trk->Set(alpha);
  p_trk->ComputeDataAll(hitp);

  return AA;
}

TMatrixD TADCntuTrack::ComputeVV(TADCntuRaw *hitp, TADCntuTrackTr* p_trk) {
  
  double errore=1000000.;
  TMatrixD VV;
  int nmeas = hitp->nhit;
  int ntass = p_trk->nass;
  int tmp_i(0);
  VV.ResizeTo(ntass,ntass);
  VV = 0.;
  for(int ii = 0; ii<nmeas;ii++){
    TADCntuHit *wr = hitp->Hit(ii);
    if(!wr->TrkAss()) continue;
    errore = wr->GetSigma();
    if(!errore)
      errore = 0.012; //120 mum
    VV(tmp_i,tmp_i) =  1./errore/errore;
    tmp_i++;
  }
  return VV ;

}

TVectorD TADCntuTrack::Mini(int nmeas, TMatrixD AA, TMatrixD VV, TVectorD Dy) {

  TMatrixD B, AAt, VVa;
  TVectorD Eta;
  VVa.ResizeTo(4,4);
  AAt.ResizeTo(4,nmeas);
  B.ResizeTo(4,nmeas);
  Eta.ResizeTo(4);

  AAt.Transpose(AA);
  VVa = AAt*VV*AA;
  VVa.Invert();
  B = VVa*AAt*VV;
  Eta = B*Dy;
  return Eta ;
}

/*-----------------------------------------------------------------*/
void TADCntuTrack::Update(TVectorD Dy, TVectorD& alpha, TVectorD Eta, TMatrixD VV, TADCntuTrackTr * ptraccia, TADCntuRaw *hitp) {

  Int_t status=0;
  TVectorD Aus; double chiqua;
  Int_t rows = Dy.GetNrows();
  Aus.ResizeTo(rows);

  alpha = Eta + alpha;
  if(fabs(alpha(2))>1 || TMath::IsNaN(alpha(2))) alpha(2) = 0;
  if(fabs(alpha(3))>1 || TMath::IsNaN(alpha(3))) alpha(3) = 0;
  status = ptraccia->Set(alpha(0),alpha(1),alpha(2),alpha(3));
  status = ptraccia->ComputeDataAll(hitp);
  ComputeDy(hitp, ptraccia);
  Aus = VV*Dy;
  chiqua = Dy*Aus;

  if(ptraccia->nass) {
    ptraccia->SetChi2(chiqua/(double)ptraccia->nass);
    ptraccia->SetChi2H(Dy, Aus, hitp);
  }
  return;
}

Double_t TADCntuTrack::ComputeChiQua(TVectorD Dy, TMatrixD VV) {

  TVectorD Aus; double chiqua;
  Int_t numero_rows = Dy.GetNrows();
  Aus.ResizeTo(numero_rows);
  Aus = VV*Dy;
  chiqua = Dy*Aus;

  return chiqua;
}

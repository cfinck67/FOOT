/*!
  \file
  \version $Id: TAGvieHorzFIRST.cxx,v 1.7 2003/06/22 19:35:02 mueller Exp $
  \brief   Implementation of TAGvieHorzFIRST.
*/

#include <math.h>
#include "gsi_geo.h"

#include "TROOT.h"
#include "TString.h"
#include "TAttFill.h"
#include "TAttLine.h"
#include "TAttText.h"
#include "TArc.h"

#include "TATntuRaw.hxx"

#include "TATparGeo.hxx"

#include "TABMntuTrack.hxx"
#include "TABMntuRaw.hxx"

#include "TABMparGeo.hxx"
//First
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAGvieHorzFIRST.hxx"

/*!
  \class TAGvieHorzFIRST TAGvieHorzFIRST.hxx "TAGvieHorzFIRST.hxx"
  \brief Horizontal projective view for TABMntu*CHitQad and TABMntu*CTrackQad **
*/

ClassImp(TAGvieHorzFIRST);

//------------------------------------------+-----------------------------------
//! Construct with observables.

TAGvieHorzFIRST::TAGvieHorzFIRST(TAGdataDsc* p_ntutrk,
				 TAGdataDsc* p_nturaw,
				 TAGparaDsc* p_bmgeo,
				 TAGdataDsc* p_ntutraw,
				 TAGparaDsc* p_tofgeo)
  : fpNtuTrk(p_ntutrk),
    fpNtuBMRaw(p_nturaw),
    fpParBMGeo(p_bmgeo),
    fpNtuTofRaw(p_ntutraw),
    fpParTofGeo(p_tofgeo)
{
  if (p_ntutrk)  AddObservable(p_ntutrk,  "@TABMntuTrack");
  if (p_nturaw)  AddObservable(p_nturaw,  "@TABMntuRaw");
  if (p_bmgeo)   AddObservable(p_bmgeo,   "@TABMparGeo");
  if (p_ntutraw)   AddObservable(p_ntutraw,   "@TATntuRaw");
  if (p_tofgeo)    AddObservable(p_tofgeo,    "@TATparGeo");

  SetName("TAGvieHorzFIRST");
  SetTitle("TAGvieHorzFIRST");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGvieHorzFIRST::~TAGvieHorzFIRST()
{
}


//------------------------------------------+-----------------------------------
//! Return textual representation of current cursor position

char* TAGvieHorzFIRST::GetObjectInfo(Int_t i_px, Int_t i_py) const
{
  if (!gPad) return (char*)"";
  Double_t d_x = gPad->PadtoX(gPad->AbsPixeltoX(i_px));
  Double_t d_y = gPad->PadtoY(gPad->AbsPixeltoY(i_py));

  return Form("%5.0f %5.0f", d_x, d_y);
}

//------------------------------------------+-----------------------------------
//! Handle pointer event.

void TAGvieHorzFIRST::ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py)
{
  //printf("ExecuteEvent: %4d : %4d %4d\n", i_event, i_px, i_py);
  return;
}

//------------------------------------------+-----------------------------------
//! Paint.

void TAGvieHorzFIRST::Paint(Option_t* option)
{
  TABMntuTrack* p_ntutrk  = 
    (TABMntuTrack*) fpNtuTrk->GenerateObject();
  TABMntuRaw*   p_nturaw = 
    (TABMntuRaw*)   fpNtuBMRaw->GenerateObject();
  TABMparGeo*   p_bmgeo = (TABMparGeo*) fpParBMGeo->Object();

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  TATntuRaw*        p_ntutraw = 
    (TATntuRaw*)        fpNtuTofRaw->GenerateObject();
  TATparGeo*        p_tofgeo = (TATparGeo*) fpParTofGeo->Object();

  if (IsZombie() || !gPad) return;

  TAttLine attline_black_soli_fat(1, 1, 2); // (col, sty, wid)
  TAttLine attline_black_soli_nor(1, 1, 1);
  TAttLine attline_black_dash_nor(1, 2, 1);
  TAttLine attline_black_dot_nor(1, 3, 1);

  TAttLine attline_trk_4(1, 1, 1);	    // (col,sty,wid) = bla,sol,1
  TAttLine attline_trk_3(1, 2, 1);	    // (col,sty,wid) = bla,das,1

  TAttLine attline_hit_u(2, 1, 1);	    // (col,sty,wid) = red,sol,1  use
  TAttLine attline_hit_b(3, 1, 1);	    // (col,sty,wid) = gre,sol,1  bad
  TAttLine attline_hit_m(4, 1, 1);	    // (col,sty,wid) = blu,sol,1  mis

  TAttFill attfill_box(0, 1);	    // (col,sty) = empty, solidline

  TAttFill attfill_tof(1, 1001);	    // (col,sty) = bla, solid  ???
  TAttFill attfill_tof_up(2, 1001);	    // (col,sty) = red, solid  use pro
  TAttFill attfill_tof_us(6, 1001);	    // (col,sty) = red, solid  use sea
  TAttFill attfill_tof_b(3, 1001);	    // (col,sty) = gre, solid  bad
  TAttFill attfill_tof_m(4, 1001);	    // (col,sty) = blu, solid  mis

  TAttText atttext_small_hcvb(21, 0., 1, 83, 10);
  TAttText atttext_small_hcvt(23, 0., 1, 83, 10);
  TAttText atttext_small_hlvc(12, 0., 1, 83, 10);
  TAttText atttext_small_hcvb_v(21, 90., 1, 83, 15);

  double bm_w = p_bmgeo->GetWidth();
  //double bm_h = p_bmgeo->GetHeigth();
  double bm_l = p_bmgeo->GetLength();

  TVector3  myloc, mylow, myhig;

  //Pad range. Using centimeters!
  gPad->Range(-100, -300, +800, +200);

  //
  //********          Target         ******* 
  //

  myloc.SetXYZ(-0.5*TG_WIDTH, 0, -0.5*TG_THICK);
  mylow = fpFirstGeo->FromVTLocalToGlobal(myloc);
  myloc.SetXYZ(+0.5*TG_WIDTH, 0, +0.5*TG_THICK);
  myhig = fpFirstGeo->FromVTLocalToGlobal(myloc);

  attline_black_soli_nor.Modify();  attfill_box.Modify();
  gPad->PaintBox(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());


  //
  //********            BM           ******* 
  //

  TVector3 bm_cen = p_bmgeo->GetCenter();  
  TVector3 bm_sid = p_bmgeo->GetSide();

  Double_t h_x(0), h_y(0), h_z(0), h_cx(0), h_cy(0), h_cz(0);
  Double_t cm_w, cm_h;
  cm_h = p_bmgeo->GetCellHeight();
  cm_w = p_bmgeo->GetCellWidth();

  // BM space frame boundary
  myloc.SetXYZ(-0.5*bm_w, 0, -0.5*bm_l);
  mylow = fpFirstGeo->FromBMLocalToGlobal(myloc);
  myloc.SetXYZ(+0.5*bm_w, 0, +0.5*bm_l);
  myhig = fpFirstGeo->FromBMLocalToGlobal(myloc);

  attline_black_soli_nor.Modify();  attfill_box.Modify();
  gPad->PaintBox(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

  /* Drawing the cells */
  for(int il= 0; il<p_bmgeo->GetLayersNumber(); il++) {
    for(int ic= 0; ic<p_bmgeo->GetCellsNumber(); ic++) {
      //Top view, V view, (X,Z)
      p_bmgeo->GetCellInfo(-1, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);

      //X has a consistent view (x points to the top) 
      myloc.SetXYZ(-cm_w+h_x, 0, -cm_h+h_z);
      mylow = fpFirstGeo->FromBMLocalToGlobal(myloc);
      myloc.SetXYZ(+cm_w+h_x, 0, +cm_h+h_z);
      myhig = fpFirstGeo->FromBMLocalToGlobal(myloc);
      
      attline_black_soli_nor.Modify();  attfill_box.Modify();
      gPad->PaintBox(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
    }
  }

  // BM track display
  double pversx(0),pversy(0);
  //double xcen(0), ycen(0);
  double zcen(0);
  double x_target(0), y_target(0);
  double xch_i, xch_f, ych_i, ych_f;
  double zmin, zmax, t_i, t_f;
  if (p_ntutrk) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the BM Tracks");

    Int_t i_ntrk = p_ntutrk->ntrk;
    for (Int_t i_t = 0; i_t < i_ntrk; i_t++) {
      TABMntuTrackTr* p_trk = p_ntutrk->Track(i_t);
      x_target = p_trk->GetX0();
      y_target = p_trk->GetY0();
      pversx = p_trk->GetUx();
      pversy = p_trk->GetUy();
      double ptra2 = pversx*pversx + pversy*pversy;
      if(ptra2>1.){
	ptra2 = 1.;
	cout <<"warning : wrong output from the fit"<<endl;
      }
      double pversz = sqrt(1. - ptra2);
      
      /* 
	 use parametric equation for track : 
	 ti, tf = parameter at 2 Z edges of the BM
      */
      zmin = zcen - bm_sid.Z()/2;
      //Extrapolate UP to the Target
      //      zmax = zcen + bm_sid.Z()/2;
      //Target is placed 7mm BEFORE the VT
      zmax = zcen + fpFirstGeo->GetVTCenter().Z() - fpFirstGeo->GetBMCenter().Z();
      t_i = (zmin-zcen)/pversz;
      t_f = (zmax-zcen)/pversz;
      
      xch_i = x_target + pversx*t_i ;
      xch_f = x_target + pversx*t_f ;
      ych_i = y_target + pversy*t_i ;
      ych_f = y_target + pversy*t_f ;
      
      myloc.SetXYZ(xch_i, 0, zmin);
      mylow = fpFirstGeo->FromBMLocalToGlobal(myloc);
      myloc.SetXYZ(xch_f, 0, zmax);
      myhig = fpFirstGeo->FromBMLocalToGlobal(myloc);

      attline_trk_4.Modify();
      //X,Z (view V, TOP view)   
      gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
    }
  }

  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Int_t i_nhit = p_nturaw->nhit;
    Info("Viewer()","Displaying the BM Hits %2d ",i_nhit);

    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
      TABMntuHit* hit = p_nturaw->Hit(i_h);
      if(hit->HorView()) {

	//Top view, V view, (X,Z)
	p_bmgeo->GetCellInfo(-1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
	
	//X has a consistent view (x points to the top) 
	myloc.SetXYZ(h_x-hit->Dist(), 0, h_z-hit->Dist());
	mylow = fpFirstGeo->FromBMLocalToGlobal(myloc);
	myloc.SetXYZ(h_x+hit->Dist(), 0, h_z+hit->Dist());
	myhig = fpFirstGeo->FromBMLocalToGlobal(myloc);
	
	attline_black_soli_nor.Modify();  attfill_tof_up.Modify();
	gPad->PaintBox(mylow.Z(),mylow.X(),myhig.Z(),myhig.X());
      }
    }

  }

  // ToF wall boundary (8pack wise)
  for (Int_t i_plane = 0; i_plane < 2; i_plane++) {
    for (Int_t i_pack = 0; i_pack < 12; i_pack++) {
      const TATparGeoPack& info = p_tofgeo->PackInfo(i_plane, i_pack);
      if (info.Valid()) {
	TVector3 el  = info.RefPoint();
	TVector3 er = el + 8.*info.SlatWidth()*info.HorzVector();
	attline_black_dot_nor.Modify();
	
	myloc.SetXYZ(el.Px()/10.,0,el.Pz()/10.);
	mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
	myloc.SetXYZ(er.Px()/10.,0,er.Pz()/10.);
	myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
	gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

	myloc.SetXYZ(el.Px()/10.,0,el.Pz()/10.);
	mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
	myloc.SetXYZ(el.Px()/10.,0,(el.Pz()+60.)/10.);
	myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
	gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

	myloc.SetXYZ(er.Px()/10.,0,er.Pz()/10.);
	mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
	myloc.SetXYZ(er.Px()/10.,0,(er.Pz()+60.)/10.);
	myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
	gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
      }
    }
  }

  //Displays the Hits reconstruced in the TofWall
  if (p_ntutraw) {
    Int_t i_nhit = p_ntutraw->nhit;
    //    cout<<i_nhit<<endl;
    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
      TATntuRawHit* hit = p_ntutraw->Hit(i_h);

      Int_t i_slat = hit->slat;
      if (i_slat > 200) continue;

      Float_t  f_adct = hit->adct;
      Float_t  f_adcb = hit->adcb;
      Double_t  f_adc  = sqrt(f_adct * f_adcb);
      if (f_adct == 0) f_adc = f_adcb;
      if (f_adcb == 0) f_adc = f_adct;

      Double_t f_gadc = (60./32.) * pow(f_adc, 0.3333);

      TVector3 el, er;
      p_tofgeo->GeoSlatFace(el, er, i_slat);

      Double_t d_gx1 = el.Pz();
      Double_t d_gy1 = el.Px();
      Double_t d_gx2 = d_gx1 + f_gadc;
      Double_t d_gy2 = er.Px();

      attline_black_soli_nor.Modify();
      if (hit->tdct || hit->tdcb) {
	attline_hit_u.Modify();

	myloc.SetXYZ((d_gy1-12.5)/10.,0,d_gx1/10.);
	mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
	myloc.SetXYZ((d_gy1-12.5)/10.,0,(d_gx1+60.)/10.);
	myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
	gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
      }

      myloc.SetXYZ((d_gy1)/10.,0,d_gx1/10.);
      mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
      myloc.SetXYZ((d_gy2)/10.,0,(d_gx2)/10.);
      myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
      gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
    }
  }

  return;
}

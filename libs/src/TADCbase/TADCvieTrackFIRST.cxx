/*!
  \file
  \version $Id: adapted from TABMvieTrackFIRST.cxx,v 1.7 2003/06/22 19:35:02 mueller Exp $
  \brief   Implementation of TADCvieTrackFIRST.
*/

#include <math.h>

#include "TROOT.h"
#include "TString.h"
#include "TAttFill.h"
#include "TAttLine.h"
#include "TAttText.h"
#include "TArc.h"

#include "TADCntuTrack.hxx"
#include "TADCntuRaw.hxx"

#include "TADCparGeo.hxx"

#include "TADCvieTrackFIRST.hxx"

/*!
  \class TADCvieTrackFIRST TADCvieTrackFIRST.hxx "TADCvieTrackFIRST.hxx"
  \brief Horizontal projective view for TADCntu*CHitQad and TADCntu*CTrackQad **
*/

ClassImp(TADCvieTrackFIRST);

//------------------------------------------+-----------------------------------
//! Construct with observables.

TADCvieTrackFIRST::TADCvieTrackFIRST(TAGdataDsc* p_ntutrk,
					     TAGdataDsc* p_nturaw,
					     TAGparaDsc* p_dcgeo)
  : fpNtuTrk(p_ntutrk),
    fpNtuRaw(p_nturaw),
    fpParDCGeo(p_dcgeo)
{
  if (p_ntutrk)  AddObservable(p_ntutrk,  "@TADCntuTrack");
  if (p_nturaw)  AddObservable(p_nturaw,  "@TADCntuRaw");
  if (p_dcgeo)   AddObservable(p_dcgeo,   "@TADCparGeo");
  SetName("TADCvieTrackFIRST");
  SetTitle("TADCvieTrackFIRST");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCvieTrackFIRST::~TADCvieTrackFIRST()
{
}


//------------------------------------------+-----------------------------------
//! Return textual representation of current cursor position

char* TADCvieTrackFIRST::GetObjectInfo(Int_t i_px, Int_t i_py) const
{
  if (!gPad) return (char*)"";
  Double_t d_x = gPad->PadtoX(gPad->AbsPixeltoX(i_px));
  Double_t d_y = gPad->PadtoY(gPad->AbsPixeltoY(i_py));

  return Form("%5.0f %5.0f", d_x, d_y);
}

//------------------------------------------+-----------------------------------
//! Handle pointer event.

void TADCvieTrackFIRST::ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py)
{
  //printf("ExecuteEvent: %4d : %4d %4d\n", i_event, i_px, i_py);
  return;
}

//------------------------------------------+-----------------------------------
//! Paint.

void TADCvieTrackFIRST::Paint(Option_t* option)
{
  TADCntuTrack* p_ntutrk  = 
    (TADCntuTrack*) fpNtuTrk->GenerateObject();
  TADCntuRaw*   p_nturaw = 
    (TADCntuRaw*)   fpNtuRaw->GenerateObject();
  TADCparGeo*   p_dcgeo = (TADCparGeo*) fpParDCGeo->Object();

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

  double dc_w = p_dcgeo->GetWidth();
  double dc_h = p_dcgeo->GetHeigth();
  double dc_l = p_dcgeo->GetLength();

  //Pad range
  gPad->Range(-1.02*dc_w, -0.51*dc_l, +1.02*dc_w, +0.51*dc_l);

  // DC space frame boundary
  Double_t h_x(0), h_y(0), h_z(0), h_cx(0), h_cy(0), h_cz(0);
  Double_t cm_w, cm_h;
  double pversx(0),pversy(0);
  double x_target(0), y_target(0);
  double xch_i, xch_f, ych_i, ych_f;
  double zmin, zmax, t_i, t_f;
  Int_t i_ntrk;

  /*
    Starting with Top View
  */

  attline_black_soli_nor.Modify();  attfill_box.Modify();
  gPad->PaintBox(-1.*dc_w, -0.5*dc_l, +0.*dc_w, +0.5*dc_l);
  gPad->PaintText(-0.6*dc_w, +0.46*dc_l, "TOP VIEW");

  cm_h = p_dcgeo->GetCellHeight();
  cm_w = p_dcgeo->GetCellWidth();

  /* Drawing the cells */
  for(int il= 0; il<p_dcgeo->GetLayersNumber(); il++) {
    for(int ic= 0; ic<p_dcgeo->GetCellsNumber(); ic++) {
      //V view, (X,Z)
      p_dcgeo->GetCellInfo(-1, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);

      //Reverse needed
      h_x *= -1;

      attline_black_soli_nor.Modify();  attfill_box.Modify();
      gPad->PaintBox(-cm_w+h_x-0.5*dc_w, -cm_h+h_z, +cm_w+h_x-0.5*dc_w, +cm_h+h_z);      
    }
  }
  
  TVector3 sid = p_dcgeo->GetSide();
  char text[200]; double chi2;
  //Drift Chamber tracks
  if (p_ntutrk) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the DC Tracks");

    i_ntrk = p_ntutrk->ntrk;
    for (Int_t i_t = 0; i_t < i_ntrk; i_t++) {
      TADCntuTrackTr* p_trk = p_ntutrk->Track(i_t);
      chi2 = p_trk->GetChi2();
      sprintf(text,"chi2 :: %lf",chi2);
      gPad->PaintText(-0.6*dc_w, +0.42*dc_l,text);

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
	 ti, tf = parameter at 2 Z edges of the DC
      */
      zmin = - sid.Z()/2;
      zmax = + sid.Z()/2;
      t_i = (zmin)/pversz;
      t_f = (zmax)/pversz;
      
      xch_i = x_target + pversx*t_i ;
      xch_f = x_target + pversx*t_f ;
      ych_i = y_target + pversy*t_i ;
      ych_f = y_target + pversy*t_f ;
      
      attline_trk_4.Modify();

      //X,Z (view V, TOP view)   
      //(X needs to be reversed SINCE TOP view has x increasing towards left)
      gPad->PaintLine(-xch_i-0.5*dc_w,zmin,-xch_f-0.5*dc_w,zmax);

    }
  }

  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Int_t i_nhit = p_nturaw->nhit;
    Info("Viewer()","Displaying the DC Hits %2d ",i_nhit);

    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {

      TADCntuHit* hit = p_nturaw->Hit(i_h);
      //View  == -1 (Top view, V view)
      if(hit->HorView() && hit->TrkAss()) {
	//Top view, V view, (X,Z)
	p_dcgeo->GetCellInfo(-1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
	//X has a consistent view (x points to the top) 
	attline_black_soli_nor.Modify();  attfill_tof_up.Modify();
	gPad->PaintBox(-(h_x-hit->Dist())-0.5*dc_w,h_z-hit->Dist(),-(h_x+hit->Dist())-0.5*dc_w,h_z+hit->Dist());

	atttext_small_hlvc.Modify();
	chi2 = hit->GetChi2();
	sprintf(text,"chi2 :: %lf",chi2);
	gPad->PaintText(-0.95*dc_w, h_z, text);

      }
      if(hit->HorView() && !(hit->TrkAss())) {
	//Top view, V view, (X,Z)
	p_dcgeo->GetCellInfo(-1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
	//X has a consistent view (x points to the top) 
	attline_black_soli_nor.Modify();  attfill_tof_m.Modify();
	gPad->PaintBox(-(h_x-hit->Dist())-0.5*dc_w,h_z-hit->Dist(),-(h_x+hit->Dist())-0.5*dc_w,h_z+hit->Dist());
      }
    }

  }


  /*
    Proceeding with Side View
  */

  attline_black_soli_nor.Modify();  attfill_box.Modify();
  gPad->PaintBox(-0.*dc_w, -0.5*dc_l, +1.*dc_w, +0.5*dc_l);
  gPad->PaintText(+0.4*dc_w, +0.46*dc_l, "SIDE VIEW");

  cm_h = p_dcgeo->GetCellHeight();
  cm_w = p_dcgeo->GetCellWidth();

  /* Drawing the cells */
  for(int il= 0; il<p_dcgeo->GetLayersNumber(); il++) {
    for(int ic= 0; ic<p_dcgeo->GetCellsNumber(); ic++) {
      //U view, (Y,Z)
      p_dcgeo->GetCellInfo(1, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);

      //Reverse needed (once again y increase from lf to rg)
      h_y *= -1;

      attline_black_soli_nor.Modify();  attfill_box.Modify();
      gPad->PaintBox(-cm_w+h_y+0.5*dc_w, -cm_h+h_z, +cm_w+h_y+0.5*dc_w, +cm_h+h_z);      
    }
  }
  
  //Drift Chamber tracks
  if (p_ntutrk) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the DC Tracks");

    i_ntrk = p_ntutrk->ntrk;
    for (Int_t i_t = 0; i_t < i_ntrk; i_t++) {
      TADCntuTrackTr* p_trk = p_ntutrk->Track(i_t);
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
	 ti, tf = parameter at 2 Z edges of the DC
      */
      zmin = - sid.Z()/2;
      zmax = + sid.Z()/2;
      t_i = (zmin)/pversz;
      t_f = (zmax)/pversz;
      
      xch_i = x_target + pversx*t_i ;
      xch_f = x_target + pversx*t_f ;
      ych_i = y_target + pversy*t_i ;
      ych_f = y_target + pversy*t_f ;
      
      attline_trk_4.Modify();

      //Y,Z (view U, Side view)   
      gPad->PaintLine(-ych_i+0.5*dc_w,zmin,-ych_f+0.5*dc_w,zmax);

    }
  }

  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Int_t i_nhit = p_nturaw->nhit;
    Info("Viewer()","Displaying the DC Hits %2d ",i_nhit);

    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {

      TADCntuHit* hit = p_nturaw->Hit(i_h);
      //View  == 1 (Side view, U view)
      if(hit->VertView() && hit->TrkAss()) {
	//Side view, U view, (Y,Z)
	p_dcgeo->GetCellInfo(1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
	//Y has a consistent view (Y points to the top) 
	attline_black_soli_nor.Modify();  attfill_tof_up.Modify();
	gPad->PaintBox(-(h_y-hit->Dist())+0.5*dc_w,h_z-hit->Dist(),-(h_y+hit->Dist())+0.5*dc_w,h_z+hit->Dist());

        atttext_small_hlvc.Modify();
	chi2 = hit->GetChi2();
	sprintf(text,"chi2 :: %lf",chi2);
	gPad->PaintText(0.05*dc_w, h_z, text);

      }
      if(hit->VertView() && !(hit->TrkAss())) {
	//Side view, U view, (Y,Z)
	p_dcgeo->GetCellInfo(1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
	//Y has a consistent view (Y points to the top) 
	attline_black_soli_nor.Modify();  attfill_tof_m.Modify();
	gPad->PaintBox(-(h_y-hit->Dist())+0.5*dc_w,h_z-hit->Dist(),-(h_y+hit->Dist())+0.5*dc_w,h_z+hit->Dist());
      }
    }

  }
  return;
}


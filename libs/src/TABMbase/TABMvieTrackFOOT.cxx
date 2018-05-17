/*!
  \file
  \version $Id: TABMvieTrackFOOT.cxx,v 1.7 2003/06/22 19:35:02 mueller Exp $
  \brief   Implementation of TABMvieTrackFOOT.
*/

#include <math.h>

#include "TROOT.h"
#include "TString.h"
#include "TAttFill.h"
#include "TAttLine.h"
#include "TAttText.h"
#include "TArc.h"
#include "foot_geo.h"

#include "TABMntuTrack.hxx"
#include "TABMntuRaw.hxx"

#include "TABMparGeo.hxx"

#include "TABMvieTrackFOOT.hxx"

/*!
  \class TABMvieTrackFOOT TABMvieTrackFOOT.hxx "TABMvieTrackFOOT.hxx"
  \brief Horizontal projective view for TABMntu*CHitQad and TABMntu*CTrackQad **
*/

ClassImp(TABMvieTrackFOOT);

//------------------------------------------+-----------------------------------
//! Construct with observables.

//~ TABMvieTrackFOOT::TABMvieTrackFOOT(TAGdataDsc* p_ntutrk,
					     //~ TAGdataDsc* p_nturaw,
					     //~ TAGparaDsc* p_bmgeo)
  //~ : fpNtuTrk(p_ntutrk),
    //~ fpNtuRaw(p_nturaw),
    //~ fpParBMGeo(p_bmgeo)
//~ {
  //~ if (p_ntutrk)  AddObservable(p_ntutrk,  "@TABMntuTrack");
  //~ if (p_nturaw)  AddObservable(p_nturaw,  "@TABMntuRaw");
  //~ if (p_bmgeo)   AddObservable(p_bmgeo,   "@TABMparGeo");
  //~ SetName("TABMvieTrackFOOT");
  //~ SetTitle("TABMvieTrackFOOT");
//~ }

TABMvieTrackFOOT::TABMvieTrackFOOT(TABMntuTrack* p_ntutrk_in,
					     TABMntuRaw* p_nturaw_in,
					     TABMparGeo* p_bmgeo_in)
  : p_ntutrk(p_ntutrk_in),
    p_nturaw(p_nturaw_in),
    p_bmgeo(p_bmgeo_in)
{

  SetName("TABMvieTrackFOOT");
  SetTitle("TABMvieTrackFOOT");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMvieTrackFOOT::~TABMvieTrackFOOT()
{
}


//------------------------------------------+-----------------------------------
//! Return textual representation of current cursor position

char* TABMvieTrackFOOT::GetObjectInfo(Int_t i_px, Int_t i_py) const
{
  if (!gPad) return (char*)"";
  Double_t d_x = gPad->PadtoX(gPad->AbsPixeltoX(i_px));
  Double_t d_y = gPad->PadtoY(gPad->AbsPixeltoY(i_py));

  return Form("%5.0f %5.0f", d_x, d_y);
}

//------------------------------------------+-----------------------------------
//! Handle pointer event.

void TABMvieTrackFOOT::ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py)
{
  //printf("ExecuteEvent: %4d : %4d %4d\n", i_event, i_px, i_py);
  return;
}

//------------------------------------------+-----------------------------------
//! Paint.

void TABMvieTrackFOOT::Paint(Option_t* option)
{
  //~ TABMntuTrack* p_ntutrk  =(TABMntuTrack*) fpNtuTrk->GenerateObject();//penso che non serva più il generate ma il find
  //~ TABMntuRaw*   p_nturaw = (TABMntuRaw*)   fpNtuRaw->GenerateObject();
  //~ TABMparGeo*   p_bmgeo = (TABMparGeo*) fpParBMGeo->Object();

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

  //Pad range
  gPad->Range(-1.02*BMN_WIDTH, -0.51*BMN_LENGTH, +1.02*BMN_WIDTH, +0.51*BMN_LENGTH);

  // BM space frame boundary
  Double_t h_x(0), h_y(0), h_z(0), h_cx(0), h_cy(0), h_cz(0);
  Double_t BMN_PASSO, BMN_STEP;
  double pversx(0),pversy(0);
  double x_target(0), y_target(0);
  double xch_i, xch_f, ych_i, ych_f;
  double zmin, zmax, t_i, t_f;
  Int_t i_ntrk;

  /*
    Starting with Top View (XZ)       View  == -1 (Top view, V view)
  */

  attline_black_soli_nor.Modify();  
  attfill_box.Modify();
  gPad->PaintBox(-1.*BMN_WIDTH, -0.5*BMN_LENGTH, +0.*BMN_WIDTH, +0.5*BMN_LENGTH);
  gPad->PaintText(-0.6*BMN_WIDTH, +0.46*BMN_LENGTH, "TOP VIEW");


  /* Drawing the cells */
  for(int il= 0; il<BMN_NLAY; il++) {
    for(int ic= 0; ic<BMN_NSENSELAY; ic++) {
      //V view, (X,Z)
      p_bmgeo->GetCellInfo(-1, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);

      //Reverse needed  ATTENZIONE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! DA CAPIRE PERCHÈ STA COSA!!!!!!!!!!!!!!!!!!!
      h_x *= -1;

      attline_black_soli_nor.Modify();  attfill_box.Modify();
      gPad->PaintBox(-BMN_PASSO+h_x-0.5*BMN_WIDTH, -BMN_STEP+h_z, +BMN_PASSO+h_x-0.5*BMN_WIDTH, +BMN_STEP+h_z);      
    }
  }
  
  char text[200];
  //Beam Monitor tracks 
  if (p_ntutrk) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the BM Tracks");

    i_ntrk = p_ntutrk->ntrk;
    for (Int_t i_t = 0; i_t < i_ntrk; i_t++) {
      TABMntuTrackTr* p_trk = p_ntutrk->Track(i_t);
      sprintf(text,"chi2new :: %lf",p_trk->GetChi2New());
      gPad->PaintText(-0.6*BMN_WIDTH, +0.42*BMN_LENGTH,text);

      //~ x_target = p_trk->GetX0();//yun: sono arrivato qua
      //~ y_target = p_trk->GetY0();
      //~ pversx = p_trk->GetUx();
      //~ pversy = p_trk->GetUy();

      //~ double ptra2 = pversx*pversx + pversy*pversy;
      //~ if(ptra2>1.){
        //~ ptra2 = 1.;
        //~ cout <<"warning : wrong output from the fit"<<endl;
      //~ }
      //~ double pversz = sqrt(1. - ptra2);
      
      //~ /* 
       //~ use parametric equation for track : 
       //~ ti, tf = parameter at 2 Z edges of the BM
      //~ */
      //~ zmin = - BMN_LENGTH/2;
      //~ zmax = + BMN_LENGTH/2;
      //~ t_i = (zmin)/pversz;
      //~ t_f = (zmax)/pversz;
      
      //~ xch_i = x_target + pversx*t_i ;
      //~ xch_f = x_target + pversx*t_f ;
      //~ ych_i = y_target + pversy*t_i ;
      //~ ych_f = y_target + pversy*t_f ;
      
      attline_trk_4.Modify();

      //X,Z (view V, TOP view)   
      //(X needs to be reversed SINCE TOP view has x increasing towards left)... ma perchè?
      //~ gPad->PaintLine(-xch_i-0.5*BMN_WIDTH,zmin,-xch_f-0.5*BMN_WIDTH,zmax);
      gPad->PaintLine(-p_trk->GetMylar1Pos().X(),p_trk->GetMylar1Pos().Z(),-p_trk->GetMylar2Pos().X(),p_trk->GetMylar2Pos().Z());
    }//attenzione, ho messo dei meno per la posizione, controllare che vada bene
  }

  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Int_t i_nhit = p_nturaw->nhit;
    Info("Viewer()","Displaying the BM Hits %2d ",i_nhit);

    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {

      TABMntuHit* hit = p_nturaw->Hit(i_h);
      if(hit->View()==-1 && hit->GetIsSelected()) {
        //Top view, V view, (X,Z)
        p_bmgeo->GetCellInfo(-1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);//charge the wire position and direction in h_x.. and h_cx...
        //X has a consistent view (x points to the top) 
        attline_black_soli_nor.Modify();  
        attfill_tof_up.Modify();
        gPad->PaintBox(-(h_x-hit->Dist())-0.5*BMN_WIDTH,h_z-hit->Dist(),-(h_x+hit->Dist())-0.5*BMN_WIDTH,h_z+hit->Dist());
        //~ gPad->PaintEllipse(-h_x,h_z,hit->Dist());
        atttext_small_hlvc.Modify();
        sprintf(text,"chi2 :: %lf",hit->GetChi2());
        gPad->PaintText(-0.95*BMN_WIDTH, h_z, text);

      }
      else if(hit->View()==-1 && !hit->GetIsSelected()) {
        //Top view, V view, (X,Z)
        p_bmgeo->GetCellInfo(-1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
        //X has a consistent view (x points to the top) 
        attline_black_soli_nor.Modify();  attfill_tof_m.Modify();
        gPad->PaintBox(-(h_x-hit->Dist())-0.5*BMN_WIDTH,h_z-hit->Dist(),-(h_x+hit->Dist())-0.5*BMN_WIDTH,h_z+hit->Dist());
      }
    }//end loop on hits

  }


  /*
    Proceeding with Side View  U view, view==1
  */

  attline_black_soli_nor.Modify();  
  attfill_box.Modify();
  gPad->PaintBox(-0.*BMN_WIDTH, -0.5*BMN_LENGTH, +1.*BMN_WIDTH, +0.5*BMN_LENGTH);
  gPad->PaintText(+0.4*BMN_WIDTH, +0.46*BMN_LENGTH, "SIDE VIEW");

  /* Drawing the cells */
  for(int il= 0; il<BMN_NLAY; il++) {
    for(int ic= 0; ic<BMN_NSENSELAY; ic++) {
      //U view, (Y,Z)
      p_bmgeo->GetCellInfo(1, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);

      //Reverse needed (once again y increase from left to right)... DA CAPIRE SE È VERO O NO...
      h_y *= -1;

      attline_black_soli_nor.Modify();  
      attfill_box.Modify();
      gPad->PaintBox(-BMN_PASSO+h_y+0.5*BMN_WIDTH, -BMN_STEP+h_z, +BMN_PASSO+h_y+0.5*BMN_WIDTH, +BMN_STEP+h_z);      
    }
  }
  
  //Beam Monitor tracks
  if (p_ntutrk) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the BM Tracks");

    i_ntrk = p_ntutrk->ntrk;
    for (Int_t i_t = 0; i_t < i_ntrk; i_t++) {
      TABMntuTrackTr* p_trk = p_ntutrk->Track(i_t);
      //~ x_target = p_trk->GetX0();
      //~ y_target = p_trk->GetY0();
      //~ pversx = p_trk->GetUx();
      //~ pversy = p_trk->GetUy();

      //~ double ptra2 = pversx*pversx + pversy*pversy;
      //~ if(ptra2>1.){
        //~ ptra2 = 1.;
        //~ cout <<"warning : wrong output from the fit"<<endl;
      //~ }
      //~ double pversz = sqrt(1. - ptra2);
      
      //~ /* 
	 //~ use parametric equation for track : 
	 //~ ti, tf = parameter at 2 Z edges of the BM
      //~ */
      //~ zmin = - BMN_LENGTH/2;
      //~ zmax = + BMN_LENGTH/2;
      //~ t_i = (zmin)/pversz;
      //~ t_f = (zmax)/pversz;
      
      //~ xch_i = x_target + pversx*t_i ;
      //~ xch_f = x_target + pversx*t_f ;
      //~ ych_i = y_target + pversy*t_i ;
      //~ ych_f = y_target + pversy*t_f ;
      
      attline_trk_4.Modify();

      //Y,Z (view U, Side view)   
      //~ gPad->PaintLine(-ych_i+0.5*BMN_WIDTH,zmin,-ych_f+0.5*BMN_WIDTH,zmax);
      gPad->PaintLine(p_trk->GetMylar1Pos().Y(),p_trk->GetMylar1Pos().Z(),p_trk->GetMylar2Pos().Y(),p_trk->GetMylar2Pos().Z());
    }
  }

  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Int_t i_nhit = p_nturaw->nhit;
    Info("Viewer()","Displaying the BM Hits %2d ",i_nhit);

    for (Int_t i_h = 0; i_h < i_nhit; i_h++) {

      TABMntuHit* hit = p_nturaw->Hit(i_h);
      //View  == 1 (Side view, U view)
      if(hit->View()==1 && hit->GetIsSelected()) {
        //Side view, U view, (Y,Z)
        p_bmgeo->GetCellInfo(1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
        //Y has a consistent view (Y points to the top) 
        attline_black_soli_nor.Modify();  
        attfill_tof_up.Modify();
        gPad->PaintBox(-(h_y-hit->Dist())+0.5*BMN_WIDTH,h_z-hit->Dist(),-(h_y+hit->Dist())+0.5*BMN_WIDTH,h_z+hit->Dist());
        //~ gPad->PaintEllipse(h_y,h_z,hit->Dist());
        atttext_small_hlvc.Modify();
        sprintf(text,"chi2 :: %lf",hit->GetChi2());
        gPad->PaintText(0.05*BMN_WIDTH, h_z, text);

      }
      if(hit->View() && !hit->GetIsSelected()) {
        //Side view, U view, (Y,Z)
        p_bmgeo->GetCellInfo(1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
        //Y has a consistent view (Y points to the top) 
        attline_black_soli_nor.Modify();  
        attfill_tof_m.Modify();
        gPad->PaintBox(-(h_y-hit->Dist())+0.5*BMN_WIDTH,h_z-hit->Dist(),-(h_y+hit->Dist())+0.5*BMN_WIDTH,h_z+hit->Dist());
      }
    }

  }
  return;
}


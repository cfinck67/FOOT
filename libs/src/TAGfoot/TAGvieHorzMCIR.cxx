/*!
  \file
  \version $Id: TAGvieHorzMCIR.cxx,v 1.7 2003/06/22 19:35:02 mueller Exp $
  \brief   Implementation of TAGvieHorzMCIR.
*/

#include <math.h>

#include "TROOT.h"
#include "TString.h"
#include "TAttFill.h"
#include "TAttLine.h"
#include "TAttText.h"
#include "TArc.h"
#include "TCanvas.h"

#include "TAVTntuCluster.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTparGeo.hxx"

#include "TABMntuTrack.hxx"
#include "TABMntuRaw.hxx"
#include "TABMparGeo.hxx"

//First
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAGvieHorzMCIR.hxx"

/*!
  \class TAGvieHorzMCIR TAGvieHorzMCIR.hxx "TAGvieHorzMCIR.hxx"
  \brief Horizontal projective view for MCIR detectors**
*/

ClassImp(TAGvieHorzMCIR);

//------------------------------------------+-----------------------------------
//! Construct with observables.

TAGvieHorzMCIR::TAGvieHorzMCIR(TAGdataDsc* p_ntutrk,
			       TAGdataDsc* p_nturaw,
			       TAGparaDsc* p_bmgeo,
			       TAGdataDsc* p_ntuvtcl,
			       TAGdataDsc* p_ntuvttr,
			       TAGparaDsc* p_vtgeo, 
			       EVENT_STRUCT* evStr)
  : fpNtuTrk(p_ntutrk),
    fpNtuBMRaw(p_nturaw),
    fpParBMGeo(p_bmgeo),
    fpNtuClust(p_ntuvtcl),
    fpNtuTrack(p_ntuvttr),
    fpParVTGeo(p_vtgeo),
    fpEvtStr(evStr)
{
  if (p_ntutrk)  AddObservable(p_ntutrk,  "@TABMntuTrack");
  if (p_nturaw)  AddObservable(p_nturaw,  "@TABMntuRaw");
  if (p_bmgeo)   AddObservable(p_bmgeo,   "@TABMparGeo");
  if (p_ntuvtcl) AddObservable(p_ntuvtcl,   "@TAVTntuCluster");
  if (p_ntuvttr) AddObservable(p_ntuvttr,   "@TAVTntuTrack");
  if (p_vtgeo)   AddObservable(p_vtgeo,   "@TAVTparGeo");

  SetName("TAGvieHorzMCIR");
  SetTitle("TAGvieHorzMCIR");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGvieHorzMCIR::~TAGvieHorzMCIR()
{
}


//------------------------------------------+-----------------------------------
//! Return textual representation of current cursor position

char* TAGvieHorzMCIR::GetObjectInfo(Int_t i_px, Int_t i_py) const
{
  if (!gPad) return (char*)"";
  Double_t d_x = gPad->PadtoX(gPad->AbsPixeltoX(i_px));
  Double_t d_y = gPad->PadtoY(gPad->AbsPixeltoY(i_py));

  return Form("%5.0f %5.0f", d_x, d_y);
}

//------------------------------------------+-----------------------------------
//! Handle pointer event.

void TAGvieHorzMCIR::ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py)
{
  //printf("ExecuteEvent: %4d : %4d %4d\n", i_event, i_px, i_py);
  return;
}

//------------------------------------------+-----------------------------------
//! Paint.

void TAGvieHorzMCIR::Paint(Option_t* option)
{
  TABMntuTrack* p_ntutrk  = 
    (TABMntuTrack*) fpNtuTrk->GenerateObject();
  TABMntuRaw*   p_nturaw = 
    (TABMntuRaw*)   fpNtuBMRaw->GenerateObject();
  TABMparGeo*   p_bmgeo = (TABMparGeo*) fpParBMGeo->Object();
  TAVTntuCluster*  p_ntuvtcl = 
    (TAVTntuCluster*)   fpNtuClust->GenerateObject();
  TAVTntuTrack*  p_ntuvttr = 
    (TAVTntuTrack*)   fpNtuTrack->GenerateObject();
  TAVTparGeo*   p_vtgeo = (TAVTparGeo*) fpParVTGeo->Object();

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

  if (IsZombie() || !gPad) return;

  TAttLine attline_black_soli_fat(1, 1, 2); // (col, sty, wid)
  TAttLine attline_black_soli_nor(1, 1, 1);
  TAttLine attline_black_dash_nor(1, 2, 1);
  TAttLine attline_black_dot_nor(1, 3, 1);

  TAttLine attline_trk_4(1, 1, 1);	    // (col,sty,wid) = bla,sol,1
  TAttLine attline_trk_3(1, 2, 1);	    // (col,sty,wid) = bla,das,1
  TAttLine attline_trk_2(2, 1, 1);	    // (col,sty,wid) = red,sol,1
  TAttLine attline_trk_mc(2, 2, 1);	    // (col,sty,wid) = red,sol,1
  TAttLine attline_trk_vt(4, 2, 1);	    // (col,sty,wid) = bla,das,1

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


  //Pad range. Using centimeters!
  //  cout<<"Using option:: "<<option<<endl;

  //  gPad->Range(-10, -50, +90, +50);
  if(!strcmp(option,"zoom")) {
    gPad->Range(+59, -2.5, +64, +2.5);
  } else {
    gPad->Range(0, -10, +70, +10);
  }
  TVector3  myloc, mylow, myhig;

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

  double bm_w = p_bmgeo->GetWidth();
  //double bm_h = p_bmgeo->GetHeigth();
  double bm_l = p_bmgeo->GetLength();

  Double_t h_x(0), h_y(0), h_z(0), h_cx(0), h_cy(0), h_cz(0);
  Double_t cm_w, cm_h;
  cm_h = p_bmgeo->GetCellHeight();
  cm_w = p_bmgeo->GetCellWidth();

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
  // double xcen(0), ycen(0);
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

    //MC truth
    double pvx = fpEvtStr->BMNpxin[0]; double pvy = fpEvtStr->BMNpyin[0];

    double ptra2 = pvx*pvx + pvy*pvy;
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
    //    zmax = zcen + bm_sid.Z()/2;
    zmax = zcen + fpFirstGeo->GetVTCenter().Z() - fpFirstGeo->GetBMCenter().Z();    
    t_i = (zmin-zcen)/pversz;
    t_f = (zmax-zcen)/pversz;
    
    xch_i = x_target + pvx*t_i ;
    xch_f = x_target + pvx*t_f ;
    ych_i = y_target + pvy*t_i ;
    ych_f = y_target + pvy*t_f ;
    
    myloc.SetXYZ(xch_i, 0, zmin);
    mylow = fpFirstGeo->FromBMLocalToGlobal(myloc);
    myloc.SetXYZ(xch_f, 0, zmax);
    myhig = fpFirstGeo->FromBMLocalToGlobal(myloc);
    
    attline_trk_mc.Modify();
    //X,Z (view V, TOP view)   
    gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
  }

  //Horiz axis
  attline_trk_3.Modify();
  gPad->PaintLine(0, 0, 100, 0);

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


  //
  //********            VTX           ******* 
  //

  //Displays the Clusters reconstruced in the VTX detector
  Int_t i_ncl;

  //Draw the vtx detectors planes
  for(int is=0; is<8; is++) {

    //Position is returned in microns
    TVector3* myP = p_vtgeo->GetPosition(is);
    /*
    cm_w = MI_MIMO_WIDTH/4;
    cm_h = MI_MIMO_THICK/2;
    */
    cm_w = 0.01;
    cm_h = 0.005;
    h_z = myP->Z()/10000;    h_x = myP->X()/10000;

    myloc.SetXYZ(-cm_w+h_x, 0, -cm_h+h_z);
    mylow = fpFirstGeo->FromVTLocalToGlobal(myloc);
    myloc.SetXYZ(+cm_w+h_x, 0, +cm_h+h_z);
    myhig = fpFirstGeo->FromVTLocalToGlobal(myloc);

    attline_black_soli_nor.Modify();  attfill_box.Modify();
    gPad->PaintBox(mylow.Z(),mylow.X(),myhig.Z(),myhig.X());
    
  }
  if (p_ntuvtcl) {

    //Displays the Clusters in the VTX detector
    for(int is=0; is<8; is++) {

      i_ncl = p_ntuvtcl->GetClustersN(is);
      
      Info("Viewer()","Displaying the VTX clst %2d ",i_ncl);
      
      for (Int_t i_cl = 0; i_cl < i_ncl; i_cl++) {

	TAVTcluster *acl = p_ntuvtcl->GetCluster(is,i_cl);
	TVector3 myG = acl->GetPositionG();
	//	cout<<" myL positions:: "<<myG.X()<<" "<<myG.Y()<<" "<<myG.Z()<<endl;
	myloc.SetXYZ(myG.X()/10000-0.03,0,myG.Z()/10000-0.03);
	mylow = fpFirstGeo->FromVTLocalToGlobal(myloc);
	myloc.SetXYZ(myG.X()/10000+0.03,0,myG.Z()/10000+0.03);
	myhig = fpFirstGeo->FromVTLocalToGlobal(myloc);

	attline_black_soli_nor.Modify();  attfill_tof_up.Modify();
	if(acl->GetFound()) {
	  attline_black_soli_nor.Modify();  attfill_tof_m.Modify();
	}
	gPad->PaintBox(mylow.Z(),mylow.X(),myhig.Z(),myhig.X());
      }
    }//Sensors Loop
    
    /*
    for (Int_t i = 0; i < fpEvtStr->miSigN; i++) {
      Double_t X_p = fpEvtStr->miSigX[i];
      //Double_t Y_p = fpEvtStr->miSigY[i];
      Double_t Z_p;
      if(fpEvtStr->miSigChip[i]%2) {
	Z_p = fpEvtStr->miSigZ[i]+0.75+0.3*fpEvtStr->miSigChip[i];
      } else  {
	Z_p = fpEvtStr->miSigZ[i]+0.65+0.3*(fpEvtStr->miSigChip[i]-4);
      }
      myloc.SetXYZ(X_p-0.05,0,Z_p-0.05);
      //      mylow = fpFirstGeo->FromVTLocalToGlobal(myloc);
      mylow = myloc;
      myloc.SetXYZ(X_p+0.05,0,Z_p+0.05);
      myhig = myloc;
      //      myhig = fpFirstGeo->FromVTLocalToGlobal(myloc);
      
      attline_black_soli_nor.Modify();  attfill_tof_b.Modify();

      gPad->PaintBox(mylow.Z(),mylow.X(),myhig.Z(),myhig.X());
      
    }

    */

  }
  if (p_ntuvttr) {
    //Displays the Tracks in the VTX detector
    
    Info("Action()","%d tracks found in VTX",p_ntuvttr->GetTracksN());
    for (Int_t i = 0; i < p_ntuvttr->GetTracksN(); ++i) {
      TAVTtrack* track = p_ntuvttr->GetTrack(i);
      TVector3 tr_or = track->Intersection(0)*(1./10000);
      TVector3 tr_af = track->Intersection(40000)*(1./10000);
      
      //X,Z (view V, TOP view)   
      TVector3 gtr_or = fpFirstGeo->FromVTLocalToGlobal(tr_or);
      TVector3 gtr_af = fpFirstGeo->FromVTLocalToGlobal(tr_af);
      attline_trk_vt.Modify();
      
      gPad->PaintLine(gtr_or.Z(), gtr_or.X(), gtr_af.Z(), gtr_af.X());

    }
  }
  return;
}

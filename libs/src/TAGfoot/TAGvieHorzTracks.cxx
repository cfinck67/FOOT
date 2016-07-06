/*!
  \file
  \version $Id: TAGvieHorzTracks.cxx,v 1.7 2003/06/22 19:35:02 mueller Exp $
  \brief   Implementation of TAGvieHorzTracks.
*/

#include <math.h>
#include "gsi_geo.h"

#include "TROOT.h"
#include "TString.h"
#include "TGaxis.h"
#include "TAttFill.h"
#include "TAttLine.h"
#include "TAttText.h"
#include "TArc.h"

#include "TATntuRaw.hxx"

#include "TATparGeo.hxx"

#include "TABMntuTrack.hxx"
#include "TABMntuRaw.hxx"

#include "TABMparGeo.hxx"
#include "TAGntuGlbTracks.hxx"

#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGvieHorzTracks.hxx"

/*!
  \class TAGvieHorzTracks TAGvieHorzTracks.hxx "TAGvieHorzTracks.hxx"
  \brief Horizontal projective view for TABMntu*CHitQad and TABMntu*CTrackQad **
*/

ClassImp(TAGvieHorzTracks);

//------------------------------------------+-----------------------------------
//! Construct with observables.

TAGvieHorzTracks::TAGvieHorzTracks(TAGdataDsc* p_glbtrk,
				   TAGdataDsc* p_ntutrk,
				   TAGdataDsc* p_nturaw,
				   TAGparaDsc* p_bmgeo,
				   TAGdataDsc* p_ntutraw,
				   TAGparaDsc* p_tofgeo)
  : fpGlbTrk(p_glbtrk),    
    fpNtuTrk(p_ntutrk),
    fpNtuBMRaw(p_nturaw),
    fpParBMGeo(p_bmgeo),
    fpNtuTofRaw(p_ntutraw),
    fpParTofGeo(p_tofgeo)
{
  if (p_glbtrk)  AddObservable(p_glbtrk,  "@TAGntuGlbTracks");
  if (p_ntutrk)  AddObservable(p_ntutrk,  "@TABMntuTrack");
  if (p_nturaw)  AddObservable(p_nturaw,  "@TABMntuRaw");
  if (p_bmgeo)   AddObservable(p_bmgeo,   "@TABMparGeo");
  if (p_ntutraw)   AddObservable(p_ntutraw,   "@TATntuRaw");
  if (p_tofgeo)    AddObservable(p_tofgeo,    "@TATparGeo");

  SetName("TAGvieHorzTracks");
  SetTitle("TAGvieHorzTracks");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGvieHorzTracks::~TAGvieHorzTracks()
{
}


//------------------------------------------+-----------------------------------
//! Return textual representation of current cursor position

char* TAGvieHorzTracks::GetObjectInfo(Int_t i_px, Int_t i_py) const
{
  if (!gPad) return (char*)"";
  Double_t d_x = gPad->PadtoX(gPad->AbsPixeltoX(i_px));
  Double_t d_y = gPad->PadtoY(gPad->AbsPixeltoY(i_py));

  return Form("%5.0f %5.0f", d_x, d_y);
}

//------------------------------------------+-----------------------------------
//! Handle pointer event.

void TAGvieHorzTracks::ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py)
{
  //printf("ExecuteEvent: %4d : %4d %4d\n", i_event, i_px, i_py);
  return;
}

//------------------------------------------+-----------------------------------
//! Paint.

void TAGvieHorzTracks::Paint(Option_t* option)
{
  TAGntuGlbTracks* p_glbtrk  = 
    (TAGntuGlbTracks*) fpGlbTrk->GenerateObject();
  TABMntuTrack* p_ntutrk  = 
    (TABMntuTrack*) fpNtuTrk->GenerateObject();
  TABMntuRaw*   p_nturaw = 
    (TABMntuRaw*)   fpNtuBMRaw->GenerateObject();
  TABMparGeo*   p_bmgeo = (TABMparGeo*) fpParBMGeo->Object();
  TATntuRaw*        p_ntutraw = 
    (TATntuRaw*)        fpNtuTofRaw->GenerateObject();
  TATparGeo*        p_tofgeo = (TATparGeo*) fpParTofGeo->Object();

  TAGgeoTrafo *fpFirstGeo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fpFirstGeo == 0x0)
    Error("SetGeoTrafoName", "No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());
  
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
  gPad->Range(-10, -170, +700, +100);

  TGaxis *axis5 = new TGaxis(0,-130,100,-130,0.,100.,80506,"-+");
  axis5->SetName("axis5");
  axis5->SetTitle("z (cm)");
  axis5->SetTitleOffset(1.2);
  axis5->SetLabelSize(0.03);
  //  axis5->SetTextFont(72);
  axis5->SetLabelOffset(0.025);
  axis5->Draw();

  TGaxis *axis6 = new TGaxis(0,-110,0,-10,0.,100.,80506,"-+");
  axis6->SetName("axis6");
  axis6->SetTitle("z (cm)");
  axis6->SetTitleOffset(1.2);
  axis6->SetLabelSize(0.03);
  //  axis6->SetTextFont(72);
  axis6->SetLabelOffset(0.025);
  axis6->Draw();
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
  // double xcen(0), ycen(0);
  double zcen(0);
  double x_target(0), y_target(0);
  double xch_i, xch_f, ych_i, ych_f;
  double zmin, zmax, t_i, t_f;
  if (p_ntutrk) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the BM Tracks");

    TVector3 bmPos = fpFirstGeo->GetBMCenter();
    TVector3 vtPos = fpFirstGeo->GetVTCenter();
    double zDiff  = vtPos.Z() - bmPos.Z();	  

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
      zmax = zcen + zDiff;
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

  //Kentros Frame
  TVector3 kece = fpFirstGeo->GetKECenter();
  TVector3 kean = fpFirstGeo->GetKEAngles();

  TVector3 kesx_h = kece + TVector3(-37.,0,66.);
  TVector3 kesx_l = kece + TVector3(-37.,0,0.);
  TVector3 kesx_c = kece + TVector3(-10.,0,66.);
  TVector3 kedx_h = kece + TVector3(37.,0,66.);
  TVector3 kedx_l = kece + TVector3(37.,0,0.);
  TVector3 kedx_c = kece + TVector3(10.,0,66.);
  gPad->PaintLine(kesx_h.Z(), kesx_h.X(), kesx_l.Z(), kesx_l.X());
  gPad->PaintLine(kedx_h.Z(), kedx_h.X(), kedx_l.Z(), kedx_l.X());
  gPad->PaintLine(kesx_h.Z(), kesx_h.X(), kesx_c.Z(), kesx_c.X());
  gPad->PaintLine(kedx_h.Z(), kedx_h.X(), kedx_c.Z(), kedx_c.X());
  //66cm lenght
  //74 cm diameter.

  /*
  TVector3 AlBoxSize = TVector3(150.0, 50.0,230.0); //full-side-length in cm
  TVector3 sx_h = TVector3(-75.,0,115.);
  TVector3 glsx_h = fpFirstGeo->FromALLocalToGlobal(sx_h);
  TVector3 sx_l = TVector3(-75.,0,-115.);
  TVector3 glsx_l = fpFirstGeo->FromALLocalToGlobal(sx_l);
  TVector3 dx_h = TVector3(75.,0,115.);
  TVector3 gldx_h = fpFirstGeo->FromALLocalToGlobal(dx_h);
  TVector3 dx_l = TVector3(75.,0,-115.);
  TVector3 gldx_l = fpFirstGeo->FromALLocalToGlobal(dx_l);
  
  gPad->PaintLine(glsx_h.Z(), glsx_h.X(), gldx_h.Z(), gldx_h.X());
  gPad->PaintLine(gldx_h.Z(), gldx_h.X(), gldx_l.Z(), gldx_l.X());
  gPad->PaintLine(gldx_l.Z(), gldx_l.X(), glsx_l.Z(), glsx_l.X());
  gPad->PaintLine(glsx_h.Z(), glsx_h.X(), glsx_l.Z(), glsx_l.X());
  */

  //Aladin Frame
  TVector3 alce = fpFirstGeo->GetALCenter();
  TVector3 alan = fpFirstGeo->GetALAngles();
  TVector3 AlBoxSize = TVector3(150.0, 50.0,230.0); //full-side-length in cm
  TVector3 sx_h = TVector3(-75.,0,115.);
  TVector3 glsx_h = fpFirstGeo->FromALLocalToGlobal(sx_h);
  TVector3 sx_l = TVector3(-75.,0,-115.);
  TVector3 glsx_l = fpFirstGeo->FromALLocalToGlobal(sx_l);
  TVector3 dx_h = TVector3(75.,0,115.);
  TVector3 gldx_h = fpFirstGeo->FromALLocalToGlobal(dx_h);
  TVector3 dx_l = TVector3(75.,0,-115.);
  TVector3 gldx_l = fpFirstGeo->FromALLocalToGlobal(dx_l);
  
  gPad->PaintLine(glsx_h.Z(), glsx_h.X(), gldx_h.Z(), gldx_h.X());
  gPad->PaintLine(gldx_h.Z(), gldx_h.X(), gldx_l.Z(), gldx_l.X());
  gPad->PaintLine(gldx_l.Z(), gldx_l.X(), glsx_l.Z(), glsx_l.X());
  gPad->PaintLine(glsx_h.Z(), glsx_h.X(), glsx_l.Z(), glsx_l.X());

    // MUSIC space frame boundary
    attline_black_soli_nor.Modify();
    myloc.SetXYZ(-100,0,-100);
    mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
    myloc.SetXYZ(100,0,-100);
    myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
    gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

    myloc.SetXYZ(-100,0,100);
    mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
    myloc.SetXYZ(100,0,100);
    myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
    gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

    myloc.SetXYZ(-100,0,-100);
    mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
    myloc.SetXYZ(-100,0,100);
    myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
    gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

    myloc.SetXYZ(100,0,-100);
    mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
    myloc.SetXYZ(100,0,100);
    myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
    gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());

    attline_black_dot_nor.Modify();
    myloc.SetXYZ(0,0,-100);
    mylow = fpFirstGeo->FromMULocalToGlobal(myloc);
    myloc.SetXYZ(0,0,100);
    myhig = fpFirstGeo->FromMULocalToGlobal(myloc);
    gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());


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
      //      gPad->PaintLine(mylow.Z(), mylow.X(), myhig.Z(), myhig.X());
    }
  }

  //Drawing the reconstructed track
  vector<TVector3> trPath;
  TVector3 FiPoi, LaPoi;

  vector<TVector3>::iterator it;
  vector<TVector3> sorPath;

  //double tst; int myid;
  for(int itr = 0; itr<p_glbtrk->nglbtrk; itr++) {

    trPath = p_glbtrk->Track(itr)->Path();
    int size = trPath.size();
    /*
    for(int ilo = 0; ilo<(int)trPath.size(); ilo++) {
      trPath.at(ilo).Print();
      if(!ilo) {
	sorPath.push_back(trPath.at(ilo));
      } else  {
	tst = trPath.at(ilo).Z();
	for(int id=0; id<sorPath.size(); id++) {
	  if(tst<sorPath.at(id).Z()) {
	    sorPath.insert(it,trPath.at(ilo));
	    myid = id;
	    break;
	  }
	}
      }
    }
    */

    //Should be sorted.
    sorPath = trPath;
    /*
    for(int ilo = 0; ilo<(int)sorPath.size(); ilo++) {
      sorPath.at(ilo).Print();
    }
    */

    int max_ite = size - 1;
    if(max_ite>=1) {
      for(int ips=0; ips<max_ite; ips++) {
	FiPoi = trPath.at(ips);
	LaPoi = trPath.at(ips+1);
	attline_hit_m.Modify();
	gPad->PaintLine(FiPoi.Z(), FiPoi.X(), LaPoi.Z(), LaPoi.X());
      }
    } else {
      Error("Action()","Not enough points to draw a path! Npoi:: %d ",max_ite);
    }
  }


  return;
}


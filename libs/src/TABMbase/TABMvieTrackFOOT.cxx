/*!
  \file
  \version $Id: TABMvieTrackFOOT.cxx,v 1.7 2003/06/22 19:35:02 mueller Exp $
  \brief   Implementation of TABMvieTrackFOOT.
*/

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

//~ TABMvieTrackFOOT::TABMvieTrackFOOT(TABMparGeo* p_bmgeo_in)
  //~ : p_bmgeo(p_bmgeo_in)
TABMvieTrackFOOT::TABMvieTrackFOOT(TABMntuTrack* p_ntutrk_in, TABMntuRaw* p_nturaw_in, TABMparGeo* p_bmgeo_in, Int_t track_ok_in, Int_t event_number_in)
  : p_ntutrk(p_ntutrk_in), p_nturaw(p_nturaw_in), p_bmgeo(p_bmgeo_in), track_ok(track_ok_in), event_number(event_number_in)
{
  cell_occupy=nullptr;
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
  //~ TABMntuTrack* p_ntutrk  =(TABMntuTrack*) fpNtuTrk->GenerateObject();//non serve più il generate ma il find nel caso!
  //~ TABMntuRaw*   p_nturaw = (TABMntuRaw*)   fpNtuRaw->GenerateObject();
  //~ TABMparGeo*   p_bmgeo = (TABMparGeo*) fpParBMGeo->Object();

  //~ cout<<"TABMvieTrackFoot::evaluate_cell_occupy: print cell_occupy"<<endl;
  //~ for(Int_t i=0;i<36;i++){
  //~ cout<<endl;
    //~ for(Int_t j=0;j<cell_occupy->at(i).size();j++)
      //~ cout<<cell_occupy->at(i)[j]<<" ";
  //~ }

  if (IsZombie() || !gPad) return;

  TAttFill attfill_box(0, 1);	    // (col,sty) = empty, solidline for box

  //~ TAttLine attline_black_soli_fat(1, 1, 2); // (color, style, width)
  //~ TAttLine attline_black_dash_nor(1, 2, 1);
  //~ TAttLine attline_black_dot_nor(1, 3, 1);
  //~ TAttLine attline_trk_4(1, 1, 1);	    // (col,sty,wid) = bla,sol,1
  //~ TAttLine attline_trk_3(1, 2, 1);	    // (col,sty,wid) = bla,das,1
  //~ TAttLine attline_hit_b(3, 1, 1);	    // (col,sty,wid) = gre,sol,1  bad
  //~ TAttFill attfill_tof(1, 1001);	    // (col,sty) = bla, solid  ???
  //~ TAttFill attfill_tof_us(6, 1001);	    // (col,sty) = red, solid  use sea
  //~ TAttFill attfill_tof_b(3, 1001);	    // (col,sty) = gre, solid  bad
  //~ TAttText atttext_small_hcvb(21, 0., 1, 83, 10);   //allign, angle, color, font, size
  //~ TAttText atttext_small_hcvt(23, 0., 1, 83, 10);
  //~ TAttText atttext_small_hcvb_v(21, 90., 1, 83, 15);
  //~ TAttLine attline_hit_m(4, 1, 1);	    // (col,sty,wid) = blu,sol,1  mis

  TAttLine attline_black_soli_nor(1, 1, 1); // to draw the cell side
  TAttLine attline_trk(2, 1, 1);	    // (col,sty,wid) = red,sol,1  used for track
  TAttFill attfill_hit_sel(2, 1001);	    // (col,sty) = red, solid  use pro for selected hit
  TAttFill attfill_hit_mis(4, 1001);	    // (col,sty) = blu, solid  first miss hit for not selected hit
  TAttFill attfill_hit_dmis(7, 1001);	    // (col,sty) = blu, solid  second miss hit for not selected hit
  TAttFill attfill_hit_tmis(8, 1001);	    // (col,sty) = blu, solid  third miss hit for not selected hit
  TAttFill attfill_hit_fmis(9, 1001);	    // (col,sty) = blu, solid  fourth or over miss hit for not selected hit
  TAttText atttext_small_hlvc(12, 0., 1, 83, 10); //used for hit chi2

  //Pad range
  gPad->Range(-1.02*p_bmgeo->GetWidth(), -0.51*p_bmgeo->GetLength(), +1.02*p_bmgeo->GetWidth(), +0.51*p_bmgeo->GetLength());

  // BM space frame boundary
  Double_t h_x(0), h_y(0), h_z(0), h_cx(0), h_cy(0), h_cz(0);
  double pversx(0),pversy(0);
  double x_target(0), y_target(0);
  double xch_i, xch_f, ych_i, ych_f;
  double zmin, zmax, t_i, t_f;
  char text[200];
  Int_t tmp_int;

  /*
    Starting with Top View (XZ)       View  == 1 (Top view, V view)
  */

  attline_black_soli_nor.Modify();  
  attfill_box.Modify();
  //~ atttext_small_hcvb.Modify();
  gPad->PaintBox(-p_bmgeo->GetWidth(), -0.5*p_bmgeo->GetLength(), 0., +0.5*p_bmgeo->GetLength());
  sprintf(text,"V (XZ)     Evnum=%d     U (YZ)",event_number);
  gPad->PaintText(-p_bmgeo->GetWidth()+0.2, p_bmgeo->GetLength()/2.-0.8,text); 

  /* Drawing the cells */
  for(int il= 0; il<p_bmgeo->GetLayersN(); il++) {
    for(int ic= 0; ic<p_bmgeo->GetCellsN(); ic++) {
      //V view, (X,Z)
      p_bmgeo->GetCellInfo(1, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);//(view,plane,id,wirepos_xyz,wiredir_xyz)
      //~ cout<<"il="<<il<<"  ic="<<ic<<"  hx="<<h_x<<"  h_Y="<<h_y<<"  hz="<<h_z<<"  il="<<il<<"  ic="<<ic<<endl;
      attline_black_soli_nor.Modify();  
      attfill_box.Modify();
      gPad->PaintBox(-p_bmgeo->GetCellWidth()-h_x-0.5*p_bmgeo->GetWidth(), -p_bmgeo->GetCellHeight()+h_z , p_bmgeo->GetCellWidth()-h_x-0.5*p_bmgeo->GetWidth() , p_bmgeo->GetCellHeight()+h_z);      
      //~ cout<<"x_min="<<-p_bmgeo->GetCellWidth()-h_x-0.5*p_bmgeo->GetWidth()<<"  ymin="<<-p_bmgeo->GetCellHeight()+h_z<<"  xmax="<<-h_x-0.5*p_bmgeo->GetWidth()+p_bmgeo->GetCellWidth()<<"  ymax"<<p_bmgeo->GetCellHeight()+h_z<<endl;
    }
  }
  
  //Beam Monitor tracks 
  if (track_ok==0 || track_ok==5) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the BM Tracks");

    for (Int_t i_t = 0; i_t < p_ntutrk->GetTracksN(); i_t++) {
      p_trk = p_ntutrk->Track(i_t);
      sprintf(text,"MyChi2Red: %lf",p_trk->GetMyChi2Red());
      gPad->PaintText(-0.6*p_bmgeo->GetWidth(), +0.40*p_bmgeo->GetLength(),text);

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
      //~ zmin = - p_bmgeo->GetLength()/2;
      //~ zmax = + p_bmgeo->GetLength()/2;
      //~ t_i = (zmin)/pversz;
      //~ t_f = (zmax)/pversz;
      
      //~ xch_i = x_target + pversx*t_i ;
      //~ xch_f = x_target + pversx*t_f ;
      //~ ych_i = y_target + pversy*t_i ;
      //~ ych_f = y_target + pversy*t_f ;
      
      //~ attline_trk_4.Modify();

      //X,Z (view V, TOP view)   
      //(X needs to be reversed SINCE TOP view has x increasing towards left)... ma perchè?
      //~ gPad->PaintLine(-xch_i-0.5*p_bmgeo->GetWidth(),zmin,-xch_f-0.5*p_bmgeo->GetWidth(),zmax);
      attline_trk.Modify();
      gPad->PaintLine(-p_trk->GetMylar1Pos().X()-p_bmgeo->GetWidth()/2.,p_trk->GetMylar1Pos().Z(),-p_trk->GetMylar2Pos().X()-p_bmgeo->GetWidth()/2.,p_trk->GetMylar2Pos().Z());
      //~ cout<<"zmin="<<p_trk->GetMylar1Pos().Z()<<"  zmax="<<p_trk->GetMylar2Pos().Z()<<endl;
    }
  }
  
  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Info("Viewer()","Displaying the BM Hits %2d ",p_nturaw->GetHitsN());

    for (Int_t i=3; i<36;) {
      tmp_int=0;
      for (Int_t k=cell_occupy->at(i).size()-1; k>=0; k--) {
        TABMntuHit* hit = p_nturaw->Hit(cell_occupy->at(i)[k]);
        if(hit->View()==1 && hit->GetIsSelected()) {
          //Top view, V view, (X,Z)
          p_bmgeo->GetCellInfo(1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);//charge the wire position and direction in h_x.. and h_cx...
          //X has a consistent view (x points to the top) 
          attline_black_soli_nor.Modify();  
          attfill_hit_sel.Modify();
          gPad->PaintBox(-(h_x-hit->Dist())-0.5*p_bmgeo->GetWidth(),h_z-hit->Dist(),-(h_x+hit->Dist())-0.5*p_bmgeo->GetWidth(),h_z+hit->Dist());
          //~ gPad->PaintEllipse(-h_x,h_z,hit->Dist());
          atttext_small_hlvc.Modify();
          sprintf(text,"chi2 :: %lf",hit->GetChi2());
          gPad->PaintText(-0.95*p_bmgeo->GetWidth(), h_z, text);
        }
        else if(hit->View()==1 && !hit->GetIsSelected()) {
          //Top view, V view, (X,Z)
          p_bmgeo->GetCellInfo(1, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
          //X has a consistent view (x points to the top) 
          attline_black_soli_nor.Modify();  
          if(tmp_int==0)
            attfill_hit_mis.Modify();
          else if(tmp_int==1)
            attfill_hit_dmis.Modify();
          else if(tmp_int==2)
            attfill_hit_tmis.Modify();
          else
            attfill_hit_fmis.Modify();
          gPad->PaintBox(-(h_x-hit->Dist())-0.5*p_bmgeo->GetWidth(),h_z-hit->Dist(),-(h_x+hit->Dist())-0.5*p_bmgeo->GetWidth(),h_z+hit->Dist());
          tmp_int++;
        }else
          cout<<"in V view: hai sbagliato qualcosa  i="<<i<<"  hit="<<cell_occupy->at(i)[k]<<"  view="<<hit->View()<<endl;//provv
      }
      i+= (i%6==5) ? 4:1; 
    }
  }


  /*
    Proceeding with Side View  U view, view==0
  */

  attline_black_soli_nor.Modify();  
  attfill_box.Modify();
  gPad->PaintBox(0., -0.5*p_bmgeo->GetLength(), p_bmgeo->GetWidth(), +0.5*p_bmgeo->GetLength());
  //~ gPad->PaintText(0, p_bmgeo->GetLength()/2.-1, "Top U view (YZ)");

  /* Drawing the cells */
  for(int il= 0; il<p_bmgeo->GetLayersN(); il++) {
    for(int ic= 0; ic<p_bmgeo->GetCellsN(); ic++) {
      //U view, (Y,Z)
      p_bmgeo->GetCellInfo(0, il, ic, h_x, h_y, h_z, h_cx, h_cy, h_cz);

      //Reverse needed (once again y increase from left to right)... DA CAPIRE SE È VERO O NO...
      h_y *= -1;

      attline_black_soli_nor.Modify();  
      attfill_box.Modify();
      gPad->PaintBox(-p_bmgeo->GetCellWidth()+h_y+0.5*p_bmgeo->GetWidth(), -p_bmgeo->GetCellHeight()+h_z, +p_bmgeo->GetCellWidth()+h_y+0.5*p_bmgeo->GetWidth(), +p_bmgeo->GetCellHeight()+h_z);      
    }
  }
  
  //Beam Monitor tracks
  if (track_ok==0 || track_ok==5) {
    //Displays the fitted tracks
    Info("Viewer()","Displaying the BM Tracks");

    for (Int_t i_t = 0; i_t < p_ntutrk->GetTracksN(); i_t++) {
      p_trk = p_ntutrk->Track(i_t);
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
      //~ zmin = - p_bmgeo->GetLength()/2;
      //~ zmax = + p_bmgeo->GetLength()/2;
      //~ t_i = (zmin)/pversz;
      //~ t_f = (zmax)/pversz;
      
      //~ xch_i = x_target + pversx*t_i ;
      //~ xch_f = x_target + pversx*t_f ;
      //~ ych_i = y_target + pversy*t_i ;
      //~ ych_f = y_target + pversy*t_f ;
      
      //~ attline_trk_4.Modify();
      attline_trk.Modify();

      //Y,Z (view U, Side view)   
      //~ gPad->PaintLine(-ych_i+0.5*p_bmgeo->GetWidth(),zmin,-ych_f+0.5*p_bmgeo->GetWidth(),zmax);
      gPad->PaintLine(-p_trk->GetMylar1Pos().Y()+p_bmgeo->GetWidth()/2.,p_trk->GetMylar1Pos().Z(),-p_trk->GetMylar2Pos().Y()+p_bmgeo->GetWidth()/2.,p_trk->GetMylar2Pos().Z());
    }
  }

  if (p_nturaw) {
    //Displays the Reconstructed hits in the orizontal view
    Info("Viewer()","Displaying the BM Hits %2d ",p_nturaw->GetHitsN());

    for (Int_t i=0; i<33;) {
      tmp_int=0;
      for (Int_t k=cell_occupy->at(i).size()-1; k>=0; k--) {
        TABMntuHit* hit = p_nturaw->Hit(cell_occupy->at(i)[k]);
        //View  == 0 (Side view, U view)
        if(hit->View()==0 && hit->GetIsSelected()) {
          //Side view, U view, (Y,Z)
          p_bmgeo->GetCellInfo(0, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
          //Y has a consistent view (Y points to the top) 
          attline_black_soli_nor.Modify();  
          attfill_hit_sel.Modify();
          gPad->PaintBox(-(h_y-hit->Dist())+0.5*p_bmgeo->GetWidth(),h_z-hit->Dist(),-(h_y+hit->Dist())+0.5*p_bmgeo->GetWidth(),h_z+hit->Dist());
          //~ gPad->PaintEllipse(h_y,h_z,hit->Dist());
          atttext_small_hlvc.Modify();
          sprintf(text,"chi2 :: %lf",hit->GetChi2());
          gPad->PaintText(0.05*p_bmgeo->GetWidth(), h_z, text);
  
        }
        if(hit->View()==0 && !hit->GetIsSelected()) {
          //Side view, U view, (Y,Z)
          p_bmgeo->GetCellInfo(0, hit->Plane(), hit->Cell(), h_x, h_y, h_z, h_cx, h_cy, h_cz);
          //Y has a consistent view (Y points to the top) 
          attline_black_soli_nor.Modify();  
          if(tmp_int==0)
            attfill_hit_mis.Modify();
          else if(tmp_int==1)
            attfill_hit_dmis.Modify();
          else if(tmp_int==2)
            attfill_hit_tmis.Modify();
          else
            attfill_hit_fmis.Modify();
          gPad->PaintBox(-(h_y-hit->Dist())+0.5*p_bmgeo->GetWidth(),h_z-hit->Dist(),-(h_y+hit->Dist())+0.5*p_bmgeo->GetWidth(),h_z+hit->Dist());
        }
      }
      i+= (i%6==2) ? 4:1; 
    }
  }
  return;
}


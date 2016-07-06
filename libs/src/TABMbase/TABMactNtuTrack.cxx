/*!
  \file
  \version $Id: TABMactNtuTrack.cxx,v 1.12 2003/07/08 18:58:35 mueller Exp $
  \brief   Implementation of TABMactNtuTrack.
*/

#include <math.h>

#include "TF1.h"
#include "TABMparCon.hxx"
#include "TABMntuRaw.hxx"
#include "TABMntuTrack.hxx"

#include "TABMactNtuTrack.hxx"

/*!
  \class TABMactNtuTrack TABMactNtuTrack.hxx "TABMactNtuTrack.hxx"
  \brief Track builder for Beam Monitor. **
*/

ClassImp(TABMactNtuTrack);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactNtuTrack::TABMactNtuTrack(const char* name,
				 TAGdataDsc* p_ntutrk,
				 TAGdataDsc* p_ntuhit,
				 TAGparaDsc* p_bmgeo,
				 TAGparaDsc* p_bmcon)
  : TAGaction(name, "TABMactNtuTrack - Track finder for BM"),
    fpNtuTrk(p_ntutrk),
    fpNtuHit(p_ntuhit),
    fpBMGeo(p_bmgeo),
    fpBMCon(p_bmcon)
{
  AddDataOut(p_ntutrk, "TABMntuTrack");
  AddDataIn(p_ntuhit,  "TABMntuRaw");
  AddPara(p_bmgeo,  "TABMparGeo");
  AddPara(p_bmcon,  "TABMparCon");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuTrack::~TABMactNtuTrack()
{}

//------------------------------------------+-----------------------------------
//! Action.
Bool_t TABMactNtuTrack::Action()
{
  TABMntuTrack* p_ntutrk = (TABMntuTrack*) fpNtuTrk->Object();
  TABMntuRaw*   p_ntuhit = (TABMntuRaw*)   fpNtuHit->Object();
  TABMparGeo*   p_bmgeo = (TABMparGeo*)    fpBMGeo->Object();
  TABMparCon*   p_bmcon = (TABMparCon*)    fpBMCon->Object();
  
  TF1 *m_mypol, *m_mypol2;
  m_mypol = p_bmcon->GetCalibX();
  m_mypol2 = p_bmcon->GetCalibY();
  
  p_ntutrk->Clear();

  Double_t chisquare_cut = 5.;

  if (!p_ntutrk->t) p_ntutrk->SetupClones();

  Int_t i_nhit = p_ntuhit->nhit;
  Int_t cell, plane, view, stat;
  Double_t dist;
  //Loop on all the wires
  int nass(0);
  
  TABMntuTrackTr aTr, aSrTr, aTr2;
  aTr.Clean();   aSrTr.Clean();   aTr2.Clean();
  int HitMult[36], my_idx, my_view, k;
  int SingleHit[36];
  int DoubleHit[36];
  
  for(int id = 0; id<36; id++) 
    {HitMult[id] = 0; SingleHit[id] = 0;DoubleHit[id] = 0;}
  int trackHits(0);
  int trackHits2(0);
  
  int trackHitsTop(0);
  int trackHitsSide(0);
  
  int OccuPlane[2][6];
  for(int iv = 0; iv<2; iv++) {
    for(int ip = 0; ip<6; ip++) {
      OccuPlane[iv][ip] = 0;
    }
  }
  for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
    TABMntuHit* p_hit = p_ntuhit->Hit(i_h);
    // time ns and distances in cm
    cell = p_hit->Cell();
    plane = p_hit->Plane();
    view = p_hit->View();
    view > 0 ? my_view = 0 : my_view = 1;
    OccuPlane[my_view][plane]++;
  }
  
  for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
    TABMntuHit* p_hit = p_ntuhit->Hit(i_h);
    // time ns and distances in cm
    cell = p_hit->Cell();
    plane = p_hit->Plane();
    view = p_hit->View();
    
    dist = p_hit->Dist();
    view > 0 ? my_view = 0 : my_view = 1;
    my_idx = my_view*18+plane*3+cell;
    HitMult[my_idx]++;
    
    //Keep only the smallest time :: should become closest to trig
    Info("Action()"," \t %d \t %lf \t %d \t %d \t %d \t %lf \t %d\n",i_h,dist,cell,plane,view,p_hit->Tdrift(),HitMult[my_idx]);
    
    //Avoid to use the planes where more than 1 hit is reconstructed
    if(HitMult[my_idx]<=1 && OccuPlane[my_view][plane]<2 && dist<0.8) {
      
      SingleHit[trackHits]=i_h;
      // cout<< "trackhits " <<trackHits<<endl;
      
      stat = aTr.AddHit(p_hit, p_bmgeo);
      p_hit->SetTrkAss(1);
      nass++; //Hits associated to the track candidate
      aTr.SetAssHits(nass);
      stat = aTr.ComputeDataWire(p_hit,trackHits);
      trackHits++;
      
      if(my_view==0){trackHitsTop++;}
      if(my_view==1){trackHitsSide++;}
      
    }
    
    //Storing the index of the doulbe-hits
    if(HitMult[my_idx]<=1 && OccuPlane[my_view][plane]>=2  && dist<0.8) { 
      DoubleHit[trackHits2]=i_h;
      trackHits2++;
    }
    
  }
  
  if(trackHitsTop>2 && trackHitsSide>2) {
    //  if(trackHits>6) {
    stat = aTr.EstimateTrackPar(p_ntuhit,p_bmgeo);
    stat = aTr.ComputeDataAll(p_ntuhit);    
    
    //Chi2 fit
    p_ntutrk->Chi2Fit(p_ntuhit,&aTr,3);
    //Sort double hits
    sortDoubleHits(p_ntuhit,p_ntutrk,p_bmgeo,aSrTr,aTr,DoubleHit,trackHits2);
    
    //Before that we have to choose which track is the best one
    Info("Action()","After Fit:: %lf %lf %lf %lf",
	 aSrTr.GetX0(),aSrTr.GetY0(),aSrTr.GetUx(),aSrTr.GetUy());
    

    if(aSrTr.GetChi2()<chisquare_cut) {    
      aSrTr.Calibrate(m_mypol,m_mypol2);
      new((*(p_ntutrk->t))[p_ntutrk->ntrk]) 
	TABMntuTrackTr(aSrTr);
      p_ntutrk->ntrk = 1;
      
    } else {
      
      //ELIMINATE HITS TOP VIEW
      /*cout <<"chi trk "<< aSrTr.GetChi2()<<endl;*/
      
      double min_aTr2 = aSrTr.GetChi2();
      k = -1;
      for(int j = 0; j<i_nhit; j++) {
	TABMntuHit* p_hitT = p_ntuhit->Hit(j);
	plane = p_hitT->Plane();
	view = p_hitT->View();
	view > 0 ? my_view = 0 : my_view = 1; 
	// cout<< " j " << j<<" plane " << plane<<endl;
	
	if(!my_view && trackHitsTop>3 && p_hitT->TrkAss()) {
	  /*cout<< " j " << j<<" plane " << plane<<endl;*/
	  
	  refitImprovedTrackTop (p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,j);
	  
	  // cout<< " chi2_top " <<aTr2.GetChi2()<<" min_chi2 "<<min_aTr2<<endl;
	  
	  if (aTr2.GetChi2()<min_aTr2) {
	    min_aTr2 = aTr2.GetChi2();
	    /*cout<< j<< " j "<<" ********SCRIVO LA TRACCIA****** chi2 piu' piccolo " << min_aTr2<<endl;*/
	    k = j;
	  }
	  p_hitT->SetTrkAss(1); 
	}
	
      }
      if(k>-1)
	refitImprovedTrackTop (p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,k);

      if(aTr2.GetChi2()<chisquare_cut) {
	aTr2.Calibrate(m_mypol,m_mypol2);
	new((*(p_ntutrk->t))[p_ntutrk->ntrk]) 
	  TABMntuTrackTr(aTr2);      
	p_ntutrk->ntrk = 1;	
      }
      
      else {
	
	//ELIMINATE HITS SIDE VIEW
	k = -1;
	for(int j = 0; j<i_nhit; j++) {
	  TABMntuHit* p_hitT = p_ntuhit->Hit(j);
	  plane = p_hitT->Plane();
	  view = p_hitT->View();
	  view > 0 ? my_view = 0 : my_view = 1; 
	  // cout<< " j " << j<<" plane " << plane<<endl;
	  
	  if(my_view && trackHitsSide>3 && p_hitT->TrkAss()) {
	    
	    /*cout<< " j " << j<<" plane side" << plane<<endl;*/
	    
	    refitImprovedTrackSide(p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,j);
	    
	    // cout<< " chi2_side" <<aTr2.GetChi2()<<" min_chi2 "<<min_aTr2<<endl;
	    
	    if (aTr2.GetChi2()<min_aTr2) {
	      min_aTr2 = aTr2.GetChi2();
	      /*cout<< j<< " j "<<" ********SCRIVO LA TRACCIA della SIDE****** chi2 piu' piccolo " << min_aTr2<<endl;*/
	      k = j;
	    }
	    p_hitT->SetTrkAss(1); 
	  }
	}
	if(k>-1)
	  refitImprovedTrackSide (p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,k);
	
      }
      
      if(aTr2.GetChi2()<chisquare_cut) {
	aTr2.Calibrate(m_mypol,m_mypol2);
	new((*(p_ntutrk->t))[p_ntutrk->ntrk]) 
	  TABMntuTrackTr(aTr2);      
	p_ntutrk->ntrk = 1;
      }
      
      else {
	/*cout <<aTr2.GetChi2()<<" chi2 "<<" ****** STO CERCANDO DI RIFITTARE*****"<<endl;*/
	//ELIMINATE HITS TOP VIEW
	/*cout <<"chi trk "<< aSrTr.GetChi2()<<endl;*/
	
	double min_aTr2 = aTr2.GetChi2();
	k = -1;
	for(int j = 0; j<i_nhit; j++) {  
	  TABMntuHit* p_hitT = p_ntuhit->Hit(j);
	  plane = p_hitT->Plane();
	  view = p_hitT->View();
	  view > 0 ? my_view = 0 : my_view = 1; 
	  // cout<< " j " << j<<" plane " << plane<<endl;
	  
	  if(!my_view && trackHitsTop>3 && p_hitT->TrkAss()) {
	    
	    /*cout<< " j " << j<<" plane " << plane<<endl;*/
	    
	    refitImprovedTrackTop (p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,j);
	    
	    // cout<< " chi2_top " <<aTr2.GetChi2()<<" min_chi2 "<<min_aTr2<<endl;
	    
	    if (aTr2.GetChi2()<min_aTr2) {
	      min_aTr2 = aTr2.GetChi2();
	      /*cout<< j<< " j "<<" ********SCRIVO LA TRACCIA****** chi2 piu' piccolo " << min_aTr2<<endl;*/
	      k = j;
	    }
	    p_hitT->SetTrkAss(1); 
	  }
	}
	if(k>-1)
	  refitImprovedTrackTop (p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,k);

	if(aTr2.GetChi2()<chisquare_cut) {
	  aTr2.Calibrate(m_mypol,m_mypol2);
	  new((*(p_ntutrk->t))[p_ntutrk->ntrk]) 
	    TABMntuTrackTr(aTr2);      
	  p_ntutrk->ntrk = 1; 
	}
	
	else {
	  
	  //ELIMINATE HITS SIDE VIEW
	  k = -1;
	  for(int j = 0; j<i_nhit; j++) {
	    TABMntuHit* p_hitT = p_ntuhit->Hit(j);
	    plane = p_hitT->Plane();
	    view = p_hitT->View();
	    view > 0 ? my_view = 0 : my_view = 1; 
	    // cout<< " j " << j<<" plane " << plane<<endl;
	    
	    if(my_view && trackHitsSide>3 && p_hitT->TrkAss()) {
	      
	      /*cout<< " j " << j<<" plane side" << plane<<endl;*/
	      
	      refitImprovedTrackSide(p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,j);
	      
	      // cout<< " chi2_side" <<aTr2.GetChi2()<<" min_chi2 "<<min_aTr2<<endl;
	      
	      if (aTr2.GetChi2()<min_aTr2) {
		min_aTr2 = aTr2.GetChi2();
		/*cout<< j<< " j "<<" ********SCRIVO LA TRACCIA della SIDE****** chi2 piu' piccolo " << min_aTr2<<endl;*/
		k = j;
	      }
	      p_hitT->SetTrkAss(1); 
	    }
	  }
	  if(k>-1)
	    refitImprovedTrackSide (p_ntuhit,p_ntutrk,p_bmgeo,trackHitsTop,trackHitsSide,aSrTr.GetChi2()*nass,aTr2,k);
	  
	}
	// cout <<aTr2.GetChi2()<<" chi2 "<<" ****** DOPO IL FIT NUOVO *****"<<endl;
	aTr2.Calibrate(m_mypol,m_mypol2);
	new((*(p_ntutrk->t))[p_ntutrk->ntrk]) 
	  TABMntuTrackTr(aTr2);      
	p_ntutrk->ntrk = 1;	
      }
    }
  }
 
  fpNtuTrk->SetBit(kValid);
  return kTRUE;
}

void TABMactNtuTrack::sortDoubleHits(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, TABMntuTrackTr &aSrTr, TABMntuTrackTr &RefTr, int add[36], int ndou)
{
  
  int nass(0), trackHits(0);
  Int_t cell, plane, view, stat, my_view;
  double diffX;
  double diffY;
  
  Int_t i_nhit = hp->nhit;
  
  //Create an improved track
  for(int j=0; j<i_nhit; j++) {
    
    TABMntuHit* dp_hit = hp->Hit(j);
    cell = dp_hit->Cell();
    plane = dp_hit->Plane();
    view = dp_hit->View();
    view > 0 ? my_view = 0 : my_view = 1;
    
    int w2 = ppg->GetID(cell);
    double z2=ppg->GetZ(w2,plane,my_view);
    
    TVector3 bs=RefTr.PointAtLocalZ(z2);
    
    for(int i=0; i<ndou; i++) { 
      if(j == add[i]) {
	double p2=ppg->GetY(w2,plane,my_view);
	if(my_view==0) {
	  diffY = p2 - bs.Y();	
	  if(TMath::Abs(diffY) < 0.8) {
	    dp_hit->SetTrkAss(1);
	  }
	}
	
	if(my_view==1) {
	  double p2=ppg->GetX(w2,plane,my_view);
	  diffX = p2 - bs.X();	
	  if(TMath::Abs(diffX) < 0.8) {
	    dp_hit->SetTrkAss(1);
	  }
	}
	
      } 
    }
    
    
    if(dp_hit->TrkAss()) {
      stat = aSrTr.AddHit(dp_hit, ppg);
      nass++; //Hits associated to the track candidate
      aSrTr.SetAssHits(nass);
      stat = aSrTr.ComputeDataWire(dp_hit,trackHits);
      trackHits++;
    }
    
  }    
  
  
  stat = aSrTr.EstimateTrackPar(hp,ppg);
  stat = aSrTr.ComputeDataAll(hp);    
  //Chi2 fit
  pntr->Chi2Fit(hp,&aSrTr,3);
  
  /*  Before that we have to choose which track is the best one*/
  Info("Action()","After Re-Fit:: %lf %lf %lf %lf",
       aSrTr.GetX0(),aSrTr.GetY0(),aSrTr.GetUx(),aSrTr.GetUy());
  
  return;
  
}

void TABMactNtuTrack::refitImprovedTrackTop (TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, int mulT, int mulS, double trchi2, TABMntuTrackTr &aRTr, int j)
{
  
  aRTr.Clean(); 
  
  Int_t view, stat, my_view, plane, k;
  Double_t hchi2;
  //Loop on all the wires
  int nass(0), trackHits(0);  
  Int_t i_nhit = hp->nhit;
  
  // 	double min_trchi2 = aSrTr.GetChi2();
  
  //Create an improved track
  for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
    TABMntuHit* p_hit = hp->Hit(i_h);
    
    // time ns and distances in cm
    view = p_hit->View();
    view > 0 ? my_view = 0 : my_view = 1;
    
    //Remove hit that has to be removed.
    
    if(!my_view && mulT>3) {
      plane = p_hit->Plane();
      hchi2 = p_hit->GetChi2();
      
      /*    hchi2_sum_U += hchi2;*/
      // cout <<" nhit "<<i_h<<" chi2hit_U "<<hchi2<<" hchi2_sum_U " <<hchi2_sum_U<< " plane " << plane<<endl;
      
      if(i_h==j && p_hit->TrkAss()) {
	k = i_h;
	p_hit->SetTrkAss(0); 
	mulT--;
	/*      cout <<i_h<<" i_h track "<< view <<" view "<<plane <<" plane "<< " MultT "<<mulT<<endl;*/
	plane = p_hit->Plane();
	// cout<<" nhit "<<i_h <<" plane out " << plane<<endl;
      } 
      
      if (i_h<j && !my_view && k == i_h)
	{
	  // p_hit->SetTrkAss(1); 
	  // mulT++;
	  // 	cout <<" Set to 1 !"<<endl;
	  // 	cout<<" nhit "<<i_h <<" plane " << plane<<" MultT "<<mulT<<endl;
	}
    }    
    
    //Avoid to use the planes where more than 1 hit is reconstructed
    if(p_hit->TrkAss()) {
      stat = aRTr.AddHit(p_hit, ppg);
      nass++; //Hits associated to the track candidate
      aRTr.SetAssHits(nass);
      stat = aRTr.ComputeDataWire(p_hit,trackHits);
      trackHits++;
    }
  }
  
  stat = aRTr.EstimateTrackPar(hp,ppg);
  stat = aRTr.ComputeDataAll(hp);    
  
  //Chi2 fit
  pntr->Chi2Fit(hp,&aRTr,3); 
  
  //Before that we have to choose which track is the best one
  Info("Action()","After Re-Fit:: %lf %lf %lf %lf",
       aRTr.GetX0(),aRTr.GetY0(),aRTr.GetUx(),aRTr.GetUy());
  
  return;
  
}

void TABMactNtuTrack::refitImprovedTrackSide(TABMntuRaw *hp, TABMntuTrack* pntr, TABMparGeo* ppg, int mulT, int mulS, double trchi2, TABMntuTrackTr &aRTr, int j)
{
  
  aRTr.Clean(); 
  
  Int_t view, stat,  my_view, plane, k;
  Double_t hchi2;
  //Loop on all the wires
  int nass(0), trackHits(0);
  
  /*double hchi2_sum_U (0), hchi2_sum_V(0), chi2_trk(0);*/
  Int_t i_nhit = hp->nhit;
  
  // 	double min_trchi2 = aSrTr.GetChi2();
  
  //Create an improved track
  for (Int_t i_h = 0; i_h < i_nhit; i_h++) {
    TABMntuHit* p_hit = hp->Hit(i_h);
    
    // time ns and distances in cm
    view = p_hit->View();
    view > 0 ? my_view = 0 : my_view = 1;
    
    //Remove hit that has to be removed.
    
    if(my_view && mulS>3) {
      
      plane = p_hit->Plane();
      hchi2 = p_hit->GetChi2();
      
      /*hchi2_sum_U += hchi2; */
      // cout <<" nhit "<<i_h<<" chi2hit_U "<<hchi2<<" hchi2_sum_U " <<hchi2_sum_U<< " plane " << plane<<endl;
      
      if(i_h==j && p_hit->TrkAss()) {
	k = i_h;
	p_hit->SetTrkAss(0); 
	mulS--;
	/*cout <<i_h<<" i_h track "<< view <<" view "<<my_view << " my_view "<<plane <<" plane "<<" MultS "<<mulS<<endl;*/
	plane = p_hit->Plane();
	// cout<<" nhit "<<i_h <<" plane out " << plane<<endl;
      } 
      
      if (i_h<j && my_view && k == i_h)
	{
	  // p_hit->SetTrkAss(1); 
	  // mulT++;
	  // cout <<" Set to 1 !"<<endl;
	  // cout<<" nhit "<<i_h <<" plane " << plane<<" MultS "<<mulS<<endl;
	}
    }    
    
    //Avoid to use the planes where more than 1 hit is reconstructed
    if(p_hit->TrkAss()) {
      stat = aRTr.AddHit(p_hit, ppg);
      nass++; //Hits associated to the track candidate
      aRTr.SetAssHits(nass);
      stat = aRTr.ComputeDataWire(p_hit,trackHits);
      trackHits++;
    }
  }
  
  stat = aRTr.EstimateTrackPar(hp,ppg);
  stat = aRTr.ComputeDataAll(hp);    
  
  //Chi2 fit
  pntr->Chi2Fit(hp,&aRTr,3); 
  
  //Before that we have to choose which track is the best one
  Info("Action()","After Re-Fit:: %lf %lf %lf %lf",
       aRTr.GetX0(),aRTr.GetY0(),aRTr.GetUx(),aRTr.GetUy());
  
  return;
  
}

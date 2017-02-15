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
using namespace std;
#include <iostream>
#include "TABMactNtuTrack.hxx"
#include "FieldManager.h"
#include "MaterialEffects.h"
#include "TGeoMaterialInterface.h"
#include "ConstField.h"
#include "Track.h"
#include "WireMeasurement.h"
#include "EventDisplay.h"
#include "TDatabasePDG.h"
#include "DAF.h"


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


  const Int_t nIter = 20; // max number of iterations
  const Double_t dPVal = 1.E-3; // convergence criterion used by GenFit

  simpleFitter = new KalmanFitter(nIter, dPVal);
  refFitter = new KalmanFitterRefTrack(nIter, dPVal);   
  dafRefFitter = new DAF(true, nIter, dPVal);
  dafSimpleFitter = new DAF(false, nIter, dPVal);

  const double BField = 0.;       // kGauss [ --> Eq. to 0.1 T]

  // init geometry and mag. field
  
  //f_BMgeoMan = p_bmgeo->GetGeoManager();

  TABMparGeo*   geo = (TABMparGeo*)    fpBMGeo->Object();
  geo->AddBM("BeamMonitor");

  new TGeoManager("Geometry", "Geane geometry");//load geometry
  TGeoManager::Import("LocalBM.root");
  
  //Bfield is along Y in our case.
  
  FieldManager::getInstance()->init(new ConstField(0.,BField,0.)); //forse servirà il campo magnetico fatto bene, ma per ora non c'è nella simulazione
  MaterialEffects::getInstance()->init(new TGeoMaterialInterface());
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactNtuTrack::~TABMactNtuTrack()
{
  delete simpleFitter;
  delete refFitter;
  delete dafSimpleFitter;
  delete dafRefFitter;
}


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

  
//******************************************************NEW tracking********************************************
        
  //parameters
  Int_t BMdebug=0;
  Int_t readyToFit = 3; //to fit (0=not fit, 1=simpleFitter, 2=refFitter, 3=dafSimpleFitter, 4=dafRefFitter)
  bool vediamo=false;  	//to use the eventDisplay of Genfit (not implemented now)
  Double_t chi2cut=200.; //temporary, an optimum chi2cut value isn't found yet (load it from beamconfig?)
  Double_t wire_err=0.003; //temporary, is the error of the wire position 
  Double_t maxError=0.5;  //temporary, used in checkTr, is the error tollerance of the fitted primary position and the exact primary position (MC) 
  
  Double_t tmp_double, res;
  bool onlyPrimary, converged;
  TABMntuTrackTr tmp_trackTr;
  Double_t x, y, z, cx, cy, cz;
  Double_t sign = 1.; //Charged particles (probably useless)
  Int_t pdg = 2212; //proton=2212 
  AbsTrackRep* rep = new RKTrackRep(sign*pdg);
  
  Int_t hit_view, tmp_int, tmp_cellx=1, tmp_celly=1, tracknum=1;
  const Int_t det_Id = 1; //beam monitor Id (useless parameter need for genfit)
  TMatrixDSym hitCov(7);
  TVectorD hitCoords(7);
  vector<vector<Int_t>> hitxplane(p_bmgeo->GetLayersNumber()*2); //number of hit for every bm plane (plane should be 12 in BM)
  TABMntuHit* p_hit;
  Int_t firedPlane=p_bmgeo->GetLayersNumber()*2; //number of plane fired
    
  TVector3 priexit(0,0,100); //primary last release in BMN coordinates (from MC)
  TVector3 init_pos(0.,0.,p_bmgeo->GetCenter().z()-p_bmgeo->GetLength());
  TVector3 init_mom(0.,0.,1.);//initial position and momentum for tracking, for BM porpouse the primary track should always have these inital values

  Track fitTrack(rep, init_pos, init_mom);

  if(i_nhit==0)
    readyToFit=0;
  
  //counter for number of possible tracks:
  for(Int_t i_h = 0; i_h < i_nhit; i_h++) {
    p_hit = p_ntuhit->Hit(i_h);
    if(ToBeConsider(p_hit->Cell(), p_hit->View(), p_hit->Plane()))
      hitxplane[p_bmgeo->GetWirePlane(p_hit->Plane(),p_hit->View())].push_back(i_h);
    }
  for(Int_t j = 0; j < hitxplane.size(); j++) {  
    if(hitxplane[j].size()!=0)
      tracknum*=hitxplane[j].size();
    else
      firedPlane--;
    }
  
  //print hitxplane
  if(BMdebug>=3){  
    cout<<"print hitxplane"<<endl;  
    Print_matrix(hitxplane);    
    }
    
  vector<TABMntuTrackTr> alltrack; 
  vector<bool> possiblePrimary(tracknum, true);
  vector<vector<Int_t>> hitxtrack(tracknum); 
  for(Int_t j=0; j<tracknum; j++)
    hitxtrack[j].resize(firedPlane);  

  tmp_int=0;
  Int_t flip=1, jump;
  
  //charge all possible tracks
  for(Int_t i=0; i<hitxplane.size(); i++) {
    if(hitxplane[i].size()!=0){
      flip*=hitxplane[i].size();
      jump=hitxtrack.size()/flip;
      for(Int_t k=0; k<hitxplane[i].size(); k++) {
        for(Int_t j=0; j<hitxtrack.size();) {
          if(k==0){
            hitxtrack[j][tmp_int]=hitxplane[i][k];
            j++;
            }
          if(k!=0){
            for(Int_t jj=j; jj<j+jump;jj++) 
              hitxtrack[jj][tmp_int]=hitxplane[i][k];
            j+=jump*2;  
            }
          }
        }
      tmp_int++;
      }
    }

  
  //check for possible tracks: reject tracks in witch there is hits on the same view that are too different
  tmp_int=0;
  for(Int_t i=0; i<hitxtrack.size(); i++) {
    tmp_cellx=1;
    tmp_celly=1;
    for(Int_t j=0; j<hitxtrack[i].size(); j++) {
      if(possiblePrimary[i]==true){
        p_hit = p_ntuhit->Hit(hitxtrack[i][j]);
        if(p_hit->View()==1 && tmp_cellx==1 && p_hit->Cell()!=1)
          tmp_cellx=p_hit->Cell();
        if(p_hit->View()==-1 && tmp_celly==1 && p_hit->Cell()!=1)
          tmp_celly=p_hit->Cell();
        if(p_hit->View()==1 && tmp_cellx!=1 && p_hit->Cell()!=1 && p_hit->Cell()!=tmp_cellx)
          possiblePrimary[i]=false;
        if(p_hit->View()==-1 && tmp_celly!=1 && p_hit->Cell()!=1 && p_hit->Cell()!=tmp_celly)
          possiblePrimary[i]=false;
        }
      }
    if(possiblePrimary[i]==true)
      tmp_int++;  
    }
    
  if((tracknum>1 && BMdebug>1) || BMdebug>=3)
    cout<<"number of total hits="<<i_nhit<<"   number of possible tracks="<<tmp_int<<"  number of fired plane="<<firedPlane<<endl;  

  if(tmp_int==0 || firedPlane<8){
    cout<<"no possible track!"<<endl;
    readyToFit=0;
    }
    
  //print hitxtrack
  if(BMdebug>=3){
    cout<<"print hitxtrack"<<endl;  
    Print_matrix(hitxtrack);
    }
  
  if(BMdebug>=3)
    cout<<"index saved, going to loop on all tracks"<<endl;    
  
  //loop on all possible tracks:
  for(Int_t i=0; i<tracknum; i++){
    
    if(possiblePrimary[i]){
      
      if(BMdebug>=2)
        cout<<"charging track number "<<i<<endl;
     
      tmp_trackTr.Clean(); 
      tmp_trackTr.SetNhit(firedPlane); 
              
      //charge hits
      for(Int_t i_h = 0; i_h <firedPlane ; i_h++) {
        
        Info("Action()","create WireHit");
        onlyPrimary=true;
    
        p_hit = p_ntuhit->Hit(hitxtrack[i][i_h]);
        
        if(p_bmcon->IsMC()){
          if(p_hit->GetIdmon()!=1) //there is a hit from a non primary particle
            onlyPrimary=false;
          else if(priexit.Z()==100)
            priexit=p_hit->GetPosm2(); //per ora lo carico ogni volta, poi sistemare meglio
          }
        
        hit_view=(p_hit->View()==1) ? 0:1;
        x = p_bmgeo->GetX(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view);
        y = p_bmgeo->GetY(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view);
        z = p_bmgeo->GetZ(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view);
    
        cx = p_bmgeo->GetCX(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view);
        cy = p_bmgeo->GetCY(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view);
        cz = p_bmgeo->GetCZ(p_bmgeo->GetID(p_hit->Cell()),p_hit->Plane(),hit_view);	    
          
        hitCoords(0)=x;
        hitCoords(1)=y;
        hitCoords(2)=z;
        hitCoords(3)=x+cx;
        hitCoords(4)=y+cy;
        hitCoords(5)=z+cz;
        hitCoords(6)= p_hit->Dist();
        
        res=p_bmcon->ResoEval(p_hit->Dist());
        hitCov.UnitMatrix();         // matrice di covarianza da settare meglio: per ora metto solo matrice diagonale con errore su posizione fili 
        hitCov *= wire_err*wire_err; //ed errore su rdrift, manca studio sulla correlazione tra le componenti... ma forse non serve
        hitCov[6][6]=res*res; 
        //hitCov*=res*res;
            
        if(BMdebug>=3){
          cout<<"show charging BM data:"<<endl;
          cout<<"x:"<<x<<"  y:"<<y<<"  z:"<<z<<endl;
          cout<<"cx:"<<cx<<"  cy:"<<cy<<"  cz:"<<cz<<endl;
          cout<<"view"<<p_hit->View()<<"  rdrift="<<p_hit->Dist()<<endl;
          }
          
        AbsMeasurement* measurement = new WireMeasurement(hitCoords, hitCov, det_Id, i_h, new TrackPoint(&fitTrack));
        fitTrack.insertMeasurement(measurement); 	 
        }//end of charge hits loop
      
      //fitting!
      if(readyToFit!=0) { 

        if(BMdebug>=3)
          cout<<"ready to fit="<<readyToFit<<endl;
        fitTrack.checkConsistency();
        
        if(readyToFit==1)  simpleFitter->processTrack(&fitTrack); 
        if(readyToFit==2)  refFitter->processTrack(&fitTrack); 
        if(readyToFit==3)  dafSimpleFitter->processTrack(&fitTrack);
        if(readyToFit==4)  dafRefFitter->processTrack(&fitTrack);
        
        fitTrack.checkConsistency();
        if(BMdebug>=3)
          cout<<"end of fitting"<<endl;
        
        converged=fitTrack.getFitStatus(rep)->isFitConverged();
        
        if(converged){
          tmp_trackTr.SetChi2New(fitTrack.getFitStatus(rep)->getChi2());
          tmp_trackTr.SetNdf(fitTrack.getFitStatus(rep)->getNdf());  
          tmp_trackTr.SetFailedPoint(fitTrack.getFitStatus(rep)->getNFailedPoints());  
          if(fitTrack.getFitStatus(rep)->getNdf()!=0)        
            tmp_trackTr.SetChi2NewRed((double) fitTrack.getFitStatus(rep)->getChi2()/fitTrack.getFitStatus(rep)->getNdf());          
          }
        else{
          tmp_trackTr.SetChi2New(990);
          tmp_trackTr.SetNdf(0);
          tmp_trackTr.SetFailedPoint(firedPlane);
          tmp_trackTr.SetChi2NewRed(100);          
          }
          
        if(tmp_trackTr.GetChi2New()>990)  //lo faccio perchè sennò ho grafici chi2 troppo larghi
          tmp_trackTr.SetChi2New(980);
        if(tmp_trackTr.GetChi2NewRed()>100)  //lo faccio perchè sennò ho grafici chi2red troppo larghi
          tmp_trackTr.SetChi2NewRed(100);
              
        if(p_bmcon->IsMC())
          tmp_trackTr.MCcheckTr(chi2cut, onlyPrimary, converged, priexit, p_bmcon->GetMylar2cut(), fitTrack, BMdebug, maxError);
    
        if(BMdebug>=2 && converged){
          cout<<"print fit status:"<<endl;
          fitTrack.getFitStatus(rep)->Print();
          }
      }//end of fitting (readytofit)
      
      alltrack.push_back(tmp_trackTr);
  
  }
}//end of loop on all possible track
  
  if(alltrack.size()!=0){
    tmp_int=0;
    tmp_double=alltrack[0].GetChi2New();
    for(Int_t i=1;i<alltrack.size();i++){
      if(tmp_double>alltrack[i].GetChi2New()){
        tmp_double=alltrack[i].GetChi2New();
        tmp_int=i;
        }
      }
    }
  else{
    alltrack.push_back(tmp_trackTr);
    tmp_int=0;
    }
  
  if(BMdebug>=3)
    cout<<"tracks with smallest chi2="<<tmp_int<<" alltrack.size="<<alltrack.size()<<endl;  
        
  //set and store trackTr parameters:
  new((*(p_ntutrk->t))[p_ntutrk->ntrack]) TABMntuTrackTr(alltrack[tmp_int]);
    p_ntutrk->ntrack = 1;  
  
  if(BMdebug>=3)
    cout<<"end of NEW tabmactntutrack"<<endl;
  
  
  fpNtuTrk->SetBit(kValid);
  return kTRUE; //in this way I cut off the old tracking
  
// ************************************************** OLD TRACKING*********************************************
  

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
 
  cout<<"end of action in tabmactntutrack"<<endl;
  fpNtuTrk->SetBit(kValid);
  return kTRUE;
}


//*******************************NEW TRACKING*********************************
//to print matrix
void TABMactNtuTrack::Print_matrix(vector<vector<int>>& vec){
  for(int i=0;i<vec.size();i++){
    for(int j=0;j<vec[i].size();j++){
      cout<<vec[i][j]<<" ";
      }
    cout<<endl;
    }  
  cout<<endl;  
return;  
}

//primary particle shouldn't have any hit in some cell (cell very far from x=0, y=0)
//this method return true if the cell can be hit by primary and false if not (it need view==1 || view==-1)
bool TABMactNtuTrack::ToBeConsider(const Int_t cell, const Int_t view, const Int_t lay){
  bool consider=true;
  if((lay%2==0 && cell==2 && view==1) || (lay%2==1 && cell==0 && view==1) || (lay%2==0 && cell==0 && view==-1) || (lay%2==1 && cell==2 && view==-1))  
    consider=false;
  return consider;
}

//*********************OLD TRACKING************************


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





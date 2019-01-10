#include "GlobalTrack.hxx"
#include "TRandom3.h"
#include "TADIparGeo.hxx"
#include "TAGgeoTrafo.hxx"

ClassImp( GlobalTrack);

Bool_t GlobalTrack::fgClusterTofEnable   = true;
Bool_t GlobalTrack::fgEnTofSingleChCalib = true;
Bool_t GlobalTrack::fgBmVtxMatch         = true;

//
// weights on Delta(Y) and Delta(ch) in the scoring function
// used for the TOF/VTX matches (VM 20/3/14)
Double_t GlobalTrack::fgW_delta_y = 1.0;       
Double_t GlobalTrack::fgW_delta_ch = 8.0;      


/*******************************************************************************/
GlobalTrack::GlobalTrack(MagneticField* Field) {//pass set-up parameters: geometry, magnetic field

  fFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());	
  if (fFirstGeo==NULL) {
    cout << "GlobalTrack::MakeGlobalTracksForward TAGgeoTrafo not found" << endl;
    exit(1);
  }
  // lad the parameters of the entrance collimator geometry
    TVector3 x_al;
  x_al=fFirstGeo->GetALCenter();
  z_coll=x_al[2]-DIST_AL_CO_Z+AL_CO_W_THICK+AL_CO1_THICK;
  rad_coll=AL_CO1_RAD_I;

  fField = Field;
  //  fTraj = new Trajectory(Field);

  fIsUsingForward = false;
  fDebugLevel = 0;

  //init
  fMomChange   = .02;
  fMaxLoopIter = 1000;
  fSavePath    = 10.;//save path in B-field every 10cm
	
  TADIparGeo geoDipole;
	
  fTofwFrontPlanePosition      = fFirstGeo->FromTWFLocalToGlobal(TVector3(0., 0., 0.));  // VM ok, but in global rf
  fTofwFrontPlaneNormal        = fFirstGeo->VecFromTWFLocalToGlobal(TVector3(0., 0., 1.));  // VM ok but in global reference frame

  fTofwRearPlanePosition       = fFirstGeo->FromTWRLocalToGlobal(TVector3(0., 0., 0.));
  fTofwRearPlaneNormal         = fFirstGeo->VecFromTWRLocalToGlobal(TVector3(0., 0., 1.));

  TVector3 x_twf, rot_twf;
  x_twf=fFirstGeo->GetTWFCenter();
  rot_twf=fFirstGeo->GetTWFAngles();
  TVector3 x_twr, rot_twr;
  x_twr=fFirstGeo->GetTWRCenter();
  rot_twr=fFirstGeo->GetTWRAngles();
  Double_t ang;
  Double_t mm;
  Double_t qq;
  ang=PI/2.+rot_twf[0]*PI/180.;
  if (std::abs(ang)>1.E-12) {
    mm=std::tan(ang);
    qq=x_twf[0]-mm*x_twf[2];
    fDistanceBetweenWallsOfTofw=std::abs(-mm*x_twr[2]+x_twr[0]-qq)/std::sqrt(1+pow(mm,2));;
  } else {
    fDistanceBetweenWallsOfTofw=std::abs(x_twf[2]-x_twr[2]);
  }

  //fDistanceBetweenWallsOfTofw  = fFirstGeo->GetTWFCenter().Z() - fFirstGeo->GetTWRCenter().Z();
  //cout << "DistanceBetweenWallsOfTofw=" << fDistanceBetweenWallsOfTofw << endl;
  //cout << endl;

	
  fAladinEntrancePlanePosition = fFirstGeo->FromALLocalToGlobal(TVector3(0., 0., -geoDipole.GetLength()/2.));
  fAladinEntrancePlaneNormal   = fFirstGeo->VecFromALLocalToGlobal(TVector3(0.,0., 1.));//in ALADiN RF
	
  fAladinExitPlanePosition     = fFirstGeo->FromALLocalToGlobal(TVector3(0., 0., geoDipole.GetLength()/2.));
  fAladinExitPlaneNormal       = fFirstGeo->VecFromALLocalToGlobal(TVector3(0.,0., 1.));
	
  // not needed to transform in global (used in local AD in TrackableParticle::PropagateInField (VM))
  //fAladinBoxPosition           = fFirstGeo->FromALLocalToGlobal(TVector3(0., 0., 0.));
  fAladinBoxPosition           = TVector3(0., 0., 0.);
  fAladinBoxSize               = TVector3(geoDipole.GetWidth()+0.0001, geoDipole.GetHeight()+0.0001, geoDipole.GetLength()+0.0001);//in aladin RF
	

  fInteractionPoint            = fFirstGeo->FromVTLocalToGlobal(TVector3(0., 0., 0.));
  fTargetPlaneNormal           = fFirstGeo->VecFromVTLocalToGlobal(TVector3(0., 0., 1.));
  //fInteractionPointToleranz    = TVector3(0.05, 0.05, 0.5);//+/- numbers //UNITS:cm  // not used (VM)

}

/*******************************************************************************/
GlobalTrack::~GlobalTrack() {
  
  ClearTracks();  
  cout<<"Is that called"<<endl;

}


/*******************************************************************************/

vector<GlobalTrackCandidate*> GlobalTrack::GetIRTracks() 
{
  return fTrackListIr;
}

vector<GlobalTrackCandidate*> GlobalTrack::GetTofTracks() 
{
  return fTrackListTof;
}

vector<GlobalTrackCandidate*> GlobalTrack::GetGlobalTracks()
{
  return fTrackListGlobal;
}

void GlobalTrack::ClearTracks() {

  if(fTrackListGlobal.size())
    FreeClear(fTrackListGlobal);

  // VM - 29/9/13
  if(fTrackListIr_new.size())
    FreeClear(fTrackListIr_new);

  if(fTrackListIr.size())
    FreeClear(fTrackListIr);

  if(fTrackListTof.size())
    FreeClear(fTrackListTof);

  if(fTrackListTofFront.size())
    FreeClear(fTrackListTofFront);

  if(fTrackListTofRear.size())
    FreeClear(fTrackListTofRear);

}

/*******************************************************************************/
Bool_t GlobalTrack::IsGoingToAladin(TVector3& pos, TVector3& dir) 
{
  //define here the ALADiN set-up acceptance in the target RF
  // for the moment the collimeter is assumed centered on the beam direction
  if (dir.Theta()* TMath::RadToDeg()< 10.) {
    Double_t p=(z_coll-pos[2])/dir[2];
    Double_t xc = pos[0]+p*dir[0];
    Double_t yc = pos[1]+p*dir[1];
    Double_t rad=std::sqrt(std::pow(xc,2)+std::pow(yc,2));
    if (rad<rad_coll) return true;
    else return false;
  } else {
    return false;
  }

}


/*******************************************************************************/

void GlobalTrack::MakeGlobalTracksForward(TAVTntuVertex* on_vtvtx, TATntuRaw* on_traw, TATparGeo* p_tofgeo, TAMCntuEve* p_mceve) 
{
  Info("GlobalTrack::Action()","*******************************************************************************");
  Info("GlobalTrack::Action()","*** New Event ************************************************************");
  Info("GlobalTrack::Action()","Enter in GlobalTrack::MakeGlobalTracksForward()");

  Info("GlobalTrack::Action()","DEBUG: VT tracks: %d  TW tracks: %d",
       on_vtvtx->GetVertexN(),on_traw->nhit);
  
  // init and reset tracking variables for event
  
  //TAGgeoTrafo* pFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());	
  //if (pFirstGeo==NULL) {
  //  cout << "GlobalTrack::MakeGlobalTracksForward TAGgeoTrafo not found" << endl;
  //  exit(1);
  //}

  fIsUsingForward = true;
  Double_t EnergyPrimary = 0.400;//GeV/n

  Float_t slWidth = 0.;
  for (Int_t i0 = 0; i0 < on_traw->nhit; i0++) { // TW track index
    
    TATntuRawHit* p_traw = on_traw->Hit(i0);
    //what about:  TATntuRawTrack, TATntuRawTrackHit
    
    Int_t i_slat = p_traw->slat;
    
    if (!TATparGeo::IsValidSlat(i_slat))
      continue;

    if (!p_traw->stat)
      continue;
    
    //if ((!p_traw->tdct && !p_traw->tdcb) || !p_traw->stat)
    //continue;//only hits in coincidence with tdc
    // changed to have coincidences of 2 TDCs (VM)
    // VM - 29/5/2013 - single TDC accepted
    // VM -  6/6/2013 - back to TDC coincidences (to avoid noise in some TDCs)
    
    if (!fgEnTofSingleChCalib && ((p_traw->tof<=0) || ((p_traw->tdct<=0) || (p_traw->tdcb<=0)) || (!p_traw->stat)))
      continue;

    if (fgEnTofSingleChCalib && 
	(((p_traw->tdct<=0) && (p_traw->tdcb<=0)) || (p_traw->light<=0) || (p_traw->tof<=0))) 
      continue;

    //retrieve quantities needed for global tracks
    
    Info("GlobalTrack::Action()","status %d, tof %lf , light %lf , y %lf ",p_traw->stat,p_traw->tof,p_traw->light,p_traw->y);
    
    Float_t f_adct = p_traw->adct;
    Float_t f_adcb = p_traw->adcb;
    Double_t f_adc = sqrt(f_adct * f_adcb);
    
    if (f_adct == 0)
      f_adc = f_adcb;
 
    if (f_adcb == 0)
      f_adc = f_adct;
 
    Int_t TofwCharge = p_traw->chg;
    Int_t Tofw_id = p_traw->GetMCid();
    if(p_mceve != NULL) {     
      if(Tofw_id<p_mceve->nhit) {
	TAMCntuEveHit* hit = p_mceve->Hit(Tofw_id-1);
	if(hit->Chg() == TofwCharge) {
	  TofwCharge = hit->Chg();
	} else {
	  TofwCharge = -100;
	}
	myMCmomentum = (hit->InitP()).Mag();
	cout<<"True hit momentum:: "<<myMCmomentum<<endl;
      }
      //Uncomment this to use the reconstructed charge.
      TofwCharge = p_traw->chg;
    }
    else {
      TofwCharge = p_traw->chg;
      // cout<<"TofwChargeRaw::"<<TofwCharge<<endl;
    }  
        
    Double_t f_tdc = p_traw->tof;
        
    Double_t TofwTimeFront = 0.;
    Double_t TofwTimeRear  = 0.;
	 
	 
    Double_t betaTheo            = TMath::Sqrt(1-((kMassNucl*kMassNucl)/((kMassNucl+EnergyPrimary)*(kMassNucl+EnergyPrimary))));
    Double_t startTgtTmeOfFlight = fFirstGeo->GetVTCenter().Z()/(kVOfLight * betaTheo*TAGgeoTrafo::MToCm()); // already in s
	 

    if(TATparGeo::IsInFrontWall(i_slat))
      TofwTimeFront = f_tdc * TAGgeoTrafo::NsToSec() - startTgtTmeOfFlight;//convert ns->s 
    else 
      TofwTimeRear  = f_tdc * TAGgeoTrafo::NsToSec() - startTgtTmeOfFlight;//convert ns->s

    
    // Calculate the position of the hit
    // checked ok (VM)

    TVector3 pos = p_tofgeo->GetSlatPos(i_slat);

    Int_t i_plane = i_slat / 100;
    Int_t i_pack  = (i_slat-100*i_plane-1)/8;

    const TATparGeoPack&  info = p_tofgeo->PackInfo(i_plane, i_pack);

    slWidth = info.SlatWidth()*TAGgeoTrafo::MmToCm()*0.5;

    TRandom3 rn3(0);
    //    Double_t ranXpos = rn3.Gaus(pos.Px()*TAGgeoTrafo::MmToCm(),slWidth);

    Double_t ranXpos = pos.Px()*TAGgeoTrafo::MmToCm()-slWidth+rn3.Uniform(2*slWidth);

    //    cout<<" "<<pos.Px()*TAGgeoTrafo::MmToCm()<<" "<<pos.Pz()<<" "<<ranXpos<<endl;
    //get y coordinate
    Double_t YByAmp = p_traw->y;

    //Get here the point we aim at
    //needed in TOF RF
    TVector3 TofwPosition(ranXpos, YByAmp, pos.Pz()*TAGgeoTrafo::MmToCm());

    TVector3 glBFWTofwPosition = fFirstGeo->FromTWFLocalToGlobal(TofwPosition);
    TVector3 glBRWTofwPosition = fFirstGeo->FromTWRLocalToGlobal(TofwPosition);

    TVector3 TofwDirection(0., 0., 1.);//dummy dummy

    //add it to a list of Aladin Candidates
    if (TofwCharge > 0) {
      if(TATparGeo::IsInFrontWall(i_slat)) {

	//	cout<<"GlobalTrack::GlbTofPosFW:: "<<glBFWTofwPosition.X()<< " "<<glBFWTofwPosition.Y()<< " "<<glBFWTofwPosition.Z()<< " "<<endl;

	fTofTrackCandidate = 
	  new GlobalTrackCandidate(glBFWTofwPosition,fFirstGeo->VecFromTWFLocalToGlobal(TofwDirection),i0, TofwCharge, TofwTimeFront, TofwTimeRear);
	fTofTrackCandidate->AttachHit(p_traw);
	fTrackListTof.push_back(fTofTrackCandidate);
	fTrackListTofFront.push_back(fTofTrackCandidate);
      } else {

	//      cout<<"GlobalTrack::GlbTofPosRW:: "<<glBRWTofwPosition.X()<< " "<<glBRWTofwPosition.Y()<< " "<<glBRWTofwPosition.Z()<< " "<<endl;

	fTofTrackCandidate = 
	  new GlobalTrackCandidate(glBRWTofwPosition,fFirstGeo->VecFromTWRLocalToGlobal(TofwDirection),i0, TofwCharge, TofwTimeFront, TofwTimeRear);
	fTofTrackCandidate->AttachHit(p_traw);
	fTrackListTof.push_back(fTofTrackCandidate);
	fTrackListTofRear.push_back(fTofTrackCandidate);
      }		 
    }
  } //end of loop over TW track index
  
  
  Int_t CountTracksGoingToAladin=0;
  Int_t n_ahits;
  //Int_t Idvtx = 0;	
  bool tr_found(false);

  fIrTrackCandidate = 0x0;
  float Xtolerance = 2*slWidth;
  if(fField->GetCurrent() != 0) Xtolerance = 0.2; 
  // Get Vertex from int region
  for (Int_t iv0 = 0; iv0 < on_vtvtx->GetVertexN(); iv0++) { // VX vertex index

    TAVTvertex* myvtx = on_vtvtx->GetVertex(iv0);
    
    Int_t IsBMmatched=myvtx->IsBmMatched();

     if (fgBmVtxMatch && !IsBMmatched) continue;

      // Get tracklets from interaction region 
      for (Int_t i0 = 0; i0 < myvtx->GetTracksN(); i0++) { // VX track index
      
	TVector3 CurPosition  = myvtx->GetTrack(i0)->GetTrackLine().GetOrigin() * TAGgeoTrafo::MuToCm();
	TVector3 CurDirection = myvtx->GetTrack(i0)->GetTrackLine().GetSlopeZ().Unit();
	
	CurPosition  = fFirstGeo->FromVTLocalToGlobal(CurPosition);
	CurDirection = fFirstGeo->VecFromVTLocalToGlobal(CurDirection);
	
	//Angular cut of KENTROS vs. ALADiN tracks
	if (!IsGoingToAladin(CurPosition,CurDirection)) continue;
	
	CountTracksGoingToAladin++;
	
	// loop over all aladin candidates
	// adjusted to avoid to clone of the same loop (VM)
	for (int tp=0;tp<2;tp++) { // 0=Front 1=rear
	  
	  unsigned int tp_size;
	  //tp_size=fTrackListTof.size(); 
	  if (!tp) tp_size=fTrackListTofFront.size();
	  else tp_size=fTrackListTofRear.size(); 
	  
	  for (unsigned int i1 = 0; i1 < tp_size; i1++) {
	    
	    Bool_t front_bool;
	    GlobalTrackCandidate* TrackTofCandidate;
	    if (!tp) {
	      front_bool=true;
	      TrackTofCandidate=fTrackListTofFront[i1];
	    } else {
	      front_bool=false;
	      TrackTofCandidate=fTrackListTofRear[i1];
	    }
	    //make an initial guess of momentum
	    
	    Double_t T             = 0.;
	    Double_t E0            = 0.;
	    //	Int_t DefaultCharge = 1;
	    //	T = EnergyPrimary * 2. * DefaultCharge; //assume a particle with A=2.*Z
	    Double_t nNucleon;	  
	    
	    // first estimation of mass
	    if (TrackTofCandidate->GetCharge() == 1)
	      nNucleon = TrackTofCandidate->GetCharge();
	    else
	      nNucleon = 2. * TrackTofCandidate->GetCharge();
	    
	    //TVector3 tof_pos=TrackTofCandidate->GetPositionTof();
	    //Double_t timef=TrackTofCandidate->GetTofFront();
	    //Double_t timer=TrackTofCandidate->GetTofRear();
	    
	    T  = EnergyPrimary * nNucleon; //assume a particle with A=2.*Z
	    E0 = nNucleon * kMassNucl;//GeV/c2
	    Double_t pc = sqrt(T*T + 2.*T*E0);//GeV

	    //In cm.
	    TVector3 PositionTolerance(Xtolerance, 15., 1.);
	    
	    //Call tracking to find path to the hit
	    TVector3 posToMat = TrackTofCandidate->GetPositionTof();
	    
	    tr_found = TrackForward(CurPosition,pc * CurDirection,
				    TrackTofCandidate->GetCharge(),
				    posToMat,
				    PositionTolerance, front_bool);
	    
	    if (tr_found) {
	      
	      //TODO: we should add here some sanity checks for our global track, e.g.:
	      //(1) cuts on position and momentum at target
	      //(2) check energy by TOF/path vs energy by momentum
	      //(3) also charge?
	      //These checks should verify that we do not look at
	      //some out-of-target interaction or ghost track
	      
	      //add it to a list of IR Candidates
	      
	      //Tracking index
	      fIrTrackCandidate = 
		new GlobalTrackCandidate(fParticle,
					 TrackTofCandidate->GetTofFront(),
					 TrackTofCandidate->GetTofRear(),
					 i0);
	      
	      fIrTrackCandidate->ComputeProperties();
	      
	      n_ahits = TrackTofCandidate->GetNAttachedHits();
	      for(int iah=0; iah<n_ahits; iah++){ 
		fIrTrackCandidate->AttachHit(TrackTofCandidate->GetAttachedHit(iah));
	      }
	    
	      
	      //Hit matching
	      if(front_bool)	  
		fIrTrackCandidate->SetMatchedId(i1); 
	      else
		fIrTrackCandidate->SetMatchedId(fTrackListTofFront.size()+i1);	
	      // how can it distinguish if the id is for front or rear wall ? 
	      
	      fIrTrackCandidate->SetMatchedPosition(TrackTofCandidate->GetPositionTof());
	      fIrTrackCandidate->SetMatchedDirection(TrackTofCandidate->GetDirectionTof());
	    
	      //Vertex matching
	      fIrTrackCandidate->SetMatchedVertexId(iv0);
	      
	      fIrTrackCandidate->SetParticle(fParticle);
	      
	      fIrTrackCandidate->SetMatchedVertex(myvtx);
	      
	      fIrTrackCandidate->RateTheMatch();
	      
	      fTrackListIr.push_back(fIrTrackCandidate);
	      
	    } else {
	      if(fDebugLevel == 2)
		cout << "DEBUG: We lost a track!" << endl;
	    }
	    
	    if(fParticle)
	      delete fParticle;
	  }
	}
      } //end of loop over VT track index

  }//end loop on Vertices index
  
  Info("GlobalTrack::Action()", "Tracklets in IR going to ALADiN: %d",CountTracksGoingToAladin);
  Info("GlobalTrack::Action()", "Tracklets matched on the TOF   : %d", (Int_t)fTrackListTof.size());
  
  Info("GlobalTrack::Action()", "Combinations of tracklets in IR matched with TOF-Wall hits:  %d", (Int_t)fTrackListIr.size());
  

  if (fgClusterTofEnable) {
    // VM - clustering TOF 29/9/2013
    vector <id_used *> GlobalTrackSelected; // list of selected global tracks
    ClusterTof(on_vtvtx,on_traw,GlobalTrackSelected);

    RefitCluster(on_vtvtx,GlobalTrackSelected);
    if (GlobalTrackSelected.size()) 
      FreeClear(GlobalTrackSelected);
  } else {
    MakeForwardTrackedMatches();
  }
 //at this point in fTrackListIr and fTrackListTof only unmatched tracks remain
  //and matches are in fTrackListGlobal
   
  //add the match quality
   
  //TODO: additional: check for unmatched IR and ALADIN subtracks?
   
  //TODO: do the Kalman
   
  //Evaluate tracking and track matching for event
  EvaluateEvent();

  if(fTrackListGlobal.size())
    PrintEvent();
   
  fTrackListTofRear.clear();
  fTrackListTofFront.clear();

  return;
   
}

/*******************************************************************************/
void GlobalTrack::MakeForwardTrackedMatches() {


  //select the best matches
  Int_t i0 = 0;
  vector<Int_t> Taken;//taken hits from the TOF-Wall
  //TODO: change all "vector" to "list" types ... is faster
  //pick best matches first by rating from low (best) to high (worst)
   
  while (fTrackListIr.size() > 0) {
    //while not matched tracklets from IR
    Double_t BestScore = 1.E+30;
    Int_t BestScoreTrack = -1;

    for (unsigned int i1 = 0; i1 < fTrackListIr.size(); i1++) {
      //loop on matched tracklets from IR
      
      //find the lowest (best) score between all remaining candidates
      if (BestScore > fTrackListIr[i1]->fRatingScore[0]) {
	//check that match is not yet taken
	Bool_t IsTaken = false;
	for (unsigned int i3 = 0; i3 < Taken.size(); i3++) {//check if hit is already taken
	  //		cout << "Taken: "<<Taken[i3]<<endl;;
	  if (Taken[i3] == fTrackListIr[i1]->GetMatchedId()) {
	    IsTaken = true;
	    break;
	  }
	}
	if (!IsTaken) {
	  //a new best score
	  BestScore = fTrackListIr[i1]->fRatingScore[0];
	  BestScoreTrack = i1;
	}
      }
    }
	 
    if (BestScoreTrack != -1) {
      //we have the best possible match, now mark as taken and save it
      Taken.push_back(fTrackListIr[BestScoreTrack]->GetMatchedId());
		
      fTrackListGlobal.push_back(fTrackListIr[BestScoreTrack]);
		
      //remove it from the IR list but do NOT delete pointer
      fTrackListIr.erase(fTrackListIr.begin() + BestScoreTrack);
		
      Int_t DeleteId = fTrackListGlobal.back()->GetTrackId();
      //delete all the other track candidates with the same ID
      //for (unsigned int i1 = 0; i1 < fTrackListIr.size(); i1++) {//loop on matched tracklets from IR
      for (Int_t i1 = fTrackListIr.size() - 1; i1 >= 0; i1--) {//loop on matched tracklets from IR
	if (fTrackListIr[i1]->GetTrackId() == DeleteId) {
	  delete fTrackListIr[i1];
	  fTrackListIr.erase(fTrackListIr.begin() + i1);
	}
      }
		
      //for ALADiN delete only pointers for now to keep the index
      if (fTrackListTof[fTrackListGlobal.back()->GetMatchedId()])
	delete fTrackListTof[fTrackListGlobal.back()->GetMatchedId()];
      //fTrackListTof.erase(fTrackListTof.begin() + fTrackListGlobal.back()->GetMatchedId());
		
    } else {
      //we didn't find a match
      Info("GlobalTrack::FTMatches()", "Abort the matching loop! Still %d  track candidates without match.",
	   (int)fTrackListIr.size() );
      break;
    }
	 
    i0++;
  }
   
  //now delete all taken tracklets from ALADiN
  sort(Taken.begin(), Taken.end());//from low to high
  while (Taken.size() > 0) {
    fTrackListTof.erase(fTrackListTof.begin() + Taken.back());
    Taken.pop_back();
  }
   
}

/*******************************************************************************/
void GlobalTrack::EvaluateEvent() {

  //fIsUsingForward

  //TODO: make storing structure for
  //tracks reconstruction ratio: global tracks (fulfilling matching cuts) versus vertex tracklets (fulfilling cuts)
  //unmatched vertex tracklets
  //unmatched music tracklets

  // ghost tracks?
  //by Z (hoping that TOF-Z is working)??
}

/*******************************************************************************/
void GlobalTrack::EvaluateRun() {

  //fIsUsingForward
}

/*******************************************************************************/
void GlobalTrack::PrintEvent() const {

  Info("GlobalTrack::PrintEvent()", "*******************************************************************************");
  Info("GlobalTrack::PrintEvent()", "*** Evaluate Event ************************************************************");
  Info("GlobalTrack::PrintEvent()", "  We have matched %d global track(s)", (int)fTrackListGlobal.size() );
  Info("GlobalTrack::PrintEvent()", "  Unmatched suitable (= satisfying the cuts) tracklets:" );

  Info("GlobalTrack::PrintEvent()", "    from the IR:     %d", (int)fTrackListIr.size() );
  Info("GlobalTrack::PrintEvent()", "    from the ALADiN: %d", (int)fTrackListTof.size() );

  Info("GlobalTrack::PrintEvent()", "  Matched global tracks:" );
  for (unsigned int i0 = 0; i0 < fTrackListGlobal.size(); i0++) {//loop on found tracks
    fTrackListGlobal[i0]->ComputeProperties();

    Info("GlobalTrack::PrintEvent()", "  Track Number:   %d" , i0 );
    Info("GlobalTrack::PrintEvent()", "  Q:              %d" , fTrackListGlobal[i0]->GetCharge());
    Info("GlobalTrack::PrintEvent()", "  Momentum:       %lf GeV/c ", fTrackListGlobal[i0]->GetMomentum());
    Info("GlobalTrack::PrintEvent()", "  Kinetic Energy: %lf  MeV/u", fTrackListGlobal[i0]->GetEnergyFromTof()* 1000. );
    Info("GlobalTrack::PrintEvent()", "  Rest Mass:      %lf  GeV/c^2", fTrackListGlobal[i0]->GetMass());

    Info("GlobalTrack::PrintEvent()", "  TOF  (F/R Wall):  %lf , %lf s",fTrackListGlobal[i0]->GetTofFront()
	 ,fTrackListGlobal[i0]->GetTofRear());
    Info("GlobalTrack::PrintEvent()", "  Path (F/R Wall): %lf, %lf cm", fTrackListGlobal[i0]->GetPathIntTofwFront(),
	 fTrackListGlobal[i0]->GetPathIntTofwRear());
		
    if (fIsUsingForward) {
      //calculate geom. matches
      Info("GlobalTrack::PrintEvent()", "  Difference in matched position: %lf cm  (x,y,z)= (%lf,%lf,%lf) cm",
	   (fTrackListGlobal[i0]->GetMatchedPosition()
	    - fTrackListGlobal[i0]->GetPositionTof()).Mag(),
	   fTrackListGlobal[i0]->GetMatchedPosition().X()
	   - fTrackListGlobal[i0]->GetPositionTof().X(), 
	   fTrackListGlobal[i0]->GetMatchedPosition().Y()
	   - fTrackListGlobal[i0]->GetPositionTof().Y(),
	   fTrackListGlobal[i0]->GetMatchedPosition().Z()
	   - fTrackListGlobal[i0]->GetPositionTof().Z());
		
    } 
		
  }
  Info("GlobalTrack::PrintEvent()", "DEBUG: 12C@400MeV/u =>momentum: %lf GeV/c" ,
       sqrt(pow(.4 * 12., 2) + 2. * (.4 * 12.) * (12. * kMassNucl)));
  Info("GlobalTrack::PrintEvent()", "DEBUG  12C@400MeV/u =>beta: %lf ", 
       sqrt(1. - 1. / pow(.4 / kMassNucl + 1., 2)));
	
  Info("GlobalTrack::PrintEvent()", "*******************************************************************************");
}

/*******************************************************************************/
void GlobalTrack::PrintRun() const {

  cout
    << "*******************************************************************************"
    << endl;
  cout
    << "*** Evaluate Run **************************************************************"
    << endl;

  cout
    << "*******************************************************************************"
    << endl;

}


/*******************************************************************************/

bool GlobalTrack::TrackForward(TVector3 InitialPosition,
			       TVector3 InitialMomentum, 
			       Double_t Charge, 
			       TVector3 PointToHit, 
			       TVector3 PointToHitTolerance, 
			       Bool_t TofFrontWall) {
  
  Double_t CheckPath=0.;
  Bool_t MomentumIsOk = false;
  bool debug = kFALSE;  
  //Fill TrackableParticle class 
  fMomChange = .02;
  
  //create the particle

  fParticle = new TrackableParticle(fField,
				    InitialPosition, InitialMomentum, Charge);

  Info("Trajectory::Trackforward()","InitPos (%lf,%lf,%lf) ; InitMom (%lf,%lf,%lf) Charge:: %lf",
       InitialPosition.X(),InitialPosition.Y(),InitialPosition.Z(),
       InitialMomentum.X(),InitialMomentum.Y(),InitialMomentum.Z(),
       Charge);

  Double_t PriorTargetMiss = 1.E+20;
  Int_t LoopCount = 0;
  while (!MomentumIsOk) {//loop for finding correct momentum
    LoopCount++;
    
    //we are somewhere in the vertex RF that is the TG RF
    //    fParticle->TrafoVertex2Target();
    
    //we assume that our particle direction points towards positive Z
    if (fParticle->GetMomentum().Z() <= 0.) {
      Warning("Trajectory::Trackforward()","Particle flies in wrong direction: %lf",fParticle->GetMomentum().Z());
      //stop and delete stuff
      if(debug) cout<<"Wrong direction"<<endl;
      return false;
    }

    //start to track
    fParticle->PropagateFieldFree(fInteractionPoint, fTargetPlaneNormal,
				 false, true);//go back to the target plane and don't add path

    fParticle->PropagateFieldFree(fAladinEntrancePlanePosition, fAladinEntrancePlaneNormal, true, false);//go to the ALADiN field region and add path

    //TODO: check if we are inside (i.e. on the border of) the aladin field box, if not: WARNING: track missed ALADiN

    //we are in the ALADiN field, now do the real stuff
    CheckPath = fParticle->PropagateInField(fAladinBoxPosition, fAladinBoxSize, true, false, fSavePath);


    if (CheckPath < 0.) {
      cout	<< "WARNING: Trajectory::TrackBackward(): We had an error in the magnetic tracking field!"
	        << CheckPath << endl;
      exit(-1);
      //TODO: add here some action
    }

    //check if particle goes in direction of TOF-Wall (i.e.: mom_z>0.)
    if (fParticle->GetMomentum().Z() <= 0.) {
      cout	<< "WARNING: Trajectory::TrackForward(): fParticle track through ALADiN, but does not come from the target "
		<< CheckPath << endl;
      fParticle->PrintPath();
      //particle does not go to TOF-Wall, increase its momentum in next iteration
      fMomChange = .02;

    } else {
      //particle goes to TOF-Wall direction, look where exactly from and try to get closer to our TOF-Wall point
      if(TofFrontWall){
	fParticle->PropagateFieldFree(fTofwFrontPlanePosition, fTofwFrontPlaneNormal, true,false);//go to the front TOF-Wall
	// added new function for the global frame (MV)
	fParticle->AddPathToTofwRearGl(fDistanceBetweenWallsOfTofw,fTofwFrontPlaneNormal);//add extra path to rear wall
	//fParticle->AddPathToTofwRear(fDistanceBetweenWallsOfTofw);//add extra path to rear wall
      }  else 
	fParticle->PropagateFieldFree(fTofwRearPlanePosition, fTofwRearPlaneNormal, true,false);
      //Comment: For the moment we assume that both TOF-Walls are parallel!
      
      if (fParticle->IsParticleInIP_Y(PointToHit,PointToHitTolerance)) {
	//check only if we are in tolerance in Y (X is to be done later)
	//Tolerance in Y is 15 cm (~5 sigmas)
	
	if (fParticle->IsParticleInIP_X(PointToHit,PointToHitTolerance)) {
	  //Now check tolerance in X
	  MomentumIsOk = true;//we finished

	} else {
	  if (fabs(PriorTargetMiss) < fabs(PointToHit.X() - fParticle->GetPosition().X())) {
	    //we are moving away, switch direction and decrease step size
	    fMomChange = -fMomChange;
	    if (LoopCount <= 2) { //don't decrease momentum change in the first two iterations
	      fMomChange *= 0.2;
	    }
	  }
	
	  PriorTargetMiss = PointToHit.X() - fParticle->GetPosition().X();
	  
	}

      } else {
	//No way to fix it changing the momentum: we just do not find a good match!

	Warning("Trajectory::Trackforward()","No Y matching is possible.");
	Warning("Trajectory::Trackforward()",
		"Particle position: (%lf,%lf,%lf)",fParticle->GetPosition().X()
		,fParticle->GetPosition().Y(),fParticle->GetPosition().Z());
	Warning("Trajectory::Trackforward()",
		"Point to Hit: (%lf,%lf,%lf)",PointToHit.X(),
		PointToHit.Y(),PointToHit.Z());
	
	fParticle->PrintPath();
	
	//stop and return empty pointer
	if(debug) cout<<"Wrong Y"<<endl;
	return false;
      }

    }
	 
    if ((LoopCount < fMaxLoopIter) && (!MomentumIsOk)) {
      //restore position and set new starting momentum (and delete path)
      fParticle->ResetAndScaleMomentum(1. + fMomChange);
    }
	 
    if (
	((LoopCount >= fMaxLoopIter) && (!MomentumIsOk)) ||
	((fField->GetCurrent() == 0) && (!MomentumIsOk))
	) {
      
      Warning("Trajectory::Trackforward()","No convergence reached after %d iterations",LoopCount);
      Warning("Trajectory::Trackforward()",
	      "Particle position: (%lf,%lf,%lf)",fParticle->GetPosition().X()
	      ,fParticle->GetPosition().Y(),fParticle->GetPosition().Z());
      Warning("Trajectory::Trackforward()",
	      "Point to Hit: (%lf,%lf,%lf)",PointToHit.X(),
	      PointToHit.Y(),PointToHit.Z());
      
      fParticle->PrintPath();
		
      //stop and return empty pointer
      if(debug)      cout<<"Too many iterations "<<endl;
      return false;
    }
  }//END OF momentum iteration loop
	
  if (MomentumIsOk) {
    Info("Trajectory::TrackForward()","We have a track hitting our point!");
    
    if(fDebugLevel) 
      fParticle->PrintPath();
  }

  //return particle for filling to the GlobalTrackCandidate structure
  return true;
}

// tof F/R clustering 
// returns vector of id_used objects containing the IDs of selected global 
// tracks (1 if no match found, 2 IDs>=0 if match found)

Bool_t GlobalTrack::ClusterTof(TAVTntuVertex* on_vtvtx,TATntuRaw* on_traw,vector <id_used *> &GlobalTrackSelected) {
   
   enum {dbg_loc=0};
   
   const Bool_t same_charge = false;  // cluster only hits with same charge ?
   
   const Double_t sigma_slat = 0.6202; // sigma of Delta(slat-F/R) distributions
   //const Double_t sigma_ch = 0.6014; // sigma of Delta(ch-F/R) distr. (not used now)
   //const Double_t sigma_beta = 0.02686; // sigma of Delta(beta distribution) (not used now)
   const Double_t sigma_y = 8.479; // simga of Delta(y-F/R) distribution
   const Int_t max_deltaslat = 5;  // max. slat difference for F/R match
   
   
   Int_t n_glbtrk = fTrackListIr.size();
   
   // count the number of vertex tracks and save the list of 
   // vtx/trk IDs in VtxTrkIDVec
   Int_t n_vtx_tracks=0;
   vector <id_used *> VtxTrkIDVec;
   id_used * vtx_trk_id;
   for (Int_t iv0 = 0; iv0 < on_vtvtx->GetVertexN(); iv0++) { // VX vertex index
	  TAVTvertex* myvtx = on_vtvtx->GetVertex(iv0);
	  n_vtx_tracks+=myvtx->GetTracksN();
	  for (int i0=0;i0<myvtx->GetTracksN();i0++) {
		 vtx_trk_id = new id_used(iv0,i0); // store list of vtx/trk ids
		 VtxTrkIDVec.push_back(vtx_trk_id);
	  }
   }
   
   if (!n_vtx_tracks) {
	  if (dbg_loc) cout << "no vtx tracks exit clustering routine" << endl;
	  return kTRUE;
   }
   
   // count number of tof hits  and store IDs in TofHitsUsed to avoid to use the same TOF hit twice
   int n_tof_hits=fTrackListTof.size();
   vector <id_used *> TofHitsUsed;
   id_used * id_tof_used;
   for (Int_t i=0;i<(Int_t) fTrackListTof.size();i++) {
	  id_tof_used = new id_used(i,-1);
	  TofHitsUsed.push_back(id_tof_used);
   }
   if (!n_tof_hits) {
	  if (dbg_loc) cout << "no tof hits exiting cluster routine" << endl;
	  return kTRUE;
   }
   
   // list of GlobalTracks and save the index in GlobalTrackUsed 
   vector <id_used *> GlobalTrackUsed;
   id_used * glb_id_used;
   for (Int_t i=0;i<(Int_t) n_glbtrk;i++) {
	  glb_id_used = new id_used(i,-1);
	  GlobalTrackUsed.push_back(glb_id_used);
   }
   
   Bool_t done=false;
   id_used * glb_sele;
   Int_t id_trk[2];
   Int_t id_vtx[2];
   Int_t id_tof[2];
   bool tof_front[2];
   TVector3 matchpos[2];
   // VM added 9/1/2014
   Int_t charge_vtx[2];
   Int_t charge_tof[2];

   TVector3 tofpos[2];
   TATntuRawHit* tofhit[2];
   Int_t slat[2];
   Double_t y[2];
   Int_t ch[2];
   Double_t mom[2];
   //Double_t mom_abs[2];
   Double_t mass[2];
   //Double_t Etot[2];
   //Double_t gamma[2];
   //Double_t beta[2];
   //Double_t xf,xr,toff,tofr;
   Int_t id_min[2];
   do {
     id_min[0] = -1;
     Double_t diffy_min=9999999;
     id_min[1] = -1;
     Double_t diffdeltatot_min=9999999;   
     for (int lp=0;lp<2;lp++) {
       // lp=0 find track with minimum diffy
       // lp=1 find a second track with same vtx track and a tof hits in the 
       // opposite plane
       if (lp && id_min[lp-1]<0) {
	 done=true;
	 if (dbg_loc) cout << "no remaining match. Stop here" << endl;
	 continue;
       }
       if (dbg_loc) cout << " start loop lp=" << lp << endl;
       // track found in the previous loop
       for (Int_t m=0; m<n_glbtrk; m++) { 
	 if (dbg_loc) cout << " glb.trk=" << m << " used=" << GlobalTrackUsed[m]->Used() << endl;
	 if (!GlobalTrackUsed[m]->Used()) {
	   id_trk[lp] = fTrackListIr[m]->GetTrackId();
	   id_vtx[lp] = fTrackListIr[m]->GetMatchedVertexId();
	   id_tof[lp] = fTrackListIr[m]->GetMatchedId();

	   TAVTvertex* tvtx = fTrackListIr[m]->GetMatchedVertex();
	   TAVTtrack * ttrk= tvtx->GetTrack(id_trk[lp]);
	   charge_vtx[lp] = ttrk->GetChargeWithMaxProbaNorm();
	   charge_tof[lp] = fTrackListIr[m]->GetAttachedHit(0)->Chg();

	   if (lp && ((id_vtx[1]!=id_vtx[0]) || (id_trk[1]!=id_trk[0]))) 
	     continue;
	   bool usedtof=false;
	   for (Int_t i=0;i<(Int_t) TofHitsUsed.size();i++) {
	     if (id_tof[lp] == TofHitsUsed[i]->GetId1()) {
	       if (TofHitsUsed[i]->Used()) {
		 usedtof=true;
		 break;
	       }
	     }
	   } 
	   
	   bool usedvtx=false;
	   for (Int_t i=0;i<(Int_t) VtxTrkIDVec.size();i++) {
	     if (id_trk[lp]==VtxTrkIDVec[i]->GetId2() && 
		 id_vtx[lp]==VtxTrkIDVec[i]->GetId1()) {
	       if (VtxTrkIDVec[i]->Used()) {
		 usedvtx=true;
		 break;
	       }
	     } 
	   }	  
	   
	   if (!usedtof && !usedvtx) {
	     if (!lp) {
	       Double_t y_tof_pos;
	       matchpos[lp]=fTrackListIr[m]->GetMatchedPosition();
	       y_tof_pos = matchpos[lp].Y();
	       tofpos[0]=fTrackListIr[m]->GetPositionTof();
	       Double_t diffy=y_tof_pos-tofpos[lp].Y();

	       Double_t diffch=charge_vtx[lp]-charge_tof[lp];
	       Double_t y_w=fgW_delta_y;
	       Double_t c_w=fgW_delta_ch;
	       Double_t difftot=std::sqrt(diffy*diffy*y_w*y_w+diffch*diffch*c_w*c_w);
	       if (!lp && std::abs(difftot)<diffy_min) {
		 diffy_min=std::abs(difftot);
		 id_min[0]=m;
		 //if (dbg_loc) cout << " selected" << endl;
	       } 	  

	       if (dbg_loc) cout << " diffy=" << diffy << endl;
	       //if (!lp && std::abs(diffy)<diffy_min) {
	       // diffy_min=std::abs(diffy);
	       //id_min[0]=m;
		 //if (dbg_loc) cout << " selected" << endl;
	       //} 	  

	     } else { // lp
	       matchpos[lp]=fTrackListIr[m]->GetMatchedPosition();
	       //tofhit[lp] = on_traw->Hit(id_tof[lp]);
	       tofhit[lp]=fTrackListIr[m]->GetAttachedHit(0);
	       slat[lp] = tofhit[lp]->GetSlat(); 
	       if (slat[lp] <100) {// Front
		 tof_front[lp]=true; 
		 //xf=matchpos[lp].X(); 
		 //toff=tofhit[lp]->ToF();
	       } else {
		 tof_front[lp]=false;
		 slat[lp]-=100;
		 //xr=matchpos[lp].X(); 
		 //tofr=tofhit[lp]->ToF();
	       }
	       
	       if (tof_front[1]!=tof_front[0]) {
		 tofpos[lp] = fTrackListIr[m]->GetMatchedPosition();
		 y[lp]=tofhit[lp]->Y();
		 ch[lp]=fTrackListIr[m]->GetCharge();
		 mom[lp]=fTrackListIr[m]->GetMomentum();
		 //mom_abs[lp] = mom[lp].Mag();
		 mass[lp] = fTrackListIr[lp]->GetMass();
		 //Etot[lp] = TMath::Sqrt(mom_abs[lp]*mom_abs[lp]+mass[lp]*mass[lp]);
		 //if (mass[lp]>0) 
		 //  gamma[lp] = Etot[lp]/mass[lp];
		 //else
		 //  gamma[lp]=0;
		 //if (gamma[lp]>0) 
		 //  beta[lp] = TMath::Sqrt(1.-(1./(gamma[lp]*gamma[lp])));
		 //else
		 //beta[lp]=0;
		 Double_t deltaslat=(slat[0]-slat[1]);
		 Int_t deltach=(ch[0]-ch[1]);
		 //Double_t deltabeta=(beta[0]-beta[1]);
		 Double_t deltay=(y[0]-y[1]);
		 Double_t deltaslat_sigma=deltaslat*1./sigma_slat;
		 //Double_t deltach_sigma=deltach*1./sigma_ch;
		 //Double_t deltabeta_sigma=deltabeta/sigma_beta;
		 Double_t deltay_sigma=deltay/sigma_y;
		 
		 // scoring variable for clustering
		 // could be modified to include more comparisons/constraints
		 // ---------------------------------------------------------
						
		 Double_t delta_tot = std::sqrt(deltaslat_sigma*deltaslat_sigma+deltay_sigma*deltay_sigma);  
		 
		 if (std::abs(delta_tot)<diffdeltatot_min && std::abs(deltaslat)<max_deltaslat && (deltach==0 || !same_charge)) { // improve here ?
		   diffdeltatot_min=std::abs(delta_tot);
		   id_min[lp]=m;
		 } 
	       } // tof_front[1]!=tof_fornt[0]
					 
	     }
	   }
	 }
       }
       if (!lp && id_min[lp]<0) done=true;
       if (id_min[lp]>=0 && !done) {
	 GlobalTrackUsed[id_min[lp]]->SetUsed(kTRUE);
	 if (!lp) {
	   glb_sele = new id_used(id_min[lp],-1);
	   GlobalTrackSelected.push_back(glb_sele);
	 } else {
	   glb_sele->SetId2(id_min[lp]);
	 }
	 id_trk[lp] = fTrackListIr[id_min[lp]]->GetTrackId();
	 id_vtx[lp] = fTrackListIr[id_min[lp]]->GetMatchedVertexId();
	 id_tof[lp] = fTrackListIr[id_min[lp]]->GetMatchedId();
	 for (Int_t i=0;i<(Int_t) TofHitsUsed.size();i++) {
	   if (id_tof[lp] == TofHitsUsed[i]->GetId1()) {
	     TofHitsUsed[i]->SetUsed(kTRUE);
	     break;
	   }
	 }
	 matchpos[lp]=fTrackListIr[id_min[lp]]->GetMatchedPosition();
	 tofpos[lp]=fTrackListIr[id_min[lp]]->GetMatchedPosition();
	 tofhit[lp]=fTrackListIr[id_min[lp]]->GetAttachedHit(0);
	 //tofhit[lp] = on_traw->Hit(id_tof[lp]);
	 slat[lp] = tofhit[lp]->GetSlat(); 
	 y[lp]=tofhit[lp]->Y();
			
	 ch[lp]=fTrackListIr[id_min[lp]]->GetCharge();
	 mom[lp]=fTrackListIr[id_min[lp]]->GetMomentum();
	 //mom_abs[lp] = mom[lp].Mag();
	 mass[lp] = fTrackListIr[id_min[lp]]->GetMass();
	 //Etot[lp] = TMath::Sqrt(mom_abs[lp]*mom_abs[lp]+mass[lp]*mass[lp]);
	 //if (mass[lp]>0)
	 //  gamma[lp] = Etot[lp]/mass[lp];
	 //else
	 //  gamma[lp]=0;
	 //if (gamma[lp]>0) 
	 //  beta[lp] = TMath::Sqrt(1.-(1./(gamma[lp]*gamma[lp])));
	 //else
	 //  beta[lp]=0;
	 if (slat[lp]<100) {// Front
	   tof_front[lp]=true; 
	   //xf=matchpos[lp].X(); 
	   //toff=tofhit[lp]->ToF();
	 } else {
	   tof_front[lp]=false;
	   //xr=matchpos[lp].X(); 
	   //tofr=tofhit[lp]->ToF();
	   slat[lp]-=100;
	 }
       }
     }
     if (!done) {
       for (Int_t i=0;i<(Int_t) VtxTrkIDVec.size();i++) {
	 if (id_trk[0]==VtxTrkIDVec[i]->GetId2() && 
	     id_vtx[0]==VtxTrkIDVec[i]->GetId1()) {
	   VtxTrkIDVec[i]->SetUsed(kTRUE);
	   break;
	 } 
       }
     }
   } while(!done);
   
   
   if(VtxTrkIDVec.size())
     FreeClear(VtxTrkIDVec);
   if (TofHitsUsed.size())
     FreeClear(TofHitsUsed);
   if (GlobalTrackUsed.size()) 
     FreeClear(GlobalTrackUsed);
   
   return kTRUE;
}

/*******************************************************************************/
Bool_t GlobalTrack::RefitCluster(TAVTntuVertex* on_vtvtx,vector <id_used *> &GlobalTrackSelected) {
   enum {dbg_loc=0};
   
   Double_t EnergyPrimary = 0.400;//GeV/n
   
   const Bool_t max_charge = true;    // assign max. charge when clustering
   // else use mean charge
   float Xtolerance;
   if(fField->GetCurrent() != 0) Xtolerance = 0.2; 
   else Xtolerance = 2.5;
   
   bool tr_found(false);
   
   if (dbg_loc) cout << "refit tracks belonging to clustered tof hits" << endl;
   
   for (Int_t i=0;i<(Int_t) GlobalTrackSelected.size();i++) {
	  Int_t id1 = GlobalTrackSelected[i]->GetId1();
	  // id2>=0 if clustering done
	  Int_t id2 = GlobalTrackSelected[i]->GetId2();
	  // get vtx/trk ids (the same in case of clustering)
	  
	  if (id2>=0) {  // F/R match found
		 Int_t vtx_n = fTrackListIr[id1]->GetMatchedVertexId();
		 Int_t trk_n = fTrackListIr[id1]->GetTrackId();
		 if (dbg_loc) cout << "cluster found refit vtx_n=" << vtx_n << " trk_n=" << trk_n << endl;
		 // same vtx/trk IDs for other track
		 TAVTvertex* myvtx = on_vtvtx->GetVertex(vtx_n);
		 TVector3 CurPosition  = myvtx->GetTrack(trk_n)->GetTrackLine().GetOrigin() * TAGgeoTrafo::MuToCm();
		 TVector3 CurDirection = myvtx->GetTrack(trk_n)->GetTrackLine().GetSlopeZ().Unit();
		 CurPosition  = fFirstGeo->FromVTLocalToGlobal(CurPosition);
		 CurDirection = fFirstGeo->VecFromVTLocalToGlobal(CurDirection);
		 // get first tof index
		 Int_t id_tof1 = fTrackListIr[id1]->GetMatchedId();
		 GlobalTrackCandidate* TrackTofCandidate[2];
		 bool slat_front[2];
		 if (id_tof1<(Int_t) fTrackListTofFront.size()) {
			TrackTofCandidate[0]=fTrackListTofFront[id_tof1];
			slat_front[0]=true;
		 } else {
			TrackTofCandidate[0]=fTrackListTofRear[id_tof1-fTrackListTofFront.size()];
			slat_front[0]=false;
		 }
		 
		 Int_t id_tof2 = fTrackListIr[id2]->GetMatchedId();
		 if (id_tof2<(Int_t) fTrackListTofFront.size()) {
			TrackTofCandidate[1]=fTrackListTofFront[id_tof2];
			slat_front[1]=true;
		 } else {
			TrackTofCandidate[1]=fTrackListTofRear[id_tof2-fTrackListTofFront.size()];
			slat_front[1]=false;
		 }
		 
		 Double_t nNucleon_fr[2];	  
		 Double_t charge_fr[2];
		 Double_t nNucleon;	  
		 Double_t charge;
		 TVector3 posToMat_fr[2];
		 TVector3 posToMat;
		 for (int k=0;k<2;k++) {
			charge_fr[k]=TrackTofCandidate[k]->GetCharge();
			if (TrackTofCandidate[0]->GetCharge() == 1)
			   nNucleon_fr[k] = TrackTofCandidate[k]->GetCharge();
			else
			   nNucleon_fr[k] = 2. * TrackTofCandidate[k]->GetCharge();
			posToMat_fr[k] = TrackTofCandidate[k]->GetPositionTof();
		 }
		 // does it make sense to use a mean charge or is it better to use
		 // only mathces with same charge ?
		 //charge = (charge_fr[0]+charge_fr[1])/2.;
		 //nNucleon = (nNucleon_fr[0]+nNucleon_fr[1])/2.;
		 
		 // try using max charge
		 if (max_charge) charge = std::max(charge_fr[0],charge_fr[1]);
		 else charge = (charge_fr[0]+charge_fr[1])/2.;
		 if (charge==1) 
			nNucleon=charge;
		 else
			nNucleon=2*charge;
		 
		 Double_t T  = EnergyPrimary * nNucleon; //assume a particle with A=2.*Z
		 Double_t E0 = nNucleon * kMassNucl;//GeV/c2
		 Double_t pc = sqrt(T*T + 2.*T*E0);//GeV
		 TVector3 PositionTolerance(Xtolerance, 15., 1.);
		 TVector3 tofdir_fr[2];
		 
		 tofdir_fr[0] = fTrackListIr[id1]->GetDirectionTof();
		 tofdir_fr[1] = fTrackListIr[id2]->GetDirectionTof();
		 for (int k=0;k<2;k++) {
			tofdir_fr[k]= fFirstGeo->VecFromGlobalToTWFLocal(tofdir_fr[k]); 
			posToMat_fr[k]= fFirstGeo->FromGlobalToTWFLocal(posToMat_fr[k]);
		 } 
		 // shift rear position to front
		 if (slat_front[0]) {
			posToMat_fr[1].SetX(posToMat_fr[1].X()-tofdir_fr[1].X()*posToMat_fr[1].Z());
			posToMat_fr[1].SetY(posToMat_fr[1].Y()-tofdir_fr[1].Y()*posToMat_fr[1].Z());	
			posToMat_fr[1].SetZ(posToMat_fr[0].Z());
			
		 } else {
			posToMat_fr[0].SetX(posToMat_fr[0].X()-tofdir_fr[0].X()*posToMat_fr[1].Z());
			posToMat_fr[0].SetY(posToMat_fr[0].Y()-tofdir_fr[0].Y()*posToMat_fr[1].Z());
			
		 }
		 posToMat.SetX((posToMat_fr[0].X()+posToMat_fr[1].X())/2.);
		 posToMat.SetY((posToMat_fr[0].Y()+posToMat_fr[1].Y())/2.);
		 posToMat.SetZ((posToMat_fr[0].Z()+posToMat_fr[1].Z())/2.);
		 TVector3 tofdir;
		 tofdir.SetX((tofdir_fr[0].X()+tofdir_fr[1].X()));
		 tofdir.SetY((tofdir_fr[0].Y()+tofdir_fr[1].Y()));
		 tofdir.SetZ((tofdir_fr[0].Z()+tofdir_fr[1].Z()));
		 tofdir=tofdir.Unit();
		 posToMat = fFirstGeo->FromTWFLocalToGlobal(posToMat);
		 tofdir=fFirstGeo->VecFromTWFLocalToGlobal(tofdir);
		 tr_found = TrackForward(CurPosition,pc * CurDirection,
								 charge,
								 posToMat,
								 PositionTolerance, true);
		 
		 if (tr_found) {
			if (slat_front[0]) {
			   fIrTrackCandidate = 
			   new GlobalTrackCandidate(fParticle,
										TrackTofCandidate[0]->GetTofFront(),
										TrackTofCandidate[1]->GetTofRear(),
										trk_n);
			} else {
			   fIrTrackCandidate = 
			   new GlobalTrackCandidate(fParticle,
										TrackTofCandidate[1]->GetTofFront(),
										TrackTofCandidate[0]->GetTofRear(),
										trk_n);
			}
			
			fIrTrackCandidate->ComputeProperties();
			
			//Hit matching
			if(slat_front[0])	  
			   fIrTrackCandidate->SetMatchedId(id_tof1); 
			else
			   fIrTrackCandidate->SetMatchedId(id_tof2);
			// add second matched id (TO DO)
			
			if(slat_front[0]) {
			   fIrTrackCandidate->AttachHit(TrackTofCandidate[0]->GetAttachedHit(0)); 
			   fIrTrackCandidate->AttachHit(TrackTofCandidate[1]->GetAttachedHit(0)); 
			} else {
			   fIrTrackCandidate->AttachHit(TrackTofCandidate[1]->GetAttachedHit(0)); 
			   fIrTrackCandidate->AttachHit(TrackTofCandidate[0]->GetAttachedHit(0)); 
			}	  
			
			fIrTrackCandidate->SetMatchedPosition(posToMat);
			fIrTrackCandidate->SetMatchedDirection(tofdir);
			
			fIrTrackCandidate->SetMatchedVertexId(vtx_n);
			
			fIrTrackCandidate->SetParticle(fParticle);
			
			fIrTrackCandidate->SetMatchedVertex(myvtx);
			
			fIrTrackCandidate->RateTheMatch();
			
			fTrackListIr_new.push_back(fIrTrackCandidate);
			
			fTrackListGlobal.push_back(fIrTrackCandidate);
			
		 } else { // !tr_found
			// save only the track with best match
			fTrackListGlobal.push_back(fTrackListIr[id1]);    
		 }
		 
		 if(fParticle)
			delete fParticle;
	  } else { // no cluster
		 fTrackListGlobal.push_back(fTrackListIr[id1]);      
	  }
   }
   
   fTrackListIr.clear();
   fTrackListIr_new.clear();
   
   return false;
}

/*******************************************************************************/

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



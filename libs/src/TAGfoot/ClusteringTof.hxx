#ifndef CLUSTERINGTOF_HXX__
#define CLUSTERINGTOF_HXX

// tof F/R clustering 
// returns vector of id_used objects containing the IDs of selected global 
// tracks (1 if no match found, 2 IDs>=0 if match found)

Bool_t GlobalTrack::ClusterTof(TAVTntuVertex* on_vtvtx,TATntuRaw* on_traw,vector <id_used *> &GlobalTrackSelected) {

  enum {dbg_loc=0};

  const Bool_t same_charge = false;  // cluster only hits with same charge ?

  const Double_t sigma_slat = 0.6202; // sigma of Delta(slat-F/R) distributions
  const Double_t sigma_ch = 0.6014; // sigma of Delta(ch-F/R) distr. (not used now)   
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
  TVector3 tofpos[2];
  TATntuRawHit* tofhit[2];
  Int_t slat[2];
  Double_t y[2];
  Int_t ch[2];
  TVector3 mom[2];
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
	      if (dbg_loc) cout << " diffy=" << diffy << endl;
	      if (!lp && std::abs(diffy)<diffy_min) {
		diffy_min=std::abs(diffy);
		id_min[0]=m;
		if (dbg_loc) cout << " selected" << endl;
	      } 	  
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
		Double_t deltach_sigma=deltach*1./sigma_ch;
		//Double_t deltabeta_sigma=deltabeta/sigma_beta;
		Double_t deltay_sigma=deltay/sigma_y;

		// scoring variable for clustering
		// could be modified to include more comparisons/constraints
		// ---------------------------------------------------------

		Double_t delta_tot = std::sqrt(deltaslat_sigma*deltaslat_sigma+deltay_sigma*deltay_sigma+deltach_sigma*deltach_sigma);  
		

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


Bool_t GlobalTrack::FindVtxCh(TAVTntuVertex* on_vtvtx) {
  enum {dbg_loc=0};
  Int_t nvtx = on_vtvtx->GetVertexN();
  for (Int_t iv0 = 0; iv0 < on_vtvtx->GetVertexN(); iv0++) { 
    TAVTvertex* myvtx = on_vtvtx->GetVertex(iv0);
    Bool_t isBM = myvtx->IsBmMatched();
    for (Int_t i0 = 0; i0 < myvtx->GetTracksN(); i0++) {
      TAVTtrack *trkv = myvtx->GetTrack(i0);
      Int_t nclus_glb = trkv->GetClustersN();
      double clu_mean_glb=0;
      for (int k=0;k<(int) nclus_glb;k++) {
	TAVTcluster* clus_glb =  trkv->GetCluster(k);
	clu_mean_glb += clus_glb->GetPixelsN();
      }
      if (nclus_glb>0) 
	clu_mean_glb /= nclus_glb*1.;
      else
	clu_mean_glb=0;
    }
  }
  return true;
}

#endif

/*!
 \file
 \version $Id: TAGactGlbTracking.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAGactGlbTracking.
 */

#include "TH1F.h"

#include "TAGntuGlbTracks.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuVertex.hxx"
#include "TATntuRaw.hxx"
#include "TAMCntuEve.hxx"

#include "TAGactGlbTracking.hxx"

#include <iomanip>

/*!
 \class TAGactGlbTracking TAGactGlbTracking.hxx "TAGactGlbTracking.hxx"
 \brief Get Beam Monitor raw data from MBS. **
 */

ClassImp(TAGactGlbTracking)
;


//------------------------------------------+-----------------------------------
//! Default constructor.


TAGactGlbTracking::TAGactGlbTracking(const char* name, TAGdataDsc* p_glb,
				     TAGdataDsc* p_vtvtx, TAGdataDsc* p_traw,
				     TAGparaDsc* p_geotof, double Current, 
				     TString dir, TAGdataDsc* p_mceve) :
  TAGaction(name, "TAGactGlbTracking - Global Tracking"),
  fpGlb(p_glb),
  fpVtvtx(p_vtvtx), fpTraw(p_traw), 
  fpParTofGeo(p_geotof) 
{

  AddDataOut(p_glb, "TAGntuGlbTracks");
  AddDataIn(p_vtvtx, "TAVTntuVertex");
  AddDataIn(p_traw, "TATntuRaw");
  fpMCeve = p_mceve;
  if(p_mceve)
    AddDataIn(p_mceve, "TAMCntuEve");
  
  // changed from 700. to 730 (to be fined tuned - VM)
  Double_t AladinCurrent = 0.;  //Default is without magfield
  // Set in HlRecoTools via SetCurrent
  if(Current) 
    AladinCurrent = Current;

  fField    = new MagneticField(AladinCurrent,dir);
  fGloTrack = new GlobalTrack(fField);//TODO: stuff geo info in here

  //Everything is enabled by default.
  //To switch options off call the methods in the hxx
  fGloTrack->EnableClusterTof();
  fGloTrack->EnableEnTofSingleChCalib();
  fGloTrack->EnableBmVtxMatch();

}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGactGlbTracking::~TAGactGlbTracking()
{  
  if (fField)
    delete fField;

  if (fGloTrack)
    delete fGloTrack;
}


//------------------------------------------+-----------------------------------
//! Setup all histograms.

void TAGactGlbTracking::CreateHistogram()
{
   DeleteHistogram();   
   fpHisMass = new TH1F("glbMass", "Mass Distribution", 200, 0, 16);

   AddHistogram(fpHisMass);
   
   SetValidHistogram(kTRUE);
}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TAGactGlbTracking::Action() 
{
  TAGntuGlbTracks* p_glb    = (TAGntuGlbTracks*) fpGlb->Object();
  TAVTntuVertex*   on_vtvtx = (TAVTntuVertex*)   fpVtvtx->Object();
  TATntuRaw*       on_traw  = (TATntuRaw*)       fpTraw->Object();
  TATparGeo*       p_tofgeo = (TATparGeo*)       fpParTofGeo->Object();
  
  TAMCntuEve*     p_mcblk = 0;
  if(fpMCeve) 
    p_mcblk = (TAMCntuEve*) fpMCeve->Object();

  //  if(!p_glb->glbtrk) 
  p_glb->Clear();

    //call global tracks with the event and sub-detector info and signals
//	fGloTrack->MakeGlobalTracksBackward(on_info, on_vttrk, on_traw, on_pchit, on_pctrk,on_trtrk);//TODO: add here info from IR detectors: bmon, start

  fGloTrack->ClearTracks();
  //Execute tracking algorithm only if there are at least a vtx track and 1 hit on the tof

  if(on_vtvtx->GetVertexN() && on_traw->nhit)
    fGloTrack->MakeGlobalTracksForward(on_vtvtx, on_traw, p_tofgeo, p_mcblk);
  //TODO: add here info from IR detectors: bmon, start
  
  vector<GlobalTrackCandidate*> my_glb_tracks;
  my_glb_tracks = fGloTrack->GetGlobalTracks();

  vector<TVector3> myPath;

  if(my_glb_tracks.size()) Info("Action()","Found :: %d tracks",(int)my_glb_tracks.size());
  double theta;
  Int_t n_ahits(0);

  for(int it = 0; it < (int)my_glb_tracks.size(); it++) {

    myPath = my_glb_tracks.at(it)->GetPath();

    TVector3 dirTgt = my_glb_tracks.at(it)->GetDirectionTarget();
    TVector3 posTgt = my_glb_tracks.at(it)->GetPositionTarget();
    theta = dirTgt.Theta();


    //TAGntuGlbTrk *mytmp = 
    new ((*(p_glb->glbtrk))[it]) TAGntuGlbTrk(myPath);

    p_glb->nglbtrk++;
    TAGntuGlbTrk* myTrk = p_glb->Track(it);
    myTrk->SetId(it);

    myTrk->SetTrackId(my_glb_tracks.at(it)->GetTrackId());
    myTrk->SetMass(my_glb_tracks.at(it)->GetMass());
    myTrk->SetCharge(my_glb_tracks.at(it)->GetCharge());
    myTrk->SetMomentum(my_glb_tracks.at(it)->GetMomentum());
    myTrk->SetToff(my_glb_tracks.at(it)->GetTofFront());
	myTrk->SetTofr(my_glb_tracks.at(it)->GetTofRear());
	myTrk->SetEnergy(my_glb_tracks.at(it)->GetEnergyFromTof());

    //Particle direction and position computed at target
    myTrk->SetTgtDirection(dirTgt);
    myTrk->SetTgtPosition(posTgt);

    //TrackableParticle pointer my_glb_tracks.at(it)->GetParticle();

    myTrk->SetMatchedVertexId(my_glb_tracks.at(it)->GetMatchedVertexId());
    
    // VM 29/9/2013 add TOF id
    myTrk->SetMatchedId(my_glb_tracks.at(it)->GetMatchedId());

    TAVTvertex * myvtx = my_glb_tracks.at(it)->GetMatchedVertex();

    myTrk->NewVertex(*myvtx);

    n_ahits = my_glb_tracks.at(it)->GetNAttachedHits();
    for(int iah=0; iah<n_ahits; iah++){ 
      TATntuRawHit * tmphit =  my_glb_tracks.at(it)->GetAttachedHit(iah);
      if(tmphit != NULL)
	myTrk->AttachHit(*tmphit);
    }
    
    myTrk->SetMatPosition(my_glb_tracks.at(it)->GetMatchedPosition());
    myTrk->SetTofPosition(my_glb_tracks.at(it)->GetPositionTof());
    myTrk->SetTofDirection(my_glb_tracks.at(it)->GetDirectionTof());
    
    if (ValidHistogram())
      fpHisMass-> Fill(my_glb_tracks.at(it)->GetMass());

    ChargeBetheBlock * ChgBB = new ChargeBetheBlock(false,myTrk);
    myTrk->SetCharge_test(ChgBB->GetCharge());

  }
  
  fpGlb->SetBit(kValid);
  return kTRUE;

}


#ifndef CLASS_GlobalTrack
#define CLASS_GlobalTrack

#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <iomanip>

#include <TROOT.h>
#include "TRandom3.h"
#include "TVector3.h"
#include "TMath.h"
#include "TString.h"
//#include <TTree.h>

//WM framework classes
#include "TAMCntuEve.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuVertex.hxx"

//FIRST
#include "TAGobject.hxx"
#include "TAGgeoTrafo.hxx"
#include "TAGroot.hxx"
#include "TrackableParticle.hxx" 

//MC info, is this needed here? Yes (MV)
#include "foot_geo.h"

//Global tracking classes
#include "GlobalTrackCandidate.hxx"
#include "MagneticField.hxx"


using namespace std;

// VM 29/9/2013
// used to store info on selected tracks 
// id1=id of first global track
// id2=id of second global track (tof f/r clustering)
class id_used {
public:
  id_used(Int_t i1, Int_t i2) {
    id1=i1;
    id2=i2;
    used=false;
  }
  inline Int_t GetId1() { return id1; }
  inline Int_t GetId2() { return id2; }
  inline void SetId1(Int_t i1) { id1=i1; }
  inline void SetId2(Int_t i2) { id2=i2; }
  inline Bool_t Used() { return used; }
  inline void SetUsed(Bool_t t) { used=t; }
private:
  Int_t id1;
  Int_t id2;
  Bool_t used;
};

class GlobalTrack: public TAGobject {

public:
  GlobalTrack(MagneticField* Field);
  ~GlobalTrack();

  void ClearTracks();

  Bool_t IsGoingToAladin(TVector3& position,TVector3& direction);
   
  void MakeGlobalTracksForward(TAVTntuVertex* on_vtvtx=0, TATntuRaw* on_traw=0, TATparGeo* p_tofgeo=0, TAMCntuEve* p_mceve=0);

  vector<GlobalTrackCandidate*> GetIRTracks();
  vector<GlobalTrackCandidate*> GetTofTracks();
  vector<GlobalTrackCandidate*> GetGlobalTracks();

  bool  TrackForward(TVector3 InitialPosition,
		     TVector3 InitialMomentum, Double_t Charge, 
		     TVector3 PointToHit,
		     TVector3 PointToHitTolerance,Bool_t TofFrontWall);
  
  Int_t fDebugLevel;    

  Int_t GetMaxLoopIter() const {
    return fMaxLoopIter;
  }

  Double_t GetMomChange() const {
    return fMomChange;
  }

  Int_t GetSavePath() const {
    return fSavePath;
  }

  void SetMaxLoopIter(Int_t maxLoopIter) {
    fMaxLoopIter = maxLoopIter;
  }

  void SetMomChange(Double_t momChange) {
    fMomChange = momChange;
  }

  void SetSavePath(Int_t savePath) {
    fSavePath = savePath;
  }
   
public:
   static void EnableClusterTof()  { fgClusterTofEnable = true;  }
   static void DisableClusterTof() { fgClusterTofEnable = false; }
   static void EnableEnTofSingleChCalib()  { fgEnTofSingleChCalib = true;  } 
   static void DisableEnTofSingleChCalib() { fgEnTofSingleChCalib = false; }
   static void EnableBmVtxMatch()  { fgBmVtxMatch = true; }
   static void DisableBmVtxMatch() { fgBmVtxMatch = false; }


private:
  void MakeForwardTrackedMatches();

  void EvaluateEvent();
  void EvaluateRun();

  void PrintEvent() const;
  void PrintRun() const;

  // VM 29/9/2013 - TOF front/rear cluster (in ClusteringTof.hxx
  Bool_t ClusterTof(TAVTntuVertex*,TATntuRaw*,vector <id_used *> &);
  // VM 29/9/2013 - refit after clustering
  Bool_t RefitCluster(TAVTntuVertex*,vector <id_used *> &);

  // VM 29/9/2013 - added for test
  Bool_t FindVtxCh(TAVTntuVertex* on_vtvtx);


  template<class C> void FreeClear(C & cntr) {
    for (typename C::iterator it = cntr.begin(); it != cntr.end(); ++it) {
      delete *it;
    }
    cntr.clear();
  }

  MagneticField *fField;
  Bool_t fIsUsingForward;

  vector<GlobalTrackCandidate*> fTrackListIr;
  vector<GlobalTrackCandidate*> fTrackListTof;
  vector<GlobalTrackCandidate*> fTrackListTofFront;
  vector<GlobalTrackCandidate*> fTrackListTofRear;
  vector<GlobalTrackCandidate*> fTrackListGlobal;
  // VM 29/9/2013 - add for TOF clustering
  vector<GlobalTrackCandidate*> fTrackListIr_new;


  TAGgeoTrafo* fFirstGeo;
  // entrance collimator pos and radius
  Double_t z_coll;    // z of coll in cm
  Double_t rad_coll;  // radius of collimator in cm
  GlobalTrackCandidate* fTofTrackCandidate;
  GlobalTrackCandidate* fIrTrackCandidate;

  //NEW from Traj
  Double_t fMomChange;//UNITS:1
  Int_t fMaxLoopIter;
  Int_t fSavePath;//UNITS:cm

  TVector3 fTofwFrontPlanePosition, fTofwFrontPlaneNormal,fTofwRearPlanePosition,fTofwRearPlaneNormal;
  Double_t fDistanceBetweenWallsOfTofw;
  TVector3 fAladinEntrancePlanePosition,fAladinEntrancePlaneNormal;
  TVector3 fAladinExitPlanePosition, fAladinExitPlaneNormal;
  TVector3 fAladinBoxPosition, fAladinBoxSize;
  TVector3 fInteractionPoint, fTargetPlaneNormal;
  TVector3 fInteractionPointToleranz;

  TrackableParticle *fParticle; 
      
  //	struct SubTrackIr
  //	Ir subtrack
  //	Ir hitsIsGoingToAladin (flag if forward directed, rest is for KENTROS)
  //	look at event display

  //	struct SubTrackAladin
  //	Aladin subtrack
  //	Aladin hits
  //	including a charge and energy
  //#  including a vector of Hypothesis
  //#  including

protected:
   static Bool_t fgClusterTofEnable;   // tof clustering enabling
   static Bool_t fgEnTofSingleChCalib; // select cuts for TOF, single channel calibration
   static Bool_t fgBmVtxMatch;         // select only VTX tracks from vtx matching the BM track
  static Bool_t fgMaskCentralSlats;   // mask problematic slats in the central region (temporary)
  static Double_t fgW_delta_y;        // weight on Delta(y) for TOF/VTX match
  static Double_t fgW_delta_ch;       // weight on Delta(ch) for TOF/VTX match
  static Bool_t fgMaskSingleAdcSlats; // VM 1/8/2014 - mask slats with 1 missing ADC (
   
  ClassDef(GlobalTrack,1);

};

#endif

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



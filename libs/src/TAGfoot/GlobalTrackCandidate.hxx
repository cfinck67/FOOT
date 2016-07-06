#ifndef CLASS_GlobalTrackCandidate
#define CLASS_GlobalTrackCandidate

#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <iomanip>

//#include <cmath>
//#include <cstring>
//#include <algorithm>


#include <TROOT.h>
#include "TRandom3.h"
#include "TVector3.h"
#include "TObject.h"
#include "TMath.h"
#include "TString.h"

#include "TATntuRaw.hxx"
#include "TAVTntuVertex.hxx"
#include "TrackableParticle.hxx"


using namespace std;

class GlobalTrackCandidate: public TObject {

public:

  //initialize a tracklet particle
  GlobalTrackCandidate(TVector3 Position, TVector3 Direction, Int_t TrackId,
		       Int_t Charge = 0, Double_t TofwTimeFront = -1., Double_t TofwTimeRear = -1.);
  GlobalTrackCandidate(TrackableParticle *Track, Double_t TofwTimeFront,
		       Double_t TofwTimeRear, Int_t TrackId);
  ~GlobalTrackCandidate();
	
  void RateTheMatch(GlobalTrackCandidate *OtherTrack, Int_t TrackId);
  void RateTheMatch();
  void SortMatches();
  void ComputeProperties();

  Int_t SortVec(std::vector<Double_t> *fVec1, std::vector<Double_t> *fVec2);
  Int_t SortVec(std::vector<Double_t> *fVec1, std::vector<Int_t> *fVec2);

  vector<Int_t> fRatingId;
  vector<Double_t> fRatingScore;

  vector<TVector3> GetPath();

  void AttachHit(TATntuRawHit* hit);

  TATntuRawHit* GetAttachedHit(int ih);

  Int_t GetNAttachedHits() {
    return fattachedHits.size();
  }

  void SetMatchedVertexId(Int_t trid) {
    fVtxID = trid;
  }

  Int_t GetMatchedVertexId() {
    return fVtxID;
  }

  void SetParticle(TrackableParticle* aPar) {
    fTrackParticle = aPar;
  }

  TrackableParticle* GetParticle() {
    return fTrackParticle;
  }

  void SetMatchedVertex(TAVTvertex *avtx) {
    fMatVtx = avtx;
  }

  TAVTvertex*  GetMatchedVertex() {
    return fMatVtx;
  }


  void SetMatchedPosition(TVector3 MatchedPosition) {
    fMatchedPosition = MatchedPosition;
  }

  TVector3 GetMatchedPosition() const {
    return fMatchedPosition;
  }

  void SetMatchedDirection(TVector3 MatchedDirection) {
    fMatchedDirection = MatchedDirection;
  }

  TVector3 GetMatchedDirection() const {
    return fMatchedDirection;
  }

  void SetMatchedId(Int_t MatchedId) {
    fMatchedId = MatchedId;
  }

  Int_t GetMatchedId() const {
    return fMatchedId;
  }

  Int_t GetCharge() const {
    return fCharge;
  }

  TVector3 GetDirectionTof() const {
    return fDirectionTof;
  }

  TVector3 GetDirectionTarget() const {
    return fDirectionTarget;
  }

  Double_t GetMomentum() const {
    return fMomentum;
  }

  Double_t GetMass() const {
    return fMass;
  }

  Double_t GetEnergyFromTof() const {
    return fEnergyFromTof;
  }

  vector<TVector3> GetPath() const {
    return fPath;
  }

  Double_t GetPathIntTofwFront() const {
    return fPathIntTofwFront;
  }

  Double_t GetPathIntTofwRear() const {
    return fPathIntTofwRear;
  }

  Double_t GetTofFront() const {
    return fTofFront;
  }

  Double_t GetTofRear() const {
    return fTofRear;
  }

  // VM - 29/9/2013
  void SetTofFront(Double_t toff) {
    fTofFront = toff;
  }

  // VM - 29/9/2013
  void SetTofRear(Double_t tofr) {
    fTofRear = tofr;
  }

  TVector3 GetPositionTof() const {
    return fPositionTof;
  }

  TVector3 GetPositionTarget() const {
    return fPositionTarget;
  }

  Int_t GetTrackId() const {
    return fTrackId;
  }
	
  void               SetFound(Bool_t flag = true)   {
    fFound = flag;          
  }
  Bool_t             GetFound()   const { 
    return fFound;          
  }

 
private:
  Int_t fTrackId;
  TVector3 fPositionTof;
  TVector3 fDirectionTof;
  TVector3 fPositionTarget;
  TVector3 fDirectionTarget;
  Int_t fCharge;
  Double_t fMomentum;
  vector<TVector3> fPath;
  vector<TATntuRawHit*> fattachedHits;
  Double_t fPathIntTofwFront, fPathIntTofwRear;
  Double_t fTofFront, fTofRear;
  Double_t fEnergyFromTof;
  Double_t fMass;

  Int_t    fVtxID;  //Index of Vertex object associated to the rec track
  TAVTvertex* fMatVtx; //Matched vertext inside the Vertex
  TrackableParticle* fTrackParticle;

  Int_t fMatchedId;//store here the ID of the matched tracklet
  TVector3 fMatchedDirection;
  TVector3 fMatchedPosition;
	
  Bool_t             fFound;                    // kTRUE is associated to a Vtx track



protected:
  ClassDef(GlobalTrackCandidate,1);


};

#endif

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



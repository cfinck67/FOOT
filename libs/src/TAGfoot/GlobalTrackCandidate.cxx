

#include "GlobalTrackCandidate.hxx"
#include "TAVTntuTrack.hxx"
#include "TMath.h"

ClassImp(GlobalTrackCandidate);


/*******************************************************************************/
//constructor for filling a tracklet particle

GlobalTrackCandidate::GlobalTrackCandidate(TVector3 Position, TVector3 Direction, Int_t TrackId,
										   Int_t Charge, Double_t TofwTimeFront,  Double_t TofwTimeRear) 
{
  //intialize
  fRatingId.clear();
  fRatingScore.clear();
  fMatchedId        = -1;
  fMatchedPosition  = TVector3(0., 0., 0.);
  fMatchedDirection = TVector3(0., 0., 0.);

  fattachedHits.clear();
  fFound = kFALSE;
  
  fCharge           = Charge;
  fMomentum         = -1.;
  fPath.clear();
  
  fPathIntTofwFront = -1.;
  fPathIntTofwRear  = -1.;
  fTofFront         = TofwTimeFront;
  fTofRear          = TofwTimeRear;

  fVtxID = -1;
  fMatVtx = NULL;
  fTrackParticle = NULL;

  fTrackId          = TrackId;
  fPositionTarget   = TVector3(0., 0., 0.);
  fDirectionTarget  = TVector3(0., 0., 0.);
  fPositionTof   = Position;
  fDirectionTof  = Direction;

}
/*******************************************************************************/
//constructor for filling a tracked particle
GlobalTrackCandidate::GlobalTrackCandidate(TrackableParticle *Track,
										   Double_t TofwTimeFront, Double_t TofwTimeRear, Int_t TrackId)
{
  //intialize
  fRatingId.clear();
  fRatingScore.clear();
  fMatchedId        = -1;
  fMatchedPosition  = TVector3(0., 0., 0.);
  fMatchedDirection = TVector3(0., 0., 0.);

  fattachedHits.clear();
  
  fCharge           = (int)Track->GetCharge();
  fMomentum         = Track->GetMomentum().Mag();
  fPath             = Track->GetPath();
  
  fPathIntTofwFront = Track->GetPathIntegralTofwFront();
  fPathIntTofwRear  = Track->GetPathIntegralTofwRear();
  fTofFront         = TofwTimeFront;
  fTofRear          = TofwTimeRear;

  fVtxID = -1;
  fMatVtx = NULL;
  fTrackParticle = Track;

  fPositionTof  = Track->GetPosition();
  fDirectionTof = Track->GetDirection();
  fTrackId         = TrackId;
  fPositionTarget  = Track->GetInitialPosition();
  fDirectionTarget = Track->GetInitialDirection();
}

/*******************************************************************************/
GlobalTrackCandidate::~GlobalTrackCandidate() 
{

}


/*******************************************************************************/
void GlobalTrackCandidate::ComputeProperties() {

  //E^2=(pc)^2+(m_0c^2)^2
  //-> (m_0c^2)=((pc)^2-T^2)/2T
  //TODO: add here a check if called before getters can be called
  
  //TODO: units are most like not yet correct
  
  //T=(gamma-1)*m_0c^2
  Double_t beta = 0.;

  if ((fTofFront > 0.) && (fTofRear > 0.)) {
    beta = 0.5 * (fPathIntTofwFront / fTofFront + fPathIntTofwRear
		  / fTofRear) / (kVOfLight * TAGgeoTrafo::MToCm());//factor 100. convert (m/s) -> (cm/s)
  } else {
    if (fTofFront > 0.) {
      beta = (fPathIntTofwFront / fTofFront) / (kVOfLight * TAGgeoTrafo::MToCm());//factor 100. convert (m/s) -> (cm/s)
    } else {
      beta = (fPathIntTofwRear / fTofRear) / (kVOfLight * TAGgeoTrafo::MToCm());//factor 100. convert (m/s) -> (cm/s)
    }
  }
  Info("GlobalTrackCandidate::ComputeProperties()"," beta: %lf",beta);
  
  if (beta >= 1.)
    beta = 1. - 1.E-12;
  Double_t gamma = 1. / max(1.E-12, 1. - beta * beta);
  gamma = TMath::Sqrt(gamma);
  fEnergyFromTof = (gamma - 1.) * kMassNucl;//GeV/n
  // energy resolution (by Gaussian error propagation):
  // dE/E=-dt/t * gamma* (gamma+1)
  
  // p=gamma*beta*m_0*c
  //  -> m_0=p/(gamma*beta*c)
  fMass = fMomentum / (gamma * beta);//GeVc^2
   //mass resolution (by Gaussian error propagation):
}

/*******************************************************************************/
void GlobalTrackCandidate::RateTheMatch(GlobalTrackCandidate* OtherTrack,
		Int_t TrackId) 
{
   //add later here maybe also the region (i.e.: Target or Tof)
   
   
   //compute rating with an objective function
   Double_t DeltaPosition = (fPositionTarget
							 - OtherTrack->GetPositionTarget()).Mag();//UNITS: cm
   
   Double_t DeltaAngle = fDirectionTarget.Angle(
												OtherTrack->GetDirectionTarget()) / TMath::RadToDeg();//UNITS: degree

   Double_t Score = 10. * DeltaPosition + 10. * DeltaAngle;
   //COMMENT: we could also add something like the charge from vertex in comparison with the charge from TOF-Wall
   
   fRatingId.push_back(TrackId);
   fRatingScore.push_back(Score);
}


/*******************************************************************************/
void GlobalTrackCandidate::RateTheMatch() 
{
   //add later here maybe also the region (i.e.: Target or Tof)
   //compute rating with an objective function
  // Double_t DeltaPosition = (fPositionTof - fMatchedPosition).Mag();//UNITS: cm
   
   //Done: we could also add something like the charge from vertex in comparison with the charge from TOF-Wall
   TAVTtrack* track         = 0x0;
   Int_t      vtxCharge     = 0;
   Float_t    vtxProbCharge = 0.;
   
   if (fMatVtx) {
      track = fMatVtx->GetTrack(fTrackId);
      if (track) {
         vtxCharge     = track->GetChargeWithMaxProba();
         vtxProbCharge = track->GetChargeMaxProba()/100.;
      }
   }
   
  // Double_t DeltaCharge = TMath::Abs(vtxCharge/vtxProbCharge - fCharge);
   Double_t DeltaCharge = TMath::Abs(vtxCharge - fCharge);
   Double_t Score = 0; //10. * DeltaPosition;
   Score +=  10. * DeltaCharge;
   
   fRatingId.push_back(-1);//just to fill something here
   fRatingScore.push_back(Score);
}


/*******************************************************************************/
void GlobalTrackCandidate::SortMatches() {
	//TODO: check that it is working well with integers
	//TODO: sorting in which direction?
	SortVec(&fRatingScore, &fRatingId);

}

/*******************************************************************************/
/*-----------------------------------------------------------------*/
// sort 2 Double_t vectors after the first vector
/*-----------------------------------------------------------------*/
Int_t GlobalTrackCandidate::SortVec(std::vector<Double_t> *fVec1,
		std::vector<Double_t> *fVec2) {

	if ((*fVec1).size() != (*fVec1).size()) {
		cerr
				<< "ERROR: vectors could not be sorted as they have a different length!"
				<< endl;
		exit(-1);
		return -1;
	}
	std::vector<Double_t> fVec1unsort(*fVec1);
	std::vector<Double_t> fVec2unsort(*fVec2);

	std::sort((*fVec1).begin(), (*fVec1).end());
	(*fVec2).clear();
	for (unsigned int i1 = 0; i1 < (*fVec1).size(); i1++) {
		for (unsigned int i2 = 0; i2 < fVec1unsort.size(); i2++) {
			if ((*fVec1)[i1] == fVec1unsort[i2]) {
				(*fVec2).push_back(fVec2unsort[i2]);
				fVec1unsort.erase(fVec1unsort.begin() + i2);
				fVec2unsort.erase(fVec2unsort.begin() + i2);
				break;
			}
		}
	}
	return 0;
}

/*******************************************************************************/
Int_t GlobalTrackCandidate::SortVec(std::vector<Double_t> *fVec1,
		std::vector<Int_t> *fVec2) {

	if ((*fVec1).size() != (*fVec1).size()) {
		cerr
				<< "ERROR: vectors could not be sorted as they have a different length!"
				<< endl;
		exit(-1);
		return -1;
	}
	std::vector<Double_t> fVec1unsort(*fVec1);
	std::vector<Int_t> fVec2unsort(*fVec2);

	std::sort((*fVec1).begin(), (*fVec1).end());
	(*fVec2).clear();
	for (unsigned int i1 = 0; i1 < (*fVec1).size(); i1++) {
		for (unsigned int i2 = 0; i2 < fVec1unsort.size(); i2++) {
			if ((*fVec1)[i1] == fVec1unsort[i2]) {
				(*fVec2).push_back(fVec2unsort[i2]);
				fVec1unsort.erase(fVec1unsort.begin() + i2);
				fVec2unsort.erase(fVec2unsort.begin() + i2);
				break;
			}
		}
	}
	return 0;
}

vector<TVector3> GlobalTrackCandidate::GetPath() {
	return fPath;
}


void GlobalTrackCandidate::AttachHit(TATntuRawHit* hit) {

  fattachedHits.push_back(hit);
  return;

}

TATntuRawHit* GlobalTrackCandidate::GetAttachedHit(int ih) {

  if(ih<0 || ih>(int)(fattachedHits.size()-1)) return NULL;
  else return fattachedHits.at(ih);

}



///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



#ifndef CLASS_TrackableParticle
#define CLASS_TrackableParticle

#include <math.h>
#include <fstream>
#include <stdio.h>
#include <stdlib.h> 
#include <vector>
#include <iostream>

#include <TROOT.h>
#include "TRandom3.h"
#include "TVector3.h"
#include "TMath.h"
#include "TString.h"

#include "TAGgeoTrafo.hxx"
#include "MagneticField.hxx"

using namespace std;

static const Double_t kElementaryCharge = 1.60217646E-19;//UNITS: Coulombs
static const Double_t kVOfLight = 2.99792458E+8;//UNITS: m/s
static const Double_t kMassProt = .938272046; //UNITS: GeVc^2
static const Double_t kMassNeut = .939565379; //UNITS: GeVc^2
static const Double_t kMassNucl = (kMassProt + kMassNeut) / 2.; //UNITS: GeVc^2


class TrackableParticle {

private:
  TVector3 fPosition;
  TVector3 fMomentum;
  TVector3 fInitialPosition;
  TVector3 fInitialMomentum;
  Double_t fCharge;
  Double_t fScaleMom;
  MagneticField *fField;
  TAGgeoTrafo *fGeoTrafo;
  vector<TVector3> fPath;
  Double_t fPathIntTofwFront;
  Double_t fPathIntTofwRear;

  Double_t fTimeInv;

  Int_t fLoopIterationMax;

  Double_t fStepMax;//UNITS:cm
  Double_t fStepMin;//UNITS:cm
  Double_t fStepBase;//UNITS:cm
  Double_t fAllowedChangeInAnglePerStep;//UNITS:radian


public:
  Int_t fDebugLevel;
  TrackableParticle(MagneticField *Field);
  TrackableParticle(MagneticField *Field, TVector3 Position,
		    TVector3 Momentum, Double_t Charge);
  TrackableParticle(const TrackableParticle& track);
   
  ~TrackableParticle();

  void Initialize();
  void Reset();
  void ResetAndScaleMomentum(Double_t MomentumScale);
  Double_t PropagateFieldFree(TVector3 PlanePosition, TVector3 PlaneNormal,
			      Bool_t AddPath, Bool_t PropagateBackwards);
  Int_t PropagateInField(TVector3 BoxPosition, TVector3 BoxSize,
			 Bool_t AddPath, Bool_t PropagateBackwards,
			 Double_t SavePathDistance);
  TVector3 EqOfMotionRhs(TVector3 LabMomentum, TVector3 Field);
  Int_t RungeKutta4(TVector3 *Pos, TVector3 *Mom, Double_t Step);
  Double_t AddPathToTofwRear(Double_t DistanceTofwRear);
  Double_t AddPathToTofwRearGl(Double_t DistanceTofwRear, TVector3 TofPlaneNormal);
  Bool_t IsParticleInIP(TVector3 InteractionPoint,
			TVector3 InteractionPointToleranz);
  Bool_t IsParticleInIP_Y(TVector3 InteractionPoint,
			  TVector3 InteractionPointToleranz);
  Bool_t IsParticleInIP_X(TVector3 InteractionPoint,
			  TVector3 InteractionPointToleranz);

  Bool_t IsInsideBox(TVector3 Point, TVector3 BoxPosition, 
		     TVector3 BoxDimension);

  Int_t PrintPath() const;

  TVector3 GetPosition() const {
    return fPosition;
  }

  ;
  TVector3 GetMomentum() const {
    return fMomentum;
  }

  ;
  TVector3 GetDirection() const {
    return fMomentum.Unit();
  }

  ;

  /*******************************************************************************/

  TVector3 GetInitialPosition() const {
    return fInitialPosition;
  }
  ;
  TVector3 GetInitialMomentum() const {
    return fInitialMomentum;
  }
  ;
  TVector3 GetInitialDirection() const {
    return fInitialMomentum.Unit();
  }
  ;

  Double_t GetCharge() const {
    return fCharge;
  }

  ;
  Double_t GetPathIntegralTofwFront() const {
    return fPathIntTofwFront;
  }

  ;
  Double_t GetPathIntegralTofwRear() const {
    return fPathIntTofwRear;
  }

  ;
  vector<TVector3> GetPath() const {
    return fPath;
  }

  ;
  void SetPosition(TVector3 Position) {
    fPosition = Position;
  }

  ;
  void SetMomentum(TVector3 Momentum) {
    fMomentum = Momentum;
  }

  ;
  void SetCharge(Double_t Charge) {
    fCharge = Charge;
  }

  ;

  void SetInitialMomentum(TVector3 initialMomentum) {
   fInitialMomentum = initialMomentum;
  }
  ;

  void SetInitialPosition(TVector3 initialPosition) {
    fInitialPosition = initialPosition;
  }
  ;


protected:
  ClassDef(TrackableParticle,1);


};

#endif

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



#include "TrackableParticle.hxx"
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"

ClassImp(TrackableParticle);

/*******************************************************************************/
TrackableParticle::TrackableParticle(MagneticField *Field) :
  fField(Field) {
  fInitialPosition = fPosition = TVector3(0., 0., 0.);
  fInitialMomentum = fMomentum = TVector3(0., 0., 0.);
  fCharge = 0.;
  
  Initialize();
  
  //load transformations
  fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fGeoTrafo == 0x0) {
    Error("SetGeoTrafoName","No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());
    exit(1);
  }
}

/*******************************************************************************/
TrackableParticle::TrackableParticle(MagneticField *Field, TVector3 Position,
				     TVector3 Momentum, Double_t Charge) {
  fInitialPosition = fPosition = Position;
  fInitialMomentum = fMomentum = Momentum;
  fField = Field;
  fCharge = Charge;
  
  Initialize();
  
  //load transformations
  fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  if (fGeoTrafo == 0x0)
    Error("SetGeoTrafoName", "No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());
}

/*******************************************************************************/
TrackableParticle::TrackableParticle(const TrackableParticle& track)
: fPosition(track.fPosition),
  fMomentum(track.fMomentum),
  fInitialPosition(track.fInitialPosition),
  fInitialMomentum(track.fInitialMomentum),
  fCharge(track.fCharge),
  fScaleMom(track.fScaleMom),
  fField(track.fField),
  fPathIntTofwFront(track.fPathIntTofwFront),
  fPathIntTofwRear(track.fPathIntTofwRear),
  fTimeInv(track.fTimeInv),
  fLoopIterationMax(track.fLoopIterationMax),
  fStepMax(track.fStepMax),
  fStepMin(track.fStepMin),
  fStepBase(track.fStepBase),
  fAllowedChangeInAnglePerStep(track.fAllowedChangeInAnglePerStep)
{
   for (UInt_t i = 0; i < track.fPath.size(); ++i)
      fPath.push_back(track.fPath[i]);
   
   fGeoTrafo =  (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   if (fGeoTrafo == 0x0) {
      Error("SetGeoTrafoName", "No GeoTrafo action called %s available", TAGgeoTrafo::GetDefaultActName().Data());
      exit(1);
   }
}

/*******************************************************************************/
void TrackableParticle::Initialize() 
{
  fPath.clear();
  fPathIntTofwFront = 0.;
  fPathIntTofwRear  = 0.;
  fScaleMom         = 1.;
  fDebugLevel       = 1;
  //parameters for tracking in field
  fLoopIterationMax = 1000000;
  
  fStepMax  = 10.;//UNITS:cm
  fStepMin  = 0.01;//UNITS:cm
  fStepBase = 10.;//UNITS:cm
  fAllowedChangeInAnglePerStep = TMath::DegToRad();//UNITS:radian
  //	const Double_t AllowedChangeInMomPerStep=0.1;//UNITS:1
  //NOTE: we could also compare at the initial point the change in velocity (force) to the current velocity OR use an estimated error of the stepping
  //EqOfMotionRhs(TVector3 LabMomentum, TVector3 Field)
  
}

/*******************************************************************************/
TrackableParticle::~TrackableParticle() {

}

/*******************************************************************************/
void TrackableParticle::Reset() {

  fPosition = fInitialPosition;
  fMomentum = fInitialMomentum;
  fPath.clear();
  fPathIntTofwFront = 0.;
  fPathIntTofwRear = 0.;
}

/*******************************************************************************/
void TrackableParticle::ResetAndScaleMomentum(Double_t MomentumScale) {
	fPosition = fInitialPosition;
	fScaleMom *= MomentumScale;
	fMomentum = fInitialMomentum * fScaleMom;
	fPath.clear();
	fPathIntTofwFront = 0.;
	fPathIntTofwRear = 0.;

}

/*******************************************************************************/
Double_t TrackableParticle::AddPathToTofwRear(Double_t DistanceTofwRear) {

//TODO:	TrafoMusic2TofWall();
// changed in the global reference frame (VM)

	Double_t Path = 0.;
	if (fMomentum.CosTheta() == 0.) {
		cout	<< "WARNING: TrackableParticle::AddPathToTofwRear(): a parallel particle. Path is set to a large value!"
				<< endl;
		Path = 1.E20;
	} else {
		//we assume that we are in TOF-Wall Frame
		Path = DistanceTofwRear / fMomentum.CosTheta();
	}

	fPathIntTofwRear += fabs(Path);
//	fPath.push_back(GetPosition());

	//TODO: TrafoTofWall2Music();
	return Path;
}

Double_t TrackableParticle::AddPathToTofwRearGl(Double_t DistanceTofwRear,TVector3 TofPlaneNormal) {

// added to calculate the distance in the global reference frame (VM)

  Double_t Path = 0.;
  if (fMomentum.Dot(TofPlaneNormal) == 0) {
    cout	<< "WARNING: TrackableParticle::AddPathToTofwRear(): a parallel particle. Path is set to a large value!"
		<< endl;
    Path = 1.E20;
  } else {
    //we assume that we are in TOF-Wall Frame
    Path = (DistanceTofwRear / fMomentum.Dot(TofPlaneNormal))*fMomentum.Mag()*TofPlaneNormal.Mag();
  }

  fPathIntTofwRear += fabs(Path);
//	fPath.push_back(GetPosition());
  
  //TODO: TrafoTofWall2Music();
  return Path;
}

/*******************************************************************************/
Double_t TrackableParticle::PropagateFieldFree(TVector3 PlanePosition,
		TVector3 PlaneNormal, Bool_t AddPath, Bool_t PropagateBackwards) {

	Double_t Path = 0.;

	TVector3 Direction = fMomentum.Unit();
	//	if (PropagateBackwards)
	//		cout<< "WARNING: TrackableParticle::PropagateFieldFree(): PropagateBackwards=true has for the moment no influence!"<< endl;
	//		Direction = -Direction;

	PlaneNormal = PlaneNormal.Unit();//normalize ... just in case
	
//		cout << "DEBUG: Position at plane:"<<fPosition.X()<<" "<<fPosition.Y()<<" "<<fPosition.Z()<<endl;
//		cout << "DEBUG: Momentum at plane:"<<fMomentum.X()<<" "<<fMomentum.Y()<<" "<<fMomentum.Z()<<endl;
//		cout << "DEBUG: plane pos:        "<<PlanePosition.X()<<" "<<PlanePosition.Y()<<" "<<PlanePosition.Z()<<endl;
//		cout << "DEBUG: plane normal:     "<<PlaneNormal.X()<<" "<<PlaneNormal.Y()<<" "<<PlaneNormal.Z()<<endl;
//	cout << "DEBUG: Direction:     "<<Direction.X()<<" "<<Direction.Y()<<" "<<Direction.Z()<<endl;

	//check if parallel:
	if (PlaneNormal.Dot(Direction) == 0.) {
		cout
				<< "WARNING: TrackableParticle::PropagateFieldFree(): a parallel particle. Path is set to 0."
				<< endl;
		Path = 1.E20;
	} else {
		//calculate intersection point and distance (line with plane)
		Path = -PlaneNormal.Dot(fPosition - PlanePosition) / PlaneNormal.Dot(
				Direction);
		//		cout <<"DEBUG: the path is:"<<Path<<endl;

	}

	//	cout << "DEBUG: Position before plane:"<<fPosition.X()<<" "<<fPosition.Y()<<" "<<fPosition.Z()<<endl;
	//	cout << "DEBUG: Momentum before plane:"<<fMomentum.X()<<" "<<fMomentum.Y()<<" "<<fMomentum.Z()<<endl;
	//	cout << "DEBUG: Direction before plane:"<<Direction.X()<<" "<<Direction.Y()<<" "<<Direction.Z()<<endl;

	fPosition = Path * Direction + fPosition;//assign new position
	fPath.push_back(GetPosition());

	//	cout << "DEBUG: Position at plane:"<<fPosition.X()<<" "<<fPosition.Y()<<" "<<fPosition.Z()<<endl;
	//	cout << "DEBUG: Momentum at plane:"<<fMomentum.X()<<" "<<fMomentum.Y()<<" "<<fMomentum.Z()<<endl;
	//	cout << "DEBUG: plane pos:        "<<PlanePosition.X()<<" "<<PlanePosition.Y()<<" "<<PlanePosition.Z()<<endl;
	//	cout << "DEBUG: plane normal:     "<<PlaneNormal.X()<<" "<<PlaneNormal.Y()<<" "<<PlaneNormal.Z()<<endl;

	if (AddPath) {
		//add to path vars
		fPathIntTofwFront += fabs(Path);
		fPathIntTofwRear  += fabs(Path);
	}

	return Path;
}

/*******************************************************************************/
Int_t TrackableParticle::PropagateInField(TVector3 BoxPosition,
					  TVector3 BoxSize, Bool_t AddPath, Bool_t PropagateBackwards,
					  Double_t SavePathDistance) 
{
  if(PropagateBackwards) {
    fTimeInv=-1.;
    //we go backwards in time ... change the particle momentum
    fMomentum *= -1.;
  } else {
    fTimeInv=1.;
  }
  
  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
  Double_t TrialStep;
  TVector3 TrialPos;
  TVector3 TrialMom;
  Double_t TrialChangeInMomPerStep;
  Double_t TrialChangeInAnglePerStep;
  Bool_t StepIsOk;
  Double_t CurrentPath = fPathIntTofwFront;
  fPath.push_back(GetPosition());
  
  Bool_t IsParticleGoingForward = false;
  Int_t CountHelicalY = 0;//counter to detect helical movement and abort loop
  
  // go back to local ref of Aladin
  fPosition   = fpFirstGeo->FromGlobalToALLocal(fPosition);
  fMomentum   = fpFirstGeo->VecFromGlobalToALLocal(fMomentum);
  //BoxPosition = fpFirstGeo->FromGlobalToALLocal(BoxPosition);
  
  for (Int_t Loop = 0; Loop < fLoopIterationMax; Loop++) {
    
    //find an adequate step size
    StepIsOk = false;
    TrialStep = fStepBase;
    while (!StepIsOk) {
      TrialPos = fPosition;
      TrialMom = fMomentum;
      RungeKutta4(&TrialPos, &TrialMom, TrialStep);
      TrialChangeInMomPerStep = (TrialMom - fMomentum).Mag() / fMomentum.Mag();
      TrialChangeInAnglePerStep = fMomentum.Angle(TrialMom);
      
      //compare the result
      if (TrialChangeInAnglePerStep > fAllowedChangeInAnglePerStep) {//we require a relative small change in the angle
	//			if ( TrialChangeInMomPerStep > AllowedChangeInMomPerStep){//we require a relative small change in the momentum
	//TrialStep is to coarse -> reduce it
	TrialStep *= fAllowedChangeInAnglePerStep / TrialChangeInAnglePerStep * 0.5;
	//				TrialStep*=AllowedChangeInMomPerStep/TrialChangeInMomPerStep*0.5;
	if (TrialStep < fStepMin) {
	  Warning("TrackableParticle::PropagateInField()",
		  "TrialStep %lf <StepMin, set it to StepMin= %lf and proceed",
		  TrialStep,fStepMin);
	  TrialStep = fStepMin;
	  StepIsOk = true;
	}
      } else {
	StepIsOk = true;
      }
    }

    //do the step
    RungeKutta4(&fPosition, &fMomentum, TrialStep);
    //add to path 
    if (AddPath) {
      //add to path vars
      fPathIntTofwFront += fabs(TrialStep);
      fPathIntTofwRear += fabs(TrialStep);
      //NOTE: do be more precise here we could calculate path with spline (or along arc)
    }
    if (CurrentPath + fabs(SavePathDistance) <= fPathIntTofwFront) {
      fPath.push_back(fpFirstGeo->FromALLocalToGlobal(GetPosition()));
      CurrentPath = fPathIntTofwFront;
    }
    
    //TODO: add error approximation for the stepping here?
    
    
    //test where we are in the game
    
    //check that we don't do circles around y-axis!
    //don't let this spiral out of control
    if ((!IsParticleGoingForward) && (fMomentum.Z() > 0))
      CountHelicalY++;
    
    if (fMomentum.Z() > 0) {
      IsParticleGoingForward = true;
    } else {
      IsParticleGoingForward = false;
    }
    if (CountHelicalY > 10) {
      cout
	<< "DEBUG: TrackableParticle::PropagateInField(): Particle with helical motion. Stop tracking."
	<< endl;
      break;
    }
    
    //leaving the field
    if (!IsInsideBox(fPosition, BoxPosition, BoxSize * 0.5)) {
      break;
    }
    
    if (Loop == fLoopIterationMax) {
      cout	<< "WARNING: TrackableParticle::PropagateInField(): Maximal looping number reached ("
		<< Loop << "). Abort tracking." << endl;
    }
    
  }//END OF STEPPING LOOP
  
  // go back to global ref.
  fPosition = fpFirstGeo->FromALLocalToGlobal(fPosition);
  fMomentum = fpFirstGeo->VecFromALLocalToGlobal(fMomentum);
  fPath.push_back(GetPosition());
  
  if (PropagateBackwards) {
    //we go backwards in time ... restore the particle momentum
    fMomentum *= -1.;
  }
  
  //	cout << "DEBUG: Exit Position:" << fPosition.X() << " " << fPosition.Y()
  //			<< " " << fPosition.Z() << endl;
  //	cout << "DEBUG: Exit Momentum:" << fMomentum.X() << " " << fMomentum.Y()
  //			<< " " << fMomentum.Z() << endl;
  
  return 0;
}

/*******************************************************************************/
//Return differential change in direction in relation to normalized motion vector.
TVector3 TrackableParticle::EqOfMotionRhs(TVector3 LabMomentum, TVector3 Field) {

	TVector3 Motion(0, 0, 0);

	//	Double_t Scale = fCharge * kElementaryCharge * kVOfLight
	//			/ LabMomentum.Mag(); //factor 100. is conversion m -> cm?

   Double_t Scale =  -fTimeInv*fCharge * kVOfLight / 1.E+9 / TAGgeoTrafo::MToCm() / LabMomentum.Mag(); //factor 100. is conversion m -> cm
	//false:	LabMomentum = LabMomentum.Unit();

	//	F=q*(v x B)
	//	where
	//	F is the force (in newtons)
	//	E is the electric field (in volts per metre)
	//	B is the magnetic field (in teslas)
	//	q is the electric charge of the particle (in coulombs)
	//	v is the instantaneous velocity of the particle (in metres per second)
	//	× is the vector cross product
	//	r= mv^2/F

	//Lorentz force
	Motion = Scale * LabMomentum.Cross(Field);

	return Motion;

}


/*******************************************************************************/
//input: current position, momentum, and desired step
//output: new position and momentum
Int_t TrackableParticle::RungeKutta4(TVector3* Pos, TVector3* Mom,
		Double_t Step) {

	Double_t HalfStep = 0.5 * Step;
	TVector3 ChangePos[5];
	TVector3 ChangeMom[5];

	TVector3 PosInter;
	TVector3 MomInter;//intermediate momentum

	ChangePos[0] = Mom->Unit();
	ChangeMom[0] = EqOfMotionRhs(*Mom, fField->GetField(*Pos));
 
	//	cout<<"DEBUG: dPos/dS ("<<ChangePos[0].X()<<","<<ChangePos[0].Y()<<","<<ChangePos[0].Z()<<") in cm/cm"<<endl;
	//	cout<<"DEBUG: dMom/dS ("<<ChangeMom[0].X()<<","<<ChangeMom[0].Y()<<","<<ChangeMom[0].Z()<<") in GeV/c/cm"<<endl;


	//TTB go half a step using force at initial point and get force at middle point (dydxt)	
	PosInter = *Pos + HalfStep * ChangePos[0];
	MomInter = *Mom + HalfStep * ChangeMom[0];

	ChangePos[1] = MomInter.Unit();//dydxt
	ChangeMom[1] = EqOfMotionRhs(MomInter, fField->GetField(PosInter));

	//TTB go again half a step using  force at middle point (dydxt)
	PosInter = *Pos + HalfStep * ChangePos[1];
	MomInter = *Mom + HalfStep * ChangeMom[1];

	ChangePos[2] = MomInter.Unit();//dydxm
	ChangeMom[2] = EqOfMotionRhs(MomInter, fField->GetField(PosInter));

	//TTB go whole step using force at new middle point (dydxm)
	PosInter = *Pos + Step * ChangePos[2];
	MomInter = *Mom + Step * ChangeMom[2];

	ChangePos[3] = MomInter.Unit();
	ChangeMom[3] = EqOfMotionRhs(MomInter, fField->GetField(PosInter));

	//calculate final step: pos and mom
	PosInter = *Pos + Step / 6.0 * (ChangePos[0] + 2. * (ChangePos[1]
			+ ChangePos[2]) + ChangePos[3]);
	MomInter = *Mom + Step / 6.0 * (ChangeMom[0] + 2. * (ChangeMom[1]
			+ ChangeMom[2]) + ChangeMom[3]);

	//assure momentum conservation
	MomInter = Mom->Mag() / MomInter.Mag() * MomInter;

	//and assign it
	*Pos = PosInter;
	*Mom = MomInter;

	return 0;
}

/*******************************************************************************/
//check all tolerances
Bool_t TrackableParticle::IsParticleInIP(TVector3 InteractionPoint,
		TVector3 InteractionPointToleranz) {

	return IsInsideBox(fPosition, InteractionPoint,
			2. * InteractionPointToleranz);

}

/*******************************************************************************/
//check only x tolerance
Bool_t TrackableParticle::IsParticleInIP_X(TVector3 InteractionPoint,
					   TVector3 InteractionPointToleranz) {

  double dist  = fPosition.X() - InteractionPoint.X();
  return (fabs(dist) < fabs(InteractionPointToleranz.X())) ? true : false;

}

Bool_t TrackableParticle::IsInsideBox(TVector3 Point, TVector3 BoxPosition, TVector3 BoxDimension){
  
  
  if ( (fabs(BoxDimension.X())>fabs(Point.X()-BoxPosition.X()))
       &&(fabs(BoxDimension.Y())>fabs(Point.Y()-BoxPosition.Y()))
       &&(fabs(BoxDimension.Z())>fabs(Point.Z()-BoxPosition.Z())) ){
    return true;	
  }else{
    return false;	
  }
  
}

/*******************************************************************************/
//check only y tolerance
Bool_t TrackableParticle::IsParticleInIP_Y(TVector3 InteractionPoint,
		TVector3 InteractionPointToleranz) {

	return (fabs(fPosition.Y() - InteractionPoint.Y()) < fabs(
			InteractionPointToleranz.Y())) ? true : false;

}

/*******************************************************************************/
Int_t TrackableParticle::PrintPath() const {

  Info("TrackableParticle::PrintPath()","*******************************************************************************");
  Info("TrackableParticle::PrintPath()","  Charge: %lf",fCharge);
  Info("TrackableParticle::PrintPath()","  Integrated path to TOF-Wall Front: %lf cm  Rear: %lf cm",fPathIntTofwFront,fPathIntTofwRear);
  Info("TrackableParticle::PrintPath()","  Current position: (%lf %lf %lf) in cm",fPosition.X(),
       fPosition.Y(),fPosition.Z());
  Info("TrackableParticle::PrintPath()","  Current momentum: (%lf %lf %lf) in GeV/c",fMomentum.X(),
       fMomentum.Y(),fMomentum.Z());
  if(fDebugLevel == 2) {
    cout << "  Path of particle (in cm):" << endl;
    for (unsigned int i0 = 0; i0 < fPath.size(); i0++) {
      cout << "(" << setw(7) << fPath[i0].X() << "," << setw(7)
	   << fPath[i0].Y() << "," << setw(7) << fPath[i0].Z() << ")"
	   << endl;
    }
  }
  Info("TrackableParticle::PrintPath()","*******************************************************************************");
  return 0;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************



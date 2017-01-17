#include "Trigger.h"
#include <cmath>

using namespace std;

ClassImp(Trigger);

/*-----------------------------------------------------------------*/
Trigger::Trigger() {
	//printf("Entro in Trigger::Trigger()\n");

	//init Trigger values and vectors
	Init();

	//my count variables
	fNumberOfWantedEvents=0;
	fNumberOfTriggeredEvents=0;
	fNumberOfWantedButNotTriggeredEvents=0;
	fNumberOfWrongTriggers=0;
	fEvaluatedEvents=0;


	//printf("Esco da Trigger::Trigger()\n");
}

/*-----------------------------------------------------------------*/
Trigger::~Trigger() {

}

/*-----------------------------------------------------------------*/
/*Int_t Trigger::BOEvent(){//beginning of event

 TR_Reset();

 return 0;
 }*/

/*-----------------------------------------------------------------*/
/*Int_t Trigger::EOEvent(){//end of event


 return 0;
 }*/

/*-----------------------------------------------------------------*/
// Trigger
/*-----------------------------------------------------------------*/
Int_t Trigger::Init() {

	Reset();
	fIsWanted = false;

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Trigger::Reset() {

	fTR_Flag = false;//trigger method already called this event?

	fTR_ST = false;//start counter

	fTR_TopLevel = false;//Top level trigger


	return 0;
}

/*-----------------------------------------------------------------*/
/**
 * Calculate triggering for this event
 */

TRIGGER_STRUCT Trigger::ComputeTrigger(EVENT_STRUCT* Evt,
		vector<TString>* RegionNames, vector<Int_t>* RegionNumbers) {

	Reset();

	//MAGARITA trigger information
	//AS:: to be fixed
	//	if (Evt->stSigED > kTR_ST_Thres)
		fTR_ST = true;

	//is this event an event which we want?
	EventIsWanted(Evt,RegionNames,RegionNumbers);

	fTR_Flag = true;//flag that trigger was calculated for this event

//	cout << "DEBUG: Event NB: " << Evt->EventNumber << " ST:" << Evt->stSigED << endl;
	TRIGGER_STRUCT Trigger;
	Trigger.EventNumber = Evt->EventNumber;
	Trigger.Magarita = fTR_ST;
	Trigger.Wanted= fIsWanted;

	//do evaluation
	Evaluate(&Trigger);

	return Trigger;

}

/*-----------------------------------------------------------------*/
/**
 * Calculate wanted events
 * definition of "wanted event":
 * an event where we have at least
 * one fragment with E_kin>kWantedEvent_Thres MeV/n
 * created by the primary ion in the target
 */

Bool_t Trigger::EventIsWanted(EVENT_STRUCT* Evt, vector<TString>* RegionNames,
		vector<Int_t>* RegionNumbers) {

	TString TargetRegionName = "target";
	Int_t TargetRegionNumber = -1;

	fIsWanted = false;

	for (unsigned int i0 = 0; i0 < RegionNames->size(); i0++) {
		if ((*RegionNames)[i0].CompareTo(TargetRegionName) == 0)
			TargetRegionNumber = (*RegionNumbers)[i0];

	}

	//loop over tracks
	for (Int_t i0 = 0; i0 < Evt->TRn; i0++) {
		if (Evt->TRreg[i0] == TargetRegionNumber) {
			if (Evt->TRpaid[i0] == 1) {//if parent particle is the primary particle

				if (i0>-1) {//if particle is an ion (we skip neutrons for the moment (8,9) || (Evt->TRtype[i0] == 8))
				  //AS:: reimplement the check once we have the trtype again in the tuples.
				  //				if ((Evt->TRtype[i0] <= -2) || (Evt->TRtype[i0] == 1)) {//if particle is an ion (we skip neutrons for the moment (8,9) || (Evt->TRtype[i0] == 8))
					//now check the kinetic energy of the particle
					Double_t CurrentMass = Evt->TRmass[i0];
					TVector3 CurrentMom(Evt->TRipx[i0], Evt->TRipy[i0],
							Evt->TRipz[i0]);
					Double_t CurrentMomMod(CurrentMom.Mag());
					Int_t CurrentA(Evt->TRbar[i0]);
					//TVector3 CurrentDirection(math.degrees(arccos(curPART_PX/curPART_P),);
					if (CurrentMomMod == 0.0)
						CurrentMomMod = 1.0;
					Double_t CurrentKinE = (std::sqrt(
							std::pow(CurrentMomMod, 2) +std::pow(CurrentMass, 2))
							- CurrentMass) * 1000.;//convert to MeV
					Double_t CurrentKinEn;
					if (CurrentA != 0) {
						CurrentKinEn = CurrentKinE / (Double_t) CurrentA;
					} else {
						CurrentKinEn = CurrentKinE;
					}

					if (CurrentKinEn > kWantedEvent_Thres) {
						fIsWanted = true;
						return fIsWanted;
					}
				}
			}
		}
	}

	return fIsWanted;

}


/*-----------------------------------------------------------------*/
/**
 * Evaluate event
 */

void Trigger::Evaluate(TRIGGER_STRUCT* Trigger) {

	Bool_t IsWanted = Trigger->Wanted;
	Bool_t IsTriggered = Trigger->TopLevel;
	if (IsWanted) {
		fNumberOfWantedEvents++;
		if (!IsTriggered)
			fNumberOfWantedButNotTriggeredEvents++;
	} else {
		if (IsTriggered)
			fNumberOfWrongTriggers++;
	}
	if (IsTriggered)
		fNumberOfTriggeredEvents++;


	fEvaluatedEvents++;

	return;
}


Int_t Trigger::InitBranches(TTree *RootTree, TRIGGER_STRUCT *Trigger) {

	RootTree->Branch("EventNumber", &(Trigger->EventNumber), "EventNumber/I");
	RootTree->Branch("Magarita", &(Trigger->Magarita), "Magarita/O");
	RootTree->Branch("Wanted", &(Trigger->Wanted), "Wanted/O");

	return 0;
}

/*-----------------------------------------------------------------*/

Int_t Trigger::FindBranches(TTree *RootTree, TRIGGER_STRUCT *Trigger) {

	RootTree->SetBranchAddress("EventNumber", &(Trigger->EventNumber));
	RootTree->SetBranchAddress("Magarita", &(Trigger->Magarita));
	RootTree->SetBranchAddress("Wanted", &(Trigger->Wanted));

	// printf("Sono in Evento::FindBranches(): ho trovato i branches \n");
	return 0;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

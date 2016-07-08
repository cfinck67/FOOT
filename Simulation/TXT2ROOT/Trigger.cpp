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
	fTR_SE = false;//small end-cap
	fTR_BE = false;//big end-cap
	fTR_BR = false;//barrel
	fTR_ON = false;//onion veto
	fTR_TWF = false;//TOF-Wall front wall slat hit
	fTR_TWFMulti = false;//TOF-Wall front wall multiple slats hit

	fTR_TopLevel = false;//Top level trigger


	return 0;
}

/*-----------------------------------------------------------------*/
/**
 * Calculate triggering for this event
 */

TRIGGER_STRUCT Trigger::ComputeTrigger(EVENT_STRUCT* Evt,
		vector<TString>* RegionNames, vector<Int_t>* RegionNumbers) {

	TString KE_SE_Names[] = { "ke_SER1", "ke_SER24" };//small endcap
	Int_t KE_SE_Numbers[] = { -1, -1 };
	TString KE_BE_Names[] = { "ke_BER1", "ke_BER60" };//big endcap
	Int_t KE_BE_Numbers[] = { -1, -1 };
	TString KE_BR_Names[] = { "ke_BR1", "ke_FR25" };//barrel
	Int_t KE_BR_Numbers[] = { -1, -1 };

	for (unsigned int i0 = 0; i0 < RegionNames->size(); i0++) {
		for (Int_t i1 = 0; i1 < 2; i1++) {
			if ((*RegionNames)[i0].CompareTo(KE_SE_Names[i1]) == 0)
				KE_SE_Numbers[i1] = (*RegionNumbers)[i0];
			if ((*RegionNames)[i0].CompareTo(KE_BE_Names[i1]) == 0)
				KE_BE_Numbers[i1] = (*RegionNumbers)[i0];
			if ((*RegionNames)[i0].CompareTo(KE_BR_Names[i1]) == 0)
				KE_BR_Numbers[i1] = (*RegionNumbers)[i0];
		}
	}
	//  cout<<"DEBUG: KENTROS regions:"<<KE_BR_Numbers[0]<<" "<<KE_BE_Numbers[0]<<" "<<KE_SE_Numbers[0]<<endl;
	//  cout<<"DEBUG: KENTROS regions:"<<KE_BR_Numbers[1]<<" "<<KE_BE_Numbers[1]<<" "<<KE_SE_Numbers[1]<<endl;
	Reset();

	//MAGARITA trigger information
	if (Evt->stSigED > kTR_ST_Thres)
		fTR_ST = true;

	//KENTROS trigger information
	//TODO: later implement not based on ED but on signal??
	//WARNING: in reality we do not have a full polar angle coverage for the barrel!
	for (Int_t i0 = 0; i0 < Evt->keSigN; i0++) {
		Int_t CurReg = Evt->keSigReg[i0];
	//	cout << "DEBUG: Current Region: " << CurReg << endl;
		//do region decoding
		if ((CurReg >= 1)
				&& (CurReg <= KE_BR_Numbers[1] - KE_BR_Numbers[0] + 1)) { // in barrel region (1 to 84?)
			if (Evt->keSigED[i0] > kTR_BR_Thres)
				fTR_BR = true;//if energy deposition bigger than threshold -> flag

		} else if ((CurReg >= KE_BE_Numbers[0] - KE_BR_Numbers[0] + 1)
				&& (CurReg <= KE_BE_Numbers[1] - KE_BR_Numbers[0] + 1)) { // in big end-cap region (84+1 to 84+59?)
			if (Evt->keSigED[i0] > kTR_BE_Thres)
				fTR_BE = true;

		} else if ((CurReg >= KE_SE_Numbers[0] - KE_BR_Numbers[0] + 1)
				&& (CurReg <= KE_SE_Numbers[1] - KE_BR_Numbers[0] + 1)) {//in small end-cap region (84+59+1 to 84+59+23?)
			if (Evt->keSigED[i0] > kTR_SE_Thres)
				fTR_SE = true;

		} else {
			cerr << "ERROR: Segnale::TR_Trigger(): KENTROS region wrong: "
					<< CurReg << " !" << endl;
			exit(-1);
		}
	}
	//  cout<<"DEBUG: "<<1<<" "<<KE_BR_Numbers[1]-KE_BR_Numbers[0]+1<<" !"<<endl;
	//  cout<<"DEBUG: "<<KE_BE_Numbers[0]-KE_BR_Numbers[0]+1<<" "<<KE_BE_Numbers[1]-KE_BR_Numbers[0]+1<<" !"<<endl;
	//  cout<<"DEBUG: "<<KE_SE_Numbers[0]-KE_BR_Numbers[0]+1<<" "<<KE_SE_Numbers[1]-KE_BR_Numbers[0]+1<<" !"<<endl;

	//TOF-Wall trigger information
	Int_t Count = 0;
	for (Int_t i0 = 0; i0 < TW_NbSlatFront; i0++) {//loop over slats in front wall
		//should be used: twSigAmpFT[i0],twSigAmpFB[i0]
		if (Evt->twSigEDF[i0] > kTR_TW_Thres)
			Count++;
		//cout<<"DEBUG: TOF ED F: "<<Evt->twSigEDF[i0]<<endl;
	}

	//TODO: I'm still not convinced that this is such a good trigger for the experiment
	// Z^2 between carbon and p is a factor of 144 -> with 4% crosstalk we always activate more than one slat!
	//seems to be more promising to flag if two non-neighbouring slats are hit
	//or to play with a threshold in the onion for rejecting events!
	if (Count > 0)
		fTR_TWF = true; //TOF-Wall front wall slat hit
	if (Count > 1)
		fTR_TWFMulti = true;//TOF-Wall front wall multiple slats hit

	//ONION veto trigger information
	for (Int_t i0 = 0; i0 < Evt->onSigN; i0++) {
	  Int_t CurReg = Evt->onSigReg[i0];
	  //TODO WARNING: check that we to the right stuff with the numbering
	  if ((CurReg <= kTR_ON_UntilRingNumber) && (Evt->onSigED[i0]
						     > kTR_ON_Thres))
	    fTR_ON = true;//onion veto
	}
	
	//Logic for top level trigger
	//MAGARITA + KENTROS
	if (fTR_ST && fTR_SE)
		fTR_TopLevel = true;
	if (fTR_ST && fTR_BE)
		fTR_TopLevel = true;
	//Barrel won't be manufactured in time   if (fTR_ST && fTR_BR) fTR_TopLevel=true;

	//MAGARITA + TOF-Wall + ONION
//	if (fTR_ST && fTR_TWFMulti) fTR_TopLevel = true;//take this out??? (see comment above)
	if (fTR_ST && fTR_TWF && !fTR_ON)
		fTR_TopLevel = true;

	//is this event an event which we want?
	EventIsWanted(Evt,RegionNames,RegionNumbers);

	fTR_Flag = true;//flag that trigger was calculated for this event

//	cout << "DEBUG: Event NB: " << Evt->EventNumber << " ST:" << Evt->stSigED << endl;
	TRIGGER_STRUCT Trigger;
	Trigger.EventNumber = Evt->EventNumber;
	Trigger.Magarita = fTR_ST;
	Trigger.SmallEndCap = fTR_SE;
	Trigger.BigEndCap = fTR_BE;
	Trigger.Barrel = fTR_BR;
	Trigger.TofWallFrontSingle = fTR_TWF;
	Trigger.TofWallFrontMulti = fTR_TWFMulti;
	Trigger.Onion = fTR_ON;
	Trigger.TopLevel = fTR_TopLevel;
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
	for (Int_t i0 = 0; i0 < Evt->trn; i0++) {
		if (Evt->trreg[i0] == TargetRegionNumber) {
			if (Evt->trpaid[i0] == 1) {//if parent particle is the primary particle
				if ((Evt->trtype[i0] <= -2) || (Evt->trtype[i0] == 1)) {//if particle is an ion (we skip neutrons for the moment (8,9) || (Evt->trtype[i0] == 8))
				//if ((Evt->trtype[i0] != 3)&&(Evt->trtype[i0]!=4)&&(Evt->trtype[i0]!=7)) {//check that particle not EM particle (e+/- or gamma)
					//now check the kinetic energy of the particle
					Double_t CurrentMass = Evt->trmass[i0];
					TVector3 CurrentMom(Evt->tripx[i0], Evt->tripy[i0],
							Evt->tripz[i0]);
					Double_t CurrentMomMod(CurrentMom.Mag());
					Int_t CurrentA(Evt->trbar[i0]);
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
	RootTree->Branch("SmallEndCap", &(Trigger->SmallEndCap), "SmallEndCap/O");
	RootTree->Branch("BigEndCap", &(Trigger->BigEndCap), "BigEndCap/O");
	RootTree->Branch("Barrel", &(Trigger->Barrel), "Barrel/O");
	RootTree->Branch("TofWallFrontSingle", &(Trigger->TofWallFrontSingle),
			"TofWallFrontSingle/O");
	RootTree->Branch("TofWallFrontMulti", &(Trigger->TofWallFrontMulti),
			"TofWallFrontMulti/O");
	RootTree->Branch("Onion", &(Trigger->Onion), "Onion/O");
	RootTree->Branch("Wanted", &(Trigger->Wanted), "Wanted/O");

	// printf("Sono in Evento::InitBranches(): ho scritto i branches \n");
	return 0;
}

/*-----------------------------------------------------------------*/

Int_t Trigger::FindBranches(TTree *RootTree, TRIGGER_STRUCT *Trigger) {

	RootTree->SetBranchAddress("EventNumber", &(Trigger->EventNumber));
	RootTree->SetBranchAddress("Magarita", &(Trigger->Magarita));
	RootTree->SetBranchAddress("SmallEndCap", &(Trigger->SmallEndCap));
	RootTree->SetBranchAddress("BigEndCap", &(Trigger->BigEndCap));
	RootTree->SetBranchAddress("Barrel", &(Trigger->Barrel));
	RootTree->SetBranchAddress("TofWallFrontSingle",
			&(Trigger->TofWallFrontSingle));
	RootTree->SetBranchAddress("TofWallFrontMulti",
			&(Trigger->TofWallFrontMulti));
	RootTree->SetBranchAddress("Onion", &(Trigger->Onion));
	RootTree->SetBranchAddress("Wanted", &(Trigger->Wanted));

	// printf("Sono in Evento::FindBranches(): ho trovato i branches \n");
	return 0;
}

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

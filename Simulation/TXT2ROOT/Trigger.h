#ifndef ROOT_Trigger
#define ROOT_Trigger

#include "TObject.h"
#include <TROOT.h>
#include <TTree.h>
#include "TRandom3.h"
#include "TVector3.h"
#include <stdlib.h>
#include <cmath>
#include <cstring>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "gsi_geo.h"
#include "Helper.h"
#include "Geometry.h"
#include "Evento.h"

using namespace std;

// coding conventions (ROOT):
// (currently not yet coherently followed)
// kMyVariable = constant
// gMyVariable = global variable
// fMyVariable = class data member


typedef struct {
	Int_t EventNumber;
	Bool_t TopLevel;//Top level trigger
	Bool_t Magarita;//start counter
	Bool_t SmallEndCap;//small end-cap
	Bool_t BigEndCap;//big end-cap
	Bool_t Barrel;//barrel
	Bool_t Onion;//onion veto
	Bool_t TofWallFrontSingle;//TOF-Wall front wall slat hit
	Bool_t TofWallFrontMulti;//TOF-Wall front wall multiple slats hit

	Bool_t Wanted;//Is this an event we want

} TRIGGER_STRUCT;

class Trigger: public TObject {

public:

	Trigger();

	virtual ~Trigger();

	//  Int_t BOEvent();
	//  Int_t EOEvent();

	//Trigger methods
	Int_t Init();
	Int_t Reset();
	TRIGGER_STRUCT ComputeTrigger(EVENT_STRUCT* Evt,
			vector<TString>* RegionNames, vector<Int_t>* RegionNumbers);



	Int_t InitBranches(TTree *RootTree, TRIGGER_STRUCT *Trigger);
	Int_t FindBranches(TTree *RootTree, TRIGGER_STRUCT *Trigger);

private:
	Bool_t EventIsWanted(EVENT_STRUCT* Evt, vector<TString>* RegionNames,
			vector<Int_t>* RegionNumbers);
	void Evaluate(TRIGGER_STRUCT* Trigger);

private:

	//Trigger signal
	Bool_t fTR_Flag;//trigger method already called this event?
	Bool_t fTR_TopLevel;//Top level trigger
	Bool_t fTR_ST;//start counter
	Bool_t fTR_SE;//small end-cap
	Bool_t fTR_BE;//big end-cap
	Bool_t fTR_BR;//barrel
	Bool_t fTR_ON;//onion veto
	Bool_t fTR_TWF;//TOF-Wall front wall slat hit
	Bool_t fTR_TWFMulti;//TOF-Wall front wall multiple slats hit

	Bool_t fIsWanted;//is a wanted event

	//evaluation variables
	Int_t fNumberOfWantedEvents;
	Int_t fNumberOfTriggeredEvents;
	Int_t fNumberOfWantedButNotTriggeredEvents;
	Int_t fNumberOfWrongTriggers;
	Int_t fEvaluatedEvents;//counts how many events where evaluated with the trigger class

	//Trigger calibration parameters
	//Energy deposition thresholds
	constexpr static Double_t kTR_ST_Thres = 0.0; //UNITS:GeV MAGARITA
	constexpr static Double_t kTR_SE_Thres = 0.002; //UNITS:GeV KENTROS small end-cap
	constexpr static Double_t kTR_BE_Thres = 0.002; //UNITS:GeV KENTROS big end-cap
	constexpr static Double_t kTR_BR_Thres = 0.002; //UNITS:GeV KENTROS barrel
	constexpr static Double_t kTR_TW_Thres = 0.002; //UNITS:GeV TOF-Wall
	constexpr static Double_t kTR_ON_Thres = 0.010; //UNITS:GeV ONION
//KENTROS: Bruno and Piernicola:
//	However, in this moment a reasonable threshold value for the deposited energy
//	can be considered as a fraction of the energy deposited by a MIP in the
//	scintillator (~ 7 MeV).
//	So we consider that about 2-3 MeV can be considered an acceptable value for the
//	threshold for the triggering signal.


	constexpr static Int_t kTR_ON_UntilRingNumber = 3; //ONION number of rings in ONION used for triggering
	//maximum is: TW_Onion_Nb
	//3: is about 95% of primaries (12C@400 on C)
	//4: is about 98% of primaries (12C@400 on C)

	constexpr static Double_t kWantedEvent_Thres =10.0; //10.0;//UNITS:MeV/n

public:

	// get methods //////////////////////////////////////////////////////////////////////////////

	Bool_t IsStartCounter() const {

		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsStartCounter(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_ST;
	}

	Bool_t IsSmallEndCap() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsSmallEndCap(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_SE;
	}

	Bool_t IsBigEndCap() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsBigEndCap(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_BE;
	}

	Bool_t IsBarrel() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsBarrel(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_BR;
	}

	Bool_t IsTOFWallFSingle() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsTOFWallFSingle(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_TWF;
	}

	Bool_t IsTOFWallFMulti() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsTOFWallFMulti(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_TWFMulti;
	}

	Bool_t IsONION() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsONION(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_ON;
	}

	Bool_t IsTopLevel() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsTopLevel(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fTR_TopLevel;
	}

	Bool_t IsWanted() const {
		if (!fTR_Flag) {
			cerr
					<< "ERROR: Trigger::IsWanted(): Call Trigger::ComputeTrigger() before calling this method!"
					<< endl;
			exit(1);
		}

		return fIsWanted;
	}


	void PrintResults() const{

		if (fEvaluatedEvents == 0) {
			cout << "Bad stats, men!" << endl;
		} else {
			cout << "=================================================="
					<< endl;
			cout << "=== Trigger Evaluation ==========================="
					<< endl;
			cout << "=================================================="
					<< endl;
			cout << " Evaluated events:            "
					<< (double) fEvaluatedEvents << endl;
			cout << " Trigger efficiency:          "
					<< ((double) fNumberOfWantedEvents
							- (double) fNumberOfWantedButNotTriggeredEvents)
							/ (double) fNumberOfWantedEvents << endl;
			cout << " False trigger:               "
					<< (double) fNumberOfWrongTriggers
							/ (double) fNumberOfTriggeredEvents << endl;
			cout << " Fraction of wanted events:   "
					<< (double) fNumberOfWantedEvents
							/ (double) fEvaluatedEvents << endl;
			cout << " Fraction of triggered events:"
					<< (double) fNumberOfTriggeredEvents
							/ (double) fEvaluatedEvents << endl;
			cout << endl;
			cout << " Threshold settings:           " << endl;
			cout << " MARARITA:    " << kTR_ST_Thres * 1000. << "MeV" << endl;
			cout << " KENTROS(SE,BE,BR): " << kTR_SE_Thres * 1000. << ", "
					<< kTR_BE_Thres * 1000. << ", " << kTR_BR_Thres * 1000.
					<< "MeV" << endl;
			cout << " TOF-Wall:    " << kTR_TW_Thres * 1000. << "MeV" << endl;
			cout << " ONION:       " << kTR_ON_Thres * 1000.
					<< "MeV, Number of rings: " << kTR_ON_UntilRingNumber
					<< endl;
			cout << " Wanted events kinetic energy threshold: "
					<< kWantedEvent_Thres << "MeV/n" << endl;
			cout << "=================================================="
					<< endl;
		}

		return;
	}


protected:
	ClassDef(Trigger,1);

};

#endif

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

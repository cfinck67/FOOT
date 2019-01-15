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

#include "Helper.h"
#include "../Geometry.h"
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

#ifndef ROOT_Segnale
#define ROOT_Segnale

#include "TObject.h"
#include "TRandom3.h"
#include "TVector3.h"
#include <stdlib.h>
#include <cmath>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>

#include "Helper.h"
#include "../Geometry.h"
//#include "Evento.h"

// VM added 9/1/2014
#include "TFile.h"
#include "TH1D.h"

using namespace std;


// coding conventions (ROOT):
// (currently not yet coherently followed)
// kMyVariable = constant
// gMyVariable = global variable
// fMyVariable = class data member

class vec_sort_pix {
 public:
  vec_sort_pix(Int_t ix,Int_t iy,Double_t d) {
    id_x=ix;
    id_y=iy;
    dist=d;
  }
  ~vec_sort_pix() {};
  Int_t idx() { return id_x; };
  Int_t idy() { return id_y; };
  Double_t dst() { return dist; };
 private:
  Int_t id_x;
  Int_t id_y;
  Double_t dist;
};

// VM 5/11/13
struct less_diff
{
    inline bool operator() (vec_sort_pix* v1, vec_sort_pix* v2)
    {
      return (v1->dst() < v2->dst());
    }
};


class Segnale: public TObject {

public:

	//  Segnale();
	Segnale(Geometry *TheGeometry);

	virtual ~Segnale();

	Int_t BOEvent();
	Int_t EOEvent();

	//Start counter methods
	Int_t SC_Init();
	Int_t SC_Reset();
	Int_t SC_Score(Double_t Charge, Double_t ER, Double_t QER, Double_t InX,
			Double_t InY, Double_t InZ, Double_t OutX, Double_t OutY,
			Double_t OutZ, Double_t Time);
	Int_t SC_Calibration();
	Int_t SC_Noise();
	Int_t SC_Digitisation();

	//MIMOSA26 methods
	Int_t MI_Init();
	Int_t MI_Reset();
	// VM 30/1/2014 particle charge needed for vtx cluster size
	Int_t MI_Score(Int_t Chip, Int_t Column, Int_t Row, Int_t ID, 
		       Int_t Chg, Double_t ED,
		       Double_t X, Double_t Y, Double_t Z, 
		       Double_t pX, Double_t pY, Double_t pZ,Double_t Time);

	Int_t MI_Noise();
	bool activatePad(Int_t ch, Int_t c_ch, Int_t co, 
			 Int_t c_co, Int_t ro, Int_t c_ro);


	//Utilities
	Int_t SortVec(std::vector<Double_t> *fVec1, std::vector<Double_t> *fVec2);

private:

	Geometry* Geo;
	TRandom3 rnd;

	//Start counter signal
	std::vector<Double_t> fSC_AmpV;
	Double_t fSC_Amp;
	Int_t fSC_AmpDig;
	Double_t fSC_ED;

	std::vector<Double_t> fSC_TimeV;
	Double_t fSC_Time;
	Int_t fSC_TimeDig;

	//MIMOSA signal
	Int_t fMI_Nb;
	std::vector<Int_t> fMI_Chip;
	std::vector<Int_t> fMI_Column;
	std::vector<Int_t> fMI_Row;
	std::vector<Int_t> fMI_ID;
	std::vector<Double_t> fMI_Time;
	std::vector<TVector3> fMI_Position;
	std::vector<TVector3> fMI_Momentum;
	std::vector<Double_t> fMI_ED;
	// VM 3/11/2013 added
	std::vector<Double_t> fMI_EDtot;
	

	//**********************************************************************************************
	//Start counter calibration parameters ///////////////////////////////////////////////////////////////
	constexpr static Double_t kSC_AmpGain = 1.E+6;
	constexpr static Double_t kSC_AmpOffset = 0.0;

	constexpr static Double_t kSC_TimeGain = 1.E+9;//convert s->ns
	constexpr static Double_t kSC_TimeOffset = 0.0;

	//conversion analog to channel
	constexpr static Double_t kSC_AmpAnaDig = 1.0;
	constexpr static Double_t kSC_TimeAnaDig = 1.E+3;//1.E+12; //1 digit = 1 ps

	//minimum amplitude threshold
	constexpr static Int_t kSC_AmpThresMin = 5;
	Double_t kSC_AmpThresMinRaw;//analog threshold calculated accorading to kSC_AmpThresMin

	//Noise and Resolution
	constexpr static Double_t kSC_MeanAmp = 0., kSC_SigmaAmp = 0.001;//UNIT:XXX
	constexpr static Double_t kSC_MeanTime = 0., kSC_SigmaTime = 1.E-6;//UNIT:s?

	//MIMOSA26 calibration parameters/////////////////////////////////////////////////////////////////////
	//old stuff
	//  constexpr static Double_t kMI_EDThreshold= 0.01E-3;//GeV 
	//  constexpr static Double_t kMI_EDThreshold= 0.01E-4;//GeV 
	//  constexpr static Double_t kMI_EDErr= 0.20;//fraction ; fluctuation of ED by diffusion
	//  constexpr static Double_t kMI_EDDiffusion=0.0018*1.5*5./7.;//cm (=sigma when referring to Gaussian)

	//from fit
	constexpr static Double_t kMI_EDDiffusion = (36.23) * 1E-4;//cm (=sigma when referring to Gaussian)
	constexpr static Double_t kMI_EDThreshold = (0.0150) * 2. * 3.142 * (36.23)
			* (36.23) * 1E-6; //0.000123727

	constexpr static Double_t kMI_EDErr = 0.1;//fraction ; fluctuation of initial ED 
	constexpr static Double_t kMI_EDErrSngl = 0.1;//fraction ; fluctuation of ED by diffusion
	// VM 3/11/13 parameterisation of cluster size vs eloss
	// cl_size = kMI_clsize_A0*pow(eloss(MeV)*kMI_clsize_B0);
	constexpr static Double_t kMI_clsize_A0 = 26.5;
	constexpr static Double_t kMI_clsize_B0 = 0.4266;
	


	//**********************************************************************************************


public:

	// get methods Start Counter //////////////////////////////////////////////////////////////////////////////
	Double_t SC_GetTime() const {
		return fSC_Time;
	}
	;

	Double_t SC_GetAmplitude() const {
		return fSC_Amp;
	}
	;

	Double_t SC_GetED() const {
		return fSC_ED;
	}
	;

	Int_t SC_GetTimeDig() const {
		return fSC_TimeDig;
	}
	;

	Int_t SC_GetAmplitudeDig() const {
		return fSC_AmpDig;
	}
	;

	// get methods MIMOSA26 ///////////////////////////////////////////////////////////////////////////////////////

	Int_t MI_GetHitPads() const {
		return fMI_Nb;
	}
	;

	TVector3 MI_GetMomentum(Int_t Number) {
	  return fMI_Momentum.at(Number);
	}
	;

	TVector3 MI_GetPosition(Int_t Number) {
	  return fMI_Position.at(Number);
	}
	;

	Int_t MI_GetChip(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetChip(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_Chip[Number];
	}
	;

	Int_t MI_GetColumn(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetColumn(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return 0.01-fMI_Column[Number];
		//		return MI_SENS_BIN_WIDTH-fMI_Column[Number];
	}
	;

	Int_t MI_GetRow(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetRow(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_Row[Number];
	}
	;

	Int_t MI_GetSenseId(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetSenseId(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_ID[Number];
	}
	;

	Double_t MI_GetTime(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetTime(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_Time[Number];
	}
	;

	Double_t MI_GetED(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetED(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_ED[Number];
	}
	;
	
	// VM added 3/11/13
	Double_t MI_GetEDtot(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetED(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_EDtot[Number];
	}
	;

	Bool_t MI_GetActive(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetActive(): given number:" << Number
					<< " is < 0 or >= " << fMI_Nb << " !" << endl;
			exit(1);
		}

		return fMI_ED[Number] > kMI_EDThreshold ? 1 : 0;
	}
	;

protected:

	template<class C> void FreeClear(C & cntr) {
	  for (typename C::iterator it = cntr.begin(); it != cntr.end(); ++it) {
	    delete *it;
	  }
	  cntr.clear();
	}

	ClassDef(Segnale,1);

};

#endif

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

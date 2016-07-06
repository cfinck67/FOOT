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

#include "gsi_geo.h"
#include "Helper.h"
#include "Geometry.h"
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

// VM added (5/11/13) for MI cluster size
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

	//KENTROS methods
	Int_t KE_Init();
	Int_t KE_Reset();
	Int_t KE_Score(Int_t DetNb, Int_t RegionType, 
		       Double_t InpX, Double_t InpY,
		       Double_t InpZ, 
		       Double_t InX, Double_t InY,
		       Double_t InZ, Double_t OutX, Double_t OutY, 
		       Double_t OutZ,
		       Double_t ER, Double_t QER, Double_t Time);
	Int_t KE_Calibration();
	Int_t KE_Noise();
	Int_t KE_Digitisation();


	//TOF-Wall methods
	Int_t TW_Init();
	Int_t TW_Reset();
	Int_t TW_Score(Int_t fWall, Int_t fSlatID, Double_t fCharge, Double_t fER,
			Double_t fQER, Double_t fInX, Double_t fInY, Double_t fInZ,
			Double_t fOutX, Double_t fOutY, Double_t fOutZ, Double_t fTime);
	Int_t TW_Calibration();
	Int_t TW_Noise();
	Int_t TW_Digitisation();

	//ONION methods
	Int_t ON_Init();
	Int_t ON_Reset();
	Int_t ON_Score(Int_t DetNb, Double_t InX, Double_t InY, Double_t InZ,
			Double_t OutX, Double_t OutY, Double_t OutZ, Double_t ER,
			Double_t QER, Double_t Time);
	Int_t ON_Calibration();
	Int_t ON_Noise();
	Int_t ON_Digitisation();

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
	
	//KENTROS signal
	std::vector<Int_t> fKE_RegV;
	std::vector<Double_t> fKE_EDV;
	std::vector<Double_t> fKE_AmpV;
	std::vector<Double_t> fKE_TimeV;

	std::vector<TVector3> fKE_Momentum;
	std::vector<TVector3> fKE_Pos;

	std::vector<Int_t> fKE_RegDigV;
	std::vector<Int_t> fKE_AmpDigV;
	std::vector<Int_t> fKE_TimeDigV;

	//TOF-Wall signal
	//std::vector<Double_t> fTW_VAmpF[2][TW_NbSlatFront];
	std::vector<std::vector<std::vector<Double_t> > > fTW_VAmpF;
	std::vector<std::vector<std::vector<Double_t> > > fTW_VTimeF;
	std::vector<std::vector<std::vector<Double_t> > > fTW_VAmpR;
	std::vector<std::vector<std::vector<Double_t> > > fTW_VTimeR;

	Double_t fTW_AmpF[2][TW_NbSlatFront];
	Double_t fTW_TimeF[2][TW_NbSlatFront];
	Double_t fTW_AmpR[2][TW_NbSlatBack];
	Double_t fTW_TimeR[2][TW_NbSlatBack];

	Double_t fTW_EDF[TW_NbSlatFront];//save the QER in the slats
	Double_t fTW_EDR[TW_NbSlatFront];

	Int_t fTW_AmpDigF[2][TW_NbSlatFront];
	Int_t fTW_TimeDigF[2][TW_NbSlatFront];
	Int_t fTW_AmpDigR[2][TW_NbSlatBack];
	Int_t fTW_TimeDigR[2][TW_NbSlatBack];

	//ONION signal
	std::vector<Int_t> fON_RegV;
	std::vector<Double_t> fON_EDV;
	std::vector<Double_t> fON_AmpV;
	std::vector<Double_t> fON_TimeV;

	std::vector<Int_t> fON_RegDigV;
	std::vector<Int_t> fON_AmpDigV;
	std::vector<Int_t> fON_TimeDigV;

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
	//  const static Double_t kMI_EDThreshold= 0.01E-3;//GeV 
	//  const static Double_t kMI_EDThreshold= 0.01E-4;//GeV 
	//  const static Double_t kMI_EDErr= 0.20;//fraction ; fluctuation of ED by diffusion
	//  const static Double_t kMI_EDDiffusion=0.0018*1.5*5./7.;//cm (=sigma when referring to Gaussian)

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
	

	//KENTROS calibration parameters//////////////////////////////////////////////////////////////////  

	constexpr static Double_t kKE_AmpGain = 1.E+9;
	constexpr static Double_t kKE_AmpOffset = 0.0;

	constexpr static Double_t kKE_TimeGain = 1.E+9;//convert s->ns
	constexpr static Double_t kKE_TimeOffset = 0.0;

	//conversion analog to channel
	constexpr static Double_t kKE_AmpAnaDig = 1.0;
	constexpr static Double_t kKE_TimeAnaDig = 1.E+3;// 1.E+12; //1 digit = 1 ps

	//minimum amplitude threshold
	constexpr static Int_t kKE_AmpThresMin = 2;

	//Noise and Resolution
	constexpr static Double_t kKE_MeanAmp = 0., kKE_SigmaAmp = 0.000;//UNIT:XXX
	constexpr static Double_t kKE_MeanTime = 0., kKE_SigmaTime = 0.0;// 1.E-6;//UNIT:s?


	//TP-MUSIC IV calibration parameters//////////////////////////////////////////////////////////////////  
	TVector3 fMU_Pla_P, fMU_Pla_V;

	constexpr static Double_t kMU_AmpGain = 1.E+9;
	constexpr static Double_t kMU_AmpOffset = 0.0;

	constexpr static Double_t kMU_TimeGain = 1.E+9;//convert s->ns
	constexpr static Double_t kMU_TimeOffset = 0.0;

	constexpr static Double_t kMU_HeiGain = 10.;//convert cm->mm

	//conversion analog to channel
	constexpr static Double_t kMU_AmpAnaDig = 1.0;
	constexpr static Double_t kMU_HeiAnaDig = 10.0;//1 digit = 0.1 mm
	constexpr static Double_t kMU_TimeAnaDig = 1.E+3;// 1.E+12; //1 digit = 1 ps

	//minimum amplitude threshold
	constexpr static Int_t kMU_AmpThresMin = 2;

	Double_t kMU_v_drift;//UNITS:cm/s 

	//P10 gas mixtures, 900mbar, 15 kV, a drift velocity of about 5.5 cm/microsec seems to me reasonable. 
	// the drift velocity as a function of E/P and CH4% in the appendix B of De Napoli's thesis.

	//Noise and Resolution
	constexpr static Double_t kMU_MeanAmp = 0., kMU_SigmaAmp = 0.000;//UNIT:XXX
	constexpr static Double_t kMU_MeanHei = 0., kMU_SigmaHei = 1.;//UNIT:mm
	constexpr static Double_t kMU_MeanTime = 0., kMU_SigmaTime = 0.0;// 1.E-6;//UNIT:s?

	//TOF-Wall calibration parameters/////////////////////////////////////////////////////////////////////
	// MV 16/5/13 (from gsi_geo.h)
	Double_t kTW_InvSigVel;

	//  const static Double_t kTW_InvSigVel=1./16.8;//UNIT:ns/cm //inverse signal velocity (1/(16.8cm/ns)) BC408 TODO:check FLUKA units

	//const static Double_t kTW_Lambda = 0.0145;//UNIT:1/cm //attenuation length (0.00107 1/mm) BC408 (30/7/12 changed according to measurements)

	// VM 16/5/13 changed to mean value from data now from gsi_geo.h
	Double_t kTW_Lambda;//UNIT:1/cm //attenuation length (0.00107 1/mm) BC408 (9/5/13 mean value from calibration parameters)	

	// VM 9/1/2014 slat dependence of attenuation coefficient 
	Double_t kTW_Lambda_Slat[201];

	Double_t kTW_Ene_peak; // added 9/5/2013 - calibration peak on data
	                       // (from gsi_geo.h)
	Double_t kTW_Birk_const; // VM added 16/5/13

	//Slat noise
	constexpr static Double_t kTW_MeanAmp = 0.;
	//const static Double_t kTW_SigmaAmp = 0.045;//UNIT:XXX
	// VM 8/4/2013 - kTW_SigmaAmp not used anymore
	// kTW_SigmaTime changed according to studies on data (MV 8/4/2013)
	constexpr static Double_t kTW_MeanTime = 0., kTW_SigmaTime = 0.45;//UNIT:ns

	// VM 8/4/2013 - ADC energy resolution parameterized as
	// sqrt(ADC)/ADC = sqrt(kTW_SigmaAmp_1/ADC+kTW_SigmaAmp_2)
	// based on studies on data
	// VM 6/5/2013 - resolution tuned with eloss (36.09->13.71)
	constexpr static Double_t kTW_SigmaAmp_1 = 13.71;
	constexpr static Double_t kTW_SigmaAmp_2 = 0.;

	// VM 8/4/2013 - common noise in ns added to both TDC value
	// appears in TOF measurement, not in Y
	constexpr static Double_t kTW_Tof_Reso = 0.47;

	constexpr static Double_t kTW_SlatCrossTalk = 0.00;//UNIT:1

	//following values are set in the constructor
	//TOF-Wall calibration factors (for individual slat light yield and gain of photomultiplier)
	Double_t kTW_AmpGainF[2][TW_NbSlatFront];
	Double_t kTW_AmpGainR[2][TW_NbSlatBack];

	Double_t kTW_AmpOffsetF[2][TW_NbSlatFront];
	Double_t kTW_AmpOffsetR[2][TW_NbSlatBack];

	//conversion analog to channel
	Double_t kTW_AmpAnaDigF[2][TW_NbSlatFront];
	Double_t kTW_AmpAnaDigR[2][TW_NbSlatBack];

	//calibration factors
	Double_t kTW_TimGainF[2][TW_NbSlatFront];
	Double_t kTW_TimGainR[2][TW_NbSlatBack];

	Double_t kTW_TimOffsetF[2][TW_NbSlatFront];
	Double_t kTW_TimOffsetR[2][TW_NbSlatBack];

	//conversion analog to channel
	Double_t kTW_TimAnaDigF[2][TW_NbSlatFront];
	Double_t kTW_TimAnaDigR[2][TW_NbSlatBack];

	//minimum amplitude threshold
	Int_t kTW_AmpThresMinF[2][TW_NbSlatFront];
	Int_t kTW_AmpThresMinR[2][TW_NbSlatBack];

	// if !0 apply energy threshold on single slats 
	// (from file /INPUT/TW_TDCthresholds_MeV.dat)
	// MV 8/4/2013
	static constexpr Short_t kTW_apply_tdc_thr = 1; 
	// common energy threshold in MeV to have a TDC hit
	// (applied only if TW_apply_tdc_thr = 0)
	// MV 8/4/2013
	// MV 12/1/2014 reduced to 0.1 MeV
	// the rear threshold is applied in TATactNtuMC
	static constexpr Double_t kTW_CommEneThr_MeV = 0.1;

	Double_t kTW_AmpThresMinRawF[2][TW_NbSlatFront];
	Double_t kTW_AmpThresMinRawR[2][TW_NbSlatBack];

	//ONION calibration parameters//////////////////////////////////////////////////////////////////  
	constexpr static Double_t kON_AmpGain = 1.E+9;
	constexpr static Double_t kON_AmpOffset = 0.0;

	constexpr static Double_t kON_TimeGain = 1.E+9;//convert s->ns
	constexpr static Double_t kON_TimeOffset = 0.0;

	//conversion analog to channel
	constexpr static Double_t kON_AmpAnaDig = 1.0;
	constexpr static Double_t kON_TimeAnaDig = 1.E+3;// 1.E+12; //1 digit = 1 ps

	//minimum amplitude threshold
	constexpr static Int_t kON_AmpThresMin = 2;

	//Noise and Resolution
	constexpr static Double_t kON_MeanAmp = 0., kON_SigmaAmp = 0.000;//UNIT:XXX
	constexpr static Double_t kON_MeanTime = 0., kON_SigmaTime = 0.0;// 1.E-6;//UNIT:s?


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

		return MI_SENS_BIN_WIDTH-fMI_Column[Number];
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

	Int_t MI_GetID(Int_t Number) const {
		if ((Number < 0) || (Number >= fMI_Nb)) {
			cerr << "ERROR: Segnale::MI_GetID(): given number:" << Number
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

	// get methods KENTROS ///////////////////////////////////////////////////////////////////////////////////
	Int_t KE_GetEntries() const {
		return fKE_RegV.size();
	}
	;

	Int_t KE_GetReg(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_RegV.size())) {
			cerr << "ERROR: Segnale::KE_GetReg(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_RegV.size() << " !" << endl;
			exit(1);
		}

		return fKE_RegV[HitNumber];
	}
	;

	Double_t KE_GetED(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_EDV.size())) {
			cerr << "ERROR: Segnale::KE_GetED(): given HitNumber:" << HitNumber
					<< " is < 0 or >=(signed int) " << fKE_EDV.size() << " !"
					<< endl;
			exit(1);
		}

		return fKE_EDV[HitNumber];
	}
	;

	Double_t KE_GetAmp(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_AmpV.size())) {
			cerr << "ERROR: Segnale::KE_GetAmp(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_AmpV.size() << " !" << endl;
			exit(1);
		}

		return fKE_AmpV[HitNumber];
	}
	;

	Double_t KE_GetTime(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_TimeV.size())) {
			cerr << "ERROR: Segnale::KE_GetTime(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_TimeV.size() << " !" << endl;
			exit(1);
		}

		return fKE_TimeV[HitNumber];
	}
	;

	TVector3 KE_GetPosition(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_TimeV.size())) {
			cerr << "ERROR: Segnale::KE_GetTime(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_TimeV.size() << " !" << endl;
			exit(1);
		}

		return fKE_Pos[HitNumber];
	}
	;

	TVector3 KE_GetMomentum(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_TimeV.size())) {
			cerr << "ERROR: Segnale::KE_GetMomentum(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_TimeV.size() << " !" << endl;
			exit(1);
		}

		return fKE_Momentum[HitNumber];
	}
	;


	//dig values
	Int_t KE_GetEntriesDig() const {
		return fKE_RegDigV.size();
	}
	;

	Int_t KE_GetRegDig(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_RegDigV.size())) {
			cerr << "ERROR: Segnale::KE_GetRegDig(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_RegDigV.size() << " !" << endl;
			exit(1);
		}

		return fKE_RegDigV[HitNumber];
	}
	;

	Int_t KE_GetAmpDig(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_AmpDigV.size())) {
			cerr << "ERROR: Segnale::KE_GetAmpDig(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_AmpDigV.size() << " !" << endl;
			exit(1);
		}

		return fKE_AmpDigV[HitNumber];
	}
	;

	Int_t KE_GetTimeDig(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fKE_TimeDigV.size())) {
			cerr << "ERROR: Segnale::KE_GetTimeDig(): given HitNumber:"
					<< HitNumber << " is < 0 or >=(signed int) "
					<< fKE_TimeDigV.size() << " !" << endl;
			exit(1);
		}

		return fKE_TimeDigV[HitNumber];
	}
	;


	// get methods TOF-Wall //////////////////////////////////////////////////////////////////////////////////////
	//fWall==0:front, fWall!=0:rear 
	//fTop==0:top,    fTop==1:bottom
	Double_t TW_GetTime(Int_t fWall, Int_t fTop, Int_t fSlatID) const {
		if ((fSlatID < 0) || (fSlatID >= (fWall == 0 ? TW_NbSlatFront
				: TW_NbSlatBack))) {
			cerr << "ERROR: Segnale::GetTime(): given Slat ID:" << fSlatID
					<< " is < 0 or >= " << (fWall == 0 ? TW_NbSlatFront
					: TW_NbSlatBack) << " !" << endl;
			exit(1);
		}

		return fWall == 0 ? fTW_TimeF[fTop][fSlatID] : fTW_TimeR[fTop][fSlatID];
	}
	;

	Double_t TW_GetAmplitude(Int_t fWall, Int_t fTop, Int_t fSlatID) const {
		if ((fSlatID < 0) || (fSlatID >= (fWall == 0 ? TW_NbSlatFront
				: TW_NbSlatBack))) {
			cerr << "ERROR: Segnale::GetAmplitude(): given Slat ID:" << fSlatID
					<< " is < 0 or >= " << (fWall == 0 ? TW_NbSlatFront
					: TW_NbSlatBack) << " !" << endl;
			exit(1);
		}

		return fWall == 0 ? fTW_AmpF[fTop][fSlatID] : fTW_AmpR[fTop][fSlatID];
	}
	;

	Double_t TW_GetED(Int_t fWall, Int_t fSlatID) const {
		if ((fSlatID < 0) || (fSlatID >= (fWall == 0 ? TW_NbSlatFront
				: TW_NbSlatBack))) {
			cerr << "ERROR: Segnale::GetED(): given Slat ID:" << fSlatID
					<< " is < 0 or >= " << (fWall == 0 ? TW_NbSlatFront
					: TW_NbSlatBack) << " !" << endl;
			exit(1);
		}

		return fWall == 0 ? fTW_EDF[fSlatID] : fTW_EDR[fSlatID];
	}
	;

	Int_t TW_GetTimeDig(Int_t fWall, Int_t fTop, Int_t fSlatID) const {
		if ((fSlatID < 0) || (fSlatID >= (fWall == 0 ? TW_NbSlatFront
				: TW_NbSlatBack))) {
			cerr << "ERROR: Segnale::GetTimeDig: given Slat ID:" << fSlatID
					<< " is < 0 or >= " << (fWall == 0 ? TW_NbSlatFront
					: TW_NbSlatBack) << " !" << endl;
			exit(1);
		}

		return fWall == 0 ? fTW_TimeDigF[fTop][fSlatID]
				: fTW_TimeDigR[fTop][fSlatID];
	}
	;

	Int_t TW_GetAmplitudeDig(Int_t fWall, Int_t fTop, Int_t fSlatID) const {
		if ((fSlatID < 0) || (fSlatID >= (fWall == 0 ? TW_NbSlatFront
				: TW_NbSlatBack))) {
			cerr << "ERROR: Segnale::GetAmplitudeDig(): given Slat ID:"
					<< fSlatID << " is < 0 or >= "
					<< (fWall == 0 ? TW_NbSlatFront : TW_NbSlatBack) << " !"
					<< endl;
			exit(1);
		}

		return fWall == 0 ? fTW_AmpDigF[fTop][fSlatID]
				: fTW_AmpDigR[fTop][fSlatID];
	}
	;

	// get methods ONION ///////////////////////////////////////////////////////////////////////////////////
	Int_t ON_GetEntries() const {
		return fON_RegV.size();
	}
	;

	Int_t ON_GetReg(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_RegV.size())) {
			cerr << "ERROR: Segnale::ON_GetReg(): given HitNumber:"
					<< HitNumber << " is < 0 or >= " << fON_RegV.size() << " !"
					<< endl;
			exit(1);
		}

		return fON_RegV[HitNumber];
	}
	;

	Double_t ON_GetED(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_EDV.size())) {
			cerr << "ERROR: Segnale::ON_GetED(): given HitNumber:" << HitNumber
					<< " is < 0 or >= " << fON_EDV.size() << " !" << endl;
			exit(1);
		}

		return fON_EDV[HitNumber];
	}
	;

	Double_t ON_GetAmp(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_AmpV.size())) {
			cerr << "ERROR: Segnale::ON_GetAmp(): given HitNumber:"
					<< HitNumber << " is < 0 or >= " << fON_AmpV.size() << " !"
					<< endl;
			exit(1);
		}

		return fON_AmpV[HitNumber];
	}
	;

	Double_t ON_GetTime(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_TimeV.size())) {
			cerr << "ERROR: Segnale::ON_GetTime(): given HitNumber:"
					<< HitNumber << " is < 0 or >= " << fON_TimeV.size()
					<< " !" << endl;
			exit(1);
		}

		return fON_TimeV[HitNumber];
	}
	;

	//dig values
	Int_t ON_GetEntriesDig() const {
		return fON_RegDigV.size();
	}
	;

	Int_t ON_GetRegDig(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_RegDigV.size())) {
			cerr << "ERROR: Segnale::ON_GetRegDig(): given HitNumber:"
					<< HitNumber << " is < 0 or >= " << fON_RegDigV.size()
					<< " !" << endl;
			exit(1);
		}

		return fON_RegDigV[HitNumber];
	}
	;

	Int_t ON_GetAmpDig(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_AmpDigV.size())) {
			cerr << "ERROR: Segnale::ON_GetAmpDig(): given HitNumber:"
					<< HitNumber << " is < 0 or >= " << fON_AmpDigV.size()
					<< " !" << endl;
			exit(1);
		}

		return fON_AmpDigV[HitNumber];
	}
	;

	Int_t ON_GetTimeDig(Int_t HitNumber) const {
		if ((HitNumber < 0) || (HitNumber >= (signed int) fON_TimeDigV.size())) {
			cerr << "ERROR: Segnale::ON_GetTimeDig(): given HitNumber:"
					<< HitNumber << " is < 0 or >= " << fON_TimeDigV.size()
					<< " !" << endl;
			exit(1);
		}

		return fON_TimeDigV[HitNumber];
	}
	;

protected:

	// VM 5/11/13
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

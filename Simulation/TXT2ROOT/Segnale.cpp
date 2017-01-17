#include "Segnale.h"
#include <cmath>


#include <iostream>
#include <fstream>
#include "TSystem.h"
#include <vector>

using namespace std;

ClassImp( Segnale);


/*-----------------------------------------------------------------*/
Segnale::Segnale(Geometry *TheGeometry) :
  Geo(TheGeometry) {
  //init Start Counter values
  Segnale::SC_Init();
  
  //init MIMOSA-26 values
  Segnale::MI_Init();
  
}

/*-----------------------------------------------------------------*/
Segnale::~Segnale() {

}

/*-----------------------------------------------------------------*/
Int_t Segnale::BOEvent() {//beginning of event


	SC_Reset();
	MI_Reset();

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::EOEvent() {//end of event

	//apply calibration factors
	SC_Calibration();//to be called first!

	//calculate digitized values (respecting thresholds)
	SC_Digitisation();

	return 0;
}

/*-----------------------------------------------------------------*/
//Start counter
/*-----------------------------------------------------------------*/
Int_t Segnale::SC_Init() {

	kSC_AmpThresMinRaw = (((float) kSC_AmpThresMin) / kSC_AmpAnaDig
			- kSC_AmpOffset) / kSC_AmpGain;

	Segnale::SC_Reset();

	return 0;
}

Int_t Segnale::SC_Reset() {
	//printf("Entro in Segnale::SC_Reset()\n");

	fSC_Amp = 0.;
	fSC_Time = 0.;
	fSC_ED = 0.;

	fSC_AmpDig = 0;
	fSC_TimeDig = 0;
	fSC_AmpV.clear();
	fSC_TimeV.clear();

	//printf("Esco da Segnale::SC_Reset()\n");
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::SC_Score(Double_t Charge, Double_t ER, Double_t QER,
		Double_t InX, Double_t InY, Double_t InZ, Double_t OutX, Double_t OutY,
		Double_t OutZ, Double_t Time) {

	Double_t CurAmp;
	Double_t CurTime;

	//calculating light intensity (no attenuation)
	CurAmp = QER;
	//time (counted from point primary started)  //possibility to add random number here accounting for resolution (Gaussian)
	CurTime = Time;

	fSC_AmpV.push_back(CurAmp);
	fSC_TimeV.push_back(CurTime);
	fSC_ED += QER;

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::SC_Calibration() {
	Bool_t LogThres = 0;

	//sort signals by arrival time
	Segnale::SortVec(&fSC_TimeV, &fSC_AmpV);

	//add up amplitudes and determine when amplitude threshold is reached and set time to this value  
	//   cout<<"DEBUG:"<<"Time Amp"<<endl;
	for (unsigned int i1 = 0; i1 < fSC_TimeV.size(); i1++) {
		//  cout<<"DEBUG:"<<fSC_TimeV[i1]<<" "<<fSC_AmpV[i1]<<endl;
		fSC_Amp += fSC_AmpV[i1];
		if ((!LogThres) && (fSC_Amp > kSC_AmpThresMinRaw)) {
			LogThres = 1;
			fSC_Time = fSC_TimeV[i1];
		}
	}

	//apply calibration factors only once per event
	fSC_Amp *= kSC_AmpGain;
	fSC_Amp += kSC_AmpOffset;

	fSC_Time *= kSC_TimeGain;
	fSC_Time += kSC_TimeOffset;

	//if amplitude threshold is not reached set time to -1.
	if (!LogThres) {
		fSC_Time = -1.;
	}

	//BIASED MC :: Temporary hack to allow Efficiency studies.
	//	cout<<"DEBUG:: "<<fSC_Time<<endl;
	//	if(fSC_Time == -1) {
	//	  fSC_Time = 0.467541;
	//	}
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::SC_Noise() {
	//add noise only once per event
	//add noise dependent on the signal height?
	//TODO: what is better noise for every energy deposit or accumulated noise here in the end (might give some strange behaviour together with thresholds)  

	fSC_Amp += rnd.Gaus(kSC_MeanAmp, kSC_SigmaAmp);
	fSC_Time += rnd.Gaus(kSC_MeanTime, kSC_SigmaTime);

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::SC_Digitisation() {
	//digitisation with amplitude thresholds only once per event
	Int_t fDummy, fDummy2;

	fDummy = Geo->castd2i(kSC_AmpAnaDig * fSC_Amp);//depending on logic +0.5 here? (and then also for kSC_AmpThresMinRaw)
	fSC_AmpDig = (fDummy > kSC_AmpThresMin) ? fDummy : 0;

	fDummy2 = Geo->castd2i(kSC_TimeAnaDig * fSC_Time);//depending on logic +0.5 here?
	fSC_TimeDig = (fDummy > kSC_AmpThresMin) ? fDummy2 : -1;//if amplitude smaller than threshold set time to -1

	// cout<<"DEBUG:"<<fSC_Amp<<" "<<fSC_AmpDig<<" "<<fSC_Time<<" "<<fSC_TimeDig<<endl;

	return 0;
}

/*-----------------------------------------------------------------*/
// MIMOSA
/*-----------------------------------------------------------------*/
Int_t Segnale::MI_Init() {

	Segnale::MI_Reset();
	return 0;
}

Int_t Segnale::MI_Reset() {

	fMI_Nb = 0;
	fMI_Chip.clear();
	fMI_Column.clear();
	fMI_Row.clear();
	fMI_ID.clear();
	fMI_Time.clear();
	fMI_Position.clear();
	fMI_Momentum.clear();
	fMI_ED.clear();
	fMI_EDtot.clear(); // VM added 3/11/2013

	return 0;
}

/*-----------------------------------------------------------------*/
// VM 30/1/2014 charge added
  Int_t Segnale::MI_Score(Int_t Chip, Int_t Column, Int_t Row, Int_t ID, 
			  Int_t Chg, 
			  Double_t ED, Double_t X, Double_t Y, Double_t Z,
			  Double_t pX, Double_t pY, Double_t pZ,Double_t Time) {
  //  cout<<"Sono in Segnale::MI_Score()"<<endl;

  Int_t CountActive = 0;//counter for number of activated pixel in this method call
  Int_t XMin, XMax;
  Int_t YMin, YMax;
  Int_t dBinsX, dBinsY;
  Bool_t FlagPad;
  Double_t curED;
  double dist_x;
  double dist_y;
  //  TVector3 ChipPos = Geo->MI_Coord(Column, Row, Chip - 1);
  TVector3 position(X,Y,Z);
  TVector3 momentum(pX,pY,pZ);
  //  Double_t BinFracX = (X - ChipPos.X()) / MI_PAD_WIDTH;
  //  Double_t BinFracY = (Y - ChipPos.Y()) / MI_PAD_HEIGHT;
  Int_t exp_ncl;

  //actually the following numbers should be calculated (like this it introduces only right cluster size fluctuation but also an additional position uncertainty)
  //  Double_t BinFracX=rnd.Uniform(-.5,+.5);
  //  Double_t BinFracY=rnd.Uniform(-.5,+.5);
  //  cout<<"DEBUG: MI coord: Chip X:"<<ChipPos.X()<<" Y:"<<ChipPos.Y()<<" Z:"<<ChipPos.Z()<<"     Binfrac:"<<BinFracX<<" "<<BinFracY<<endl;
  //  cout<<"DEBUG: MI coord: Part X:"<<X<<" Y:"<<Y<<" Z:"<<Z<<" RealBinfrac:"<<(X-ChipPos.X())/MI_PAD_WIDTH<<" "<<(Y-ChipPos.Y())/MI_PAD_HEIGHT<<endl;


  //the following assures that pixel directly traversed by any particle are alwayed active (=100% efficiency)
  //TODO: change this to condition: traversed by an ion???

  //if pad already in the list (=same chip,column,row) add additional energy deposited
  FlagPad = 0;
  for (unsigned int iL = 0; iL < fMI_Column.size(); iL++) {
    //cout<<"DEBUG: Add energy Col Row Store "<<iC<<" "<<iR<<" "<<iL<<" "<<fMI_Column.size()<<endl;
    if (activatePad(fMI_Chip[iL],Chip,fMI_Column[iL],Column,fMI_Row[iL],Row)) {
      FlagPad = 1;
      fMI_ED[iL] += 2. * kMI_EDThreshold;
      break;
    }
  }
  //if new pad
  if (!FlagPad) {
    fMI_Nb++;
    fMI_Chip.push_back(Chip);
    fMI_Column.push_back(Column);
    fMI_Row.push_back(Row);
    fMI_ID.push_back(ID);
    fMI_Time.push_back(Time);
    fMI_Position.push_back(position);
    fMI_Momentum.push_back(momentum);
    fMI_ED.push_back(2. * kMI_EDThreshold);
    fMI_EDtot.push_back(ED); // VM added 3/11/2013
    if (fMI_ED.back() > kMI_EDThreshold)
      CountActive++;
    // 	cout<<"DEBUG: New pad: "<<fMI_Nb<<" Chip Col Row "<<fMI_Chip.back()<<" "<<fMI_Column.back()<<" "<<fMI_Row.back()<<" "<<fMI_ID.back()<<" "<<fMI_Time.back()<<" EDhit:"<<ED<<" EDpad:"<<fMI_ED.back()<<" EDThres:"<<kMI_EDThreshold<<" ActPixsofar:"<<CountActive<<endl;
  }

  //   cout<<"DEBUG: ED "<<ED<<" Threshold_min "<<kMI_EDThreshold/2.*(1.-4.*kMI_EDErr) <<endl;
  //   cout<<"DEBUG: log( "<<ED*2./kMI_EDThreshold/std::abs(1.-4.*kMI_EDErr)<<") sqrt("<<2.0*kMI_EDDiffusion*std::log(ED*2./kMI_EDThreshold/std::abs(1.-4.*kMI_EDErr))<<")"<<endl;

  // VM 3/11/2013 commented
  //ED *= rnd.Gaus(1.0, kMI_EDErr); //ED fluctuation per event

  //  if (ED<=kMI_EDThreshold/3.) return 0; //if a very small energy deposition -> skip it
  // VM 3/11/2013 changed

  //if (ED <= kMI_EDThreshold / 2. * std::abs(1. - 2. * kMI_EDErrSngl))
  //  return 0; //if a very small energy deposition -> skip it
  //  if (ED<=kMI_EDThreshold/2.*std::abs(1.-2.*kMI_EDErr)){
  if (ED <= kMI_EDThreshold / 2000.) 
    return 0;

  //    XMin= max(Column-1,0);
  //    XMax= min(Column+1,MI_SENS_BIN_WIDTH-1);
  //    YMin= max(Row-1,0);
  //    YMax= min(Row+1,MI_SENS_BIN_HEIGHT-1);
  //  }else{

  //  dBinsX=int( kMI_EDDiffusion*std::sqrt(2.0*std::log(ED*2./kMI_EDThreshold/std::abs(1.-2.*kMI_EDErr)))/MI_PAD_WIDTH +0.5 );
  //  dBinsY=int( kMI_EDDiffusion*std::sqrt(2.0*std::log(ED*2./kMI_EDThreshold/std::abs(1.-2.*kMI_EDErr)))/MI_PAD_HEIGHT+0.5 );

  // VM 3/11/2013 changed
  //dBinsX = 50;
  //dBinsY = 50;
  const Int_t cl_w = 6;
  dBinsX = cl_w;
  dBinsY = cl_w;
  XMin = max(Column - dBinsX, 0);
  XMax = min(Column + dBinsX, MI_SENS_BIN_WIDTH - 1);

  YMin = max(Row - dBinsY, 0);
  YMax = min(Row + dBinsY, MI_SENS_BIN_HEIGHT - 1);
  //  }

  //TODO: quit this
  //  if(ED<=kMI_EDThreshold){
  //    XMin= max(Column-1,0);
  //    XMax= min(Column+1,MI_SENS_BIN_WIDTH-1);
  //    YMin= max(Row-1,0);
  //    YMax= min(Row+1,MI_SENS_BIN_HEIGHT-1);    
  //  }


  //    cout<<"DEBUG: XMin XMax "<<XMin<<" "<<XMax<<" YMin YMax "<<YMin<<" "<<YMax<<" dBinsX dBinsY "<<dBinsX<<" "<<dBinsY<<" Col Row "<<Column<<" "<<Row<<" ED "<<ED <<endl;

  // VM 30/1/2014 parameterization of cluster size from fits with a concolution
  // of Landau and gaussian function for each charge 

  Int_t chg=Chg;

  const Double_t width_para[6] = {0.111,0.108,0.124,0.136,0.1846,0.3575};
  const Double_t mpv_para[6] = {3.186,5.54,8.304,10.66,12.58,13.84};
  const Double_t gsigma[6] = {0.4507,0.5731,0.6076,0.6861,0.6852,0.6692};

  Double_t ed_mev=ED*1000.;
  Double_t exp_mpv;
  Double_t f_exp_ncl(0);
  if (chg>0 && chg<7) {
    f_exp_ncl = rnd.Landau(mpv_para[chg-1],width_para[chg-1]);
    f_exp_ncl += rnd.Gaus(0.,gsigma[chg-1]);
  } else {
    cout << "WARNING chg=" << chg << endl;
    f_exp_ncl=1;
  }
  if (f_exp_ncl>40) {
    f_exp_ncl = rnd.Landau(mpv_para[chg-1],width_para[chg-1]);
    f_exp_ncl += rnd.Gaus(0.,gsigma[chg-1]);
  }
  exp_ncl=((Int_t) (f_exp_ncl+0.5));
  if (exp_ncl<1) {
    exp_ncl=1;
  }
  if (exp_ncl>30) exp_ncl=30;

  //Double_t diff = std::sqrt(exp_ncl/3.1415); // pixels
  //Double_t diff_cm = diff*MI_PAD_WIDTH;
  Double_t dist;
  //Int_t ncl=0;
  Double_t Column_rand;
  Double_t Row_rand;

  // VM changed 5/11/13
  // the distance of each pixel from the central one is stored in a vector
  // the vector is sorted in increasing distances. A number of closest
  // pixels is activated according to the parameterisatin in function
  // of the energy loss

  vector <vec_sort_pix *> vec_pix;
  for (Int_t iC = XMin; iC <= XMax; iC++) {
    for (Int_t iR = YMin; iR <= YMax; iR++) {
      curED = ED;
      Column_rand = Column+rnd.Uniform(-0.5,0.5);
      Row_rand = Row+rnd.Uniform(-0.5,0.5);
      dist_x = ((double) (iC - Column_rand));
      dist_y = ((double) (iR - Row_rand));      
      dist=std::sqrt(dist_x*dist_x+dist_y*dist_y);
      //cout << "iC=" << iC << " iR=" << iR << " dist=" << dist << endl;
      vec_sort_pix * pix_info = new vec_sort_pix(iC,iR,dist);
      vec_pix.push_back(pix_info);
    }
  }
  std::sort(vec_pix.begin(), vec_pix.end(), less_diff());
  //cout << endl;
  //cout << "size=" << vec_pix.size() << endl;
  for (Int_t i=0;i<exp_ncl;i++) {
    //vec_sort_pix * pix_info = vec_pix[i]
    //  cout << "i=" << i << " dist=" << vec_pix[i]->dst() << endl;
    curED=2.*kMI_EDThreshold;
    FlagPad = 0;
    for (unsigned int iL = 0; iL < fMI_Column.size(); iL++) {
      if (activatePad(fMI_Chip[iL],Chip,fMI_Column[iL],vec_pix[i]->idx(),fMI_Row[iL],vec_pix[i]->idy())) {
	FlagPad = 1;
	fMI_ED[iL] += curED;
	break;
      }
    }
    //if new pad
    if (!FlagPad) {
      fMI_Nb++;
      fMI_Chip.push_back(Chip);
      fMI_Column.push_back(vec_pix[i]->idx());
      fMI_Row.push_back(vec_pix[i]->idy());
      fMI_ID.push_back(ID);
      fMI_Time.push_back(Time);
      fMI_Position.push_back(position);
      fMI_Momentum.push_back(momentum);
      fMI_ED.push_back(curED);
      fMI_EDtot.push_back(ED);
      if (fMI_ED.back() > kMI_EDThreshold)
	CountActive++;
    }      
  }
  if (vec_pix.size()) 
    FreeClear(vec_pix);

 return 0;
}

bool Segnale::activatePad(Int_t ch, Int_t c_ch, Int_t co, Int_t c_co, Int_t ro, Int_t c_ro) {
  bool act = kFALSE;
  if(ch == c_ch) {
    if((co == c_co)){
      if((ro == c_ro)) 
	act = kTRUE;
    }
  }
  return act;
}
/*-----------------------------------------------------------------*/
Int_t Segnale::MI_Noise() {
	//TODO: add content once there is some idea about the noise scale (and systematics?)

	//add randomly noise (loop from 0 to XX noisy pads with a probability)
	//add 1.1*kMI_EDThreshold; (if pad not hit still)
	return 0;
}

/*-----------------------------------------------------------------*/
// Int_t Segnale::MI_Calibration(){


//   return 0;
// }


/*-----------------------------------------------------------------*/
// sort 2 Double_t vectors after the first vector
/*-----------------------------------------------------------------*/
Int_t Segnale::SortVec(std::vector<Double_t> *fVec1,
		std::vector<Double_t> *fVec2) {

	if ((*fVec1).size() != (*fVec1).size()) {
		cout
				<< "WARNING: vectors could not be sorted as they have a different length!"
				<< endl;
		return -1;
	}
	std::vector < Double_t > fVec1unsort(*fVec1);
	std::vector < Double_t > fVec2unsort(*fVec2);

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

///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

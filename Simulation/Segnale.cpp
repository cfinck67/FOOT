#include "Segnale.h"
#include <cmath>


#include <iostream>
#include <fstream>
#include "TSystem.h"
#include <vector> // VM 5/11/13

using namespace std;

ClassImp( Segnale);


/*-----------------------------------------------------------------*/
Segnale::Segnale(Geometry *TheGeometry) :
  Geo(TheGeometry) {
  //printf("Entro in Segnale::Segnale()\n");
  //init Start Counter values

  Segnale::SC_Init();
  
  //init MIMOSA-26 values
  Segnale::MI_Init();
  
  //init KENTROS values and vectors
  Segnale::KE_Init();
  
  //init TOF-Wall values and vectors
  Segnale::TW_Init();
  
  //init ONION values and vectors
  Segnale::ON_Init();
  
  //printf("Esco da Segnale::Segnale()\n");
}

/*-----------------------------------------------------------------*/
Segnale::~Segnale() {

}

/*-----------------------------------------------------------------*/
Int_t Segnale::BOEvent() {//beginning of event


	SC_Reset();
	MI_Reset();
	KE_Reset();
	TW_Reset();
	ON_Reset();

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::EOEvent() {//end of event

	//apply calibration factors
	SC_Calibration();//to be called first!
	KE_Calibration();
	TW_Calibration();
	ON_Calibration();

	//add noise (adding calibration, offsets)
	//SC_Noise();
	//MI_Noise();//exists but does nothing
	//KE_Noise();
	//MU_Noise();
	TW_Noise();//add (mis-)calibration of individual slats 
	//ON_Noise();

	//calculate digitized values (respecting thresholds)
	SC_Digitisation();
	KE_Digitisation();
	TW_Digitisation();
	ON_Digitisation();

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
  TVector3 ChipPos = Geo->MI_Coord(Column, Row, Chip - 1);
  TVector3 position(X,Y,Z);
  TVector3 momentum(pX,pY,pZ);
  Double_t BinFracX = (X - ChipPos.X()) / MI_PAD_WIDTH;
  Double_t BinFracY = (Y - ChipPos.Y()) / MI_PAD_HEIGHT;
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
//KENTROS
/*-----------------------------------------------------------------*/
Int_t Segnale::KE_Init() {

	Segnale::KE_Reset();
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::KE_Reset() {
	//printf("Entro in Segnale::KE_Reset()\n");

	fKE_RegV.clear();
	fKE_AmpV.clear();
	fKE_TimeV.clear();
	fKE_EDV.clear();

	fKE_Momentum.clear();
	fKE_Pos.clear();

	fKE_RegDigV.clear();
	fKE_AmpDigV.clear();
	fKE_TimeDigV.clear();

	//printf("Esco da Segnale::KE_Reset()\n");
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::KE_Score(Int_t DetNb, Int_t RegionType, Double_t InpX,
			Double_t InpY, Double_t InpZ, Double_t InX,
			Double_t InY, Double_t InZ, Double_t OutX, 
			Double_t OutY,Double_t OutZ, 
			Double_t ER, Double_t QER, Double_t Time) {

	Double_t CurAmp;
	Double_t CurTime;

	Double_t TransportDistance = 0.;

	//calculate distance of ED to photomultiplier
	if (RegionType == 1) { // Barrel scintillation light transport time
	        Double_t z_out = Geo->GetKECenter().Z() - KE_BAR_LIGHTGUIDE_LENGTH;
		TransportDistance = ((InZ + OutZ) / 2. - z_out);

	} else if (RegionType == 2) { //Fiber scintillation light transport time
		TransportDistance = 0.;

	} else if (RegionType == 3) { // Big EndCap scintillation light transport time
		Double_t r0 = sqrt(
				pow(((InX + OutX) / 2.), 2) + pow(((InY + OutY) / 2.), 2));
		Double_t r_out = KE_BEC_RAD_OU + KE_BEC_LIGHTGUIDE_LENGTH;
		TransportDistance = (r_out - r0);

	} else if (RegionType == 4) { // Small EndCap scintillation light transport time
		Double_t r0 = sqrt(
				pow(((InX + OutX) / 2.), 2) + pow(((InY + OutY) / 2.), 2));
		Double_t r_out = KE_SEC_RAD_OU + KE_SEC_LIGHTGUIDE_LENGTH;
		TransportDistance = (r_out - r0);

	}

	//amplitude with attenuation after law of Lambert-Beer
	CurAmp = QER * std::exp(-gKE_Lambda * TransportDistance);
	//time (counted from point primary started)  //possibility to add random number here accounting for resolution (Gaussian)
	CurTime = Time + TransportDistance * gKE_InvSigVel;

	//save for all signals 
	//in this case it is not sorted by time but earliest time is taken (the effect should be really small)!
	Bool_t New = true;
	for (unsigned int i0; i0 < fKE_RegV.size(); i0++) {
		if (fKE_RegV[i0] == DetNb) {
			New = false;
			fKE_AmpV[i0] += CurAmp;
			if (Time < fKE_TimeV[i0])
				fKE_TimeV[i0] = Time;//take earliest arrival time
			fKE_EDV[i0] += QER;
		}
	}
	if (New == true) {
	  fKE_RegV.push_back(DetNb);
	  fKE_AmpV.push_back(CurAmp);
	  fKE_TimeV.push_back(CurTime);
	  fKE_EDV.push_back(QER);
	  fKE_Pos.push_back(TVector3(InX,InY,InZ));
	  fKE_Momentum.push_back(TVector3(InpX,InpY,InpZ));
	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::KE_Calibration() {

	//apply calibration factors only once per event

	for (unsigned int i1 = 0; i1 < fKE_RegV.size(); i1++) {

		fKE_AmpV[i1] *= kKE_AmpGain;
		fKE_AmpV[i1] += kKE_AmpOffset;

		fKE_TimeV[i1] *= kKE_TimeGain;
		fKE_TimeV[i1] += kKE_TimeOffset;

		//subtract measured START time from Start Counter
		//TODO: once calibration for Start Counter and KENTROS are known this should be done not here but for the digital values 
		if (fSC_Time != -1.) {
			fKE_TimeV[i1] -= fSC_Time;//WARNING: comment this just for testing
		} else {
			fKE_TimeV[i1] = -1.;
		}

	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::KE_Noise() {

	for (unsigned int i1 = 0; i1 < fKE_RegV.size(); i1++) {

		//add noise
		fKE_AmpV[i1] += rnd.Gaus(kKE_MeanAmp, kKE_SigmaAmp);
		fKE_TimeV[i1] += rnd.Gaus(kKE_MeanTime, kKE_SigmaTime);

	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::KE_Digitisation() {
	//digitisation with amplitude thresholds only once per event
	Int_t Dummy;

	for (unsigned int i1 = 0; i1 < fKE_TimeV.size(); i1++) {
		Dummy = Geo->castd2i(kKE_AmpAnaDig * fKE_AmpV[i1]);//depending on logic +0.5 here?
		if (Dummy > kKE_AmpThresMin) {
			fKE_RegDigV.push_back(fKE_RegV[i1]);
			fKE_AmpDigV.push_back(Dummy);
			fKE_TimeDigV.push_back(Geo->castd2i(kKE_TimeAnaDig * fKE_TimeV[i1]));//depending on logic +0.5 here?
		}
	}

	return 0;
}


/*-----------------------------------------------------------------*/
// TOF-Wall
/*-----------------------------------------------------------------*/
Int_t Segnale::TW_Init() {

// enable(!=0)/disable(0) energy threshold for TDC hit
	
  // VM 16/5/13 (values from gsi_geo.h)
  kTW_InvSigVel = TW_InvSigVel;
  kTW_Lambda = TW_Lambda;
  kTW_Ene_peak = TW_Ene_peak;
  kTW_Birk_const = TW_Birk_const;

  // VM 9/1/2014 lambda values for each slat loaded from file
  const Double_t minl=0.001;
  // load lambda values for single channel calibration
  TFile *prevFile = gFile;
  string c_dir = gSystem->Getenv("FLUKAGSI");
  c_dir = c_dir + "/config/SingleChannelCalib.root";
  TFile *fltof = new TFile(c_dir.data(),"READ");
  if (fltof->IsZombie()) {
    cout << "Error opening file " << c_dir.data() << endl;
    exit(0);
  }
  fltof->cd();
  TH1D * th1_lam_bot = ((TH1D *) gDirectory->Get("AlphavsSlat"));
  TH1D * th1_lam_top = ((TH1D *) gDirectory->Get("AlphavsSlat_hi"));
  Double_t lam_bot, lam_top, lam_mean;
  Int_t slatn;
  for (int i=1;i<=th1_lam_bot->GetNbinsX();i++) {
    slatn = th1_lam_bot->GetXaxis()->GetBinCenter(i);
    lam_bot=th1_lam_bot->GetBinContent(i);
    lam_top=th1_lam_top->GetBinContent(i);
    if (lam_bot>minl && lam_top>minl) lam_mean=(lam_bot+lam_top)/2.;
    else if (lam_bot>minl && lam_top<minl) lam_mean=lam_bot;
    else if (lam_bot<minl && lam_top>minl) lam_mean=lam_top;
    else lam_mean=0;
    if (slatn<201 && slatn>=0) {
      if (lam_mean>minl) 
    	kTW_Lambda_Slat[slatn]=lam_mean;
      else 
    	kTW_Lambda_Slat[slatn]=kTW_Lambda;
    }
  }
  prevFile->cd();

	// Taking the threshold for TW_time 
  // VM 16/5/13 common low energy (coservative) threshold 
  // single slat thresholds applied in l0mcreco (TATactNtuMC)

  // VM 16/5/13 calibration factors based on calculation
  // VM 9/1/2014 slat by slat dependence added
  //double sc_factor = std::exp(-TW_Lambda*TW_tofWallF_Y/2.)*250./
  //  (1+TW_Birk_const*TW_Ene_peak);
  Double_t sc_factor;

  if (kTW_apply_tdc_thr) {
    for (int b = 1; b < 197; b++) {
      sc_factor=std::exp(-kTW_Lambda_Slat[b]*TW_tofWallF_Y/2.)*250./
	(1+TW_Birk_const*TW_Ene_peak);
      if (b>0 && b<=96){
	kTW_AmpThresMinF[0][b-1] = kTW_CommEneThr_MeV*sc_factor;
	kTW_AmpThresMinF[1][b-1] = kTW_CommEneThr_MeV*sc_factor;
      }else if(b>100 && b<=196) {
	kTW_AmpThresMinR[0][b-101] = kTW_CommEneThr_MeV*sc_factor;
	kTW_AmpThresMinR[1][b-101] = kTW_CommEneThr_MeV*sc_factor;
      }
    }
  } else {
    for (int b = 1; b < 197; b++) {
      if (b>0 && b<=96){
	kTW_AmpThresMinF[0][b-1] = 0.;
	kTW_AmpThresMinF[1][b-1] = 0.;
      }else if(b>100 && b<=196) {
	kTW_AmpThresMinR[0][b-101] = 0.;
	kTW_AmpThresMinR[1][b-101] = 0.;
      }
    }
  }
  //printing the assigned values and scaling !!
  //for (Int_t i0 = 0; i0 < 2; i0++) {	// top or bottom	
  //  for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {
  //    cout<< "Front Top/bottom, sl no:"<<i1<<" "<<kTW_AmpThresMinF[i0][i1] <<endl;
  //  }
  //  for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {
  //    cout<< "Rear top/bottom, sl no:"<<i1+101<<" "<<kTW_AmpThresMinR[i0][i1] <<endl;
  //  }		
  //}


	for (Int_t i0 = 0; i0 < 2; i0++) {	//Looping over top and bottom

		fTW_VAmpF.push_back(vector<vector<Double_t> > ());
		fTW_VTimeF.push_back(vector<vector<Double_t> > ());
		for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {
			fTW_VAmpF[i0].push_back(vector<Double_t> ());
			fTW_VTimeF[i0].push_back(vector<Double_t> ());

			kTW_AmpGainF[i0][i1] = 1.E+6 * .25;//1.0;
			kTW_AmpOffsetF[i0][i1] = 0.0;
			kTW_AmpAnaDigF[i0][i1] = 1.0;
			//kTW_AmpThresMinF[i0][i1] = 100;//5

			kTW_TimGainF[i0][i1] = 1.E+9;//convert s->ns  //1.0;
			kTW_TimOffsetF[i0][i1] = 0.0;
			kTW_TimAnaDigF[i0][i1] = 1.E+3;

			kTW_AmpThresMinRawF[i0][i1] = (((float) kTW_AmpThresMinF[i0][i1])
					/ kTW_AmpAnaDigF[i0][i1] - kTW_AmpOffsetF[i0][i1])
					/ kTW_AmpGainF[i0][i1];

		}

		fTW_VAmpR.push_back(vector<vector<Double_t> > ());
		fTW_VTimeR.push_back(vector<vector<Double_t> > ());
		for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {
			fTW_VAmpR[i0].push_back(vector<Double_t> ());
			fTW_VTimeR[i0].push_back(vector<Double_t> ());

			kTW_AmpGainR[i0][i1] = 1.E+6 * .25;//1.0;
			kTW_AmpOffsetR[i0][i1] = 0.0;
			kTW_AmpAnaDigR[i0][i1] = 1.0;
			//kTW_AmpThresMinR[i0][i1] = 100;//5

			kTW_TimGainR[i0][i1] = 1.E+9;//convert s->ns
			kTW_TimOffsetR[i0][i1] = 0.0;
			kTW_TimAnaDigR[i0][i1] = 1.E+3;//this should be more or less correct scaling with the S254 data

			kTW_AmpThresMinRawR[i0][i1] = (((float) kTW_AmpThresMinR[i0][i1])
					/ kTW_AmpAnaDigR[i0][i1] - kTW_AmpOffsetR[i0][i1])
					/ kTW_AmpGainR[i0][i1];

		}
	}

	Segnale::TW_Reset();
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::TW_Reset() {
	//printf("Entro in Segnale::TOFWallReset()\n");

	for (Int_t i0 = 0; i0 < 2; i0++) {
		for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {
			fTW_VAmpF[i0][i1].clear();
			fTW_VTimeF[i0][i1].clear();

			fTW_AmpF[i0][i1] = 0.;
			fTW_TimeF[i0][i1] = 0.;
			fTW_EDF[i1] = 0.;

			fTW_AmpDigF[i0][i1] = 0;
			fTW_TimeDigF[i0][i1] = 0;
		}
		for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {
			fTW_VAmpR[i0][i1].clear();
			fTW_VTimeR[i0][i1].clear();

			fTW_AmpR[i0][i1] = 0.;
			fTW_TimeR[i0][i1] = 0.;
			fTW_EDR[i1] = 0.;

			fTW_AmpDigR[i0][i1] = 0;
			fTW_TimeDigR[i0][i1] = 0;
		}
	}
	//printf("Esco da Segnale::TOFWallReset()\n");
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::TW_Score(Int_t Wall, Int_t SlatID, Double_t Charge, Double_t ER,
		Double_t QER, Double_t InX, Double_t InY, Double_t InZ, Double_t OutX,
		Double_t OutY, Double_t OutZ, Double_t Time) {

	Double_t CurAmplitude[2];//0=top, 1=bottom
	Double_t CurTime[2];//0=top, 1=bottom
	Double_t LightInt;
	Double_t EnergyDeposited;
	Double_t MeanY;

	if ((SlatID < 0)
			|| (SlatID >= (Wall == 0 ? TW_NbSlatFront : TW_NbSlatBack))) {
		cerr << "ERROR: Segnale::TOFWallScore(): given Slat ID:" << SlatID
				<< " is < 0 or >= " << (Wall == 0 ? TW_NbSlatFront
				: TW_NbSlatBack) << " !" << endl;
		exit(1);
	}

	//calculating light intensity
	//possibility to add random number
	LightInt = QER;
	EnergyDeposited = ER;//for the ED variables
	MeanY = (InY + OutY) / 2.;

	//attenuation after law of Lambert-Beer
	// VM 9/1/2014 added slat dependence of lambda
	//CurAmplitude[0] = LightInt * std::exp(
	//		-(kTW_Lambda * (TW_Slat_Y / 2. + (Wall == 0 ? TW_ShiftOfSlats_F_Y
	//				: TW_ShiftOfSlats_R_Y) - MeanY))); //top
	//CurAmplitude[1] = LightInt * std::exp(
	//		-(kTW_Lambda * (TW_Slat_Y / 2. + (Wall == 0 ? TW_ShiftOfSlats_F_Y
	//				: TW_ShiftOfSlats_R_Y) + MeanY))); //bottom
	Int_t SlatIDn;
	if (!Wall) SlatIDn=SlatID+1;
	else SlatIDn=SlatID+101;
	CurAmplitude[0] = LightInt * std::exp(
			-(kTW_Lambda_Slat[SlatIDn] * (TW_Slat_Y / 2. + (Wall == 0 ? TW_ShiftOfSlats_F_Y
					: TW_ShiftOfSlats_R_Y) - MeanY))); //top
	CurAmplitude[1] = LightInt * std::exp(
			-(kTW_Lambda_Slat[SlatIDn] * (TW_Slat_Y / 2. + (Wall == 0 ? TW_ShiftOfSlats_F_Y
					: TW_ShiftOfSlats_R_Y) + MeanY))); //bottom

	//calculate here  time from point where primary was fired (not when it was detected in START counter, comes later), 
	//possibility to add random number here accounting for resolution (Gaussian)

	//AS: in order to produce the fucking biased MC we need to add by hand the time needed
	//for a given Fragment to travel from firing point to target.

	//	Time += 2.97954/1.E+9;  //Time needed to travel from SC to TG (in s)
	//	Time += 0.467541/1.E+9; //Time needed to travel from -10cm to SC. (to be subtracted later on, in s)

	CurTime[0] = Time + kTW_InvSigVel * (TW_Slat_Y / 2.
			+ (Wall == 0 ? TW_ShiftOfSlats_F_Y : TW_ShiftOfSlats_R_Y) - MeanY); //top
	CurTime[1] = Time + kTW_InvSigVel * (TW_Slat_Y / 2.
			+ (Wall == 0 ? TW_ShiftOfSlats_F_Y : TW_ShiftOfSlats_R_Y) + MeanY); //bottom


	for (Int_t i0 = 0; i0 < 2; i0++) {
		if (Wall == 0) {//front wall
			fTW_VAmpF[i0][SlatID].push_back(CurAmplitude[i0]);
			fTW_VTimeF[i0][SlatID].push_back(CurTime[i0]);
			if (i0 == 0)
				fTW_EDF[SlatID] += EnergyDeposited;

			//if there is a neighbouring slat add cross talk
			if (SlatID - 1 >= 0) {
				fTW_VAmpF[i0][SlatID - 1].push_back(
						kTW_SlatCrossTalk * CurAmplitude[i0]);
				fTW_VTimeF[i0][SlatID - 1].push_back(CurTime[i0]);
				if (i0 == 0)
					fTW_EDF[SlatID - 1] += kTW_SlatCrossTalk * EnergyDeposited;
			}
			if (SlatID + 1 < TW_NbSlatFront) {
				fTW_VAmpF[i0][SlatID + 1].push_back(
						kTW_SlatCrossTalk * CurAmplitude[i0]);
				fTW_VTimeF[i0][SlatID + 1].push_back(CurTime[i0]);
				if (i0 == 0)
					fTW_EDF[SlatID + 1] += kTW_SlatCrossTalk * EnergyDeposited;
			}
		} else {//rear wall
			fTW_VAmpR[i0][SlatID].push_back(CurAmplitude[i0]);
			fTW_VTimeR[i0][SlatID].push_back(CurTime[i0]);
			if (i0 == 0)
				fTW_EDR[SlatID] += EnergyDeposited;

			//if there is a neighbouring slat add cross talk
			if (SlatID - 1 >= 0) {
				fTW_VAmpR[i0][SlatID - 1].push_back(
						kTW_SlatCrossTalk * CurAmplitude[i0]);
				fTW_VTimeR[i0][SlatID - 1].push_back(CurTime[i0]);
				if (i0 == 0)
					fTW_EDR[SlatID - 1] += kTW_SlatCrossTalk * EnergyDeposited;
			}
			if (SlatID + 1 < TW_NbSlatBack) {
				fTW_VAmpR[i0][SlatID + 1].push_back(
						kTW_SlatCrossTalk * CurAmplitude[i0]);
				fTW_VTimeR[i0][SlatID + 1].push_back(CurTime[i0]);
				if (i0 == 0)
					fTW_EDR[SlatID + 1] += kTW_SlatCrossTalk * EnergyDeposited;
			}
		}
	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::TW_Calibration() {
	Bool_t LogThres = 0;

	for (Int_t i0 = 0; i0 < 2; i0++) {	// looping over top and bottom
		//      cout<<"DEBUG:"<<"Time Amp F"<<endl;
		for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {

			LogThres = 0;
			//sort signals by arrival time
			Segnale::SortVec(&fTW_VTimeF[i0][i1], &fTW_VAmpF[i0][i1]);
			//add up amplitudes and determine when amplitude threshold is reached and set time to this value
			for (unsigned int i2 = 0; i2 < fTW_VTimeF[i0][i1].size(); i2++) {
				//          cout<<"DEBUG:"<<fTW_VTimeF[i0][i1][i2]<<" "<<fTW_VAmpF[i0][i1][i2]<<endl;
				fTW_AmpF[i0][i1] += fTW_VAmpF[i0][i1][i2];
				if ((!LogThres) && (fTW_AmpF[i0][i1]
						> kTW_AmpThresMinRawF[i0][i1])) {
					LogThres = 1;
					fTW_TimeF[i0][i1] = fTW_VTimeF[i0][i1][i2];
				}
			}

			//apply calibration factors for each slat (separated top and bottom)  
			fTW_AmpF[i0][i1] *= kTW_AmpGainF[i0][i1];
			fTW_AmpF[i0][i1] += kTW_AmpOffsetF[i0][i1];

			fTW_TimeF[i0][i1] *= kTW_TimGainF[i0][i1];
			fTW_TimeF[i0][i1] += kTW_TimOffsetF[i0][i1];

			//subtract measured START time from Start Counter
			//TODO: once calibration for Start Counter and TOF-Wall are known this should be done not here but for the digital values 
			if (fSC_Time != -1.) {

			  fTW_TimeF[i0][i1] = fTW_TimeF[i0][i1] - fSC_Time;

			} else {
				fTW_TimeF[i0][i1] = -1.;
			}

			//if amplitude threshold is not reached set time to -1.
			if (!LogThres) {
				fTW_TimeF[i0][i1] = -1.;
			}
		}
		//     cout<<"DEBUG:"<<"Time Amp R"<<endl;
		for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {

			LogThres = 0;
			//sort signals by arrival time
			Segnale::SortVec(&fTW_VTimeR[i0][i1], &fTW_VAmpR[i0][i1]);
			//add up amplitudes and determine when amplitude threshold is reached and set time to this value
			for (unsigned int i2 = 0; i2 < fTW_VTimeR[i0][i1].size(); i2++) {
				//       cout<<"DEBUG:"<<fTW_VTimeR[i0][i1][i2]<<" "<<fTW_VAmpR[i0][i1][i2]<<endl;
				fTW_AmpR[i0][i1] += fTW_VAmpR[i0][i1][i2];
				if ((!LogThres) && (fTW_AmpR[i0][i1]
						> kTW_AmpThresMinRawR[i0][i1])) {
					LogThres = 1;
					fTW_TimeR[i0][i1] = fTW_VTimeR[i0][i1][i2];
				}
			}

			//apply calibration factors for each slat (separated top and bottom)  
			fTW_AmpR[i0][i1] *= kTW_AmpGainR[i0][i1];
			fTW_AmpR[i0][i1] += kTW_AmpOffsetR[i0][i1];

			fTW_TimeR[i0][i1] *= kTW_TimGainR[i0][i1];
			fTW_TimeR[i0][i1] += kTW_TimOffsetR[i0][i1];

			//subtract measured START time from Start Counter
			//TODO: once calibration for Start Counter and TOF-Wall are known this should be done not here but for the digital values 
			if (fSC_Time != -1.) {
				fTW_TimeR[i0][i1] = fTW_TimeR[i0][i1] - fSC_Time;
			} else {
				fTW_TimeR[i0][i1] = -1.;
			}

			//if amplitude threshold is not reached set time to -1.
			if (!LogThres) {
				fTW_TimeR[i0][i1] = -1.;
			}
		}
	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::TW_Noise() {
	//add noise only once per event
	//add noise dependent on the signal height?

	for (Int_t i0 = 0; i0 < 2; i0++) {
		for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {
		  
		  //fTW_AmpF[i0][i1] += rnd.Gaus(kTW_MeanAmp, kTW_SigmaAmp*fTW_AmpF[i0][i1]);
		  // MV changed 8/4/2013 
		  if (fTW_AmpF[i0][i1]>0) 
		    fTW_AmpF[i0][i1] += rnd.Gaus(kTW_MeanAmp, std::sqrt(kTW_SigmaAmp_1/fTW_AmpF[i0][i1]+kTW_SigmaAmp_2)*fTW_AmpF[i0][i1]);
		  if (fTW_AmpF[i0][i1]<0) fTW_AmpF[i0][i1]=0;
		  
		  fTW_TimeF[i0][i1] += rnd.Gaus(kTW_MeanTime, kTW_SigmaTime);
		  //if (fTW_TimeF[i0][i1]<=0) fTW_TimeF[i0][i1]=0;
		}
		for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {

		  //fTW_AmpR[i0][i1] += rnd.Gaus(kTW_MeanAmp, kTW_SigmaAmp*fTW_AmpR[i0][i1]);
		  // MV changed 8/4/2013
		  if (fTW_AmpR[i0][i1]>0) 
		    fTW_AmpR[i0][i1] += rnd.Gaus(kTW_MeanAmp, std::sqrt(kTW_SigmaAmp_1/fTW_AmpR[i0][i1]+kTW_SigmaAmp_2)*fTW_AmpR[i0][i1]);	
		  if (fTW_AmpR[i0][i1]<=0) fTW_AmpR[i0][i1]=0;

		  fTW_TimeR[i0][i1] += rnd.Gaus(kTW_MeanTime, kTW_SigmaTime);

		  //if (fTW_TimeR[i0][i1]<=0) fTW_TimeR[i0][i1]=0;

		}
	}

	// MV TOF resolution effect added 8/4/2013
	double tof_corr;
	for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {
	  tof_corr=rnd.Gaus(0.,kTW_Tof_Reso);
	  for (Int_t i0 = 0; i0 < 2; i0++) {
	    fTW_TimeF[i0][i1] += tof_corr;
	    if (fTW_TimeF[i0][i1]<=0) fTW_TimeF[i0][i1]=0;
	  }
	}
	for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {
	  tof_corr=rnd.Gaus(0.,kTW_Tof_Reso);
	  for (Int_t i0 = 0; i0 < 2; i0++) {
	    fTW_TimeR[i0][i1] += tof_corr;
	    if (fTW_TimeR[i0][i1]<=0) fTW_TimeR[i0][i1]=0;
	  }
	}


	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::TW_Digitisation() {
	//digitisation with amplitude thresholds for each slat (separated top and bottom) only once per event
	Int_t Dummy, Dummy2;


	for (Int_t i0 = 0; i0 < 2; i0++) {
		//     cout<<"DEBUG: TOFW signals "<<i0<<endl;
		for (Int_t i1 = 0; i1 < TW_NbSlatFront; i1++) {
			Dummy = Geo->castd2i(kTW_AmpAnaDigF[i0][i1] * fTW_AmpF[i0][i1]);//depending on logic +0.5 here?
			//fTW_AmpDigF[i0][i1] = (Dummy > kTW_AmpThresMinF[i0][i1]) ? Dummy : 0;
			fTW_AmpDigF[i0][i1] = Dummy;

			Dummy2 = Geo->castd2i(kTW_TimAnaDigF[i0][i1] * fTW_TimeF[i0][i1]);//depending on logic +0.5 here?
			fTW_TimeDigF[i0][i1] = (Dummy > kTW_AmpThresMinF[i0][i1]) ? Dummy2
					: -1;//if amplitude smaller than threshold set time to -1
		}
		for (Int_t i1 = 0; i1 < TW_NbSlatBack; i1++) {
			Dummy = Geo->castd2i(kTW_AmpAnaDigR[i0][i1] * fTW_AmpR[i0][i1]);//depending on logic +0.5 here?
			//fTW_AmpDigR[i0][i1] = (Dummy > kTW_AmpThresMinR[i0][i1]) ? Dummy
			//					: 0;
			fTW_AmpDigR[i0][i1] = Dummy;

			//			if (Dummy>0) printf("R sl=%d t/b=%d ene=%d thr=%d enedig=%d	\n",i1,i0,Dummy,kTW_AmpThresMinR[i0][i1],fTW_AmpDigR[i0][i1]);
			Dummy2 = Geo->castd2i(kTW_TimAnaDigR[i0][i1] * fTW_TimeR[i0][i1]);//depending on logic +0.5 here?
			fTW_TimeDigR[i0][i1] = (Dummy > kTW_AmpThresMinR[i0][i1]) ? Dummy2
					: -1;//if amplitude smaller than threshold set time to -1

			//       cout<<"DEBUG:"<<fTW_TimeF[i0][i1]<<" "<<fTW_TimeDigF[i0][i1]<<" "<<fTW_AmpF[i0][i1]<<" "<<fTW_AmpDigF[i0][i1]
			//                <<" "<<fTW_TimeR[i0][i1]<<" "<<fTW_TimeDigR[i0][i1]<<" "<<fTW_AmpR[i0][i1]<<" "<<fTW_AmpDigR[i0][i1]<<endl;
		}
	}

	return 0;
}

/*-----------------------------------------------------------------*/
//ONION
/*-----------------------------------------------------------------*/
Int_t Segnale::ON_Init() {

	Segnale::ON_Reset();
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::ON_Reset() {
	//printf("Entro in Segnale::ON_Reset()\n");

	fON_RegV.clear();
	fON_AmpV.clear();
	fON_TimeV.clear();
	fON_EDV.clear();

	fON_RegDigV.clear();
	fON_AmpDigV.clear();
	fON_TimeDigV.clear();

	//printf("Esco da Segnale::ON_Reset()\n");
	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::ON_Score(Int_t DetNb, Double_t InX, Double_t InY, Double_t InZ,
		Double_t OutX, Double_t OutY, Double_t OutZ, Double_t ER, Double_t QER,
		Double_t Time) {

	Double_t CurAmp;
	Double_t CurTime;

	//amplitude (assuming amplitude directly \propto energy release)
	CurAmp = QER;
	//time (counted from point primary started)  //possibility to add random number here accounting for resolution (Gaussian)
	CurTime = Time;

	//save for all signals 
	//in this case it is not sorted by time but earliest time is taken (the effect should be really small)!
	Bool_t New = true;
	for (unsigned int i0; i0 < fON_RegV.size(); i0++) {
		if (fON_RegV[i0] == DetNb) {
			New = false;
			fON_AmpV[i0] += CurAmp;
			if (Time < fON_TimeV[i0])
				fON_TimeV[i0] = Time;//take earliest arrival time
			fON_EDV[i0] += ER;
		}
	}
	if (New == true) {
		fON_RegV.push_back(DetNb);
		fON_AmpV.push_back(CurAmp);
		fON_TimeV.push_back(CurTime);
		fON_EDV.push_back(ER);
	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::ON_Calibration() {

	//apply calibration factors only once per event

	for (unsigned int i1 = 0; i1 < fON_RegV.size(); i1++) {

		fON_AmpV[i1] *= kON_AmpGain;
		fON_AmpV[i1] += kON_AmpOffset;

		fON_TimeV[i1] *= kON_TimeGain;
		fON_TimeV[i1] += kON_TimeOffset;

		//subtract measured START time from Start Counter
		//TODO: once calibration for Start Counter and ONION are known this should be done not here but for the digital values 
		if (fSC_Time != -1.) {
			fON_TimeV[i1] -= fSC_Time;//WARNING: comment this just for testing
		} else {
			fON_TimeV[i1] = -1.;
		}

	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::ON_Noise() {

	for (unsigned int i1 = 0; i1 < fON_RegV.size(); i1++) {

		//add noise
		fON_AmpV[i1] += rnd.Gaus(kON_MeanAmp, kON_SigmaAmp);
		fON_TimeV[i1] += rnd.Gaus(kON_MeanTime, kON_SigmaTime);

	}

	return 0;
}

/*-----------------------------------------------------------------*/
Int_t Segnale::ON_Digitisation() {
	//digitisation with amplitude thresholds only once per event
	Int_t Dummy;

	for (unsigned int i1 = 0; i1 < fON_TimeV.size(); i1++) {
		Dummy = Geo->castd2i(kON_AmpAnaDig * fON_AmpV[i1]);//depending on logic +0.5 here?
		if (Dummy > kON_AmpThresMin) {
			fON_RegDigV.push_back(fON_RegV[i1]);
			fON_AmpDigV.push_back(Dummy);
			fON_TimeDigV.push_back(Geo->castd2i(kON_TimeAnaDig * fON_TimeV[i1]));//depending on logic +0.5 here?
		}
	}

	return 0;
}

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


#include "ChargeBetheBlock.hxx"


ClassImp(ChargeBetheBlock);

ChargeBetheBlock::ChargeBetheBlock(Bool_t mc,TAGntuGlbTrk* Trk) {
 
  isMC = mc;
  Charge_bb=-1;

  TAGgeoTrafo* fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());  
  

  if (Trk->GetNAttachedHits()>0) {

    Double_t mom = Trk->GetMomentum();    
    Double_t mass = Trk->GetMass() ;   
    //Double_t chg_orig = Trk->GetCharge();
    Double_t beta = -1.;
    if (mom>1.E-5 && mass>1.E-5) {
      beta = mom/TMath::Sqrt((mom*mom)+(mass*mass));
    }
    TVector3 twdir = Trk->GetTofDirection(); 
    TVector3 twdir_loc = fpFirstGeo->VecFromGlobalToTWRLocal(twdir);  
    Double_t costheta = twdir_loc.CosTheta(); 
    Double_t deltaX = TW_Slat_Z/costheta;  // crossed length in cm
    TATntuRawHit * tofhit = Trk->GetHit(0);
    Double_t eloss = tofhit->e_loss;
    if (beta>0 && beta<1 && eloss>0) {
      Double_t e_bb = deltaE_z2_tof(deltaX,beta,mass);
      Charge_bb = find_z_mv(isMC, eloss, e_bb);
    }
  }
}

ChargeBetheBlock::~ChargeBetheBlock() {

}

Double_t ChargeBetheBlock::deltaE_z2_tof(Double_t deltaX, Double_t beta,
					 Double_t mass) 
{

  
  Double_t K=0.307075;	  // MeV cm2/g for A = 1 g/mol, so unit = MeV.
  Double_t ro=1.032;	  // mean density g/cm3
  Double_t HC=1.104;	  // H:C ratio for BC408 scitillator
  Double_t Zmed=(6+HC*1)/(1+HC);
  Double_t Amed=(12+HC*1)/(1+HC);
  Double_t I=4.7042E-5;	 // Mean excitation energy for scintillator BC408 [MeV]
  //Double_t J=0.000028816*TMath::Sqrt(ro*Zmed/Amed);	//hw
  Double_t Q=K*ro*(Zmed/Amed);		//Kroz2Z/A
  Double_t me=0.5109;                           // MeV
  // calculating the beta,gamma and Tmax
  
  //Double_t beta = mom/TMath::Sqrt((mom*mom)+(mass*mass));
  //if (beta<0 || beta>=1) {
    //if (dbg_loc) cout << "deltaE_z2_tof WARNING beta= " << beta << endl;
  //}
  Double_t beta2 = beta*beta;
  Double_t gamma=TMath::Sqrt(1/(1-beta2));
  Double_t gamma2 = gamma *gamma;  
  Double_t Tmax= 2*gamma2*beta2*me/(1+(2*gamma*(me/(mass*1000.)))+pow(me/(mass*1000.),2));
  Double_t logarithm=(0.5*log(2*me*beta2*gamma2*Tmax/(I*I))-beta2);
  Double_t dEdX=Q/beta2*logarithm;
  Double_t dE = dEdX*deltaX;
  
  return dE;

}


Int_t ChargeBetheBlock::find_z_mv(Bool_t mc, Double_t ene, Double_t ebb) {

  //const Double_t mine=1.E-5;
  Double_t peaks_mc[6] = {1.,2.,3.,4.,5.,6.};
  Double_t sigma_mc[6] = {0.14,0.15,0.18,0.21,0.26,0.28};
  Double_t peaks_dt[6] = {1.,2.,3.,4.,5.,6.};
  Double_t sigma_dt[6] = {0.14,0.15,0.18,0.21,0.26,0.28};

  Double_t ext_ch;
  if (ebb>1.E-5) {
    ext_ch = sqrt(ene/ebb);
  } else {
    ext_ch=-10.;
  }

  Int_t chg=-2;

  if (ext_ch>0) {
    chg=-2;
    Double_t min_ratio = 999999.;

    if (mc) {
      for (int i=0;i<6;i++) {
	if ((TMath::Abs(ext_ch-peaks_mc[i])/sigma_mc[i])<min_ratio) {
	  min_ratio=TMath::Abs(ext_ch-peaks_mc[i])/sigma_mc[i];
	  chg=i+1;
	}
      }
    } else {
      for (int i=0;i<6;i++) {
	if ((TMath::Abs(ext_ch-peaks_dt[i])/sigma_dt[i])<min_ratio) {
	  min_ratio=TMath::Abs(ext_ch-peaks_dt[i])/sigma_dt[i];
	  chg=i+1;
	}
      }
    }

  }

  return chg;
}

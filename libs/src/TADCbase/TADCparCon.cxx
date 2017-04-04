/*!
  \file
  \version $Id: adapted from TABMparCon.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TADCparCon.
*/

#include <string.h>

#include <fstream>
#include <math.h>

#include "TSystem.h"
#include "TString.h"
#include "TFile.h"

#include "TADCparCon.hxx"
#include "TAGroot.hxx"

//##############################################################################

/*!
  \class TADCparCon TADCparCon.hxx "TADCparCon.hxx"
  \brief conf parameters for the beam monitor. **
*/

ClassImp(TADCparCon);

//------------------------------------------+-----------------------------------
//! Default constructor.
TADCparCon::TADCparCon() {

  m_isMC = false;
  vdrift = 1./400;
  vector<double> myt0s;
  myt0s.resize(36);
  v_t0s = myt0s;

  f_mypol = new TF1("mymcpol","[0]+[1]*pow(x,1)+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",-0.01,-0.003);
  f_mypol2 = new TF1("mymcpol2","[0]+[1]*pow(x,1)+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",-0.01,-0.004);

}

//------------------------------------------+-----------------------------------
//! Destructor.

TADCparCon::~TADCparCon()
{}


//------------------------------------------+-----------------------------------
//! Read mapping data from file \a name .

Bool_t TADCparCon::FromFile(const TString& name) {

  Clear();
  
  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  char bufConf[1024];
  Double_t myArg1(0); 

  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'V')) {
      sscanf(bufConf, "V %lf",&myArg1);
      if(myArg1>0 && myArg1<1) {
	vdrift = myArg1;
      } else {
	Error(""," Plane Map Error:: check config file!!");
	return kTRUE;
      }
    } 
  }

  return kFALSE;
}

void TADCparCon::SetIsMC(bool ism) {
  m_isMC = ism;
}

bool TADCparCon::IsMC() {
  return m_isMC;
}

void TADCparCon::loadT0s(const TString& name) {

  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  char bufConf[1024];
  int myArg4(0); 
  int myArg2(0); 
  int myArg3(0); 
  double myArg1(0);

  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    return;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'#')) {
      sscanf(bufConf, "#%lf %d %d %d",&myArg1,&myArg2,&myArg3,&myArg4);
      if((myArg2== -1 || myArg2==1) && (myArg3>=0 && myArg3<=5) && (myArg4>=0 || myArg4<=2)) {
      	int tmpv = myArg2;
      	if(myArg2<0)tmpv = 0;
          	int chidx = myArg4+myArg3*3+tmpv*18;
          	v_t0s.at(chidx) = myArg1;
        } 
        else {
        	Error(""," Plane Map Error:: check config file!!");
        	return;
      }
    }
  }

  return;
}

void TADCparCon::SetT0s(vector<double> t0s) {

  if(t0s.size() == 36) {
    v_t0s = t0s;
  } else {
    Error("Parameter()","Vectors size mismatch:: fix the t0 vector inmput size!!! %d ",t0s.size());
  }

  return;
}

Double_t TADCparCon::GetT0(int view, int plane, int cell) {

  Double_t t0(0.);

  int my_view = 0; 
  if(view<0) my_view = 1;
  
  int my_idx = my_view*18+plane*3+cell;

  t0 = v_t0s.at(my_idx);

  return t0;

}


void TADCparCon::SetVDrift(Double_t v) {

  vdrift = v;

 return;
}

Double_t TADCparCon::GetVDrift() {
  return vdrift;
}

//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TADCparCon::Clear(Option_t*)
{
  v_t0s.clear();
  v_t0s.resize(36);
  vdrift = 1./400;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TADCparCon::ToStream(ostream& os, Option_t*) const
{
  os << "TADCparCon " << GetName() << endl;
  os << "vdrift:: " <<vdrift<< endl;

  return;
}


void TADCparCon::LoadSTrel(TString sF) {

  ifstream inS; TFile *f; char fname[200], bufConf[200];
  char name[200]; int idx;
  inS.open(sF.Data());
  Info("Action()","Processing STrel from %s  LIST file!",sF.Data());
  while (inS.getline(bufConf, 200, '\n')) {
    sscanf(bufConf,"%s",fname);
    Info("Action()","Adding %s STrel to be processed!",fname);
    f = new TFile(fname,"READ");
    f->cd();

    Int_t i_run = gTAGroot->CurrentRunNumber();
    sprintf(name,"%d",i_run);
    gDirectory->cd(name);
    //    gDirectory->ls();

    sprintf(name,"strel_fun_new");
    //	  cout<<" "<<name<<endl;
    m_myFunSpl = ((TF1*)gDirectory->Get(name));
    m_myVFunSpl.push_back(m_myFunSpl);
    
  }
  Info("Action()","Processed data from %s LIST file!",sF.Data());
  inS.close();
  
  return;
  
}

 /*-------------------------------------------------*/

double TADCparCon::STrel_Delta1(double time) {
  double p0=0.007471, p1=-0.005854, p2= 6.38379e-05, p3=-2.61452e-07, p4= 3.78368e-10;
  double res;
  res = p0 + p1*time + p2*time*time + p3*pow(time,3) + p4*pow(time,4);
  return res;
}

 /*-------------------------------------------------*/

double TADCparCon::STrel_Delta2(double time) {
  double p0=-0.031722, p1=0.00055413, p2= -8.75471e-06, p3= 5.19529e-08, 
    p4=-9.49872e-11 ;
  double res;
  res = p0 + p1*time + p2*time*time + p3*pow(time,3) + p4*pow(time,4);
  return res;
}

/*----------------------------------------*/

double TADCparCon::STrel_Delta3(double time) {
  double p0= -0.00864077, p1= 0.000225237, p2= -3.39075e-06, p3=  2.02131e-08, 
    p4=  -3.68566e-11;
  double res;
  res = p0 + p1*time + p2*time*time + p3*pow(time,3) + p4*pow(time,4);
  return res;
}

double TADCparCon::STrelCorr(double time, int ic, int ip, int iv) {

  double res = 0;
  bool ana = kTRUE;

  int t_ic, t_ip, t_iv;
  t_ic = ic; t_ip = ip; t_iv = iv;

  int howManyfiles(0);
  if(!ana) {
    howManyfiles = m_myVFunSpl.size();
    for(int ih =0; ih<howManyfiles; ih++) {
      Info("Action()","STrel:: %lf %d %lf %d ",time,ih,res,m_myVFunSpl.size());
      if(m_myVFunSpl.at(ih)) {
	res += (m_myVFunSpl.at(ih))->Eval(time);
	Info("Action()","STrel:: %d %lf ",ih,(m_myVFunSpl.at(ih))->Eval(time));
      }
    }
    
    //  res *= 0.3;
  } else {
    res -= STrel_Delta1(fabs(time)); 
    res -= STrel_Delta2(fabs(time)); 
    res -= STrel_Delta3(fabs(time)); 
  }

  return res;

}

void TADCparCon::LoadReso(TString sF) {

  TFile *f; 
  Info("Action()","Processing Reso vs R from %s  LIST file!",sF.Data());
  
  f = new TFile(sF.Data(),"READ");
  f->cd();
  my_hreso = ((TH1D*)gDirectory->Get("myHp"));
  
  return;
  
}

double TADCparCon::ResoEval(double dist) {

  double sigma(0.12);
  int mybin(-1);
  if(my_hreso) {
    mybin = my_hreso->FindBin(dist);
    sigma = my_hreso->GetBinContent(mybin)/1e4;
  }
  if ( mybin == -1 ) 
    sigma = 0.12;
  return sigma;
  
}

TF1* TADCparCon::GetCalibX() {

  return f_mypol;
}


TF1* TADCparCon::GetCalibY() {

  return f_mypol2;

}

void TADCparCon::ConfigureTrkCalib() {

  /* OLD setup
   1  p0           1.69143e-02   1.78395e-05   1.25294e-08  -1.12766e-02
   2  p1           2.21824e+01   4.37184e-03   1.05774e-05  -5.07310e-06
   3  p2           8.09415e+03   6.11105e-01   3.85959e-03   5.18272e-08
   4  p3           1.39385e+06   8.36591e+01   6.64637e-01  -3.42786e-10
   5  p4           1.13567e+08   1.04695e+04   5.41532e+01   3.66862e-13
   6  p5           3.53414e+09   9.05361e+05   1.68521e+03   3.57968e-14

   1  p0          -1.15688e-02   2.02851e-05   1.00354e-08  -2.74132e+00
   2  p1          -1.96039e+00   4.12901e-03   1.36445e-06  -5.52166e-02
   3  p2          -1.16613e+02   5.45908e-01   1.77382e-04  -1.21665e-03
   4  p3           2.46797e+04   6.95556e+01   2.22149e-02  -9.68817e-06
   5  p4           2.01676e+06   8.23400e+03   2.67413e+00  -5.28596e-08
   6  p5          -2.05344e+06   7.32900e+05   2.08980e+01  -3.10577e-11
  */

  //X correction
  if(m_isMC) {
    f_mypol->SetParameters(0.,1.,0.,0.,0.,0.);
  } else {
    f_mypol->SetName("mypol");
    f_mypol->SetParameters( 70.61e-05,
			    2.685e-01,
			    1.203e+01,
			   -2.421e+04,
			    1.112e+04,
			    4.325e+08);
  }

  /*
   1  p0          -1.92457e-05   9.96755e-06   2.67768e-08   2.75109e-03
   2  p1           3.65559e-01   1.37338e-02   1.88348e-05  -4.51475e-06
   3  p2          -2.01191e+01   6.94988e+00   7.23064e-03   1.17480e-08
   4  p3          -4.47220e+04   4.31045e+03   2.29598e+00   4.64828e-11
   5  p4           2.79566e+05   5.85631e+05   2.16321e+01  -1.73594e-12
   6  p5           1.90396e+09   2.74307e+08   9.07879e+02   1.93312e-15

   1  p0          -1.01338e-04   9.87766e-06   2.56656e-08   5.76352e-02
   2  p1           5.49605e-01   1.32980e-02   1.94777e-05  -4.83048e-04
   3  p2          -2.40775e+01   7.04670e+00   8.12795e-03   3.90372e-07
   4  p3          -4.67887e+04   4.61691e+03   2.69340e+00  -3.47984e-09
   5  p4           1.98095e+06   6.33007e+05   2.08960e+01   3.79568e-12
   6  p5           1.44651e+09   3.07455e+08   6.89752e+02  -2.36315e-14
  */

  //Y correction
  if(m_isMC) {
    f_mypol2->SetParameters(0.,1.,0.,0.,0.,0.);
  } else {
    f_mypol2->SetName("mypol2");
    f_mypol2->SetParameters(-21.95e-05,
			     2.953e-01,
			     17.41e+01,
			    -2.699e+04,
			    -1.061e+07,
			     1.667e+09);
  }
  return;

}

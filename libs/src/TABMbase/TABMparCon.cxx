/*!
  \file
  \version $Id: TABMparCon.cxx,v 1.2 2003/06/22 19:34:21 mueller Exp $
  \brief   Implementation of TABMparCon.
*/

#include <string.h>

#include <fstream>
#include <math.h>
#include <vector>

using namespace std; 
#include "TSystem.h"
#include "TString.h"
#include "TFile.h"

#include "TABMparCon.hxx"
#include "TAGroot.hxx"

//##############################################################################

/*!
  \class TABMparCon TABMparCon.hxx "TABMparCon.hxx"
  \brief conf parameters for the beam monitor. **
*/

ClassImp(TABMparCon);

//------------------------------------------+-----------------------------------
//! Default constructor.
TABMparCon::TABMparCon() {

  m_isMC = false;
  vdrift = 1./400;
  rdrift_cut = 10.;
  enxcell_cut = 0.00000001;
  chi2red_cut = 5.;
  angz_cut = 5.;
  angzres_cut=5.;
  fitter_index = 0;
  bm_debug=0;
  bm_vietrack=0;
  total_ev_num=0;
  manageT0BM=0;
  minnhit_cut=0;
  maxnhit_cut=20;
  rejmax_cut=36;
  
  vector<double> myt0s(36,-10000);
  //~ myt0s.resize(36);
  v_t0s = myt0s;

  f_mypol = new TF1("mymcpol","[0]+[1]*pow(x,1)+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",-0.01,-0.003);
  f_mypol2 = new TF1("mymcpol2","[0]+[1]*pow(x,1)+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",-0.01,-0.004);

}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMparCon::~TABMparCon()
{}


//------------------------------------------+-----------------------------------
//! Read mapping data from file \a name .

Bool_t TABMparCon::FromFile(const TString& name) {

  Clear();
  
  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  char bufConf[1024], tmp_char[200];
  Double_t myArg1(0); 
  Int_t myArgInt(-1), myArgIntmax(-1);
  
  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    }else if(strchr(bufConf,'V')) {
      sscanf(bufConf, "V %lf",&myArg1);
      if(myArg1>0 && myArg1<1) 
        vdrift = myArg1;
      else {
	      Error(""," Plane Map Error:: check config file!! (V)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'R')) {
      sscanf(bufConf, "R %lf",&myArg1);
      if(myArg1>0) 
        rdrift_cut = myArg1;
      else {
	      Error(""," Plane Map Error:: check config file!! (R)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'D')) {
      sscanf(bufConf, "D %d",&myArgInt);
      if(myArgInt>=0) 
        bm_debug = myArgInt;
      else {
	      Error(""," Plane Map Error:: check config file!! (D)");
	      return kTRUE;
        }
        bm_debug=max(bm_debug,GlobalPar::GetPar()->Debug());
    }else if(strchr(bufConf,'P')) {
      sscanf(bufConf, "P %d",&myArgInt);
      if(myArgInt>=0) 
        bm_vietrack = myArgInt;
      else {
	      Error(""," Plane Map Error:: check config file!! (P)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'N')) {
      sscanf(bufConf, "N %lf",&myArg1);
      if(myArg1>0) 
        angzres_cut = myArg1;
      else {
	      Error(""," Plane Map Error:: check config file!! (N)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'H')) {
      sscanf(bufConf, "H %d %d",&myArgInt, &myArgIntmax);
      if(myArgInt>=0 && myArgIntmax>0 && myArgIntmax<37 && myArgIntmax>=myArgInt){ 
        minnhit_cut = myArgInt;
        maxnhit_cut = myArgIntmax;
      }else {
	      Error(""," Plane Map Error:: check config file!! (H)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'E')) {
      sscanf(bufConf, "E %lf",&myArg1);
      if(myArg1>0)
        enxcell_cut = myArg1;
      else {
	      Error(""," Plane Map Error:: check config file!! (E)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'C')) {
      sscanf(bufConf, "C %lf",&myArg1);
      if(myArg1>0)
        chi2red_cut = myArg1;
      else {
	      Error(""," Plane Map Error:: check config file!! (C)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'L')) {
      sscanf(bufConf, "L %d",&myArgInt);
      if(myArgInt>0 && myArgInt<7)
        planehit_cut = myArgInt;
      else {
	      Error(""," Plane Map Error:: check config file!! (L)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'Z')) {
      sscanf(bufConf, "Z %d %s",&myArgInt, tmp_char);
      if(myArgInt==0 || myArgInt==1){
        manageT0BM = myArgInt;
        bmt0file=tmp_char;
      }else {
	      Error(""," Plane Map Error:: check config file!! (L)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'A')) {
      sscanf(bufConf, "A %lf",&myArg1);
      if(myArg1>0)
        angz_cut = myArg1;
      else {
	      Error(""," Plane Map Error:: check config file!! (A)");
	      return kTRUE;
        }
    //~ }else if(strchr(bufConf,'M')) {
      //~ sscanf(bufConf, "M %d %s",&myArgInt, tmp_char);
      //~ if(myArgInt==0 || myArgInt==1){
        //~ m_isMC = myArgInt;
        //~ datafile_name=tmp_char;
      //~ }else {
	      //~ Error(""," Plane Map Error:: check config file!! (M)");
	      //~ return kTRUE;
        //~ }
    }else if(strchr(bufConf,'T')) {
      sscanf(bufConf, "T %d %lf",&myArgInt, &myArg1);
      if(myArgInt>0 || myArg1>0.){
        part_in_charge = myArgInt;
        part_in_mom=myArg1;
      }else {
	      Error(""," Plane Map Error:: check config file!! (T)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'J')) {
      sscanf(bufConf, "J %d",&myArgInt);
      if(myArgInt>=0){
        rejmax_cut = myArgInt;
      }else {
	      Error(""," Plane Map Error:: check config file!! (J)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'K')) {
      sscanf(bufConf, "K %s",tmp_char);
        parmapfile=tmp_char;
    }else if(strchr(bufConf,'F')) {
      sscanf(bufConf, "F %d",&myArgInt);
      if(myArgInt>=0)
        fitter_index = myArgInt;
      else {
	      Error(""," Plane Map Error:: check config file!! (F)");
	      return kTRUE;
        }
      }
  }//end of readline
  if(m_isMC){
    part_in_charge=PRIM_Z;
    part_in_mom=(Double_t)PRIM_A*PRIM_T;
  }

  return kFALSE;
}


void TABMparCon::PrintT0s(TString &input_file_name){
  ofstream outfile;
  TString name="./config/"+bmt0file;
  outfile.open(name.Data(),ios::out);
  outfile<<"calculated_from: "<<input_file_name.Data()<<"    number_of_events= "<<total_ev_num<<endl;
  for(Int_t i=0;i<36;i++)
    if(v_t0s[i]!=-10000)
      outfile<<"cellid= "<<i<<"  T0_time= "<<v_t0s[i]<<endl;
    else
      cout<<"ERROR in TABMparCON::PrintT0s: v_t0s not set properly for cell_id="<<i<<"  v_t0s="<<v_t0s[i]<<endl;  
  outfile.close();
  return;
}

void TABMparCon::loadT0s() {
  ifstream infile;
  TString name="./config/"+bmt0file;
  infile.open(name.Data(),ios::in);
  Int_t file_evnum;
  char tmp_char[200], dataset[200];
  vector<Double_t> fileT0(36,-1000.);
  Int_t tmp_int=-1, status=0;  
  if(infile.is_open() && infile.good())
    infile>>tmp_char>>dataset>>tmp_char>>file_evnum;
  else
    status=1;
  if(file_evnum<total_ev_num)
    cout<<"TABMparCon::loadT0s::WARNING!!!!!!!!!!!!!!!!!!!!! you load a T0 file calculated from "<<dataset<<" which have only "<<file_evnum<<" events, while the input file have a larger number of events="<<total_ev_num<<endl;  
  for(Int_t i=0;i<36;i++)
    if(!infile.eof() && tmp_int==i-1)
      infile>>tmp_char>>tmp_int>>tmp_char>>fileT0[i];
    else{
      cout<<"TABMparCon::loadT0s::Error in the T0 file "<<bmt0file<<"!!!!!! check if it is write properly"<<endl;  
      status=1;
      }
  infile.close();
  if(status==0)
    v_t0s=fileT0;
  else
    cout<<"TABMparCon::loadT0s::ERROR, the T0 are calculated from the input file directly"<<endl;


  //~ TString name_exp = name;
  //~ gSystem->ExpandPathName(name_exp);

  //~ char bufConf[1024];
  //~ int myArg4(0); 
  //~ int myArg2(0); 
  //~ int myArg3(0); 
  //~ double myArg1(0);

  //~ ifstream incF;
  //~ incF.open(name_exp.Data());
  //~ if (!incF) {
    //~ Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    //~ return;
  //~ }

  //~ while (incF.getline(bufConf, 200, '\n')) {
    //~ if(strchr(bufConf,'!')) {
      //~ //      Info("FromFile()","Skip comment line:: %s",bufConf);
    //~ } else if(strchr(bufConf,'#')) {
      //~ sscanf(bufConf, "#%lf %d %d %d",&myArg1,&myArg2,&myArg3,&myArg4);
      //~ if((myArg2== -1 || myArg2==1) && (myArg3>=0 && myArg3<=5) && (myArg4>=0 || myArg4<=2)) {
        //~ int tmpv = myArg2;
        //~ if(myArg2<0)tmpv = 0;//per shift delle view che in file sono -1 e 1, mentre qua serve 0 e 1
        //~ int chidx = myArg4+myArg3*3+tmpv*18;
        //~ v_t0s.at(chidx) = myArg1;
      //~ } else {
        //~ Error(""," Plane Map Error:: check config file!!");
        //~ return;
      //~ }
    //~ }
  //~ }

  return;
}

void TABMparCon::SetT0s(vector<Double_t> t0s) {

  if(t0s.size() == 36) {
    v_t0s = t0s;
  } else {
    Error("Parameter()","Vectors size mismatch:: fix the t0 vector inmput size!!! %d ",t0s.size());
  }

  return;
}


void TABMparCon::SetT0(Int_t cha, Double_t t0in){

if(cha<36 && cha>=0) 
  v_t0s[cha]=t0in;   
else {
    Error("Parameter()","Channel out of Range!!! cha=%d",cha);
  }

  return;
}

void TABMparCon::CoutT0(){
  //~ TAGparaDsc* myp_bmmap=gTAGroot->FindParaDsc("myp_bmmap", "TABMparMap");
  //~ TABMparMap* bmmap = (TABMparMap*) myp_bmmap->Object();
  cout<<"Print BM T0 time:"<<endl;
  for(Int_t i=0;i<v_t0s.size();i++)
    cout<<"cell_id="<<i<<"  T0="<<v_t0s[i]<<endl;
    //~ cout<<"cell_id="<<i<<"  TDC_channel="<<bmmap->cell2tdc(i)<<"  T0="<<v_t0s[i]<<endl;
}


//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TABMparCon::Clear(Option_t*)
{
  v_t0s.clear();
  v_t0s.resize(36);
  vdrift = 1./400;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TABMparCon::ToStream(ostream& os, Option_t*) const
{
  os << "TABMparCon " << GetName() << endl;
  os << "vdrift:: " <<vdrift<< endl;

  return;
}


void TABMparCon::LoadSTrel(TString sF) {

  ifstream inS; TFile *f; char fname[200], bufConf[200];
  char name[200]; int idx;
  inS.open(sF.Data());
  if(!inS.is_open())
    cout<<"ERROR in TABMparCon::LoadSTrel:  cannot find the file "<<sF.Data()<<endl;
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

Double_t TABMparCon::FirstSTrel(Double_t tdrift){
  return 0.032891770+0.0050746330*tdrift-(5.1692440e-05)*tdrift*tdrift+(1.8928600e-07)*tdrift*tdrift*tdrift-(2.4652420e-10)*tdrift*tdrift*tdrift*tdrift; 
}


void TABMparCon::LoadReso(TString sF) {

  TFile *f; 
  Info("Action()","Processing Reso vs R from %s  LIST file!",sF.Data());
  
  f = new TFile(sF.Data(),"READ");
  f->cd();
  my_hreso = ((TH1D*)gDirectory->Get("myHp"));
  
  return;
  
}

double TABMparCon::ResoEval(Double_t dist) {

  double sigma(0.12);
  int mybin(-1);
  if(my_hreso) {
    mybin = my_hreso->FindBin(dist);
    sigma = my_hreso->GetBinContent(mybin)/10000;
  }
  return sigma>0 ? sigma:0.12;
  
}

TF1* TABMparCon::GetCalibX() {

  return f_mypol;
}


TF1* TABMparCon::GetCalibY() {

  return f_mypol2;

}

void TABMparCon::ConfigureTrkCalib() {

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

//~ double TABMparCon::STrel_Delta1(double time) {
  //~ double p0=0.007471, p1=-0.005854, p2= 6.38379e-05, p3=-2.61452e-07, p4= 3.78368e-10;
  //~ double res;
  //~ res = p0 + p1*time + p2*time*time + p3*pow(time,3) + p4*pow(time,4);
  //~ return res;
//~ }

 //~ /*-------------------------------------------------*/

//~ double TABMparCon::STrel_Delta2(double time) {
  //~ double p0=-0.031722, p1=0.00055413, p2= -8.75471e-06, p3= 5.19529e-08, p4=-9.49872e-11 ;
  //~ double res;
  //~ res = p0 + p1*time + p2*time*time + p3*pow(time,3) + p4*pow(time,4);
  //~ return res;
//~ }

//~ /*----------------------------------------*/

//~ double TABMparCon::STrel_Delta3(double time) {
  //~ double p0= -0.00864077, p1= 0.000225237, p2= -3.39075e-06, p3=  2.02131e-08, p4=  -3.68566e-11;
  //~ double res;
  //~ res = p0 + p1*time + p2*time*time + p3*pow(time,3) + p4*pow(time,4);
  //~ return res;
//~ }

//~ //provv, da ottimizzare
//~ double TABMparCon::STrelCorr(double time, int ic, int ip, int iv) {//per ora serve solo il tempo perchè per ogni cella le strel sono le stesse...

  //~ double res = 0;
  //~ bool ana = kTRUE;

  //~ int t_ic, t_ip, t_iv;
  //~ t_ic = ic; t_ip = ip; t_iv = iv;

  //~ int howManyfiles(0);
  //~ if(!ana) {
    //~ howManyfiles = m_myVFunSpl.size();
    //~ for(int ih =0; ih<howManyfiles; ih++) {
      //~ Info("Action()","STrel:: %lf %d %lf %d ",time,ih,res,m_myVFunSpl.size());
      //~ if(m_myVFunSpl.at(ih)) {
        //~ res += (m_myVFunSpl.at(ih))->Eval(time);
        //~ Info("Action()","STrel:: %d %lf ",ih,(m_myVFunSpl.at(ih))->Eval(time));
      //~ }
    //~ }
    
    //~ //  res *= 0.3;
  //~ } else {
    //~ res -= STrel_Delta1(fabs(time)); 
    //~ res -= STrel_Delta2(fabs(time)); 
    //~ res -= STrel_Delta3(fabs(time)); 
  //~ }

  //~ return res;

//~ }


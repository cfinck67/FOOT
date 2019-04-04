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

  //~ m_isMC = false;
  //~ rdrift_cut = 10.;
  //~ enxcell_cut = 0.00000001;
  //~ chi2red_cut = 5.;
  //~ angz_cut = 5.;
  //~ angzres_cut=5.;
  //~ fitter_index = 0;
  //~ bm_debug=0;
  //~ bm_vietrack=0;
  //~ manageT0BM=0;
  //~ minnhit_cut=0;
  //~ maxnhit_cut=20;
  //~ rejmax_cut=36;
  
  //~ vector<Double_t> myt0s(36,-10000);
  //~ v_t0s = myt0s;

  //~ f_mypol = new TF1("mymcpol","[0]+[1]*pow(x,1)+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",-0.01,-0.003);
  //~ f_mypol2 = new TF1("mymcpol2","[0]+[1]*pow(x,1)+[2]*pow(x,2)+[3]*pow(x,3)+[4]*pow(x,4)+[5]*pow(x,5)",-0.01,-0.004);
  rand= new TRandom3();
  rdrift_err=0.015;  
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMparCon::~TABMparCon()
{delete rand;}


//------------------------------------------+-----------------------------------
//! Read mapping data from file \a name .

Bool_t TABMparCon::FromFile(const TString& name) {

  Clear();
  
  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  char bufConf[1024], tmp_char[200];
  Double_t myArg1(-1),myArg2(-1),myArg3(-1),myArg4(-1),myArg5(-1),myArg6(-1); 
  Int_t myArgInt(-1), myArgIntmax(-1), myArgIntmin(-1);
  
  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {    
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
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
      if(myArgInt>=0 && myArgIntmax>0 && myArgIntmax>=myArgInt){ 
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
      sscanf(bufConf, "Z %d %d %lf %lf %s",&myArgInt, &myArgIntmax, &myArg1, &myArg2, tmp_char);
      if((myArgInt==0 || myArgInt==1) && (myArgIntmax==1 || myArgIntmax==0 || myArgIntmax==2 || myArgIntmax==3)  &&  myArg1>=0 && myArg2>=0){
        manageT0BM = myArgInt;
        t0_switch=myArgIntmax;
        t0_sigma=myArg1;
        hit_timecut=myArg2;
        bmt0file=tmp_char;
      }else {
	      Error(""," Plane Map Error:: check config file!! (Z)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'B')) {
      sscanf(bufConf, "B %d %s",&myArgInt, tmp_char);
      if(myArgInt==0 || myArgInt==1){
        manageADCped=myArgInt;
        bmpedfile=tmp_char;
      }else {
	      Error(""," Plane Map Error:: check config file!! (B)");
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
    }else if(strchr(bufConf,'M')) {
      sscanf(bufConf, "M %d %lf %lf %lf %lf %lf %d",&myArgInt, &myArg1, &myArg2, &myArg3, &myArg4, &myArg5, &myArgIntmax);
      if((myArgInt==0 || myArgInt==1) && myArg1>=0 && myArg2>=0 && myArg3>=0 && myArg4>=0 && myArg5>=0 && myArgIntmax>=0 && myArgIntmax<6){
        smearhits = myArgInt;
        fakehits_mean=myArg1;
        fakehits_sigmaleft=myArg2;
        fakehits_sigmaright=myArg3;
        mceff_mean=myArg4;
        mceff_sigma=myArg5;
        smearrdrift=myArgIntmax;
      }else {
	      Error(""," Plane Map Error:: check config file!! (M)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'T')) {
      sscanf(bufConf, "T %d %lf",&myArgInt, &myArg1);
      if(myArgInt>0 || myArg1>0.){
        part_in_charge = myArgInt;
        part_in_mom=myArg1;
      }else {
	      Error(""," Plane Map Error:: check config file!! (T)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'W')) {
      sscanf(bufConf, "W %d",&myArgInt);
      strel_switch = myArgInt;        
    }else if(strchr(bufConf,'J')) {
      sscanf(bufConf, "J %d",&myArgInt);
      if(myArgInt>=0){
        rejmax_cut = myArgInt;
      }else {
	      Error(""," Plane Map Error:: check config file!! (J)");
	      return kTRUE;
        }
    }else if(strchr(bufConf,'S')) {
      sscanf(bufConf, "S %d %lf %lf %lf %lf %lf %lf",&myArgInt,&myArg1, &myArg2, &myArg3, &myArg4, &myArg5, &myArg6);
      if(myArg4<180. && myArg5<180. && myArg6<180.){
        calibro=myArgInt;
        meas_shift.SetXYZ(myArg1,myArg2,myArg3);
        meas_tilt.SetXYZ(myArg4,myArg5,myArg6);
      }else if(myArgInt==0){
        calibro=0;
        meas_shift.SetXYZ(0.,0.,0.);
        meas_tilt.SetXYZ(0.,0.,0.);
      }else{
	      Error(""," Plane Map Error:: check config file!! (S)");
	      return kTRUE;
        }  
    }else if(strchr(bufConf,'K')) {
      sscanf(bufConf, "K %s",tmp_char);
        parmapfile=tmp_char;
    }else if(strchr(bufConf,'F')) {
      sscanf(bufConf, "F %d %d %d %lf",&myArgInt, &myArgIntmax, &myArgIntmin, &myArg1);
      if(myArgInt>=0 && myArgIntmax>=0 && myArgIntmin>=0 && myArg1>=0.){
        fitter_index = myArgInt;
        prefit_enable=myArgIntmax;
        num_ite=myArgIntmin;
        par_move=myArg1;
      }else {
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


void TABMparCon::PrintT0s(TString &input_file_name, Long64_t tot_num_ev){
  ofstream outfile;
  TString name="./config/"+bmt0file;
  outfile.open(name.Data(),ios::out);
  outfile<<"calculated_from: "<<input_file_name.Data()<<"    number_of_events= "<<tot_num_ev<<"  t0_switch= "<<t0_switch<<endl;
  for(Int_t i=0;i<36;i++)
    outfile<<"cellid= "<<i<<"  T0_time= "<<v_t0s[i]<<endl;
  outfile.close();
  return;
}


Bool_t TABMparCon::loadT0s(Long64_t tot_num_ev) {
  ifstream infile;
  TString name="./config/"+bmt0file;
  infile.open(name.Data(),ios::in);
  Int_t file_evnum, old_t0switch;
  char tmp_char[200], dataset[200];
  vector<Double_t> fileT0(36,-10000.);
  Int_t tmp_int=-1, status=0;  
  if(infile.is_open() && infile.good())
    infile>>tmp_char>>dataset>>tmp_char>>file_evnum>>tmp_char>>old_t0switch;
  else
    status=1;

  if(file_evnum<tot_num_ev)
    cout<<"TABMparCon::loadT0s::WARNING!!!!!! you load a T0 file calculated from "<<dataset<<" which have only "<<file_evnum<<" events, while the input file have a larger number of events="<<tot_num_ev<<endl;  
  if(old_t0switch!=t0_switch){
    cout<<"TABMparCon::loadT0s::ERROR!!!!!! you load a T0 file calculated from "<<dataset<<" in which the t0 were calculated with a t0_switch="<<old_t0switch<<", now your t0_switch is "<<t0_switch<<endl;
    status=1;  
  }
  for(Int_t i=0;i<36;i++)
    if(!infile.eof() && tmp_int==i-1)
      infile>>tmp_char>>tmp_int>>tmp_char>>fileT0.at(i);
    else{
      cout<<"TABMparCon::loadT0s::Error in the T0 file "<<bmt0file<<"!!!!!! check if it is write properly"<<endl;  
      status=1;
      }
  infile.close();
  if(status==0)
    v_t0s=fileT0;
  else{
    cout<<"TABMparCon::loadT0s::ERROR, the T0 can not be charged"<<endl;
    return kTRUE;
  }

  //check if the T0 are ok 
  for(Int_t i=0;i<36;i++)
    if(v_t0s[i]==-10000)
      cout<<"WARNING IN TABMparCon::LoadT0! channel not considered in tdc map tdc_cha=i="<<i<<" T0 for this channel is set to -10000"<<endl;
    else if(v_t0s[i]==-20000)
      cout<<"WARNING IN TABMparCon::LoadT0! channel with too few elements to evaluate T0: tdc_cha=i="<<i<<" T0 for this channel is set to -20000"<<endl;

  return kFALSE;
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


void TABMparCon::loadADCped(Int_t mapcha) {
  ifstream infile;
  TString name="./config/"+bmpedfile;
  infile.open(name.Data(),ios::in);
  Int_t file_evnum, chanum;
  Double_t tmp_double, tmp_2double;
  char tmp_char[200], dataset[200];
  Int_t tmp_int=-1, status=0;  
  if(infile.is_open() && infile.good())
    infile>>tmp_char>>dataset>>tmp_char>>file_evnum>>tmp_char>>chanum;
  else{
    cout<<"ERROR!   TABMparCon::loadADCped:: the ADCped file="<<bmpedfile<<" not found"<<endl;  
    return;
  }
  if(chanum!=mapcha){
    cout<<"ERROR! TABMparCon::loadADCped::chanum!=mapcha   chanum="<<chanum<<"  mapcha="<<mapcha<<endl;
    return;    
  }
  vector<Double_t> pedmean(chanum,-1000.);
  vector<Double_t> pedrms(chanum,-1000.);
  for(Int_t i=0;i<chanum;i++){
    if(!infile.eof() && tmp_int==i-1){
      infile>>tmp_char>>tmp_int>>tmp_char>>tmp_double>>tmp_char>>tmp_2double;
      pedmean.at(i)=tmp_double;
      pedrms.at(i)=tmp_2double;
    }else{
      cout<<"ERROR!  TABMparCon::loadADCped::Error in the ADCped file="<<bmpedfile<<"!!!!!! check if it is write properly"<<endl;  
      return;
      }
  }
  infile.close();
  adc_ped_mean=pedmean;
  adc_ped_rms=pedrms;

  return;
}


void TABMparCon::PrintADCped(TString &input_file_name, Long64_t tot_num_ev){
  ofstream outfile;
  TString name="./config/"+bmpedfile;
  outfile.open(name.Data(),ios::out);
  outfile<<"calculated_from: "<<input_file_name.Data()<<"    number_of_events= "<<tot_num_ev<<"    number_of_channel= "<<adc_ped_mean.size()<<endl;
  for(Int_t i=0;i<adc_ped_mean.size();i++)
      outfile<<"adc_chan= "<<i<<"  ped_mean= "<<adc_ped_mean[i]<<"   ped_rms= "<<adc_ped_rms[i]<<endl;
  outfile.close();
  return;
}



void TABMparCon::SetADCchanum(Int_t cha){
if(adc_ped_mean.size()>0){
    cout<<"ERROR in TABMparCon::SetADCchanum:: adc_ped_mean.size()!=0;    cha="<<cha<<"  adc_ped_mean.size()="<<adc_ped_mean.size()<<endl;
    return;  
}  
adc_ped_mean.resize(cha);  
adc_ped_rms.resize(cha);  

return;
}


void TABMparCon::SetADCped(Int_t cha, Double_t pedin, Double_t rmsin){
  if(cha>adc_ped_mean.size()-1){
    cout<<"ERROR in TABMparCon::SetADCped:: cha>adc_ped_mean.size()-1;    cha="<<cha<<"  adc_ped_mean.size()-1="<<adc_ped_mean.size()-1<<endl;
    return;
  }
  
  adc_ped_mean[cha]=pedin;
  adc_ped_rms[cha]=rmsin;
return;
}


void TABMparCon::CoutADCped(){
  cout<<"Print BM ADC pedestals and rms:"<<endl;
  for(Int_t i=0;i<adc_ped_mean.size();i++)
    cout<<"channel="<<i<<"  ped mean="<<adc_ped_mean[i]<<"   ped_rms="<<adc_ped_rms[i]<<endl;
}


//------------------------------------------+-----------------------------------
//! Clear geometry info.

void TABMparCon::Clear(Option_t*)
{
  m_isMC = false;
  rdrift_cut = 10.;
  enxcell_cut = 0.00000001;
  chi2red_cut = 5.;
  angz_cut = 5.;
  angzres_cut=5.;
  fitter_index = 0;
  bm_debug=0;
  bm_vietrack=0;
  manageT0BM=0;
  minnhit_cut=0;
  maxnhit_cut=20;
  rejmax_cut=36;
  num_ite=0;
  par_move=0.0001;
  
  vector<Double_t> myt0s(36,-10000);
  //~ myt0s.resize(36);
  v_t0s = myt0s;
  
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TABMparCon::ToStream(ostream& os, Option_t*) const
{
  os << "TABMparCon " << GetName() << endl;

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
  
  if(tdrift<0 && t0_switch==2)
    return 0.03289 + 0.008*tdrift;
  
  Double_t rdrift;
  
  if(strel_switch==1){ //garfield strel
    rdrift=0.00915267+0.00634507*tdrift+2.02527e-05*tdrift*tdrift-7.60133e-07*tdrift*tdrift*tdrift+5.55868e-09*tdrift*tdrift*tdrift*tdrift-1.68944e-11*tdrift*tdrift*tdrift*tdrift*tdrift+1.87124e-14*tdrift*tdrift*tdrift*tdrift*tdrift*tdrift;  
  }else if(strel_switch==2){//
    rdrift= 0.00972903*tdrift-8.21676e-05*tdrift*tdrift+3.66446e-07*tdrift*tdrift*tdrift-5.85882e-10*tdrift*tdrift*tdrift*tdrift;  
  }else if(strel_switch==3){//
    rdrift= 0.0087776*tdrift-6.41845e-05*tdrift*tdrift+2.4946e-07*tdrift*tdrift*tdrift-3.48422e-10*tdrift*tdrift*tdrift*tdrift;  
  }else if(strel_switch==4){//HIT 2014
    rdrift= 0.0092254*tdrift-7.1192e-5*tdrift*tdrift+3.01951e-7*tdrift*tdrift*tdrift-4.66646e-10*tdrift*tdrift*tdrift*tdrift;  
  }else if (strel_switch==5){
    rdrift= (0.032891770+0.0075746330*tdrift-(5.1692440e-05)*tdrift*tdrift+(1.8928600e-07)*tdrift*tdrift*tdrift-(2.4652420e-10)*tdrift*tdrift*tdrift*tdrift)/0.78;     
  }else if (strel_switch==6){//from strel calibration
    rdrift= -0.118715 + (0.0098028*tdrift) + (-0.000119206*tdrift*tdrift) + (8.75103e-07*tdrift*tdrift*tdrift) + (-3.16015e-09*tdrift*tdrift*tdrift*tdrift) + (4.37948e-12*tdrift*tdrift*tdrift*tdrift*tdrift);    
  }else if (strel_switch==7){//from strel calibration
    tdrift+=43.7;
    rdrift= 0.0201024 + (0.00408601*tdrift) + (-4.42738e-05*tdrift*tdrift) + (4.9932e-07*tdrift*tdrift*tdrift) + (-2.45383e-09*tdrift*tdrift*tdrift*tdrift) + (4.08383e-12*tdrift*tdrift*tdrift*tdrift*tdrift);     
  }else{
    //FIRST strel embedded in old Framework
    rdrift= 0.032891770+0.0075746330*tdrift-(5.1692440e-05)*tdrift*tdrift+(1.8928600e-07)*tdrift*tdrift*tdrift-(2.4652420e-10)*tdrift*tdrift*tdrift*tdrift;
  }
  
  return rdrift<0 ? 0.:rdrift;
  
}


Double_t TABMparCon::InverseStrel(Double_t rdrift){
  //~ if(strel_switch==5){
    TF1 f1("f1","1./0.78*(0.032891770+0.0075746330*x-(5.1692440e-05)*x*x+(1.8928600e-07)*x*x*x-(2.4652420e-10)*x*x*x*x)", 0., 320.);
    return f1.GetX(rdrift);
  //~ }else if(strel_switch==0){
    //~ TF1 f1("f1","0.032891770+0.0075746330*x-(5.1692440e-05)*x*x+(1.8928600e-07)*x*x*x-(2.4652420e-10)*x*x*x*x", 0., 320.);
    //~ return f1.GetX(rdrift);
  //~ }else if(strel_switch==4){
    //~ TF1 f1("f1","0.0092254*x-7.1192e-5*x*x+3.01951e-7*x*x*x-4.66646e-10*x*x*x*x", 0., 320.);
    //~ return f1.GetX(rdrift);
  //~ }
  
  
  return 0.;
}


Double_t TABMparCon::FirstSTrelMC(Double_t tdrift, Int_t mc_switch){
  
  if(tdrift<0 && t0_switch==2)
      return 0.03289 + 0.008*tdrift;
  
  if(mc_switch==1){ //garfield strel
    return 0.00915267+0.00634507*tdrift+2.02527e-05*tdrift*tdrift-7.60133e-07*tdrift*tdrift*tdrift+5.55868e-09*tdrift*tdrift*tdrift*tdrift-1.68944e-11*tdrift*tdrift*tdrift*tdrift*tdrift+1.87124e-14*tdrift*tdrift*tdrift*tdrift*tdrift*tdrift;  
  }else if(mc_switch==2){//
    return 0.00972903*tdrift-8.21676e-05*tdrift*tdrift+3.66446e-07*tdrift*tdrift*tdrift-5.85882e-10*tdrift*tdrift*tdrift*tdrift;  
  }else if(mc_switch==3){//
    return 0.0087776*tdrift-6.41845e-05*tdrift*tdrift+2.4946e-07*tdrift*tdrift*tdrift-3.48422e-10*tdrift*tdrift*tdrift*tdrift;  
  }else if(mc_switch==4){//HIT 2014
    return 0.0092254*tdrift-7.1192e-5*tdrift*tdrift+3.01951e-7*tdrift*tdrift*tdrift-4.66646e-10*tdrift*tdrift*tdrift*tdrift;  
  }else if (mc_switch==5)
    return (0.032891770+0.0075746330*tdrift-(5.1692440e-05)*tdrift*tdrift+(1.8928600e-07)*tdrift*tdrift*tdrift-(2.4652420e-10)*tdrift*tdrift*tdrift*tdrift)/0.78; 

  //FIRST strel embedded in old Framework
  return 0.032891770+0.0075746330*tdrift-(5.1692440e-05)*tdrift*tdrift+(1.8928600e-07)*tdrift*tdrift*tdrift-(2.4652420e-10)*tdrift*tdrift*tdrift*tdrift; 
  
}



void TABMparCon::LoadReso(TString sF) {

  TFile *f; 
  Info("Action()","Processing Reso vs R from %s  LIST file!",sF.Data());
  
  f = new TFile(sF.Data(),"READ");
  f->cd();
  my_hreso = ((TH1D*)gDirectory->Get("myHp"));
  
  return;
  
}

Double_t TABMparCon::ResoEval(Double_t dist) {
  //~ return 0.001;
  Double_t sigma;
  Int_t mybin(-1);
  if(my_hreso) {
    mybin = my_hreso->FindBin(dist);
    sigma = my_hreso->GetBinContent(mybin)/10000;
  }
  return sigma>0 ? sigma:0.12;
  
}

//~ TF1* TABMparCon::GetCalibX() {

  //~ return f_mypol;
//~ }


//~ TF1* TABMparCon::GetCalibY() {

  //~ return f_mypol2;

//~ }

//~ void TABMparCon::ConfigureTrkCalib() {

  //~ /* OLD setup
   //~ 1  p0           1.69143e-02   1.78395e-05   1.25294e-08  -1.12766e-02
   //~ 2  p1           2.21824e+01   4.37184e-03   1.05774e-05  -5.07310e-06
   //~ 3  p2           8.09415e+03   6.11105e-01   3.85959e-03   5.18272e-08
   //~ 4  p3           1.39385e+06   8.36591e+01   6.64637e-01  -3.42786e-10
   //~ 5  p4           1.13567e+08   1.04695e+04   5.41532e+01   3.66862e-13
   //~ 6  p5           3.53414e+09   9.05361e+05   1.68521e+03   3.57968e-14

   //~ 1  p0          -1.15688e-02   2.02851e-05   1.00354e-08  -2.74132e+00
   //~ 2  p1          -1.96039e+00   4.12901e-03   1.36445e-06  -5.52166e-02
   //~ 3  p2          -1.16613e+02   5.45908e-01   1.77382e-04  -1.21665e-03
   //~ 4  p3           2.46797e+04   6.95556e+01   2.22149e-02  -9.68817e-06
   //~ 5  p4           2.01676e+06   8.23400e+03   2.67413e+00  -5.28596e-08
   //~ 6  p5          -2.05344e+06   7.32900e+05   2.08980e+01  -3.10577e-11
  //~ */

  //~ //X correction
  //~ if(m_isMC) {
    //~ f_mypol->SetParameters(0.,1.,0.,0.,0.,0.);
  //~ } else {
    //~ f_mypol->SetName("mypol");
    //~ f_mypol->SetParameters( 70.61e-05,
			    //~ 2.685e-01,
			    //~ 1.203e+01,
			   //~ -2.421e+04,
			    //~ 1.112e+04,
			    //~ 4.325e+08);
  //~ }

  //~ /*
   //~ 1  p0          -1.92457e-05   9.96755e-06   2.67768e-08   2.75109e-03
   //~ 2  p1           3.65559e-01   1.37338e-02   1.88348e-05  -4.51475e-06
   //~ 3  p2          -2.01191e+01   6.94988e+00   7.23064e-03   1.17480e-08
   //~ 4  p3          -4.47220e+04   4.31045e+03   2.29598e+00   4.64828e-11
   //~ 5  p4           2.79566e+05   5.85631e+05   2.16321e+01  -1.73594e-12
   //~ 6  p5           1.90396e+09   2.74307e+08   9.07879e+02   1.93312e-15

   //~ 1  p0          -1.01338e-04   9.87766e-06   2.56656e-08   5.76352e-02
   //~ 2  p1           5.49605e-01   1.32980e-02   1.94777e-05  -4.83048e-04
   //~ 3  p2          -2.40775e+01   7.04670e+00   8.12795e-03   3.90372e-07
   //~ 4  p3          -4.67887e+04   4.61691e+03   2.69340e+00  -3.47984e-09
   //~ 5  p4           1.98095e+06   6.33007e+05   2.08960e+01   3.79568e-12
   //~ 6  p5           1.44651e+09   3.07455e+08   6.89752e+02  -2.36315e-14
  //~ */

  //~ //Y correction
  //~ if(m_isMC) {
    //~ f_mypol2->SetParameters(0.,1.,0.,0.,0.,0.);
  //~ } else {
    //~ f_mypol2->SetName("mypol2");
    //~ f_mypol2->SetParameters(-21.95e-05,
			     //~ 2.953e-01,
			     //~ 17.41e+01,
			    //~ -2.699e+04,
			    //~ -1.061e+07,
			     //~ 1.667e+09);
  //~ }
  //~ return;

//~ }

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


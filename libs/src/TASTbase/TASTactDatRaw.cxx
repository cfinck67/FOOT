/*!
  \file
  \version $Id: TASTactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TASTactDatRaw.
*/

#include "TASTparMap.hxx"

#include "WDEvent.hh"
#include "WDEvent.hh"
#include "TAGdaqEvent.hxx"
#include "TASTdatRaw.hxx"
#include "TASTparTime.hxx"
#include "TASTactDatRaw.hxx"
#include <TCanvas.h>
#include <unistd.h>
#include "TGraphErrors.h"
#include <stdint.h>

/*!
  \class TASTactDatRaw TASTactDatRaw.hxx "TASTactDatRaw.hxx"
  \brief Get Beam Monitor raw data from WD. **
*/

ClassImp(TASTactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTactDatRaw::TASTactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_datdaq,
			     TAGparaDsc* p_parmap,
			     TAGparaDsc* p_parTime)
  : TAGaction(name, "TASTactDatRaw - Unpack ST raw data"),
    fpDatRaw(p_datraw),
    fpDatDaq(p_datdaq),
    fpParMap(p_parmap),
    fpParTime(p_parTime)
{
  AddDataOut(p_datraw, "TASTdatRaw");
  AddDataIn(p_datdaq, "TAGdaqEvent");
  AddPara(p_parmap, "TASTparMap");
  AddPara(p_parTime, "TASTparTime");

  
  m_debug = false;
  m_nev=0;
}


//------------------------------------------+-----------------------------------
//! Destructor.

TASTactDatRaw::~TASTactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TASTactDatRaw::Action() {

    if(GetDebugLevel()) { cout<<" Entering the TASTactDatRaw action "<<endl; }

   TASTdatRaw*    p_datraw = (TASTdatRaw*)   fpDatRaw->Object();
   TAGdaqEvent*   p_datdaq = (TAGdaqEvent*)  fpDatDaq->Object();
   TASTparMap*    p_parmap = (TASTparMap*)   fpParMap->Object();
   TASTparTime*    p_parTime = (TASTparTime*)   fpParTime->Object();
  
   
   Int_t nFragments = p_datdaq->GetFragmentsN();


   //decoding fragment and fillin the datRaw class
   for (Int_t i = 0; i < nFragments; ++i) {
     TString type = p_datdaq->GetClassType(i);
     if (type.Contains("WDEvent")) {
       const WDEvent* evt = static_cast<const WDEvent*> (p_datdaq->GetFragment(i));
       DecodeHits(evt, p_parTime, p_datraw, p_parmap);
     }
   }
   

   double TrigTime=0, Charge=0, max_amp=0;
   vector<TASTrawHit*> myHits;
   int board_id =0, ch_num=0;
   double single_time =0;
   int nvalid=0;
   
   myHits = p_datraw->GetHits();
   for(int iHit=0;iHit<(int)myHits.size();iHit++){
     ch_num = myHits.at(iHit)->GetChannel();
     if(ComputeArrivalTime(myHits.at(iHit), &single_time, &max_amp)){
       myHits.at(iHit)->SetArrivalTime(single_time);
       myHits.at(iHit)->SetAmplitude(max_amp);
       TrigTime+=single_time;
       nvalid++;
     }
   }
   if(nvalid)TrigTime = TrigTime/(double)nvalid;
   p_datraw->SetTriggerTime(TrigTime);

   if(ValidHistogram()){
     hTrigTime->Fill(TrigTime);
     for(int iHit=0;iHit<(int)myHits.size();iHit++){
       if(ch_num>=0 && ch_num<8) hArrivalTime[ch_num]->Fill(TrigTime-myHits.at(iHit)->GetArrivalTime());
       if(ch_num>=0 && ch_num<8) hAmplitude[ch_num]->Fill(myHits.at(iHit)->GetAmplitude()); 
     }
   }
     
   //evaluate the charge of the single channels
   double single_q=0, q=0;
   myHits = p_datraw->GetHits();
   for(int iHit=0; iHit<(int)myHits.size();iHit++){
     ch_num = myHits.at(iHit)->GetChannel();

     if(p_parmap->IsSTChannel(ch_num)){
       single_q = ComputeCharge(myHits.at(iHit));
       myHits.at(iHit)->SetCharge(single_q);
       if(ValidHistogram()){
   	 if(ch_num>=0 && ch_num<8)hCharge[ch_num]->Fill(single_q); 
       }
       q+= single_q;
     }
   }
   p_datraw->SetCharge(q);
   if(ValidHistogram())hTotCharge->Fill(q); 

   m_nev++;
   
   fpDatRaw->SetBit(kValid);
   
  return kTRUE;
}

//------------------------------------------+-----------------------------------
//! Decoding

Bool_t TASTactDatRaw::DecodeHits(const WDEvent* evt, TASTparTime *p_parTime, TASTdatRaw *p_datraw, TASTparMap *p_parMap)
{
  

  u_int word;
  
  int iW=0;
  int board_id=0, ch_num=0;
  float time_bin=0;
  float temperature=0, range=0, sampling_freq;
  int flags=0;
  int trig_cell=0;
  int fe_settings;
  int adc_sa=0;
  double v_sa=0;
  double tmp_tcal;
  double tmo_signal;
  int bco_counter, trig_type, ser_evt_number;
  vector<double> w_time;
  vector<double> w_amp;
  vector<float> w_tcal;


  if(GetDebugLevel()) { 
    /*
    printf("%08x     valuessize::%08x\n", evt->evtSize,  evt->values.size());
    for (Int_t i = 0; i < evt->evtSize-1; ++i) {
      if(i<30)printf("first::%08x\n",  evt->values[i]);
      if(i>evt->evtSize-5)printf("last::%08x\n",  evt->values[i]);
    }
    
    printf("last %08x\n", evt->values.at(evt->values.size()-2));
    printf("last %08x\n", evt->values.at(evt->values.size()-1));
    printf("\n");
    */
  }
  

 
  iW=0;
  bool foundFooter = false;
  while(iW < evt->evtSize && !foundFooter){

    if(evt->values.at(iW) == GLB_EVT_HEADER){
      if(m_debug)printf("found glb header::%08x %08x\n", evt->values.at(iW), evt->values.at(iW+1));
      iW+=6; //
      
      //found evt_header
      if(evt->values.at(iW) == EVT_HEADER){
	if(m_debug)printf("found evt header::%08x   %08x   %08x\n", evt->values.at(iW),evt->values.at(iW+1),evt->values.at(iW+2));
      
	iW++;
	trig_type = evt->values.at(iW) & 0xffff;
	ser_evt_number = (evt->values.at(iW) >> 16) & 0xffff;
      
	iW++;
	bco_counter = (int)evt->values.at(iW);
      
	iW++;
	while((evt->values.at(iW) & 0xffff)== BOARD_HEADER){
	  board_id = (evt->values.at(iW)>>16)  & 0xffff;
	  if(m_debug)printf("found board header::%08x num%d\n", evt->values.at(iW), board_id);
	  iW++;
	  temperature = *((float*)&evt->values.at(iW));
	  if(m_debug)printf("temperatrue::%08x num%d\n", evt->values.at(iW), board_id);

	
	  iW++;
	  range = *((float*)&evt->values.at(iW));
	
	  if(m_debug)printf("range::%08x num%d\n", evt->values.at(iW), board_id);
	
	  iW++;
	  sampling_freq = (float)(( evt->values.at(iW)>> 16)&0xffff);
	  flags = evt->values.at(iW) & 0xffff;
	  if(m_debug)printf("sampling::%08x    %08x   %08x    num%d\n", evt->values.at(iW),evt->values.at(iW+1),evt->values.at(iW+2), board_id);
	
	  iW++;
	
	  while((evt->values.at(iW) & 0xffff)== CH_HEADER){

	    char tmp_chstr[3]={'0','0','\0'};
	    tmp_chstr[1] = (evt->values.at(iW)>>24)  & 0xff;
	    tmp_chstr[0] = (evt->values.at(iW)>>16)  & 0xff;
	    ch_num = atoi(tmp_chstr);
	    if(m_debug)printf("found channel header::%08x num%d\n", evt->values.at(iW), ch_num);
	  
	    iW++;
	    trig_cell = (evt->values.at(iW)>>16) &0xffff;
	  
	    fe_settings = ((evt->values.at(iW))&0xffff);
	    iW++;
	    
	    int adctmp=0;
	    int delta=0,deltaold=0;
	    bool jump_up=false;
	    vector<double> w_adc;
	    w_amp.clear();
	    
	    for(int iSa=0;iSa<512;iSa++){
	      // if(m_debug)printf("found sample isa::%d    ::%08x\n", iSa, word);
	      adc_sa = evt->values.at(iW);
	      adctmp = ((adc_sa >> 16) & 0xffff);
	      w_adc.push_back(adctmp);
	      adctmp  = (adc_sa & 0xffff);
	      w_adc.push_back(adctmp);
	      iW++;
	    }

	    int adcold=w_adc.at(0);
	    for(int iSa=0;iSa<w_adc.size();iSa++){
	      if(w_adc.at(iSa)-adcold>30000){
		w_adc.at(iSa) -= 65536;
	      }
	      adcold = w_adc.at(iSa);
	    }
	    	    
	    for(int iSa=0;iSa<w_adc.size();iSa++){
	      v_sa = w_adc.at(iSa)/65536.+range;
	      w_amp.push_back(v_sa);
	    }
	    	    
	    if(p_parMap->IsSTChannel(ch_num) && p_parMap->IsSTBoard(board_id)){
	      p_parTime->GetTimeArray(board_id, ch_num, trig_cell, &w_time);
	      p_datraw->AddWaveform(ch_num, w_time ,w_amp);
	    }
	    w_amp.clear();
	    w_time.clear();
	  
	  }
	}
      }
      
      if(evt->values.at(iW) == EVT_FOOTER){
	if(m_debug)printf("found footer\n");
	iW++;
	foundFooter = true;
      }else{
	printf("warining:: footer not found, event corrupted, iW::%d   last word::%08x!!\n",iW, evt->values.at(iW));
	break;
      }  
    }
  }


  
  return true;
}




double TASTactDatRaw::ComputeMaxAmplitude(TASTrawHit*myHit){


  vector<double> currAmpArray, currTimeArray;

  double window=50; //integration time window width
  double time_amin=0;
  int bin_amin=0;
  double tleft=0, tright=0;

  double max_amp=0;
  currAmpArray = myHit->GetAmplitudeArray();
  bin_amin = std::distance(currAmpArray.begin(), std::min_element(currAmpArray.begin(),currAmpArray.end()));
  
  return -currAmpArray.at(bin_amin);

}


void TASTactDatRaw::SavePlot(TGraph WaveGraph, TF1 fun1, TF1 fun2, TASTrawHit *myHit){


  TCanvas c("c","",600,600);
  c.cd();

  WaveGraph.Draw("APL");
  WaveGraph.SetMarkerSize(0.5);
  WaveGraph.SetMarkerStyle(22);
  WaveGraph.SetMarkerColor(kBlue);
  WaveGraph.GetXaxis()->SetRangeUser(0,100);

  fun1.SetLineColor(kRed);
  fun2.SetLineColor(kGreen);

  fun1.SetNpx(10000);
  fun2.SetNpx(10000);
  
  fun1.Draw("same");
  fun2.Draw("same");

  c.Print(Form("waveform_ch%d_nev%d.png", myHit->GetChannel(), m_nev));
  

}



bool TASTactDatRaw::ComputeArrivalTime(TASTrawHit*myHit, double *tarr, double *ampl){

  vector<double> tmp_amp = myHit->GetAmplitudeArray();
  tmp_amp.erase(tmp_amp.begin(),tmp_amp.begin()+2);
  vector<double> tmp_time = myHit->GetTimeArray();
  tmp_time.erase(tmp_time.begin(),tmp_time.begin()+2);
  vector<double> tmp_unc;
  tmp_unc.assign(1022,0.01);
  
  int min_bin = std::distance(tmp_amp.begin(), std::min_element(tmp_amp.begin(), tmp_amp.end()));
  int max_bin = std::distance(tmp_amp.begin(), std::max_element(tmp_amp.begin(), tmp_amp.end()));
  int cross_bin=min_bin;
  
  double time_crossbin = tmp_time.at(cross_bin);
  double time_binmin = tmp_time.at(min_bin);
  double time_binmax = tmp_time.at(max_bin);

  double amp_crossbin = tmp_amp.at(cross_bin);
  double amp_binmin = tmp_amp.at(min_bin);
  double amp_binmax = tmp_amp.at(max_bin);


  double tleft= time_crossbin-8;
  double tright= time_crossbin+4;

  //to be commented...
  TF1 f("f", "-[0]/(1+TMath::Exp(-(x-[1])/[2]))/(1+TMath::Exp((x-[3])/[4]))+[5]",tleft,tright);
  f.SetParameter(0,1);
  f.SetParameter(1,time_crossbin);
  f.SetParameter(2,1);
  f.SetParameter(3,time_crossbin+2);
  f.SetParameter(4,2);
  f.SetParameter(5,0.05);
  
  TGraphErrors WaveGraph(tmp_time.size(), &tmp_time[0], &tmp_amp[0], 0, &tmp_unc[0]);
  WaveGraph.Fit("f","Q", "",tleft, tright);

  TF1 f1("f1", "-0.2*[0]/(1+TMath::Exp(-(x-[1])/[2]))/(1+TMath::Exp((x-[3])/[4]))+[0]/(1+TMath::Exp(-(x-[1]-1.5)/[2]))/(1+TMath::Exp((x-[3]-1.5)/[4]))",0,50);
  f1.SetLineColor(kGreen);
  f1.FixParameter(0, f.GetParameter(0));
  f1.FixParameter(1, f.GetParameter(1));
  f1.FixParameter(2, f.GetParameter(2));
  f1.FixParameter(3, f.GetParameter(3));
  f1.FixParameter(4, f.GetParameter(4));
  f1.FixParameter(5, f.GetParameter(5));

  if(m_debug)SavePlot(WaveGraph, f,f1, myHit);
  
  tleft = f1.GetMinimumX();
  tright = f1.GetMaximumX();
  
  double t1=f1.GetX(0.0,tleft,tright);
  
  *tarr = t1;
  *ampl = -f.GetMinimum();
  
  if(t1>tleft && t1<tright){
    return true;
  }else{
    return false;
 }
}

double TASTactDatRaw::ComputeCharge(TASTrawHit *currHit){


  double dt = 0, amp;
  double q=0;
  const double mulf = 1/50.; //1/50 Ohm. When applied, you get the charge in nC 


  vector<double> currAmpArray, currTimeArray;

  double window=50; //integration time window width
  double time_amin=0;
  int bin_amin=0;
  double tleft=0, tright=0;
  
  currAmpArray = currHit->GetAmplitudeArray();
  currTimeArray = currHit->GetTimeArray();
  
  bin_amin = std::distance(currAmpArray.begin(), std::min_element(currAmpArray.begin(),currAmpArray.end()));
  time_amin = currAmpArray.at(bin_amin);
  tleft = time_amin-5;
  tright = time_amin+window-5; //this window has to be tuned
  
  
  for(int iSa=0;iSa<currAmpArray.size()-1;iSa++){
    dt = currTimeArray.at(iSa+1) - currTimeArray.at(iSa);
    amp = -currAmpArray.at(iSa); //the signal amplitude is negative
    if(currTimeArray.at(iSa)>tleft && currTimeArray.at(iSa)< tright)q+=amp*dt*mulf;
  }
    
  return q;

}


void TASTactDatRaw::CreateHistogram(){

  DeleteHistogram();

  char histoname[100]="";
  cout<<"I have created the ST histo. "<<endl;
  sprintf(histoname,"stTrigTime");
  hTrigTime = new TH1F(histoname, histoname, 256, 0., 256.);
  AddHistogram(hTrigTime);

  sprintf(histoname,"stTotCharge");
  hTotCharge = new TH1F(histoname, histoname, 100, 0., 5.);
  AddHistogram(hTotCharge);
  
  for(int iCh=0;iCh<8;iCh++){
    sprintf(histoname,"stDeltaTime_ch%d", iCh);
    hArrivalTime[iCh]= new TH1F(histoname, histoname, 100, -5., 5.);
    AddHistogram(hArrivalTime[iCh]);

    sprintf(histoname,"stCharge_ch%d", iCh);
    hCharge[iCh]= new TH1F(histoname, histoname, 100, 0., 5.);
    AddHistogram(hCharge[iCh]);

    sprintf(histoname,"stMaxAmp_ch%d", iCh);
    hAmplitude[iCh]= new TH1F(histoname, histoname, 120, -0.1, 1.1);
    AddHistogram(hAmplitude[iCh]);
  }

  SetValidHistogram(kTRUE);
  
}

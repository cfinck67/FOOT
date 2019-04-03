/*!
  \file
  \version $Id: TASTactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TASTactDatRaw.
*/

#include "TASTparMap.hxx"

#include "WDEvent.hh"
#include "TAGdaqEvent.hxx"
#include "TASTdatRaw.hxx"
#include "TASTparTime.hxx"
#include "TASTactDatRaw.hxx"
#include <TCanvas.h>


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

   TASTdatRaw*    p_datraw = (TASTdatRaw*)   fpDatRaw->Object();
   TAGdaqEvent*   p_datdaq = (TAGdaqEvent*)  fpDatDaq->Object();
   TASTparMap*    p_parmap = (TASTparMap*)   fpParMap->Object();
   TASTparTime*    p_parTime = (TASTparTime*)   fpParTime->Object();
  
   
   Int_t nFragments = p_datdaq->GetFragmentsN();

   for (Int_t i = 0; i < nFragments; ++i) {
     TString type = p_datdaq->GetClassType(i);
     if (type.Contains("WDEvent")) {
       const WDEvent* evt = static_cast<const WDEvent*> (p_datdaq->GetFragment(i));
       DecodeHits(evt, p_parTime, p_datraw);
     }
   }
   

   double TrigTime=0, Charge=0;

   //evaluate the trigger time
   p_datraw->SumWaveforms();
   TrigTime = ComputeArrivalTime(p_datraw->GetWaveCFD());
   p_datraw->SetTriggerTime(TrigTime);
   if(ValidHistogram())hTrigTime->Fill(TrigTime); //mettere check valid histo
   //evaluate the arrival time of the single channels

   vector<TASTrawHit*> myHits;
   int board_id =0, ch_num=0;
 
   
   double single_time =0;
   myHits = p_datraw->GetHitsCFD();
   for(int iHit=0;iHit<(int)myHits.size();iHit++){
     single_time = ComputeArrivalTime(myHits.at(iHit));
     //  printf("trigtime::%lf    singletime::%lf\n", TrigTime, single_time);
     ch_num = myHits.at(iHit)->GetChannel();
     myHits.at(iHit)->SetArrivalTime(single_time);
     if(ValidHistogram()){
       if(ch_num>=0 && ch_num<8) hArrivalTime[ch_num]->Fill(TrigTime-single_time); //mettere check valid histo
     }
   }

   
   //evaluate the charge of the single channels
   double single_q=0, q=0,max_amp=0;
   myHits = p_datraw->GetHits();
   for(int iHit=0; iHit<(int)myHits.size();iHit++){
     ch_num = myHits.at(iHit)->GetChannel();

     if(p_parmap->IsSTChannel(ch_num)){
       single_q = ComputeCharge(myHits.at(iHit));
       myHits.at(iHit)->SetCharge(single_q);
       if(ValidHistogram()){
	 if(ch_num>=0 && ch_num<8)hCharge[ch_num]->Fill(single_q); //mettere check valid histo
       }
       q+= single_q;

       
       max_amp = ComputeMaxAmplitude(myHits.at(iHit));
       if(ValidHistogram()){
	 if(ch_num>=0 && ch_num<8) hAmplitude[ch_num]->Fill(max_amp); //mettere check valid histo
       }
     }
   }
   p_datraw->SetCharge(q);
   if(ValidHistogram())hTotCharge->Fill(q); //mettere check valid histo

   m_nev++;
   
   fpDatRaw->SetBit(kValid);
   
  return kTRUE;
}

//------------------------------------------+-----------------------------------
//! Decoding

Bool_t TASTactDatRaw::DecodeHits(const WDEvent* evt, TASTparTime *p_parTime, TASTdatRaw *p_datraw)
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
  
  // printf("%08x     valuessize::%08x\n", evt->evtSize,  evt->values.size());
  // for (Int_t i = 0; i < evt->evtSize-1; ++i) {

  //   if(i<10)printf("first::%08x\n",  evt->values[i]);
  //   if(i>evt->evtSize-5)printf("last::%08x\n",  evt->values[i]);
  // }

  // printf("last %08x\n", evt->values.at(evt->values.size()-2));
  // printf("last %08x\n", evt->values.at(evt->values.size()-1));
  // printf("\n");


  
  

  
  iW=0;
  bool foundFooter = false;
  while(iW < evt->evtSize && !foundFooter){

    if(evt->values.at(iW) == GLB_EVT_HEADER){
      if(m_debug)printf("found glb header::%08x %08x\n", evt->values.at(iW), evt->values.at(iW+1));
      iW+=3;
    }

    //found time header
    if(evt->values.at(iW) == FILE_HEADER && evt->values.at(iW+1) == TIME_HEADER){

      if(m_debug)printf("found time calibration header::%08x %08x\n", evt->values.at(iW), evt->values.at(iW+1));
      iW+=2;
      
      while((evt->values.at(iW) & 0xffff)== BOARD_HEADER){

  	board_id = (evt->values.at(iW)>>16)  & 0xffff;
  	if(m_debug)printf("found board header::%08x num%d\n", evt->values.at(iW), board_id);
  	iW++;
	
  	while((evt->values.at(iW) & 0xffff)== CH_HEADER){
	  char tmp_chstr[2]={'0','0'};
	  tmp_chstr[1] = (evt->values.at(iW)>>24)  & 0xff;
	  tmp_chstr[0] = (evt->values.at(iW)>>16)  & 0xff;
	  ch_num = atoi(tmp_chstr);
  	  if(m_debug)printf("found channel header::%08x num%d\n", evt->values.at(iW), ch_num);
  	  iW++;
	  
  	  for(int iCal=0;iCal<1024;iCal++){
	    time_bin = *((float*)&evt->values.at(iW));
	    p_parTime->SetTimeCal(board_id, ch_num, iCal,(double)time_bin);
  	    iW++;
  	  }
  	}
      }
    }
  
  
  
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

	  char tmp_chstr[2]={'0','0'};
	  tmp_chstr[1] = (evt->values.at(iW)>>24)  & 0xff;
	  tmp_chstr[0] = (evt->values.at(iW)>>16)  & 0xff;
	  ch_num = atoi(tmp_chstr);
	  if(m_debug)printf("found channel header::%08x num%d\n", evt->values.at(iW), ch_num);
	  
	  //	  if(p_parMap->GetTDID

	  
	  iW++;
	  trig_cell = (evt->values.at(iW)>>16) &0xffff;
	  
	  fe_settings = ((evt->values.at(iW))&0xffff);
	  iW++;

	  w_amp.clear();
	  for(int iSa=0;iSa<512;iSa++){
	    if(m_debug)printf("found sample isa::%d    ::%08x\n", iSa, evt->values.at(iW));
	    adc_sa = evt->values.at(iW);
	    v_sa = ((adc_sa >> 16) & 0xffff)/65536.-0.5+range;
	    w_amp.push_back(v_sa);
	    v_sa = (adc_sa & 0xffff)/65536.-0.5+range;
	    w_amp.push_back(v_sa);
	    iW++;
	  }
	  
	  if(board_id == 27){
	    p_parTime->GetTimeArray(board_id, ch_num, trig_cell, &w_time);
	    p_datraw->AddWaveform(ch_num, w_time ,w_amp);
	  }
	  w_amp.clear();
	  w_time.clear();
	  
	}
      }
    }


    
    if(evt->values.at(iW) == EVT_FOOTER){
      //      printf("found footer\n");
      iW++;
      foundFooter = true;
    }else{
      printf("warining:: footer not found, event corrupted, iW::%d   last word::%08x!!\n",iW, evt->values.at(iW));
      break;
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




double TASTactDatRaw::ComputeArrivalTime(TASTrawHit*myHit){

  
  vector<double> tmp_amp = myHit->GetAmplitudeArray();
  vector<double> tmp_time = myHit->GetTimeArray();
  
  int min_bin = std::distance(tmp_amp.begin(), std::min_element(tmp_amp.begin(), tmp_amp.end()));
  int max_bin = std::distance(tmp_amp.begin(), std::max_element(tmp_amp.begin(), tmp_amp.end()));
  int cross_bin=min_bin;
  
  while(tmp_amp.at(cross_bin) <0 && cross_bin<tmp_amp.size()){
    cross_bin++;
  }
  
  double time_crossbin = tmp_time.at(cross_bin);
  double time_binmin = tmp_time.at(min_bin);
  double time_binmax = tmp_time.at(max_bin);

  double amp_crossbin = tmp_amp.at(cross_bin);
  double amp_binmin = tmp_amp.at(min_bin);
  double amp_binmax = tmp_amp.at(max_bin);

  int bin_fit_l= min_bin;
  int bin_fit_r= max_bin;
  while(tmp_amp.at(bin_fit_l) < amp_binmin*0.8 && bin_fit_l<tmp_amp.size() && bin_fit_l>=0){
    bin_fit_l++;
  }
  while(tmp_amp.at(bin_fit_r) > amp_binmax*0.5 && bin_fit_r<tmp_amp.size() && bin_fit_r>=0){
    bin_fit_r--;
  }

  
  double tleft= tmp_time.at(bin_fit_l);
  double tright= tmp_time.at(bin_fit_r);
  
  // TCanvas c("c","",600,600);
  // c.cd();
  TGraph WaveGraph(tmp_time.size(), &tmp_time[0], &tmp_amp[0]);
  //WaveGraph.Draw("APL");
  WaveGraph.SetMarkerSize(0.5);
  WaveGraph.SetMarkerStyle(22);
  WaveGraph.SetMarkerColor(kBlue);
  WaveGraph.GetXaxis()->SetRangeUser(20,40);
  WaveGraph.Fit("pol1","Q", "",tleft, tright);
  //  c.Print(Form("waveform_ch%d_nev%d.png", myHit->GetChannel(), m_nev));
  
  TF1 *fitfun =((TF1*) WaveGraph.GetFunction("pol1")); 
  
  double q = fitfun->GetParameter(0);
  double m = fitfun->GetParameter(1);
  double zeroTime = -q/m;
  
  return zeroTime;

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

  //  DeleteHistogram();

  char histoname[100]="";

  sprintf(histoname,"TrigTime");
  hTrigTime = new TH1D(histoname, histoname, 2560, 0., 256.);
  AddHistogram(hTrigTime);

  sprintf(histoname,"TotCharge");
  hTotCharge = new TH1D(histoname, histoname, 100, 0., 5.);
  AddHistogram(hTotCharge);
  
  for(int iCh=0;iCh<8;iCh++){
    sprintf(histoname,"DeltaTime_ch%d", iCh);
    hArrivalTime[iCh]= new TH1D(histoname, histoname, 100, -5., 5.);
    AddHistogram(hArrivalTime[iCh]);

    sprintf(histoname,"Charge_ch%d", iCh);
    hCharge[iCh]= new TH1D(histoname, histoname, 100, 0., 5.);
    AddHistogram(hCharge[iCh]);

    sprintf(histoname,"MaxAmp_ch%d", iCh);
    hAmplitude[iCh]= new TH1D(histoname, histoname, 120, -0.1, 1.1);
    AddHistogram(hAmplitude[iCh]);
  }

  SetValidHistogram();
  
}

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


  p_datraw->SumWaveforms();
  double TrigTime =   GetTriggerTime(p_datraw);
  cout << "trigtime::" << TrigTime << endl;
  
   return true;
}




double TASTactDatRaw::GetTriggerTime(TASTdatRaw *p_datraw){

  TASTrawHit* myCFDHit = p_datraw->GetWaveCFD();
  
  vector<double> tmp_amp = myCFDHit->GetAmplitudeArray();
  vector<double> tmp_time = myCFDHit->GetTimeArray();
  
  int min_bin = std::distance(tmp_amp.begin(), std::min_element(tmp_amp.begin(), tmp_amp.end()));
  int cross_bin=min_bin;

  while(tmp_amp.at(cross_bin) < 0 && cross_bin<tmp_amp.size()){
    cross_bin++;
  }

  for(int i=0;i<1024;i++){
    cout <<"time::" << tmp_time.at(i) << "   amp::" << tmp_amp.at(i) << endl;
  }
  
  double time_crossbin = tmp_amp.at(cross_bin);

  
  TGraph WaveGraph(tmp_time.size(), &tmp_time[0], &tmp_amp[0]);
  WaveGraph.Fit("pol1","", "",time_crossbin-0.2, time_crossbin+0.8);

  TF1 *fitfun =((TF1*) WaveGraph.GetFunction("pol1")); 
  
  double q = fitfun->GetParameter(1);
  double m = fitfun->GetParameter(0);
    
  double triggerTime = -q/m;
    
  return triggerTime;

  
}

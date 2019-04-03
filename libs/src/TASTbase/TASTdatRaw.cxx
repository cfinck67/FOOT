/*!
  \file
  \version $Id: TASTdatRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TASTdatRaw.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TASTdatRaw.hxx"
#include "TF1.h"



/*!
  \class TASTdatRaw TASTdatRaw.hxx "TASTdatRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TASTrawHit);

//------------------------------------------+-----------------------------------
//! Destructor.

TASTrawHit::~TASTrawHit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTrawHit::TASTrawHit(int ch_num, vector<double> time, vector<double> amplitude){
  m_ch_num = ch_num;
  m_time = time;
  m_amplitude = amplitude;
  m_tarr = -10000;
  m_charge = -10000;
  
}





void TASTrawHit::Clear(Option_t* op/*option*/)
{
  m_board_id = -100;
  m_ch_num = -100;
  m_tarr = -10000;
  m_charge = -10000;
  m_time.clear();
  m_amplitude.clear();
}

//##############################################################################



ClassImp(TASTdatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTdatRaw::TASTdatRaw(): TAGdata(),fListOfWaveforms(0x0), fListOfWaveforms_cfd(0x0) {

  fdTrgTime = -1000000000000.;
    

}



//------------------------------------------+-----------------------------------
//! Destructor.

TASTdatRaw::~TASTdatRaw(){

   for(int i=0;i<fListOfWaveforms.size();i++){
    delete fListOfWaveforms.at(i);
    delete fListOfWaveforms_cfd.at(i);
  }

   delete fSumWaves;
   delete fSumWaves_cfd;
   
   
}


void TASTdatRaw::AddWaveform(int ch_num, vector<double> time, vector<double> amplitude){

  fListOfWaveforms.push_back(new TASTrawHit(ch_num, time ,amplitude));


  vector<double> amplitude_cfd;
  amplitude_cfd.assign(amplitude.size(), 0);
  
  for(int i=0;i<1024;i++){
    if(i>3 && i<1021){
      amplitude_cfd.at(i)+=(0.5*amplitude.at(i)-amplitude.at(i-3));
    }else{
      amplitude_cfd.at(i)=0;
    }
  }

  fListOfWaveforms_cfd.push_back(new TASTrawHit(ch_num, time ,amplitude_cfd));


}


void TASTdatRaw::SumWaveforms(){

  //fSumWaves

  vector<double> tmp_time, tmp_amp, tmp_amp_cfd;
  tmp_time.assign(1024,0);
  tmp_amp.assign(1024,0);
  tmp_amp_cfd.assign(1024,0);
  
  for(int iWa=0;iWa<fListOfWaveforms.size()-1;iWa++){
    for(int i=0;i<1024;i++){
      tmp_amp.at(i)+=(fListOfWaveforms.at(iWa)->GetAmplitudeArray()).at(i);
      tmp_time.at(i)=i*256/1024.; //to change
    }
  }

 
  for(int i=0;i<1024;i++){
    if(i>3 && i<1021){
      tmp_amp_cfd.at(i)+=(0.5*tmp_amp.at(i)-tmp_amp.at(i-3));
    }else{
      tmp_amp_cfd.at(i)=0;
    }
  }

  fSumWaves = new TASTrawHit(WAVE_ID,tmp_time, tmp_amp);
  fSumWaves_cfd = new TASTrawHit(WAVE_CFD_ID,tmp_time, tmp_amp_cfd);
  
}

  


//------------------------------------------+-----------------------------------
//! Clear event.

void TASTdatRaw::Clear(Option_t*)
{
  TAGdata::Clear();

  fListOfWaveforms.clear();
  fListOfWaveforms_cfd.clear();

  
  return;
}


/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TASTdatRaw::ToStream(ostream& os, Option_t* option) const
{
  return;
}



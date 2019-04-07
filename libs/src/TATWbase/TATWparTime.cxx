/*!
  \file
  \version $Id: TATWparTime.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
  \brief   Implementation of TATWparTime.
*/

#include <string.h>

#include <fstream>

#include "TSystem.h"
#include "TString.h"

#include "TATWparTime.hxx"

//##############################################################################

/*!
  \class TATWparTime TATWparTime.hxx "TATWparTime.hxx"
  \brief Map parameters for onion and daisy. **
*/

ClassImp(TATWparTime);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATWparTime::TATWparTime() {

  //  m_GotCalib=false;
  //to initialize

  int key=0;
  
  for(int iBo=0;iBo<100;iBo++){
    for(int iCh=0;iCh<18;iCh++){
      key = iBo+iCh*100;
      //AS      m_GotCalib[key] = false;
      m_GotCalib[key] = true;
      for(int i=0;i<1024;i++){
	//AS time_parcal[key].push_back(0);
	time_parcal[key].push_back(1.e-9*256./1024);
     }
    }
  }





}

//------------------------------------------+-----------------------------------
//! Destructor.

TATWparTime::~TATWparTime()
{}



 
//------------------------------------------+-----------------------------------
//! Clear event.

void TATWparTime::Clear(Option_t*){
  time_parcal.clear();
  return;
}


void TATWparTime::ToStream(ostream& os, Option_t*) const
{
  
}

//set time calibration (get from first event)
void TATWparTime::SetTimeCal(int iBo, int iCha, vector<float> tvec){

  int key = iBo+100*iCha;
  for(int i=0;i<tvec.size();i++){
    time_parcal[key].at(i) = (double)tvec.at(i)*sec2Nano;
  }
  m_GotCalib[key] = true;
  
  
  return;
}

//get time array
bool TATWparTime::GetTimeArray(int iBo, int iCha, int TrigCell, vector<double> *timecal){

 
int key = iBo+iCha*100;
    
  if(!m_GotCalib.find(key)->second){
    printf("calibration not found for channel %d board %d!!!\n", iCha, iBo);
    return false;
  }
  
  
  double t=0;
  vector<double> tmp_calib = time_parcal.find(key)->second;
  
  //the sampling time is retreived summing the time bin starting from the trigger cell
  for(int i=0;i<1024;i++){
    timecal->push_back(t);
    t+=tmp_calib.at((i+TrigCell)%1024);
  }
  
  
  return true;;
}

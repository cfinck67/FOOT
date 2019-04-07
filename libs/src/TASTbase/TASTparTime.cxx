/*!
  \file
  \version $Id: TASTparTime.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
  \brief   Implementation of TASTparTime.
*/

#include <string.h>

#include <fstream>

#include "TSystem.h"
#include "TString.h"

#include "TASTparTime.hxx"

//##############################################################################

/*!
  \class TASTparTime TASTparTime.hxx "TASTparTime.hxx"
  \brief Map parameters for onion and daisy. **
*/

ClassImp(TASTparTime);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTparTime::TASTparTime() {


  int key=0;
  for(int iBo=0;iBo<NMAX_BO_ID;iBo++){
    for(int iCh=0;iCh<NMAX_CH_ID;iCh++){
      key = iBo+iCh*100;
      //AS      m_GotCalib[key] = false;
      m_GotCalib[key] = true;
      for(int i=0;i<1024;i++){
	//AS	time_parcal[key].push_back(0);
	time_parcal[key].push_back(1.e-9*256./1024);
      }
    }
  }





}

//------------------------------------------+-----------------------------------
//! Destructor.

TASTparTime::~TASTparTime(){}



 
//------------------------------------------+-----------------------------------
//! Clear event.

void TASTparTime::Clear(Option_t*){
  time_parcal.clear();
  return;
}


void TASTparTime::ToStream(ostream& os, Option_t*) const
{
  
}

//set time calibration (get from first event)
void TASTparTime::SetTimeCal(int iBo, int iCha,  vector<float> tvec){
  
  int key = iBo+100*iCha;
  for(int i=0;i<tvec.size();i++){
    time_parcal[key].at(i) = (double)tvec.at(i)*sec2Nano;
  }
  m_GotCalib[key] = true;
  
  return;
}



//get time array
bool TASTparTime::GetTimeArray(int iBo, int iCha, int TrigCell, vector<double> *timecal){

  
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
  

  
  return true;
}

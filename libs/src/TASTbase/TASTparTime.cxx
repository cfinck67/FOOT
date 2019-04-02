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

  //  m_GotCalib=false;
  //to initialize

  int key=0;
  
  for(int iBo=0;iBo<100;iBo++){
    for(int iCh=0;iCh<18;iCh++){
      key = iBo+iCh*100;
      m_GotCalib[key] = false;
      for(int i=0;i<1024;i++){
	time_parcal[key].push_back(0);
      }
    }
  }





}

//------------------------------------------+-----------------------------------
//! Destructor.

TASTparTime::~TASTparTime()
{}



 
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
void TASTparTime::SetTimeCal(int iBo, int iCha, int iSa, double tbin){

  int key = iBo+100*iCha;
  time_parcal[key].at(iSa) = tbin*sec2Nano;
  m_GotCalib[key] = true;
  
  
  
  return;
}

//get time array
void TASTparTime::GetTimeArray(int iBo, int iCha, int TrigCell, vector<double> *timecal){

  int key = iBo+iCha*100;
  double t=0;

  vector<double> tmp_calib = time_parcal.find(key)->second;

  if(m_GotCalib.find(key)->second){
  
  //the sampling time is retreived summing the time bin starting from the trigger cell
  for(int i=0;i<1024;i++){
    timecal->push_back(t);
    t+=tmp_calib.at((i+TrigCell)%1024);
  }
  }else{
    printf("calibration not found for board %d   channel %d\n", iBo, iCha);
  }

  
  return;
}

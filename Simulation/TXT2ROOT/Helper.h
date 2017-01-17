#ifndef _Helper_
#define _Helper_


#include <vector>
#include <iomanip>
#include <iostream>


#include "TObject.h"


#include "../foot_geo.h"

using namespace std; 


const static Int_t  gMIMONb=8;
const static Int_t  gQMAX=6;
const static Int_t  gCLUSIZEMAX=15;
const static Int_t  gCLUSIZEBIN=10;

class Helper {

public:

  Helper();
  virtual ~Helper();
  
  void PressEnter();
  
  //convert from FLUKA particle type and charge to scoring charge (Qeff=0: channel for other particles)
  virtual Int_t Qeff(Int_t FLUKAType,Int_t Q);
  
//convert Double_t to Int_t without crashing - hell knows why (deadly FORTRAN-C++ mix?).
  virtual Int_t castd2i(Double_t number);

  //setters//
//  Int_t Clean();

  //getters//
//  Int_t GetCurrentEventNb() const {return fEventCur;};
  
  //output//
//  Int_t PrintEvaluate();
//  Int_t Dump();


private:

private:
  
//  Int_t Evaluate(Int_t Verbosity);
  
  
   
  
protected:
  ClassDef(Helper,1);

};


#endif


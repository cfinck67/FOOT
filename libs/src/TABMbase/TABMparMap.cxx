/*!
  \file
  \version $Id: TABMparMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
  \brief   Implementation of TABMparMap.
*/

#include <string.h>

#include <fstream>

#include "TSystem.h"
#include "TString.h"

#include "TABMparMap.hxx"

//##############################################################################

/*!
  \class TABMparMap TABMparMap.hxx "TABMparMap.hxx"
  \brief Map and Geometry parameters for Tof wall. **
*/

ClassImp(TABMparMap);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMparMap::TABMparMap() {

  vector<int> tchaID;             tchaID.clear();   
  vector<int> tchaBoID;           tchaBoID.clear();   
  vector<int> tchaPlane;	  tchaPlane.clear();
  vector<int> tchaView;	          tchaView.clear(); 
  vector<int> tchaSense;	  tchaSense.clear();

  chaID =    tchaID;   
  chaBoID =  tchaBoID;   
  chaPlane = tchaPlane;
  chaView =  tchaView; 
  chaSense = tchaSense;
  
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMparMap::~TABMparMap()
{}


//------------------------------------------+-----------------------------------
//! Read mapping data from file \a name .

Bool_t TABMparMap::FromFile(const TString& name) {

  Clear();
  
  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);


  char bufConf[1024];
  int myArg1(0); 
  int myArg2(0); int myArg3(0); 
  int myArg4(0); int myArg5(0); 

  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'#')) {
      sscanf(bufConf, "#%d %d %d %d %d",&myArg1,&myArg2,&myArg3,&myArg4,&myArg5);
      if((myArg1>-1 && myArg1<128) && (myArg2>=0 && myArg2<24) && (myArg3>=0 || myArg3<=5) && (myArg4==-1 || myArg4==1) && (myArg5>-1 && myArg5<6)) {
	chaID.push_back(myArg1);
	chaBoID.push_back(myArg2);
	chaPlane.push_back(myArg3);
	chaView.push_back(myArg4);
	chaSense.push_back(myArg5);
      } else {
	Error(""," Plane Map Error:: check config file!!");
	return kTRUE;
      }
    } else if(strchr(bufConf,'T')) {
      sscanf(bufConf, "T%d",&trefCh);
      if(trefCh>-1 && trefCh<64) {
	//	Warning("FromFile()","Reference Trigger channel:: %d",trefCh);
      } else {
	Error("FromFile()","Reference Tr channel:: check config file!!");
	return kTRUE;
      }
    }
  }

  return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TABMparMap::Clear(Option_t*)
{
  TAGpara::Clear();
  chaID.clear();   
  chaBoID.clear();   
  chaPlane.clear();
  chaView.clear(); 
  chaSense.clear();
  return;
}

/*------------------------------------------+---------------------------------*/
//! Returns \c true if wire number \a i_wire is out of range

Bool_t TABMparMap::CheckWire(Int_t i_wire) const
{
  if (i_wire >= 0 && i_wire <  n_wires) return kFALSE;
  Error("CheckWire()", "wire number %d out of range", n_wires);
  return kTRUE;
}

/*------------------------------------------+---------------------------------*/
//! Returns \c true if TDC channel \a i_c is out of range

Bool_t TABMparMap::CheckAddr(Int_t i_c) const
{
  if (i_c >= 0 && i_c < dim_cha ) return kFALSE;
  Error("CheckAddr()", "TDC address (c) = (%d) out of range", i_c);
  return kTRUE;
}

/*------------------------------------------+---------------------------------*/
bool TABMparMap::GetWireIDFromTDC(int channel, int board, int &senseID, int &plane, int &view) {

  int my_iw = -1; bool hitFound = kFALSE;
  senseID = -1; plane = -1; view = -2;

  for(int iw=0; iw<(int)chaID.size(); iw++) {
    if(getID(iw) == channel && getBoID(iw) == board)  { my_iw = iw; break;}
  }
  //Only if the hit is found
  if(my_iw != -1) {
    senseID = getSense(my_iw);
    plane = getPlane(my_iw);
    view = getView(my_iw);
    hitFound = kTRUE;
  }

  return hitFound;

}



/*!
  \file
  \version $Id: TASTparMap.cxx,v 1.5 2003/06/09 18:41:04 mueller Exp $
  \brief   Implementation of TASTparMap.
*/

#include <string.h>

#include <fstream>

#include "TSystem.h"
#include "TString.h"

#include "TASTparMap.hxx"

//##############################################################################

/*!
  \class TASTparMap TASTparMap.hxx "TASTparMap.hxx"
  \brief Map parameters for onion and daisy. **
*/

ClassImp(TASTparMap);

//------------------------------------------+-----------------------------------
//! Default constructor.

TASTparMap::TASTparMap() {

  vector<int> tdchaID;             tdchaID.clear();   
  vector<int> tdboaID;             tdboaID.clear();   
  vector<int> dechaID;             dechaID.clear();   

  TDchaID =    tdchaID;   
  TDboaID =    tdboaID;   
  DetID    =   dechaID;   
  
}

//------------------------------------------+-----------------------------------
//! Destructor.

TASTparMap::~TASTparMap()
{}


//------------------------------------------+-----------------------------------
//! Read mapping data from file \a name .

Bool_t TASTparMap::FromFile(const TString& name) {

  Clear();
  
  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  char bufConf[1024];
  int myArg1(0), myArg2(0), myArg3(0); 
 
  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("FromFile()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'T')) {
      //Cha id and board
      sscanf(bufConf, "T%d %d %d",&myArg1,&myArg2,&myArg3);
      if((myArg1>-1 && myArg1<64) && (myArg2>-1 && myArg2<64) && (myArg3>-1 && myArg3<20)) {
	DetID.push_back(myArg1);
	TDchaID.push_back(myArg2);
	TDboaID.push_back(myArg3);
      } else {
	Error(""," Plane Map Error:: check config file!!");
	return kTRUE;
      }
    }
  }

  /*
    OLD imple

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    } else if(strchr(bufConf,'#')) {
      //Det id, Det channel, tdc, adc, adc board.
      sscanf(bufConf, "#%d %d %d %d %d %d",&myArg1,&myArg2,&myArg3,&myArg4,&myArg5,&myArg6);
      if((myArg1>-1 && myArg1<2) && (myArg2>-1 && myArg2<4) && (myArg3>-1 && myArg3<128) && (myArg4>-1 && myArg4<32) && (myArg5>-1 && myArg5<32)) {
	DetID.push_back(myArg1);
	DetchaID.push_back(myArg2);
	TDchaID.push_back(myArg3);
	TDboaID.push_back(myArg4);
	ADchaID.push_back(myArg5);
	ADboaID.push_back(myArg6);
      } else {
	Error(""," Plane Map Error:: check config file!!");
	return kTRUE;
      }
    }
  }


  */
  
  return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TASTparMap::Clear(Option_t*)
{
  TAGpara::Clear();
  TDchaID.clear();   
  TDboaID.clear();   
  DetID.clear();   
  return;
}

/*------------------------------------------+---------------------------------*/
bool TASTparMap::IsTrigID(int channel, int board, int& trid) {

  bool found = kFALSE;
  trid = -1;
  for(unsigned int il = 0; il<DetID.size(); il++) {
    if(getTDID(il) == channel && getTDboaID(il) == board)  { 
      trid = getDetID(il);
      found = kTRUE;
    }
  }

  return found;

}


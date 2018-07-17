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

  //~ vector<int> tchaID;             tchaID.clear();   
  //~ vector<int> tchaBoID;           tchaBoID.clear();   
  //~ vector<int> tchaPlane;	  tchaPlane.clear();
  //~ vector<int> tchaView;	          tchaView.clear(); 
  //~ vector<int> tchaSense;	  tchaSense.clear();

  //~ chaID =    tchaID;   
  //~ chaBoID =  tchaBoID;   
  //~ chaPlane = tchaPlane;
  //~ chaView =  tchaView; 
  //~ chaSense = tchaSense;
  tdc_maxcha=64;
  
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMparMap::~TABMparMap()
{}


//------------------------------------------+-----------------------------------
//! Read mapping data from file \a name .

Bool_t TABMparMap::FromFile(const TString& name, TABMparGeo *bmgeo) {

  Clear();
  
  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  char bufConf[1024];
  Int_t myArg1(0), myArg2(0), myArg3(0), myArg4(0); 

  ifstream incF;
  incF.open(name_exp.Data());
  if (!incF) {
    Error("ERROR in TABMparMap::FromFile()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  while (incF.getline(bufConf, 200, '\n')) {
    if(strchr(bufConf,'!')) {
      //      Info("FromFile()","Skip comment line:: %s",bufConf);
    }else if(strchr(bufConf,'#')) {
      sscanf(bufConf, "#%d %d %d %d %d",&myArg1,&myArg2,&myArg3,&myArg4);
      if((myArg1>-1 && myArg1<tdc_maxcha) && (myArg2>=0 || myArg2<=5) && (myArg3==-1 || myArg3==1) && (myArg4>-1 && myArg4<3)) {
        //~ chaBoID.push_back(myArg2);
        //~ chaPlane.push_back(myArg3);
        //~ chaView.push_back(myArg4);
        //~ chaSense.push_back(myArg5);
        //~ tdc2cell_map.insert(std::make_pair(myArg1,bmgeo->GetBMNcell(myArg2,myArg3,myArg4)));
        if(tdc2cell_vec[myArg1]<0)
          tdc2cell_vec[myArg1]=bmgeo->GetBMNcell(myArg2,myArg3,myArg4);
        else{
          Error("TABMparMap::FromFile()","channel already set; check config file!!");
          return kTRUE;
        }
      } else {
        Error(""," TABMparMap Plane Map Error:: check config file!! (#)");
        return kTRUE;
      }
    } else if(strchr(bufConf,'T')) {
        sscanf(bufConf, "T %d",&myArg1);
      if(myArg1<0 || myArg1>tdc_maxcha) {
        Error("FromFile()","TABMparMap::Reference Tr channel:: check config file!!(T)");
        return kTRUE;
      }else{
        trefCh=myArg1;
        tdc2cell_vec[trefCh]=-1000;
      }
    } else if(strchr(bufConf,'S')) {
        sscanf(bufConf, "S %d",&myArg1);
      if(myArg1<0) {
        Error("FromFile()","TABMparMap::Scaler sca830ch ERROR:: check config file!!(S)");
        return kTRUE;
      }else
        sca830ch=myArg1;
    } else if(strchr(bufConf,'H')) {
        sscanf(bufConf, "H %d",&myArg1);
      if(myArg1<0) {
        Error("FromFile()","TABMparMap::hm_ev_read ERROR:: check config file!!(H)");
        return kTRUE;
      }else
        hm_ev_read=myArg1;
    } else if(strchr(bufConf,'M')) {
        sscanf(bufConf, "M %d",&myArg1);
        if(!(myArg1==64 || myArg1==128 || myArg1==256)) {
          Error("FromFile()","TABMparMap::tdc_maxcha not = 64 || 128 || 256:: check config file!!(M)");
          return kTRUE;
        }else{
          tdc_maxcha=myArg1;
          for(Int_t i=0;i<tdc_maxcha;i++)
            tdc2cell_vec.push_back(-1);
          }
    }
  }

  //check on the map
  Int_t tmp_int(0);
  for(Int_t i=0;i<tdc_maxcha;i++)
    if(tdc2cell_vec[i]>=0)
      tmp_int++;
  if(tmp_int!=36){
    cout<<"ERROR in TABMparMap:FromFile():error in the map! you haven't set 36 channel for the BM! number of channel set="<<tmp_int<<endl;
    return kTRUE;
  }
  

  return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Clear event.

void TABMparMap::Clear(Option_t*)
{
  TAGpara::Clear();
  //~ chaID.clear();   
  //~ chaBoID.clear();   
  //~ chaPlane.clear();
  //~ chaView.clear(); 
  //~ chaSense.clear();
  tdc2cell_vec.clear();
  return;
}

/*------------------------------------------+---------------------------------*/
//~ //! Returns \c true if wire number \a i_wire is out of range

//~ Bool_t TABMparMap::CheckWire(Int_t i_wire) const
//~ {
  //~ if (i_wire >= 0 && i_wire <  n_wires) return kFALSE;
  //~ Error("CheckWire()", "wire number %d out of range", n_wires);
  //~ return kTRUE;
//~ }

//~ /*------------------------------------------+---------------------------------*/
//~ //! Returns \c true if TDC channel \a i_c is out of range

//~ Bool_t TABMparMap::CheckAddr(Int_t i_c) const
//~ {
  //~ if (i_c >= 0 && i_c < dim_cha ) return kFALSE;
  //~ Error("CheckAddr()", "TDC address (c) = (%d) out of range", i_c);
  //~ return kTRUE;
//~ }

/*------------------------------------------+---------------------------------*/
bool TABMparMap::GetWireIDFromTDC(int channel, int board, int &senseID, int &plane, int &view) {

  //~ int my_iw = -1; bool hitFound = kFALSE;
  //~ senseID = -1; plane = -1; view = -2;

  //~ for(int iw=0; iw<(int)chaID.size(); iw++) {
    //~ if(getID(iw) == channel && getBoID(iw) == board)  { my_iw = iw; break;}
  //~ }
  //~ //Only if the hit is found
  //~ if(my_iw != -1) {
    //~ senseID = getSense(my_iw);
    //~ plane = getPlane(my_iw);
    //~ view = getView(my_iw);
    //~ hitFound = kTRUE;
  //~ }

  //~ return hitFound;

  return kTRUE;//Provv, da modificare
}


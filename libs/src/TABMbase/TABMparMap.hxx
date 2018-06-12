#ifndef _TABMparMap_HXX
#define _TABMparMap_HXX
/*!
  \file
  \version $Id: TABMparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TABMparMap.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>

#include "TString.h"
#include <map>

#include "TAGpara.hxx"
#include "TABMparGeo.hxx"
#include "TABMparCon.hxx"

using namespace std;

//##############################################################################

class TABMparMap : public TAGpara {
  public:

    //~ enum { n_wires = 36,		    // ugly, to make root happy ...
	   //~ dim_cha  =  64,
	   //~ dim_addr = dim_cha
    //~ };
  
  TABMparMap();
    virtual         ~TABMparMap();

    Bool_t          FromFile(const TString& name, TABMparGeo *bmgeo);
    bool            GetWireIDFromTDC(int channel, int board, int &senseID, int &plane, int &view);
    Int_t GetTdcMaxcha(){return tdc_maxcha;};
    Int_t tdc2cell(Int_t tdccha){return tdc2cell_vec[tdccha];};
    Int_t cell2tdc(Int_t cell){return std::find(tdc2cell_vec.begin(),tdc2cell_vec.end(),cell)-tdc2cell_vec.begin();};
    
    virtual void    Clear(Option_t* opt="");

  //    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    //~ inline int getID(int add) { return chaID.at(add); }
    //~ inline int getBoID(int add) { return chaBoID.at(add); }
    //~ inline int getSense(int add) { return chaSense.at(add); }
    //~ inline int getPlane(int add) { return chaPlane.at(add); }
    //~ inline int getView(int add) { return chaView.at(add); }
    //~ inline int getTrefch(){return trefCh;}
    
    ClassDef(TABMparMap,1)

  private:
    //~ Bool_t          CheckWire(Int_t i_wire) const;
    //~ Bool_t          CheckAddr(Int_t i_c) const;

  private:
    Int_t trefCh;  //trigger reference channel
    Int_t    tdc_maxcha;//tdc number of channel
    vector<Int_t> tdc2cell_vec;//each position of this vector correspond to a tdc channel, the value stored correspond to the bm cell index (0-35) or -1000 if is the trefCh, otherwise is -1

    //~ vector<int> chaID;
    //~ vector<int> chaBoID;
    //~ vector<int> chaPlane;
    //~ vector<int> chaView;
    //~ vector<int> chaSense;
};

#endif

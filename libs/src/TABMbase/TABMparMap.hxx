#ifndef _TABMparMap_HXX
#define _TABMparMap_HXX
/*!
  \file
  \version $Id: TABMparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TABMparMap.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TString.h"

#include "TAGpara.hxx"

//##############################################################################

class TABMparMap : public TAGpara {
  public:

    enum { n_wires = 36,		    // ugly, to make root happy ...
	   dim_cha  =  64,
	   dim_addr = dim_cha
    };
  
  TABMparMap();
    virtual         ~TABMparMap();

    Bool_t          FromFile(const TString& name);

    bool            GetWireIDFromTDC(int channel, int board, int &senseID, int &plane, int &view);
  
    virtual void    Clear(Option_t* opt="");

  //    virtual void    ToStream(ostream& os = cout, Option_t* option = "") const;

    inline int getID(int add) { return chaID.at(add); }
    inline int getBoID(int add) { return chaBoID.at(add); }
    inline int getSense(int add) { return chaSense.at(add); }
    inline int getPlane(int add) { return chaPlane.at(add); }
    inline int getView(int add) { return chaView.at(add); }

    ClassDef(TABMparMap,1)

  private:
    Bool_t          CheckWire(Int_t i_wire) const;
    Bool_t          CheckAddr(Int_t i_c) const;

  private:
    int trefCh;
    vector<int> chaID;
    vector<int> chaBoID;
    vector<int> chaPlane;
    vector<int> chaView;
    vector<int> chaSense;
};

#endif

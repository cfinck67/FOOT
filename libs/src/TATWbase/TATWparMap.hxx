#ifndef _TATWparMap_HXX
#define _TATWparMap_HXX
/*!
  \file
  \version $Id: TATWparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TATWparMap.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TString.h"

#include "TAGpara.hxx"

//##############################################################################

class TATWparMap : public TAGpara {
  public:

  TATWparMap();
    virtual         ~TATWparMap();

    Bool_t          FromFile(const TString& name);

    bool            GetIDFromTDC(int channel, int board, int &detID, int &chaID);
    bool            GetIDFromADC(int channel, int board, int &detID, int &chaID);

    virtual void    Clear(Option_t* opt="");

    inline int getTDID(int add) { return TDchaID.at(add); }
    inline int getTDboaID(int add) { return TDboaID.at(add); }
    inline int getADID(int add) { return ADchaID.at(add); }
    inline int getADboaID(int add) { return ADboaID.at(add); }
    inline int getDetChaID(int add) { return DetchaID.at(add); }
    inline int getDetID(int add) { return DetID.at(add); }

    ClassDef(TATWparMap,1)

  private:
    Bool_t          CheckAddr(Int_t i_c) const;

    int trefCh;
    vector<int> TDchaID;
    vector<int> TDboaID;
    vector<int> ADchaID;
    vector<int> ADboaID;
    vector<int> DetchaID;
    vector<int> DetID;
};

#endif

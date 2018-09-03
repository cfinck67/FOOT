#ifndef _TATRparMap_HXX
#define _TATRparMap_HXX
/*!
  \file
  \version $Id: TATRparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TATRparMap.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TString.h"

#include "TAGpara.hxx"

//##############################################################################

class TATRparMap : public TAGpara {
  public:

  TATRparMap();
    virtual         ~TATRparMap();

    Bool_t          FromFile(const TString& name);

    bool            IsTrigID(int channel, int board, int& trid);

    virtual void    Clear(Option_t* opt="");

    inline int getTDID(int add) { return TDchaID.at(add); }
    inline int getTDboaID(int add) { return TDboaID.at(add); }
    inline int getDetID(int add) { return DetID.at(add); }

    ClassDef(TATRparMap,1)

  private:

    vector<int> TDchaID;
    vector<int> TDboaID;
    vector<int> DetID;
};

#endif

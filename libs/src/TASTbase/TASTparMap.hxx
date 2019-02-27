#ifndef _TASTparMap_HXX
#define _TASTparMap_HXX
/*!
  \file
  \version $Id: TASTparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
  \brief   Declaration of TASTparMap.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
using namespace std;

#include "TString.h"

#include "TAGpara.hxx"

//##############################################################################

class TASTparMap : public TAGpara {
  public:

  TASTparMap();
    virtual         ~TASTparMap();

    Bool_t          FromFile(const TString& name);

    bool            IsTrigID(int channel, int board, int& trid);

    virtual void    Clear(Option_t* opt="");

    inline int getTDID(int add) { return TDchaID.at(add); }
    inline int getTDboaID(int add) { return TDboaID.at(add); }
    inline int getDetID(int add) { return DetID.at(add); }

    ClassDef(TASTparMap,1)

  private:

    vector<int> TDchaID;
    vector<int> TDboaID;
    vector<int> DetID;
};

#endif

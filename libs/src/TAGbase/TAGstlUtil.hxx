#ifndef _TAGstlUtil_HXX
#define _TAGstlUtil_HXX
/*!
  \file
  \version $Id: TAGstlUtil.hxx,v 1.3 2003/06/15 18:26:03 mueller Exp $
  \brief   Declaration of TAGstlUtil.
*/
/*------------------------------------------+---------------------------------*/

#include <vector>
#include <map>
using namespace std;

#include "TAGobject.hxx"

class TAGstlUtil : public TAGobject {
  public:
                    TAGstlUtil();
    virtual         ~TAGstlUtil();

    ClassDef(TAGstlUtil,0)

  private:
};

//#include "TAGstlUtil.icc" 

#endif

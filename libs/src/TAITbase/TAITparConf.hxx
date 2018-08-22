#ifndef _TAITparConf_HXX
#define _TAITparConf_HXX
/*!
  \file
  \version $Id: TAITparConf.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAITparConf.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

#include "TAVTbaseParConf.hxx"

//##############################################################################

class TAITparConf : public TAVTbaseParConf {
   
public:

   TAITparConf();
    virtual ~TAITparConf();
   
   ClassDef(TAITparConf,1)
};

#endif

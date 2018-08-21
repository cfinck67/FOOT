#ifndef _TAVTparConf_HXX
#define _TAVTparConf_HXX
/*!
  \file
  \version $Id: TAVTparConf.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAVTparConf.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TString.h"
#include "TVector3.h"

#include "TAVTbaseParConf.hxx"

//##############################################################################

class TAVTparConf : public TAVTbaseParConf {
   
public:

   TAVTparConf();
    virtual ~TAVTparConf();

public:
   
   ClassDef(TAVTparConf,1)
};

#endif

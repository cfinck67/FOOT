#ifndef _TAITparCal_HXX
#define _TAITparCal_HXX
/*!
  \file
  \version $Id: TAITparCal.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAITparCal.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TArrayF.h"
#include "TF1.h"

#include "TAVTbaseParCal.hxx"

//##############################################################################

class TAITparCal : public TAVTbaseParCal {

public:

   TAITparCal();
    virtual ~TAITparCal();

   ClassDef(TAITparCal,1)
};

#endif

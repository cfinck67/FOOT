#ifndef _TAVTparCal_HXX
#define _TAVTparCal_HXX
/*!
  \file
  \version $Id: TAVTparCal.hxx,v 1.2 2003/06/22 19:33:36 mueller Exp $
  \brief   Declaration of TAVTparCal.
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TArrayF.h"
#include "TF1.h"

#include "TAVTbaseParCal.hxx"

//##############################################################################

class TAVTparCal : public TAVTbaseParCal {

public:

   TAVTparCal();
    virtual ~TAVTparCal();

   ClassDef(TAVTparCal,1)
};

#endif

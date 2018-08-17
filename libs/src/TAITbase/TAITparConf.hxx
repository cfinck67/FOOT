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

#include "TAVTparConf.hxx"

//##############################################################################

class TAITparConf : public TAVTparConf {
   
private: 
   static       TString fgkDefItConfName; // default detector configuration file

public:

   TAITparConf();
    virtual ~TAITparConf();

public:
   
   static const Char_t* GetDefItConfName()     { return fgkDefItConfName.Data(); }

   ClassDef(TAITparConf,1)
};

#endif

#ifndef _TAITdigitizerE_HXX
#define _TAITdigitizerE_HXX
/*!
 \file
 \version $Id: TAITdigitizerE.hxx,v $
 \brief   Declaration of TAITdigitizerE.
 */
/*------------------------------------------+---------------------------------*/


#include "TAGparaDsc.hxx"
#include "TAVTdigitizerE.hxx"

class TAITdigitizerE : public TAVTdigitizerE {
   
public:
   TAITdigitizerE(TAGparaDsc* parGeo);
   virtual ~TAITdigitizerE();
   
   ClassDef(TAITdigitizerE,0)
};

#endif

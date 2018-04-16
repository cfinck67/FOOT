#ifndef _TAITdigitizer_HXX
#define _TAITdigitizer_HXX
/*!
 \file
 \version $Id: TAVTdigitizer.hxx,v $
 \brief   Declaration of TAITdigitizer.
 */
/*------------------------------------------+---------------------------------*/

#include "TAGparaDsc.hxx"
#include "TAVTdigitizer.hxx"


class TAITdigitizer : public TAVTdigitizer {
   
public:
   TAITdigitizer(TAGparaDsc* parGeo);
   virtual ~TAITdigitizer();
   
  
   ClassDef(TAITdigitizer,0)
};

#endif

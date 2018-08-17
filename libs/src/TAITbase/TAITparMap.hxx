#ifndef _TAITparMap_HXX
#define _TAITparMap_HXX
/*!
 \file
 \version $Id: TAITparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
 \brief   Declaration of TAITparMap.
 */
/*------------------------------------------+---------------------------------*/


#include "TAVTparMap.hxx"

//##############################################################################

class TAITparMap : public TAVTparMap {
   
public:
   TAITparMap();
   virtual ~TAITparMap();

public:
   static Int_t GetRealId(Int_t mcSensorId);
   
   ClassDef(TAITparMap,1)
  
};

#endif

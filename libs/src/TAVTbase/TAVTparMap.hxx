#ifndef _TAVTparMap_HXX
#define _TAVTparMap_HXX
/*!
 \file
 \version $Id: TAVTparMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
 \brief   Declaration of TAVTparMap.
 */
/*------------------------------------------+---------------------------------*/


#include <vector> 

#include "TAVTbaseParMap.hxx"

//##############################################################################

class TAVTparMap : public TAVTbaseParMap {
   
public:
   TAVTparMap();
   virtual ~TAVTparMap();


   ClassDef(TAVTparMap,1)
  
};

#endif

#ifndef _TAGpointDisplay_HXX_
#define _TAGpointDisplay_HXX_

#ifndef ROOT_TEvePointSet
#include "TEvePointSet.h"
#endif


/** TAGpointDisplay a class to display point on event
 
*/

class TAGpointDisplay : public TEvePointSet
{

public:
  TAGpointDisplay(const Text_t* name);
  virtual ~TAGpointDisplay();
   
  //! Add Point
  void AddPoint(Float_t x, Float_t y, Float_t z);
  
  //! Reset point
  void ResetPoints();
  
  ClassDef(TAGpointDisplay, 1);   
};


#endif

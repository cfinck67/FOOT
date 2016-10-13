#ifndef _TAITactAscRaw_HXX
#define _TAITactAscRaw_HXX


#include "TAITactBaseRaw.hxx"

/*!
 \file
 \version $Id: TAITactAscRaw.hxx $
 \brief   Declaration of TAITactAscRaw.
 */
/*------------------------------------------+---------------------------------*/
class TAGdataDsc;

class TAITactAscRaw : public TAITactBaseRaw {
public:
   
   explicit TAITactAscRaw(const char* name=0, TAGdataDsc* p_datraw=0, TAGdataDsc* p_datasc=0, TAGparaDsc* p_geomap=0, TAGparaDsc* p_config=0);
   virtual  ~TAITactAscRaw();
   
   virtual Bool_t  Action();
      
private:
   
   TAGdataDsc*       fpDatAsc;		    // input data dsc
   
   ClassDef(TAITactAscRaw,0)
};

#endif

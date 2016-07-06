#ifndef _TAVTactAscRaw_HXX
#define _TAVTactAscRaw_HXX


#include "TAVTactBaseRaw.hxx"

/*!
 \file
 \version $Id: TAVTactAscRaw.hxx $
 \brief   Declaration of TAVTactAscRaw.
 */
/*------------------------------------------+---------------------------------*/
class TAGdataDsc;

class TAVTactAscRaw : public TAVTactBaseRaw {
public:
   
   explicit TAVTactAscRaw(const char* name=0, TAGdataDsc* p_datraw=0, TAGdataDsc* p_datasc=0, TAGparaDsc* p_geomap=0, TAGparaDsc* p_config=0);
   virtual  ~TAVTactAscRaw();
   
   virtual Bool_t  Action();
      
private:
   
   TAGdataDsc*       fpDatAsc;		    // input data dsc
   
   ClassDef(TAVTactAscRaw,0)
};

#endif

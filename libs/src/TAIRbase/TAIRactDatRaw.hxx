#ifndef _TAIRactDatRaw_HXX
#define _TAIRactDatRaw_HXX

/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"
#include "TABMparMap.hxx"
#include "BM_struct.h"

class TAIRactDatRaw : public TAGaction {
  public:
    explicit        TAIRactDatRaw(const char* name=0,
				 TAGdataDsc* p_datraw=0, 
				 TAGparaDsc* p_parmap=0,
				 BM_struct* bmstruct=0);
    virtual         ~TAIRactDatRaw();

    virtual Bool_t  Action();

    ClassDef(TAIRactDatRaw,0)

  private:
    TAGdataDsc*     fpNtuDatRaw;		    // output data dsc
    TAGparaDsc*     fpParMap;		    // BM config!!!! from TABMparCOn...
    BM_struct*      bmstruct;
};

#endif

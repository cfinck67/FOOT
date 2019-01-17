#ifndef _TABMactDatRaw_HXX
#define _TABMactDatRaw_HXX
/*!
  \file
  \version $Id: TABMactDatRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TABMactDatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"
#include "TABMparCon.hxx"
#include "BM_struct.h"
#include <fstream>

class TABMactDatRaw : public TAGaction {
  public:

    //~ Bool_t openFile(TABMparCon*);  //load in datastream the data file, provv da modificare cancellare

    explicit        TABMactDatRaw(const char* name=0,
                                  TAGdataDsc* p_datraw=0,
                                  TAGparaDsc* p_parmap=0,
                                  TAGparaDsc* p_parcon=0,
                                  TAGparaDsc* p_pargeo=0,
                                  TAGdataDsc* p_timraw=0,
                                  BM_struct*  p_evtstruct=0);
    virtual         ~TABMactDatRaw();

    virtual Bool_t  Action();

    ClassDef(TABMactDatRaw,0)

  private:
    TAGdataDsc*     fpDatRaw;		    // output data dsc
    TAGparaDsc*     fpParMap;		    // parameter dsc
    TAGparaDsc*     fpParCon;		    // parameter dsc
    TAGparaDsc*     fpParGeo;		    // parameter dsc
    TAGdataDsc*     fpTimRaw;		    // input data dsc
    BM_struct*      fpEvtStruct;
    //~ ifstream datastream;                  // file di dati in input (cosa provvisoria per test calibrazione)
  
};

#endif

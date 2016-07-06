#ifndef _TAGvieHorzMCIR_HXX
#define _TAGvieHorzMCIR_HXX
/*!
  \file
  \version $Id: TAGvieHorzMCIR.hxx,v 1.3 2003/06/22 11:05:58 mueller Exp $
  \brief   Declaration of TAGvieHorzMCIR.
*/
/*------------------------------------------+---------------------------------*/

#include "Evento.h"

#include "TPad.h"
#include "TVector3.h"

#include "TAGview.hxx"
#include "TAGdataDsc.hxx"
#include "TAGparaDsc.hxx"

class TAGvieHorzMCIR : public TAGview {
  public:
    explicit        TAGvieHorzMCIR(TAGdataDsc* p_ntutrk=0,
				 TAGdataDsc* p_nturaw=0,
				 TAGparaDsc* p_bmgeo=0,
				 TAGdataDsc* p_ntuvtcl=0,
				 TAGdataDsc* p_ntuvttr=0,
				 TAGparaDsc* p_vtgeo=0, 
				 EVENT_STRUCT* evStr=0);

    virtual         ~TAGvieHorzMCIR();

    virtual char*   GetObjectInfo(Int_t px, Int_t py) const;
    virtual void    ExecuteEvent(Int_t i_event, Int_t i_px, Int_t i_py);

    virtual void    Paint(Option_t* option="");

    ClassDef(TAGvieHorzMCIR,0)

  private:
    TAGdataDsc*     fpNtuTrk;
    TAGdataDsc*     fpNtuBMRaw;
    TAGparaDsc*     fpParBMGeo;
    TAGdataDsc*     fpNtuClust;
    TAGdataDsc*     fpNtuTrack;
    TAGparaDsc*     fpParVTGeo;
    EVENT_STRUCT*   fpEvtStr;

};

#endif

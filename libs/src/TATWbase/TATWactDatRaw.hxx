#ifndef _TATWactDatRaw_HXX
#define _TATWactDatRaw_HXX
/*!
  \file
  \version $Id: TATWactDatRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TATWactDatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"
#include "TWaveformContainer.hxx"
#include "TATWparTime.hxx"
#include "TATWdatRaw.hxx"

class WDEvent;

#define GLB_EVT_HEADER 0xeadebaba
#define FILE_HEADER 0x30514457
#define TIME_HEADER 0x454d4954
#define BOARD_HEADER 0x00002342
#define CH_HEADER 0x00003043
#define EVT_HEADER 0x52444845
#define EVT_FOOTER 0xfafefafe


class TATWactDatRaw : public TAGaction {

public:

  explicit        TATWactDatRaw(const char* name=0,
				TAGdataDsc* p_datraw=0,
				TAGdataDsc* p_datdaq=0,
				TAGparaDsc* p_parmap=0,
				TAGparaDsc* p_parTime=0);
  virtual         ~TATWactDatRaw();

  virtual Bool_t  Action();

  ClassDef(TATWactDatRaw,0)
    
  private:
    
  TAGdataDsc*     fpDatRaw;		    // output data dsc
  TAGdataDsc*     fpDatDaq;		    // input data dsc
  TAGparaDsc*     fpParMap;		    // parameter dsc
  TAGparaDsc*     fpParTime;		    // parameter dsc

  bool m_debug;
  
 private:

  Bool_t DecodeHits(const WDEvent* evt, TATWparTime *p_parTime, TATWdatRaw *p_datraw, TWaveformContainer &w);

};

#endif

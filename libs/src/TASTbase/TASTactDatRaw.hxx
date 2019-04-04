#ifndef _TASTactDatRaw_HXX
#define _TASTactDatRaw_HXX
/*!
  \file
  \version $Id: TASTactDatRaw.hxx,v 1.3 2003/06/15 18:27:04 mueller Exp $
  \brief   Declaration of TASTactDatRaw.
*/
/*------------------------------------------+---------------------------------*/

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"
#include "TASTparTime.hxx"
#include "TASTdatRaw.hxx"
#include "TASTparMap.hxx"
#include "TGraph.h"
#include "TF1.h"
#include "TH1.h"

class WDEvent;

#define GLB_EVT_HEADER 0xeadebaba
#define FILE_HEADER 0x30514457
#define TIME_HEADER 0x454d4954
#define BOARD_HEADER 0x00002342
#define CH_HEADER 0x00003043
#define EVT_HEADER 0x52444845
#define EVT_FOOTER 0xfafefafe


class TASTactDatRaw : public TAGaction {

public:

  explicit        TASTactDatRaw(const char* name=0,
				TAGdataDsc* p_datraw=0,
				TAGdataDsc* p_datdaq=0,
				TAGparaDsc* p_parmap=0,
				TAGparaDsc* p_parTime=0);
  virtual         ~TASTactDatRaw();

  virtual Bool_t  Action();
  virtual void CreateHistogram();
  
  ClassDef(TASTactDatRaw,0)
    
  private:
    
  TAGdataDsc*     fpDatRaw;		    // output data dsc
  TAGdataDsc*     fpDatDaq;		    // input data dsc
  TAGparaDsc*     fpParMap;		    // parameter dsc
  TAGparaDsc*     fpParTime;		    // parameter dsc

  TH1D *hArrivalTime[8];
  TH1D *hCharge[8];
  TH1D *hAmplitude[8];
  TH1D *hTrigTime;
  TH1D *hTotCharge;
  
  bool m_debug;
  int m_nev;
  
 private:
  Bool_t DecodeHits(const WDEvent* evt, TASTparTime* p_parTime, TASTdatRaw *p_datraw, TASTparMap *p_parmap);
  double ComputeArrivalTime(TASTrawHit *hit, bool *isok);
  double ComputeCharge(TASTrawHit *hit);
  double ComputeMaxAmplitude(TASTrawHit *hit);
};

#endif

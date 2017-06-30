#ifndef _TAVTactNtuMC_HXX
#define _TAVTactNtuMC_HXX
/*!
 \file
 \version $Id: TAVTactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAVTactNtuMC.
 */
/*------------------------------------------+---------------------------------*/

#include "Evento.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

class TH2F;
class TH1F;
class TAVTdigitizer;

class TAVTactNtuMC : public TAGaction {
   
public:
   explicit TAVTactNtuMC(const char* name=0, TAGdataDsc* p_nturaw=0, TAGparaDsc* p_geomap = 0, TAGparaDsc* pParMap =0,EVENT_STRUCT* evtStr=0);
   virtual ~TAVTactNtuMC();
   
   //! Base action 
   virtual Bool_t  Action();

   //! Base creation of histogram
   virtual  void   CreateHistogram();
   
private:
   TAGdataDsc*     fpNtuRaw;		    // output data dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   TAGparaDsc*     fpParMap;		    // map para dsc
   EVENT_STRUCT*   fpEvtStr;         // MC structure
   
   TAVTdigitizer*  fDigitizer;       // cluster size digitizer
   
   Int_t           fDebugLevel;       // debug level
   
   TH2F*           fpHisPixelMap[8];  // pixel map per sensor
   TH2F*           fpHisPosMap[8];    // pixel map per sensor   
   TH1F*           fpHisRateMap[8];   // rates per sensor
   TH1F*           fpHisRateMapQ[8];  // rates per quadrant
   
   TH1F*           fpHisPixel[8];     // number pixels per cluster MC

   ClassDef(TAVTactNtuMC,0)
};

#endif

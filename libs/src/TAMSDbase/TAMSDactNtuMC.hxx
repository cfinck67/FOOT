#ifndef _TAMSDactNtuMC_HXX
#define _TAMSDactNtuMC_HXX
/*!
 \file
 \version $Id: TAMSDactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAMSDactNtuMC.
 */
/*------------------------------------------+---------------------------------*/

#include "Evento.h"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

#include "TAMSD_ContainerHit.hxx"
#include "TAMSD_ContainerPoint.hxx"


class TH2F;
class TH1F;
class TAMSDactNtuMC : public TAGaction {
   
public:
   explicit TAMSDactNtuMC(const char* name=0, TAGdataDsc* p_nturaw=0, TAGparaDsc* p_geomap = 0, TAGparaDsc* pParMap =0,EVENT_STRUCT* evtStr=0);
   virtual ~TAMSDactNtuMC();
   
   //! Base action 
   virtual Bool_t  Action();
   void FillAdditonalInfo( TAMSD_Hit* hit, int i );

   //! Base creation of histogram
   virtual  void   CreateHistogram();
   
private:
   TAGdataDsc*     fpNtuRaw;		    // output data dsc
   TAGparaDsc*     fpGeoMap;		    // geometry para dsc
   TAGparaDsc*     fpParMap;		    // map para dsc
   EVENT_STRUCT*   fpEvtStr;
   
   Int_t           fDebugLevel;         // debug level
   
   TH2F*           fpHisPixelMap[8];  // pixel map per sensor
   TH2F*           fpHisPosMap[8];    // pixel map per sensor   
   TH1F*           fpHisRateMap[8];   // rates per sensor
   TH1F*           fpHisRateMapQ[8];  // rates per quadrant
   
   ClassDef(TAMSDactNtuMC,0)
};

#endif

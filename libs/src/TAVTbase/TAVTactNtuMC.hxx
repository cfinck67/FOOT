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

#include "TAVTbaseDigitizer.hxx"
#include "TAVTdigitizerE.hxx"

#include "TAVTntuHit.hxx"

#include "TAVTactBaseNtuMC.hxx"

class TAVTntuRaw;
class TAVTparGeo;

class TAVTactNtuMC : public TAVTactBaseNtuMC {

// class TAVTbaseDigitizerg;
// class TAVTntuHit;

public:
   explicit TAVTactNtuMC(const char* name=0, TAGdataDsc* p_nturaw=0, TAGparaDsc* p_geomap = 0, EVENT_STRUCT* evtStr=0);
   virtual ~TAVTactNtuMC() {};
   
   //! Base action 
   virtual bool   Action();
   
   // Fill noise over sensors
   void           FillNoise();

   
private:
   TAGdataDsc*    fpNtuRaw;		    // output data dsc

private:
   void           FillNoise(Int_t sensorId) ;
   void           SetMCinfo(TAVTntuHit* pixel, Int_t hitId);
   void           CreateDigitizer();
   void           FillPixels( Int_t sensorId, Int_t mcId );
  
   
   ClassDef(TAVTactNtuMC,0)
};

#endif

























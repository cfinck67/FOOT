#ifndef _TAITactNtuMC_HXX
#define _TAITactNtuMC_HXX
/*!
 \file
 \version $Id: TAITactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAITactNtuMC.
 */
/*------------------------------------------+---------------------------------*/

#include "Evento.hxx"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

#include "TAITdigitizerE.hxx"

#include "TAITntuHit.hxx"

#include "TAVTactBaseNtuMC.hxx"

class TAITntuRaw;

class TAITactNtuMC : public TAVTactBaseNtuMC {

// class TAVTbaseDigitizerg;
// class TAVTntuHit;

public:
   explicit TAITactNtuMC(const char* name=0, TAGdataDsc* p_nturaw=0, TAGparaDsc* p_geomap = 0, EVENT_STRUCT* evtStr=0);
   virtual ~TAITactNtuMC() {};
   
   //! Base action 
   bool           Action();
   
   // Fill noise over sensors
   void           FillNoise();

   
private:
   TAGdataDsc*    fpNtuRaw;		    // output data dsc

private:
   void           FillNoise(Int_t sensorId) ;
   void           SetMCinfo(TAITntuHit* pixel, Int_t hitId);
   void           CreateDigitizer();
   void           FillPixels( Int_t sensorId, Int_t mcId );
  
   
   ClassDef(TAITactNtuMC,0)
};

#endif

























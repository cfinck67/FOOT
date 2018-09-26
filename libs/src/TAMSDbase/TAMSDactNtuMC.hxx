#ifndef _TAMSDactNtuMC_HXX
#define _TAMSDactNtuMC_HXX
/*!
 \file
 \version $Id: TAMSDactNtuMC.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAMSDactNtuMC.
 */
/*------------------------------------------+---------------------------------*/

#include "Evento.hxx"

#include "TAGaction.hxx"
#include "TAGdataDsc.hxx"

#include "TAMSDdigitizer.hxx"

#include "TAMSDntuHit.hxx"

#include "TAVTactBaseNtuMC.hxx"

class TAMSDntuRaw;
class TAMSDparGeo;

class TAMSDactNtuMC : public TAVTactBaseNtuMC {

// class TAMSDbaseDigitizerg;
// class TAMSDntuHit;

public:
   explicit TAMSDactNtuMC(const char* name=0, TAGdataDsc* p_nturaw=0, TAGparaDsc* p_geomap = 0, EVENT_STRUCT* evtStr=0);
   virtual ~TAMSDactNtuMC() {};
   
   //! Base action 
   virtual bool   Action();
   
   // Fill noise over sensors
   void           FillNoise();

   
private:
   TAGdataDsc*    fpNtuRaw;		    // output data dsc

private:
   void           FillNoise(Int_t sensorId) ;
   void           SetMCinfo(TAMSDntuHit* pixel, Int_t hitId);
   void           CreateDigitizer();
   void           FillPixels( Int_t sensorId, Int_t mcId );
  
   
   ClassDef(TAMSDactNtuMC,0)
};

#endif

























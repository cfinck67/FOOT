#ifndef _TAVTactNtuRaw_HXX
#define _TAVTactNtuRaw_HXX

#include "TAVTactBaseRaw.hxx"

/*!
 \file
 \version $Id: TAVTactNtuRaw.hxx $
 \brief   Declaration of TAVTactNtuRaw.
 */

/*------------------------------------------+---------------------------------*/
class TAGdataDsc;
class DECardEvent;

using namespace std;
class TAVTactNtuRaw : public TAVTactBaseRaw {
public:
   
   explicit TAVTactNtuRaw(const char* name=0, TAGdataDsc* p_datraw=0, TAGdataDsc* p_datmbs=0, TAGparaDsc* p_pargeo=0, TAGparaDsc* p_parconf=0, TAGparaDsc* pParMap=0);
   virtual  ~TAVTactNtuRaw();
   
   virtual Bool_t  Action();
   
private:
   TAGdataDsc*     fpDatDaq;		    // input data dsc
   
private:   
   //! Find vertex data
   Bool_t DecodeEvent(const DECardEvent* evt);
      
   //! Get the starting point of each event
   Bool_t GetVtxHeader();
   
   //! Get the starting point of each frame
   Bool_t GetSensorHeader(Int_t iSensor);
   
   //! Get frame and returns frameRaw
   Bool_t GetFrame(Int_t iSensor, MI26_FrameRaw* data);
   
   ClassDef(TAVTactNtuRaw,0)
};

#endif

#ifndef _TAVTactBaseRaw_HXX
#define _TAVTactBaseRaw_HXX

#include "TAVTmi26Type.hxx"

#include "TAGaction.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"


class TAVTntuRaw;
class TH2F;
class TH1F;

/*!
 \file
 \version $Id: TAVTactBaseRaw.hxx $
 \brief   Declaration of TAVTactBaseRaw.
 */
/*------------------------------------------+---------------------------------*/

class TAVTactBaseRaw : public TAGaction {
public:
   
   explicit TAVTactBaseRaw(const char* name=0, TAGdataDsc* p_datraw=0, TAGparaDsc* p_geomap=0, TAGparaDsc* p_config=0);
   virtual  ~TAVTactBaseRaw();
     
   //! Decode rawdata
   virtual Bool_t DecodeFrame();
   
   //! Get number of sensors
   Int_t GetSensorsN() const { return fNSensors; }
   
   //! Base creation of histogram
   void CreateHistogram();
   
public:
   
   static  UInt_t GetKeyHeader(Int_t idx) { return fgKeyHeader[idx]; }
   static  UInt_t GetTailHeader()         { return fgkTailHeader;    }
   static  Int_t GetHeaderSize()          { return fgkHeaderSize;    }
   static  Int_t GetLineWidth()           { return fgkLineWidth;     }
   
protected: 
   
   static const UInt_t  fgKeyHeader[];
   static const Int_t   fgkHeaderSize;
   static const Int_t   fgkLineWidth;
   static const UInt_t  fgkTailHeader;
   
protected:
      
   TAGdataDsc*       fpNtuRaw;		    // output data dsc
   TAGparaDsc*       fpGeoMap;		    // geo para dsc
   TAGparaDsc*       fpConfig;		    // config para dsc
   
   const UInt_t*     fData;

   Int_t             fNSensors;
   Int_t             fIndex;
   UInt_t            fCurrentTriggerCnt;
   Int_t             fEventSize;
   Bool_t            fReadingEvent;
   Bool_t            fOverflow;
   
   Int_t             fEventsOverflow; 
   Int_t             fNStatesInLine; 
      
   TH2F*             fpHisPixelMap[8];  // pixel map per sensor
   TH1F*             fpHisRateMap[8];   // pixel map per sensor
   TH1F*             fpHisRateMapQ[8];  // pixel map per sensor quadrant
   TH1F*             fpHisEvtLength[8]; // event data length for each sensor (all 3 frames)
   
protected:
   
   //! Add pixel to list
   void  AddPixel( Int_t input, Int_t value, Int_t aLine, Int_t aColumn);
   
   //! Get Sensor number
   Int_t GetSensor(UInt_t key);
   
   //! Check trigger counts
   Bool_t CheckTriggerCnt(UInt_t trig);
   
   
   ClassDef(TAVTactBaseRaw,0)

};

#endif

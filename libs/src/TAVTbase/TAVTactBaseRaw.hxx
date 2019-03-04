#ifndef _TAVTactBaseRaw_HXX
#define _TAVTactBaseRaw_HXX

#include "TAVTmi26Type.hxx"

#include "TAGactionFile.hxx"
#include "TAGparaDsc.hxx"
#include "TAGdataDsc.hxx"
#include <vector>


class TAVTntuRaw;
class TH2F;
class TH1F;

/*!
 \file
 \version $Id: TAVTactBaseRaw.hxx $
 \brief   Declaration of TAVTactBaseRaw.
 */
/*------------------------------------------+---------------------------------*/

using namespace std;

class TAVTactBaseRaw : public TAGactionFile {
public:
   
   explicit TAVTactBaseRaw(const char* name=0, TAGdataDsc* p_datraw=0, TAGparaDsc* p_geomap=0, TAGparaDsc* p_config=0);
   virtual  ~TAVTactBaseRaw();
   
   //! Get number of sensors
   Int_t GetSensorsN() const { return fNSensors; }
   
   //! Base creation of histogram
   void CreateHistogram();
   
public:
   static  UInt_t  GetKeyHeader(Int_t idx)                  { return fgkKeyHeader[idx];   }
   static  Int_t   GetHeaderSize()                          { return fgkFrameHeaderSize;  }
   static  UInt_t  GetDataTail(Int_t idx)                   { return fgkKeyTail[idx];     }
   static  Int_t   GetLineWidth()                           { return fgkLineWidth;        }

protected:
      
   TAGdataDsc*       fpNtuRaw;		    // output data dsc
   TAGparaDsc*       fpGeoMap;		    // geo para dsc
   TAGparaDsc*       fpConfig;		    // config para dsc
   
   vector<UInt_t>    fData;             // data array to fill

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
   static const UInt_t  fgkKeyHeader[];
   static const Int_t   fgkFrameHeaderSize;
   static const UInt_t  fgkKeyTail[];
   static const Int_t   fgkLineWidth;
   static const UInt_t  fgkFrameHeader;
   static const UInt_t  fgkFrameTail;
   
protected:
   
   //! Add pixel to list
   void  AddPixel( Int_t input, Int_t value, Int_t aLine, Int_t aColumn);
   
   //! Get Sensor number
   Int_t  GetSensor(UInt_t key);

   //! decode frame
   Bool_t DecodeFrame();

   ClassDef(TAVTactBaseRaw,0)

};

#endif

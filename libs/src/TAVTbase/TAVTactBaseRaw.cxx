/*!
 \file
 \version $Id: TAVTactBaseRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
 \brief   Implementation of TAVTactBaseRaw.
 */

#include "DECardEvent.hh"

#include "TH2F.h"

#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"

#include "TAVTntuRaw.hxx"
#include "TAVTactBaseRaw.hxx"
#include "TAVTmi26Type.hxx"

/*!
 \class TAVTactBaseRaw TAVTactBaseRaw.hxx "TAVTactBaseRaw.hxx"
 \brief Base class to get vertex raw data. **
 */

ClassImp(TAVTactBaseRaw);

const UInt_t TAVTactBaseRaw::fgkKeyHeader[]      = {0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8007, 0x8008};
const Int_t  TAVTactBaseRaw::fgkFrameHeaderSize  = 5;
const Int_t  TAVTactBaseRaw::fgkLineWidth        = 9;
const UInt_t TAVTactBaseRaw::fgkFrameHeader      = 0x80088007;
const UInt_t TAVTactBaseRaw::fgkFrameTail        = 0xaaa8aaa7;
const UInt_t TAVTactBaseRaw::fgkKeyTail[]        = {0xaaa1, 0xaaa2, 0xaaa3, 0xaaa4, 0xaaa5, 0xaaa6, 0xaaa7, 0xaaa8};

//------------------------------------------+-----------------------------------
//! Default constructor.

TAVTactBaseRaw::TAVTactBaseRaw(const char* name, TAGdataDsc* pDatRaw, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAGactionFile(name, "TAVTactBaseRaw - Base action for unpack vertex raw data"),
  fpNtuRaw(pDatRaw),
  fpGeoMap(pGeoMap),
  fpConfig(pConfig),
  fData(0x0),
  fNSensors(8),
  fIndex(0),
  fCurrentTriggerCnt(0),
  fReadingEvent(0),
  fOverflow(0),
  fEventsOverflow(0), 
  fNStatesInLine(0)
{
   AddDataOut(pDatRaw, "TAVTntuRaw");
   AddPara(pGeoMap, "TAVTparGeo");
   AddPara(pConfig, "TAVTparConf");
   
   TAVTparGeo* parGeo = (TAVTparGeo*) fpGeoMap->Object();
   fNSensors = parGeo->GetNSensors();
   
   Int_t size = parGeo->GetNSensors()*sizeof(MI26_FrameRaw);
   fData.resize(size);
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactBaseRaw::~TAVTactBaseRaw()
{
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactBaseRaw::CreateHistogram()
{
   DeleteHistogram();
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
	  if (TAVTparConf::IsMapHistOn()) {
		 fpHisPixelMap[i] = new TH2F(Form("vtPixelMap%d", i+1), Form("Vertex - pixel map for sensor %d", i+1), 
									 pGeoMap->GetNPixelY(), 0, pGeoMap->GetNPixelY(), 
									 pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX());
		 fpHisPixelMap[i]->SetStats(kFALSE);
		 AddHistogram(fpHisPixelMap[i]);
	  }
	  fpHisRateMap[i] = new TH1F(Form("vtRateMap%d", i+1), Form("Vertex - rate per line for sensor %d", i+1), 
								 pGeoMap->GetNPixelX(), 0, pGeoMap->GetNPixelX());
	  AddHistogram(fpHisRateMap[i]);

	  fpHisRateMapQ[i] = new TH1F(Form("vtRateMapQ%d", i+1), Form("Vertex - rate per quadrant for sensor %d", i+1), 
								 10, 0, 5);
	  AddHistogram(fpHisRateMapQ[i]);
	  
	  fpHisEvtLength[i] = new TH1F(Form("vtEvtLength%d", i+1), Form("Vertex - event length sensor %d", i+1), 
								   1000, 0, 1000);
	  AddHistogram(fpHisEvtLength[i]);
   }
   
   SetValidHistogram(kTRUE);
   return;
}

// --------------------------------------------------------------------------------------
Int_t TAVTactBaseRaw::GetSensor(UInt_t key)
{
   TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   
   key = (key >> 16) & 0xFFFF;
   for (Int_t i = 0; i <  pGeoMap->GetNSensors(); ++i) {
      if (fgkKeyHeader[i] == key)
         return i;
   }
   return -1;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactBaseRaw::DecodeFrame()
{
   // Read the information of a frame for a given sensor
   // We use extensively the structure definined by Gille Clauss
   /*
    Events Words:
    1) Header;
    2) TriggerCnt;
    3) TimeStamp;
    4) FrameCnt;
    5) DataLength;
    6) ADataW16[140]; //data MI26
    7) Trailer;
    */
   
   TAVTntuRaw*  pDatRaw = (TAVTntuRaw*)  fpNtuRaw->Object();
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   TAVTparGeo*  pGeoPar = (TAVTparGeo*)  fpGeoMap->Object();
   
   fIndex = 0;
   while (fIndex < fEventSize) {
      MI26_FrameRaw *frame = (MI26_FrameRaw*)(&fData[fIndex]);
      Int_t iSensor        = GetSensor(frame->Header);
      
      if (!CheckTriggerCnt(frame->TriggerCnt)) {
         Warning("DecodeFrame()", "Wrong trigger number %x instead of %x for sensor %d, re-synchrnizing", frame->TriggerCnt, fgCurrentTriggerCnt, iSensor);
         fgCurrentTriggerCnt = frame->TriggerCnt;
      }
      
      Int_t dataLength    = ((frame->DataLength & 0xFFFF0000)>>16);
      UShort_t *frameData = (UShort_t*)(&fData[fIndex+fgkFrameHeaderSize]);
      fIndex             += fgkFrameHeaderSize + dataLength + 1;
      dataLength         *= 2; // go to short
      
      if (ValidHistogram()) {
         fpHisEvtLength[iSensor]->Fill(frame->TriggerCnt % 1000, dataLength/2);
         if (frame->TriggerCnt % 1000 == 0) fpHisEvtLength[iSensor]->Reset();
      }
      
      if (iSensor == -1) {
         Warning("DecodeFrame()", "Wrong header key %x\n", frame->Header);
         return false;
      }
      
      // -+-+- Pointers AND LOOP to usefull data, i.e. line and states
      MI26__TStatesLine* lineStatus;
      MI26__TState*      state;
      
      Int_t index = 0;
      while( index < dataLength) { // Loop over usefull data
         // first 16 bits word is the Status/Line
         lineStatus = (MI26__TStatesLine*)frameData;
         
         if (fDebugLevel > 3)
            printf("frame %x %x #state %d Line add %d ovf %d\n", frameData[0], frameData[1], lineStatus->F.StateNb,
                   lineStatus->F.LineAddr, lineStatus->F.Ovf);
         
         frameData += 1; // goto next word
         index += 2;
         
         if( lineStatus->F.Ovf > 0 ) { // Stop when overflow
            if(fEventsOverflow%1000==0 && fReadingEvent)
               printf("WARNING : overflow while reading %d at frame %d and sensor %d, total overflow number is %d\n",
                      fReadingEvent, frame->FrameCnt, iSensor, fEventsOverflow);
            fOverflow = true;
         }
         
         if(fDebugLevel>3)
            printf("  line %d, #states %d, overflow %d, reading event  %d\n",
                   lineStatus->F.LineAddr, lineStatus->F.StateNb, lineStatus->F.Ovf, fReadingEvent);
         
         fNStatesInLine = 0;
         // Next words are the states if any
         for( Int_t iState = 0; iState < lineStatus->F.StateNb; ++iState ) { // loop over states found on sensor
            state = (MI26__TState*)frameData;
            frameData += 1; // goto next word
            index++;
            if(fDebugLevel > 3)
               printf("                  number of states %d, number of hits %d,\n", lineStatus->F.StateNb, state->F.HitNb+1);
            
            fNStatesInLine++;
            
            // A state contains HitNb+1 pixels
            // the first pixel being on the left at the column ColAddr
            for( Int_t iPixel=0; iPixel < state->F.HitNb+1; iPixel++) { // loop on pixels in the state
               
               if(fDebugLevel > 3)
                  printf("   line %3d, col %3d\n", lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
               
               // create a new pixel only if we are reading an event
               // and if the line is in the proper limit
               if (!lineStatus->F.Ovf) {
                  AddPixel(iSensor, 1, lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
                  if(fDebugLevel>3)
                     printf("sensor %d, line %d, col %d\n", iSensor, lineStatus->F.LineAddr, state->F.ColAddr+iPixel);
                  if (pDatRaw->GetPixelsN(iSensor) > pConfig->GetAnalysisPar().HitsInPlaneMaximum) return false;
                  
               }
            }
            
            if(fDebugLevel>3) 
               printf("                  state %d, #pixels %d, column %d at mem.pos %ld\n", 
                      iState, state->F.HitNb+1, state->F.ColAddr, (long int)state);
         } // end loop over states
         
      } // end loop over usefull data
   } // while still sensor to unpack
   
   return true;   
}

// --------------------------------------------------------------------------------------
void TAVTactBaseRaw::AddPixel( Int_t iSensor, Int_t value, Int_t aLine, Int_t aColumn) 
{
   // Add a pixel to the vector of pixels
   // require the following info
   // - input = number of the sensors
   // - value = analog value of this pixel
   // - line & column = position of the pixel in the matrix
   
   TAVTntuRaw* pDatRaw = (TAVTntuRaw*) fpNtuRaw->Object();
   TAVTparGeo* pGeoMap = (TAVTparGeo*) fpGeoMap->Object();
   
   TAVTntuHit* pixel   = (TAVTntuHit*)pDatRaw->NewPixel(iSensor, value, aLine, aColumn);
   
   double v = pGeoMap->GetPositionV(aLine);
   double u = pGeoMap->GetPositionU(aColumn);
   TVector3 pos(u,v,0);
   pixel->SetPosition(pos);

   
   if (ValidHistogram()) {
	  if (TAVTparConf::IsMapHistOn()) 
		 fpHisPixelMap[iSensor]->Fill(aLine, aColumn);
	  
	  fpHisRateMap[iSensor]->Fill(aColumn);
	  
	  for (Int_t i = 0; i < 4; ++i) {
		 if (aColumn >= 258*i && aColumn < (i+1)*258)
			fpHisRateMapQ[iSensor]->Fill(i+1);
	  }
   }
}


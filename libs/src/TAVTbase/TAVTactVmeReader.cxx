/*!
  \file
  \brief   Implementation of TAVTactVmeReader.
*/

#include "TH2F.h"

#include "TAGroot.hxx"
#include "TAVTparConf.hxx"
#include "TAVTparGeo.hxx"
#include "TAVTdatRaw.hxx"
#include "TAVTactVmeReader.hxx"

/*!
  \class TAVTactVmeReader 
  \brief Reader action for ascii files M26-28 (Catania Test). **
   (binary file never been used)
*/

ClassImp(TAVTactVmeReader);


const UInt_t TAVTactVmeReader::fgkKeyHeader[]      = {0x8001, 0x8002, 0x8003, 0x8004, 0x8005, 0x8006, 0x8007, 0x8008};
const Int_t  TAVTactVmeReader::fgkFrameHeaderSize  = 5;
const Int_t  TAVTactVmeReader::fgkLineWidth        = 9;
const UInt_t TAVTactVmeReader::fgkEventHeader      = 0xfafafafa;
const UInt_t TAVTactVmeReader::fgkEventTail        = 0xabcdabcd;
const UInt_t TAVTactVmeReader::fgkFrameHeader      = 0x80088007;
const UInt_t TAVTactVmeReader::fgkFrameTail        = 0xaaa8aaa7;
const UInt_t TAVTactVmeReader::fgkKeyTail[]        = {0xaaa1, 0xaaa2, 0xaaa3, 0xaaa4, 0xaaa5, 0xaaa6, 0xaaa7, 0xaaa8};


     TString TAVTactVmeReader::fgDefaultFolderName = "run_";
     TString TAVTactVmeReader::fgDefaultExtName    = ".dat";
      UInt_t TAVTactVmeReader::fgCurrentTriggerCnt = 0;

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTactVmeReader::TAVTactVmeReader(const char* name, TAGdataDsc* pDatRaw, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAVTactBaseRaw(name, pDatRaw, pGeoMap, pConfig),
  fData(0x0),
  fIndex(0),
  fReadingEvent(0),
  fRunNumber(-1),
  fOverflow(0),
  fEventsOverflow(0), 
  fNStatesInLine(0)
{
   SetTitle("TAVTactVmeReader - reader for VME reader");
   fBaseName ="FIFOdata_ch";
   
   TAVTparGeo* parGeo = (TAVTparGeo*) fpGeoMap->Object();
   Int_t size = parGeo->GetNSensors()*sizeof(MI26_FrameRaw);
   fData      = new UInt_t[size];
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactVmeReader::~TAVTactVmeReader()
{
   delete fData;
}

//------------------------------------------+-----------------------------------
//! Open ascii data sources.
Int_t TAVTactVmeReader::Open(const TString& name, Option_t* opt)
{
   TString inputFileName;
   
   static Bool_t first = true;
   if (first)
	  SetRunNumber(name);
   else 
	  first = false;
	
   Bool_t valid = true;
   Bool_t isOk  = false;
   TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();

   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
     Int_t planeStatus = pConfig->GetStatus(i);
	  if (planeStatus == -1) continue;
	  
	  // Close any previous open file
	  if( fRawFileAscii[i].is_open() && !fRawFileAscii[i].eof()) {
		 isOk = true;
	  } else {
		 fRawFileAscii[i].close();
		 inputFileName = Form("%s/%s%04d/%s%d%s", fPrefixName.Data(), fgDefaultFolderName.Data(), fRunNumber, fBaseName.Data(), i, fgDefaultExtName.Data());
		 fRawFileAscii[i].open(inputFileName.Data());
		 if( fRawFileAscii[i].fail() ) { // end the reading if file opening failed
			cout << endl << "TAVTactVmeReader::Open(), cannot open file " << inputFileName.Data() << endl;
			isOk = false;
		 } else {
			isOk = true;
		 }
	  }
	  valid &= isOk;
   }
   return valid;
}

//------------------------------------------+-----------------------------------
//! Close input file.
void TAVTactVmeReader::Close()
{
   TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i)
	  fRawFileAscii[i].close();
}

//------------------------------------------+-----------------------------------
//! Process
Bool_t TAVTactVmeReader::Process()
{
   if (Valid()) return kTRUE;
   if (IsZombie()) return kFALSE;
   
   if (!FetchEvent()) {
	  SetBit(kEof);
	  SetBitAllDataOut(kEof);
	  return false;
   } else {
	  SetBit(kValid);
   }
   
   DecodeFrame();
   
   fpDatRaw->SetBit(kValid);

   return true;
}

//------------------------------------------+-----------------------------------
//! Fetch next event
Bool_t TAVTactVmeReader::FetchEvent()
{
   TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   
   fIndex = 0;
   for (Int_t iSensor = 0; iSensor < pGeoMap->GetNSensors(); ++iSensor) {
	  Int_t planeStatus = pConfig->GetStatus(iSensor);
	  if (planeStatus == -1) continue;	
	  
     if (!GetEventHeader(iSensor)) return false;

	  if (!GetStart(iSensor)) return false;

	  MI26_FrameRaw* data = new MI26_FrameRaw;
	  GetFrame(iSensor, data);
	  UInt_t trigger = data->TriggerCnt;
	  GetNextFrames(iSensor, trigger);
	  delete data;
   }   
   
   if(fDebugLevel>3)
	  for (Int_t i = 0; i < fEventSize; ++i) 
		 printf("Data %x\n", fData[i]);
   
   return true;
}

// private method
// --------------------------------------------------------------------------------------
Bool_t TAVTactVmeReader::GetEventHeader(Int_t iSensor)
{
   Char_t tmp[GetLineWidth()];
   
   do {
      fRawFileAscii[iSensor] >> tmp;
      TString line = tmp;
      TString key  = Form("%x", fgkEventHeader);
      if (line.Contains(key)) {
         fRawFileAscii[iSensor] >> fEventNumber;
         return true;
      }
   } while (!fRawFileAscii[iSensor].eof());
   
   return false;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactVmeReader::GetStart(Int_t iSensor) 
{
   Char_t tmp[GetLineWidth()];

   do {
	  fRawFileAscii[iSensor] >> tmp;
	  TString line = tmp;
	  TString key  = Form("%x", fgkFrameHeader);
	  if (line.Contains(key)) {
		 return true;
	  }
   } while (!fRawFileAscii[iSensor].eof());
   return false;
}

// --------------------------------------------------------------------------------------
void TAVTactVmeReader::GetNextFrames(Int_t iSensor, UInt_t trigger) 
{
   MI26_FrameRaw* data = new MI26_FrameRaw;
   Bool_t sameTrigger = true;
   Int_t index = 0;
   do {
	  index = fIndex;
	  GetStart(iSensor);
	  GetFrame(iSensor, data);
	  if (data->TriggerCnt != trigger) {
		 sameTrigger = false;
		 break;
	  }
   } while(sameTrigger);
   
   fRawFileAscii[iSensor].seekg((index-fIndex-1)*GetLineWidth(), ios::cur); // length of line = 9 char
   fIndex = index;
   fEventSize = fIndex;
   delete data;
}

// --------------------------------------------------------------------------------------
void TAVTactVmeReader::GetFrame(Int_t iSensor, MI26_FrameRaw* data)
{
   Char_t tmp[GetLineWidth()];
   TString header = Form("%x%x",GetKeyHeader(iSensor), GetKeyHeader(iSensor));
   sscanf(header.Data(), "%x", &fData[fIndex++]);
   for (UInt_t i = 0; i < 4; ++i) {
	  fRawFileAscii[iSensor] >> tmp;
	  sscanf(tmp, "%x", &fData[fIndex++]);
   }
   
   memcpy(data, &fData[fIndex-GetHeaderSize()], GetHeaderSize()*4);
   
   // !!!!!!!! pb with sensor 3 & 7
   //dataLength  = ( ((data->DataLength & 0xFFFF0000)>>16) + (data->DataLength & 0x0000FFFF) )/2.;
   UInt_t dataLength  =  ((data->DataLength & 0xFFFF0000)>>16);
   
   for (UInt_t i = 0; i < dataLength; ++i) {
	  fRawFileAscii[iSensor] >> tmp;
	  sscanf(tmp, "%x", &fData[fIndex++]);
   }
   fData[fIndex++] = ((GetDataTail(iSensor) << 16) & 0xFFFF0000)  |  GetDataTail(iSensor);
}


// --------------------------------------------------------------------------------------
Int_t TAVTactVmeReader::GetSensor(UInt_t key)
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
Bool_t TAVTactVmeReader::DecodeFrame() 
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
   
   TAVTdatRaw* pDatRaw  = (TAVTdatRaw*) fpDatRaw->Object();
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   TAVTparGeo*  pGeoPar = (TAVTparGeo*) fpGeoMap->Object();

   fIndex = 0;
   while (fIndex < fEventSize) {
	  MI26_FrameRaw *frame = (MI26_FrameRaw*)(&fData[fIndex]);
	  Int_t iSensor        = GetSensor(frame->Header);
      
	  if (!CheckTriggerCnt(frame->TriggerCnt)) {
		 Warning("DecodeFrame()", "Wrong trigger number %x instead of %x for sensor %d, re-synchrnizing\n", frame->TriggerCnt, fgCurrentTriggerCnt, iSensor);
       fgCurrentTriggerCnt = frame->TriggerCnt;
        return true;
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
void TAVTactVmeReader::SetRunNumber(const TString& filename)
{
   TString name(filename);
   Int_t pos1 = name.Last('/');
   Int_t pos2 = name.First("_");
   Int_t len  = name.Length();
   
   fPrefixName = name(0, pos1);
   
   TString tmp = name(pos2+1, len-pos2);
   fRunNumber = tmp.Atoi();
   gTAGroot->SetRunNumber(fRunNumber);
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactVmeReader::CheckTriggerCnt(UInt_t trig)
{
   if (fgCurrentTriggerCnt != trig)
      return false;
   
   return true;
}


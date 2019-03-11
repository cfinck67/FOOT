/*!
  \file
  \brief   Implementation of TAVTactVmeReader.
*/

#include "DECardEvent.hh"

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

TString TAVTactVmeReader::fgDefaultFolderName = "run_";
TString TAVTactVmeReader::fgDefaultExtName    = ".dat";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTactVmeReader::TAVTactVmeReader(const char* name, TAGdataDsc* pDatRaw, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAVTactBaseRaw(name, pDatRaw, pGeoMap, pConfig),
  fRunNumber(-1)
{
   SetTitle("TAVTactVmeReader - reader for VME reader");
   fBaseName ="FIFOdata_ch";
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactVmeReader::~TAVTactVmeReader()
{
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
   
   fpNtuRaw->SetBit(kValid);

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
      TString key  = Form("%x", DECardEvent::GetVertexHeader());
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

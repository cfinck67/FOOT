/*!
  \file
  \version $Id: TAITactAscReader.cxx,v 1.12 2003/07/08 18:54:19 mueller Exp $
  \brief   Implementation of TAITactAscReader.
*/

#include "TAGroot.hxx"
#include "TAITparConf.hxx"
#include "TAITactBaseRaw.hxx"
#include "TAITactAscReader.hxx"

/*!
  \class TAITactAscReader 
  \brief Reader action for ascii files (Catania format). **
*/

ClassImp(TAITactAscReader);

TString TAITactAscReader::fgDefaultBaseName    = "FIFOdata_ch";
TString TAITactAscReader::fgDefaultExtName     = ".dat";
TString TAITactAscReader::fgDefaultFolderName  = "run_";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITactAscReader::TAITactAscReader(const char* name, TAGdataDsc* pAscEvent, TAGparaDsc* pConfig)
: TAGactionFile(name, "TAITactAscReader - ascii file reader", "READ"),
  fpAscEvent(pAscEvent),
  fpConfig(pConfig),
  fData(0x0),
  fIndex(0),
  fEventSize(0),
  fRunNumber(-1),
  fPrefixName(),
  fDebugLevel(0)
{
   AddPara(pConfig, "TAITparConf");
   AddDataOut(pAscEvent, "TAITrawEvent");
   TAITparConf* config = (TAITparConf*) fpConfig->Object();
   Int_t size = config->GetSensorsN()*sizeof(MI26_FrameRaw);
   fData      = new UInt_t[size];
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactAscReader::~TAITactAscReader()
{
   delete fData;
}

//------------------------------------------+-----------------------------------
//! Open ascii data sources.
Int_t TAITactAscReader::Open(const TString& name, Option_t* opt)
{
   TString inputFileName;
   
   static Bool_t first = true;
   if (first)
	  SetRunNumber(name);
   else 
	  first = false;
	  
   Bool_t valid = true;
   Bool_t isOk  = false;
   TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();

   for (Int_t i = 0; i < pConfig->GetSensorsN(); ++i) {
	  Int_t planeStatus = pConfig->GetSensorPar(i).Status;
	  if (planeStatus == -1) continue;
	  
	  // Close any previous open file
	  if( fRawFileAscii[i].is_open() && !fRawFileAscii[i].eof()) {
		 isOk = true;
	  } else {
		 fRawFileAscii[i].close();
		 inputFileName = Form("%s/%s%04d/%s%d%s", fPrefixName.Data(), fgDefaultFolderName.Data(), fRunNumber, fgDefaultBaseName.Data(), i, fgDefaultExtName.Data());
		 fRawFileAscii[i].open(inputFileName.Data());
		 if( fRawFileAscii[i].fail() ) { // end the reading if file opening failed
			cout << endl << "TAITactAscReader::Open(), cannot open file " << inputFileName.Data() << endl;
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
void TAITactAscReader::Close()
{
   TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();
   for (Int_t i = 0; i < pConfig->GetSensorsN(); ++i) 
	  fRawFileAscii[i].close();
}

//------------------------------------------+-----------------------------------
//! Returns \a true if an input file or connection is open.
Bool_t TAITactAscReader::IsOpen() const
{
   return true;
}

//------------------------------------------+-----------------------------------
//! Process
Bool_t TAITactAscReader::Process()
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
   
   TAITrawEvent* pAscEvent = (TAITrawEvent*) fpAscEvent->Object();
   pAscEvent->AddEvent(fEventSize, fData);
   
   fpAscEvent->SetBit(kValid);

   return true;
}

//------------------------------------------+-----------------------------------
//! Fetch next event
Bool_t TAITactAscReader::FetchEvent()
{
   TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();
   
   fIndex = 0;
   for (Int_t iSensor = 0; iSensor < pConfig->GetSensorsN(); ++iSensor) {
	  Int_t planeStatus = pConfig->GetSensorPar(iSensor).Status;
	  if (planeStatus == -1) continue;	
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
Bool_t TAITactAscReader::GetStart(Int_t iSensor) 
{
   Char_t tmp[TAITactBaseRaw::GetLineWidth()];
   
   do {
	  fRawFileAscii[iSensor] >> tmp;
	  TString line = tmp;
	  TString key  = Form("%x%x",TAITactBaseRaw::GetKeyHeader(iSensor), TAITactBaseRaw::GetKeyHeader(iSensor));
	  if (line.Contains(key)) {
		 return true;
	  }
   } while (!fRawFileAscii[iSensor].eof());
   return false;
}

// --------------------------------------------------------------------------------------
void TAITactAscReader::GetNextFrames(Int_t iSensor, UInt_t trigger) 
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
   
   fRawFileAscii[iSensor].seekg((index-fIndex-1)*TAITactBaseRaw::GetLineWidth(), ios::cur); // length of line = 9 char
   fIndex = index;
   fEventSize = fIndex;
   delete data;
}

// --------------------------------------------------------------------------------------
void TAITactAscReader::GetFrame(Int_t iSensor, MI26_FrameRaw* data)
{
   Char_t tmp[TAITactBaseRaw::GetLineWidth()];
   TString header = Form("%x%x",TAITactBaseRaw::GetKeyHeader(iSensor), TAITactBaseRaw::GetKeyHeader(iSensor));
   sscanf(header.Data(), "%x", &fData[fIndex++]);
   for (UInt_t i = 0; i < 4; ++i) {
	  fRawFileAscii[iSensor] >> tmp;
	  sscanf(tmp, "%x", &fData[fIndex++]);
   }

   memcpy(data, &fData[fIndex-TAITactBaseRaw::GetHeaderSize()], TAITactBaseRaw::GetHeaderSize()*4);
   
   // !!!!!!!! pb with sensor 3 & 7
   //dataLength  = ( ((data->DataLength & 0xFFFF0000)>>16) + (data->DataLength & 0x0000FFFF) )/2.;
   UInt_t dataLength  =  ((data->DataLength & 0xFFFF0000)>>16);
   
   for (UInt_t i = 0; i < dataLength; ++i) {
	  fRawFileAscii[iSensor] >> tmp;
	  sscanf(tmp, "%x", &fData[fIndex++]);
   }
   fData[fIndex++] = TAITactBaseRaw::GetTailHeader();
}

//------------------------------------------+-----------------------------------
void TAITactAscReader::SetRunNumber(const TString& filename)
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



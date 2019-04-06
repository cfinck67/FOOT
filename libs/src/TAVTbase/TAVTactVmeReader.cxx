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
TString TAVTactVmeReader::fgDefaultExtName    = ".ZS";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTactVmeReader::TAVTactVmeReader(const char* name, TAGdataDsc* pDatRaw, TAGparaDsc* pGeoMap, TAGparaDsc* pConfig)
: TAVTactBaseRaw(name, pDatRaw, pGeoMap, pConfig),
  fRunNumber(-1)
{
   SetTitle("TAVTactVmeReader - reader for VME reader");
   fBaseName ="data_FPGA_Mouser993P0160_V1_ch";
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
		 inputFileName = Form("%s/%s%04d/800%d_%s%d%s", fPrefixName.Data(), fgDefaultFolderName.Data(), fRunNumber, i, fBaseName.Data(), i, fgDefaultExtName.Data());
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
   MI26_FrameRaw* data = new MI26_FrameRaw;
   
   TAVTparGeo*  pGeoMap = (TAVTparGeo*)  fpGeoMap->Object();
   TAVTparConf* pConfig = (TAVTparConf*) fpConfig->Object();
   
   // loop over boards
   for (Int_t i = 0; i < pGeoMap->GetNSensors(); ++i) {
      Int_t planeStatus = pConfig->GetStatus(i);
      if (planeStatus == -1) continue;
      
      if (GetSensorHeader(i)) {
         
         fFirstFrame = true;
         // loop over frame (3 max)
         while (GetFrame(i, data)) {
            DecodeFrame(i, data);
         }
      } else {
         SetBit(kEof);
         SetBitAllDataOut(kEof);
      }
   }

   delete data;

   SetBit(kValid);
   fpNtuRaw->SetBit(kValid);

   fPrevEventNumber   = fEventNumber;
   fPrevTriggerNumber = fTriggerNumber;
   fPrevTimeStamp     = fTimeStamp;
   
   return true;
}

// private method

// --------------------------------------------------------------------------------------
Bool_t TAVTactVmeReader::GetSensorHeader(Int_t iSensor)
{
   Char_t tmp[255];

   do {
      fRawFileAscii[iSensor] >> tmp;
      TString line = tmp;
      TString key  = Form("%x", GetKeyHeader(iSensor));
      if (line.Contains(key)) {
         if (fDebugLevel > 0)
            printf("sensor header %s %d\n", tmp, (int) fRawFileAscii[iSensor].tellg()/9+1);
         
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fEventNumber);
         
         // fake trigger
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fTriggerNumber);

         // fake time stamp
         fRawFileAscii[iSensor] >> tmp;
         
         FillHistoEvt(iSensor);

         return true;
      }
   } while (!fRawFileAscii[iSensor].eof());
   

   return false;
}

// --------------------------------------------------------------------------------------
Bool_t TAVTactVmeReader::GetFrame(Int_t iSensor, MI26_FrameRaw* data)
{
   Char_t tmp[255];
   fIndex = 0;
   
   // check frame header
   fRawFileAscii[iSensor] >> tmp;
   TString line = tmp;
   TString key  = Form("%x", (GetFrameHeader() & 0xFFF)); // protection against wrong header !!!

   if (line.Contains(key)) {
      TString key1  = Form("%x", GetFrameHeader() );
      sscanf(key1.Data(), "%x", &fData[fIndex++]);
      
      for (UInt_t i = 0; i < 4; ++i) {
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fData[fIndex++]);
      }
      
      UInt_t dataLength  =  ((fData[fIndex-1] & 0xFFFF0000)>>16);
      for (UInt_t i = 0; i < dataLength; ++i) {
         fRawFileAscii[iSensor] >> tmp;
         sscanf(tmp, "%x", &fData[fIndex++]);
      }
      
      memcpy(data, &fData[0], sizeof(MI26_FrameRaw));
      FillHistoFrame(data);
      
   } else {
      return false;;
   }
   
   // go to frame trailer
   do {
      fRawFileAscii[iSensor] >> tmp;
      TString line = tmp;
      TString key  = Form("%x", (GetFrameTail() & 0xFFF));
      sscanf(tmp, "%x", &fData[fIndex++]);
      if (line.Contains(key))
         break;

   } while (!fRawFileAscii[iSensor].eof());
   
   fEventSize = fIndex;
   
   if(fDebugLevel>3)
      for (Int_t i = 0; i < fEventSize; ++i)
         printf("Data %08x\n", fData[i]);

   return true;;
   
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

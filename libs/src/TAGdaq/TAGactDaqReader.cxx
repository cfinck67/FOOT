

#include "TAGactDaqReader.hxx"

ClassImp(TAGactDaqReader);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAGactDaqReader::TAGactDaqReader(const char* name)
  : TAGactionFile(name, "TAGactDaqReader - DAQ file reader", "READ")
{
   fDaqFileReader = new EventReader();
   fDaqEvent      = new TAGdaqEvent();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAGactDaqReader::~TAGactDaqReader()
{
   delete fDaqFileReader;
   delete fDaqEvent;
}

//------------------------------------------+-----------------------------------
//! Open data source.
Int_t TAGactDaqReader::Open(const TString& name, Option_t*)
{
   Int_t b_bad = 0;
   
   std::string filename ( name.Data() );
   fDaqFileReader->openFile( filename );
   if ( !fDaqFileReader->getIsOpened() ) {
      Error("Open()", "Cannot open file %s", name.Data());
      b_bad = -1;
   }
   
   if( !fDaqFileReader->endOfFileReached() ) {
      fDaqFileReader->readFileHeader();
      fDaqFileHeader = fDaqFileReader->getFileHeader();
      fDaqFileHeader->printData();
   }

  return b_bad;
}

//------------------------------------------+-----------------------------------
//! Close input file.
void TAGactDaqReader::Close()
{
   fDaqFileReader->closeFile();
}

//------------------------------------------+-----------------------------------
//! Returns \a true if an input file or connection is open.
Bool_t TAGactDaqReader::IsOpen() const
{
   return fDaqFileReader->getIsOpened();
}

//------------------------------------------+-----------------------------------
//! Process Reader.
Bool_t TAGactDaqReader::Process()
{
  if (Valid()) return kTRUE;
  if (IsZombie()) return kFALSE;

   fDaqEvent->Clear();
   
   fDaqFileReader->getNextEvent();

   // Global event information
   InfoEvent* evInfo = fDaqFileReader->getInfoEvent();
   fDaqEvent->SetInfoEvent(evInfo);
   
   // Trigger data
   TrgEvent*  evTrg  = fDaqFileReader->getTriggerEvent();
   fDaqEvent->SetTrgEvent(evTrg);

   // TDC # 0 and # 1 as an example
   const TDCEvent* evTDC0 = static_cast<const TDCEvent*>(fDaqFileReader->getFragmentID(dataV1190 | 0x30));
   if (evTDC0)
      fDaqEvent->AddFragment(evTDC0);
   
   const TDCEvent* evTDC1 = static_cast<const TDCEvent*>(fDaqFileReader->getFragmentID(dataV1190 | 0x31));
   if (evTDC1)
      fDaqEvent->AddFragment(evTDC1);

   // vertex
   const DECardEvent* evVTX = static_cast<const DECardEvent*>(fDaqFileReader->getFragmentID(dataVTX));
   if (evVTX)
      fDaqEvent->AddFragment(evVTX);
   
   // WD for ST and TW (marker is hard coded !!!)
   const WDEvent* evWD = static_cast<const WDEvent*>(fDaqFileReader->getFragmentID(0x0463230));
   if (evWD)
      fDaqEvent->AddFragment(evWD);
   
   if (fDaqFileReader->endOfFileReached())
      return false;
   
   if( fDaqFileReader->check())
      return false;
   
  return true;
}


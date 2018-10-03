

#include "TAGactDaqReader.hxx"

ClassImp(TAGactDaqReader);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGactDaqReader::TAGactDaqReader(const char* name)
  : TAGactionFile(name, "TAGactDaqReader - DAQ file reader", "READ")
{
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGactDaqReader::~TAGactDaqReader()
{
}

//------------------------------------------+-----------------------------------
//! Add input data descriptor.

void TAGactDaqReader::SetupChannel(TAGdataDsc* /*p_data*/)
{
}

//------------------------------------------+-----------------------------------
//! Open MBS data source.

Int_t TAGactDaqReader::Open(const TString& /*name*/, Option_t*)
{
   Int_t b_bad = 0;
   
  return b_bad;
}

//------------------------------------------+-----------------------------------
//! Close input file.

void TAGactDaqReader::Close()
{
}

//------------------------------------------+-----------------------------------
//! Returns \a true if an input file or connection is open.

Bool_t TAGactDaqReader::IsOpen() const
{
   return false;
}

//------------------------------------------+-----------------------------------
//! Process MbsReader.

Bool_t TAGactDaqReader::Process()
{
  if (Valid()) return kTRUE;
  if (IsZombie()) return kFALSE;

  return Valid();
}


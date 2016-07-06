/*!
  \file
  \version $Id: TAGconfigFile.cxx,v 1.1 2003/06/15 18:22:01 mueller Exp $
  \brief   Implementation of TAGconfigFile.
*/

#include "TSystem.h"
#include "TString.h"

#include "TAGconfigFile.hxx"

/*!
  \class TAGconfigFile TAGconfigFile.hxx "TAGconfigFile.hxx"
  \brief Configuration file reader. **
*/

ClassImp(TAGconfigFile);

//------------------------------------------+-----------------------------------
//! Constructor.

TAGconfigFile::TAGconfigFile()
  : TAGnamed("TAGmbsIStream","<closed>")
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGconfigFile::~TAGconfigFile()
{}

//------------------------------------------+-----------------------------------
//! Open file with name \a name .

Bool_t TAGconfigFile::Open(const TString& name)
{
  Close();

  TString name_exp = name;
  gSystem->ExpandPathName(name_exp);

  fStream.open(name_exp.Data());
  if (!fStream) {
    Error("Open()", "failed to open file '%s'", name_exp.Data());
    return kTRUE;
  }

  fLine = "";
  SetName(name_exp);
  return kFALSE;
}

//------------------------------------------+-----------------------------------
//! Close file.

void TAGconfigFile::Close()
{
  fStream.close();
  fLine = "";
  SetName("<closed>");
  return;
}

//------------------------------------------+-----------------------------------
//! Read next line.

//  !!! check and improve handling when last line has no final \n !!!

Bool_t TAGconfigFile::NextLine()
{
  if (!fStream.good()) return kFALSE;
  TString line;

  while(1) {
    line.ReadLine(fStream, kFALSE);
    if (!fStream.fail()) {
      fLine = line;
    } else {
      fLine = "";
    }
    if (fStream.fail()) break;
    if (fLine.Length() > 0 && ! fLine.BeginsWith("#")) break;
  }

  return !fStream.fail();
}

//------------------------------------------+-----------------------------------
//! ostream insertion.

void TAGconfigFile::ToStream(ostream& os, Option_t* option) const
{
  os << "TAGconfigFile " << GetName() << endl;
  return;
}

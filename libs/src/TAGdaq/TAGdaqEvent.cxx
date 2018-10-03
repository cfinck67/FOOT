/*!
  \file
  \version $Id: TAGdaqEvent.cxx,v 1.6 2003/06/19 17:53:28 mueller Exp $
  \brief   Implementation of TAGdaqEvent.
*/

#include <string.h>

#include "TString.h"

#include "TAGdaqEvent.hxx"

/*!
  \class TAGdaqEvent TAGdaqEvent.hxx "TAGdaqEvent.hxx"
  \brief Representation of MBS raw data event. **
*/

ClassImp(TAGdaqEvent);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGdaqEvent::TAGdaqEvent()
{
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGdaqEvent::~TAGdaqEvent()
{
}


//------------------------------------------+-----------------------------------
//! Returns \c true if object needs automatic delete when read from tree.

Bool_t TAGdaqEvent::NeedAutoDelete() const
{
  return kFALSE;
}

///------------------------------------------+-----------------------------------
//! Clear event.

void TAGdaqEvent::Clear(Option_t*)
{
  TAGdata::Clear();
  fData.clear();
  fOffset.clear();
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGdaqEvent::ToStream(ostream& /*os*/, Option_t* /*option*/) const
{
}

/*------------------------------------------+---------------------------------*/
//! Setup sub event offset vector

void TAGdaqEvent::SetupOffset()
{
  fOffset.clear();
  Int_t i_offset = 0;
  while (i_offset < (Int_t)fData.size()) {
    fOffset.push_back(i_offset);
    i_offset += fData[i_offset] + 3;
  }
}


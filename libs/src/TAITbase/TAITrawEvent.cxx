/*!
  \file
  \version $Id: TAITrawEvent.cxx $
  \brief   Implementation of TAITrawEvent.
*/

#include <string.h>

#include "TString.h"

#include "TAITrawEvent.hxx"

/*!
  \class TAITrawEvent TAITrawEvent.hxx "TAITrawEvent.hxx"
  \brief Representation of ascii raw data event. **
*/

ClassImp(TAITrawEvent);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITrawEvent::TAITrawEvent()
: TAGdata(),
  fData(0x0),
  fEventTrigger(0),
  fEventSize(0)
{
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITrawEvent::~TAITrawEvent()
{
   delete fData;
}

//------------------------------------------+-----------------------------------
//! Add a sub event.
void TAITrawEvent::AddEvent(Int_t size, const UInt_t data[])
{
   fEventSize = size;
   
   if (fData) 
	  delete fData;
   fData = new UInt_t[size];

   memcpy(fData, data, size*sizeof(UInt_t));
}

//------------------------------------------+-----------------------------------
//! Returns \c true if object needs automatic delete when read from tree.
Bool_t TAITrawEvent::NeedAutoDelete() const
{
   return kFALSE;
}

///------------------------------------------+-----------------------------------
//! Clear event.
void TAITrawEvent::Clear(Option_t*)
{
   TAGdata::Clear();
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAITrawEvent::ToStream(ostream& os, Option_t* option) const
{
   return;
}


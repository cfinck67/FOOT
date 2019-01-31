/*!
  \file
  \version $Id: TAGdaqEvent.cxx,v 1.6 2003/06/19 17:53:28 mueller Exp $
  \brief   Implementation of TAGdaqEvent.
*/

#include "TAGdaqEvent.hxx"

/*!
  \class TAGdaqEvent TAGdaqEvent.hxx "TAGdaqEvent.hxx"
  \brief Representation of raw data event. **
*/

ClassImp(TAGdaqEvent);

//------------------------------------------+-----------------------------------
//! Default constructor.
TAGdaqEvent::TAGdaqEvent()
 : TAGdata(),
   fInfoEvent(0x0),
   fTrgEvent(0x0)
{
   fListOfFragments.clear();
   fListOfClassTypes.clear();
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGdaqEvent::~TAGdaqEvent()
{
}

//------------------------------------------+-----------------------------------
//! Add fragment
void TAGdaqEvent::AddFragment(const BaseFragment* frag)
{
   fListOfFragments.push_back(frag);
   
   string type = frag->classType();
   fListOfClassTypes.push_back(type);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAGdaqEvent::Clear(Option_t*)
{
   TAGdata::Clear();
   fListOfFragments.clear();
   fListOfClassTypes.clear();
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAGdaqEvent::ToStream(ostream& /*os*/, Option_t* /*option*/) const
{
}


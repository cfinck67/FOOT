/*!
  \file
  \version $Id: TAGnamed.cxx,v 1.4 2003/06/09 18:39:32 mueller Exp $
  \brief   Implementation of TAGnamed.
*/

#include "TClass.h"

#include "TAGnamed.hxx"

/*!
  \class TAGnamed TAGnamed.hxx "TAGnamed.hxx"
  \brief Mother of all named TAG objects. **
*/

ClassImp(TAGnamed);

//------------------------------------------+-----------------------------------
//! Destructor.

TAGnamed::~TAGnamed()
{}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGnamed::ToStream(ostream& os, Option_t* option) const
{
  os <<"TAGnamed: " << IsA()->GetName() << "  " << GetName() 
     << "  " << GetTitle() << endl;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGnamed::Print(Option_t* option) const
{
  ToStream(cout, option);
  return;
}


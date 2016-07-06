/*!
  \file
  \version $Id: TAGobject.cxx,v 1.4 2003/06/09 18:40:09 mueller Exp $
  \brief   Implementation of TAGobject.
*/

#include "TClass.h"

#include "TAGobject.hxx"

/*!
  \class TAGobject TAGobject.hxx "TAGobject.hxx"
  \brief Mother of most unnamed TAG objects. **
*/

ClassImp(TAGobject);

//------------------------------------------+-----------------------------------
//! Destructor.

TAGobject::~TAGobject()
{}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGobject::ToStream(ostream& os, Option_t* option) const
{
  os <<"TAGobject: " << IsA()->GetName() << "  " << GetName() 
     << "  " << GetTitle() << endl;
  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGobject::Print(Option_t* option) const
{
  ToStream(cout, option);
  return;
}


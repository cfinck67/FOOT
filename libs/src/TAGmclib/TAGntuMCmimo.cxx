/*!
  \file
  \version $Id: TATntuRaw.cxx,v 1.10 2003/06/22 11:48:38 mueller Exp $
  \brief   Implementation of TATntuRaw.
*/

#include <bitset>
using namespace std;

#include "TString.h"

#include "TAGntuMCmimo.hxx"

/*!
  \class TAGntuMCmimoHit TAGntuMCmimo.hxx "TAGntuMCmimo.hxx"
  \brief Ntuplize ToF raw data - hit object. **
*/

ClassImp(TAGntuMCmimoHit);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuMCmimoHit::TAGntuMCmimoHit()
  : id(-100000)
{
  inpos.SetXYZ(-100.,-100.,-100);
  ip.SetXYZ(0.,0.,0);
}

//------------------------------------------+-----------------------------------
//! Construct with data

// VM changed 3/11/13 (energy loss de passed)
TAGntuMCmimoHit::TAGntuMCmimoHit(Int_t i_id, TVector3 i_ipos, 
				 TVector3 i_imom, Double_t d)
  : id(i_id)
{
  inpos.SetXYZ(i_ipos.X(),i_ipos.Y(),i_ipos.Z());
  ip.SetXYZ(i_imom.X(),i_imom.Y(),i_imom.Z());
  de=d;
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuMCmimoHit::~TAGntuMCmimoHit()
{}

//------------------------------------------+-----------------------------------
TVector3 TAGntuMCmimoHit::InitPos()
{
  return inpos;
}


TVector3 TAGntuMCmimoHit::InitP()
{
  return ip;
}

Int_t  TAGntuMCmimoHit::ID()
{
  return id;
}

// VM added 3/11/13
Double_t  TAGntuMCmimoHit::DE()
{
  return de;
}

//------------------------------------------+-----------------------------------
void TAGntuMCmimoHit::SetInitPos(TVector3 pos)
{
  inpos = pos;
  return;
}

void TAGntuMCmimoHit::SetInitP(TVector3 mom)
{
  ip = mom;
  return;
}

void TAGntuMCmimoHit::SetID(int aid)
{
  id =aid;
  return;
}

// VM added 3/11/13
void TAGntuMCmimoHit::SetDE(Double_t d)
{
  de =d;
  return;
}

//##############################################################################

/*!
  \class TAGntuMCmimo TAGntuMCmimo.hxx "TAGntuMCmimo.hxx"
  \brief Ntuplize ToF raw data - container. **
*/

ClassImp(TAGntuMCmimo);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuMCmimo::TAGntuMCmimo()
  : nhit(0),
    h(0)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuMCmimo::~TAGntuMCmimo()
{
  delete h;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAGntuMCmimo::SetupClones()
{
  if (!h) h = new TClonesArray("TAGntuMCmimoHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear.

void TAGntuMCmimo::Clear(Option_t*)
{

  TAGdata::Clear();

  nhit  = 0;
  h->Delete();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGntuMCmimo::ToStream(ostream& os, Option_t* option) const
{
  os << "TAGntuMCmimo" 
     << Form("  nhit=%3d", nhit) 
     << endl;

  if (nhit == 0) return;
  
  os << "ind slat stat  adct  adcb  tdct  tdcb" << endl;
  
  for (Int_t i = 0; i < nhit; i++) {
    const TAGntuMCmimoHit* p_hit = Hit(i);
    os << Form("%3d %4lf", i, p_hit->id)
       << endl;
  }

  return;
}


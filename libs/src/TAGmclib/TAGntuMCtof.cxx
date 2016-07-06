/*!
  \file
  \version $Id: TATntuRaw.cxx,v 1.10 2003/06/22 11:48:38 mueller Exp $
  \brief   Implementation of TATntuRaw.
*/

#include <bitset>
using namespace std;

#include "TString.h"

#include "TAGntuMCtof.hxx"

/*!
  \class TAGntuMCtofHit TAGntuMCtof.hxx "TAGntuMCtof.hxx"
  \brief Ntuplize ToF raw data - hit object. **
*/

ClassImp(TAGntuMCtofHit);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuMCtofHit::TAGntuMCtofHit()
  : id(-100000), tof(-10.)
{
  inpos.SetXYZ(-100.,-100.,-100);
  ip.SetXYZ(0.,0.,0);
}

//------------------------------------------+-----------------------------------
//! Construct with data

TAGntuMCtofHit::TAGntuMCtofHit(Int_t i_id, TVector3 i_ipos, 
			       TVector3 i_imom, Double_t i_tof)
  : id(i_id), tof(i_tof)
{
  inpos.SetXYZ(i_ipos.X(),i_ipos.Y(),i_ipos.Z());
  ip.SetXYZ(i_imom.X(),i_imom.Y(),i_imom.Z());
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuMCtofHit::~TAGntuMCtofHit()
{}

//------------------------------------------+-----------------------------------
TVector3 TAGntuMCtofHit::InitPos()
{
  return inpos;
}

TVector3 TAGntuMCtofHit::InitP()
{
  return ip;
}

Int_t  TAGntuMCtofHit::ID()
{
  return id;
}

Double_t  TAGntuMCtofHit::GetTof()
{
  return tof;
}

//------------------------------------------+-----------------------------------
void TAGntuMCtofHit::SetInitPos(TVector3 pos)
{
  inpos = pos;
  return;
}

void TAGntuMCtofHit::SetInitP(TVector3 mom)
{
  ip = mom;
  return;
}

void TAGntuMCtofHit::SetID(int aid)
{
  id =aid;
  return;
}

void TAGntuMCtofHit::SetTof(double atof)
{
  tof =atof;
  return;
}

//##############################################################################

/*!
  \class TAGntuMCtof TAGntuMCtof.hxx "TAGntuMCtof.hxx"
  \brief Ntuplize ToF raw data - container. **
*/

ClassImp(TAGntuMCtof);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuMCtof::TAGntuMCtof()
  : nhit(0),
    h(0)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuMCtof::~TAGntuMCtof()
{
  delete h;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAGntuMCtof::SetupClones()
{
  if (!h) h = new TClonesArray("TAGntuMCtofHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear.

void TAGntuMCtof::Clear(Option_t*)
{

  TAGdata::Clear();

  nhit  = 0;
  h->Delete();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGntuMCtof::ToStream(ostream& os, Option_t* option) const
{
  os << "TAGntuMCtof" 
     << Form("  nhit=%3d", nhit) 
     << endl;

  if (nhit == 0) return;
  
  os << "ind slat stat  adct  adcb  tdct  tdcb" << endl;
  
  for (Int_t i = 0; i < nhit; i++) {
    const TAGntuMCtofHit* p_hit = Hit(i);
    os << Form("%3d %4lf", i, p_hit->id)
       << endl;
  }

  return;
}


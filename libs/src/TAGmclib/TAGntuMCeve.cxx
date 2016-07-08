/*!
  \file
  \version $Id: TATntuRaw.cxx,v 1.10 2003/06/22 11:48:38 mueller Exp $
  \brief   Implementation of TATntuRaw.
*/

#include <bitset>
using namespace std;

#include "TString.h"

#include "TAGntuMCeve.hxx"

/*!
  \class TAGntuMCeveHit TAGntuMCeve.hxx "TAGntuMCeve.hxx"
  \brief Ntuplize ToF raw data - hit object. **
*/

ClassImp(TAGntuMCeveHit);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuMCeveHit::TAGntuMCeveHit()
  : id(-100000), chg(-100000), mothid(-100000)
{
  mass = -100000.;
  reg = -1000;
  bar = -1000;
  dead = -1000;
  type = -1000;
  inpos.SetXYZ(-100.,-100.,-100);
  fipos.SetXYZ(-100.,-100.,-100);
  ip.SetXYZ(0.,0.,0);
  mothp.SetXYZ(0.,0.,0);
}

//------------------------------------------+-----------------------------------
//! Construct with data

// VM 17/11/13 added info for pile-up
TAGntuMCeveHit::TAGntuMCeveHit(Int_t i_id, Int_t i_chg, Int_t i_type, 
			       Int_t i_reg, Int_t i_bar, Int_t i_dead,
			       Double_t i_mass, TVector3 i_ipos, 
			       TVector3 i_fpos, TVector3 i_imom, 
			       Int_t i_mid, TVector3 i_mothmom,
			       Int_t i_pileup)
  : id(i_id), chg(i_chg), type(i_type), reg(i_reg), bar(i_bar),dead(i_dead),
    mass(i_mass), mothid(i_mid), pileup(i_pileup)
{

  inpos.SetXYZ(i_ipos.X(),i_ipos.Y(),i_ipos.Z());
  fipos.SetXYZ(i_fpos.X(),i_fpos.Y(),i_fpos.Z());
  ip.SetXYZ(i_imom.X(),i_imom.Y(),i_imom.Z());
  mothp.SetXYZ(i_mothmom.X(),i_mothmom.Y(),i_mothmom.Z());
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuMCeveHit::~TAGntuMCeveHit()
{}

//------------------------------------------+-----------------------------------
TVector3 TAGntuMCeveHit::InitPos()
{
  return inpos;
}

TVector3 TAGntuMCeveHit::FinalPos()
{
  return fipos;
}

TVector3 TAGntuMCeveHit::InitP()
{
  return ip;
}

TVector3 TAGntuMCeveHit::MotherP()
{
  return mothp;
}

Double_t  TAGntuMCeveHit::Mass()
{
  return mass;
}

Int_t  TAGntuMCeveHit::ID()
{
  return id;
}

Int_t  TAGntuMCeveHit::Type()
{
  return type;
}

Int_t  TAGntuMCeveHit::Dead()
{
  return dead;
}

Int_t  TAGntuMCeveHit::Reg()
{
  return reg;
}

Int_t  TAGntuMCeveHit::Bar()
{
  return bar;
}

Int_t  TAGntuMCeveHit::MotherID()
{
  return mothid;
}

Int_t  TAGntuMCeveHit::Chg()
{
  return chg;
}

// VM 17/11/13
Int_t  TAGntuMCeveHit::PileUp()
{
  return pileup;
}

//------------------------------------------+-----------------------------------
void TAGntuMCeveHit::SetInitPos(TVector3 pos)
{
  inpos = pos;
  return;
}

void TAGntuMCeveHit::SetFinalPos(TVector3 pos)
{
  fipos = pos;
  return;
}

void TAGntuMCeveHit::SetInitP(TVector3 mom)
{
  ip = mom;
  return;
}

void TAGntuMCeveHit::SetMotherP(TVector3 mom)
{
  mothp = mom;
  return;
}


void TAGntuMCeveHit::SetMass(double amass)
{
  mass =amass;
  return;
}

void TAGntuMCeveHit::SetID(int aid)
{
  id =aid;
  return;
}

void TAGntuMCeveHit::SetType(int atyp)
{
  type = atyp;
  return;
}

void TAGntuMCeveHit::SetDead(int adead)
{
  dead =adead;
  return;
}

void TAGntuMCeveHit::SetReg(int areg)
{
  reg =areg;
  return;
}

void TAGntuMCeveHit::SetBar(int abar)
{
  bar =abar;
  return;
}

void TAGntuMCeveHit::SetMotherID(int aid)
{
  mothid =aid;
  return;
}

void TAGntuMCeveHit::SetChg(int achg)
{
  chg = achg;
  return;
}

// VM 17/11/13
void TAGntuMCeveHit::SetPileUp(int ipup)
{
  pileup = ipup;
  return;
}

//##############################################################################

/*!
  \class TAGntuMCeve TAGntuMCeve.hxx "TAGntuMCeve.hxx"
  \brief Ntuplize ToF raw data - container. **
*/

ClassImp(TAGntuMCeve);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuMCeve::TAGntuMCeve()
  : nhit(0),
    h(0)
{}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuMCeve::~TAGntuMCeve()
{
  delete h;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAGntuMCeve::SetupClones()
{
  if (!h) h = new TClonesArray("TAGntuMCeveHit");
  return;
}

//------------------------------------------+-----------------------------------
//! Clear.

void TAGntuMCeve::Clear(Option_t*)
{

  TAGdata::Clear();

  nhit  = 0;
  h->Delete();

  return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAGntuMCeve::ToStream(ostream& os, Option_t* option) const
{
  os << "TAGntuMCeve" 
     << Form("  nhit=%3d", nhit) 
     << endl;

  if (nhit == 0) return;
  
  os << "ind slat stat  adct  adcb  tdct  tdcb" << endl;
  
  for (Int_t i = 0; i < nhit; i++) {
    const TAGntuMCeveHit* p_hit = Hit(i);
    os << Form("%3d %4lf", i, p_hit->mass)
       << endl;
  }

  return;
}


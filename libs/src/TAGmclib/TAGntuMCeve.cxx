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
  time = -1000;
  tof = -1000;
  trlen = -1000;
  inpos.SetXYZ(-100.,-100.,-100);
  fipos.SetXYZ(-100.,-100.,-100);
  ip.SetXYZ(0.,0.,0);
  fp.SetXYZ(0.,0.,0);
  mothip.SetXYZ(0.,0.,0);
  mothfp.SetXYZ(0.,0.,0);
}

//------------------------------------------+-----------------------------------
//! Construct with data

// VM 17/11/13 added info for pile-up
TAGntuMCeveHit::TAGntuMCeveHit(Int_t i_id, Int_t i_chg, Int_t i_type, 
			       Int_t i_reg, Int_t i_bar, Int_t i_dead,
			       Double_t i_mass,  Int_t i_moth, 
			       Double_t i_time,
			       Double_t i_tof, Double_t i_trlen,
			       TVector3 i_ipos, TVector3 i_fpos, 
			       TVector3 i_ip,TVector3 i_fp,
			       TVector3 i_mothip,
			       TVector3 i_mothfp,Int_t i_pileup)
: id(i_id), chg(i_chg), type(i_type), reg(i_reg), bar(i_bar),dead(i_dead),
    mass(i_mass), mothid(i_moth), time(i_time), tof(i_tof), trlen(i_trlen)
{

  inpos.SetXYZ(i_ipos.X(),i_ipos.Y(),i_ipos.Z());
  fipos.SetXYZ(i_fpos.X(),i_fpos.Y(),i_fpos.Z());
  ip.SetXYZ(i_ip.X(),i_ip.Y(),i_ip.Z());
  fp.SetXYZ(i_fp.X(),i_fp.Y(),i_fp.Z());
  mothip.SetXYZ(i_mothip.X(),i_mothip.Y(),i_mothip.Z());
  mothfp.SetXYZ(i_mothfp.X(),i_mothfp.Y(),i_mothfp.Z());
  pileup = i_pileup;

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

TVector3 TAGntuMCeveHit::FinalP()
{
  return fp;
}

TVector3 TAGntuMCeveHit::MotherInitP()
{
  return mothip;
}

TVector3 TAGntuMCeveHit::MotherFinalP()
{
  return mothfp;
}

Double_t  TAGntuMCeveHit::Time()
{
  return time;
}

Double_t  TAGntuMCeveHit::Tof()
{
  return tof;
}

Double_t  TAGntuMCeveHit::Trlen()
{
  return trlen;
}

Double_t  TAGntuMCeveHit::Mass()
{
  return mass;
}

Int_t  TAGntuMCeveHit::FlukaID()
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

void TAGntuMCeveHit::SetMotherInitP(TVector3 mom)
{
  mothip = mom;
  return;
}


void TAGntuMCeveHit::SetMotherFinalP(TVector3 mom)
{
  mothfp = mom;
  return;
}


void TAGntuMCeveHit::SetMass(double amass)
{
  mass =amass;
  return;
}

void TAGntuMCeveHit::SetFlukaID(int aid)
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
    
{
  SetupClones();
}

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
  //Alberto
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


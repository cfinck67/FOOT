/*!
  \file
  \version $Id: TATntuRaw.cxx,v 1.10 2003/06/22 11:48:38 mueller Exp $
  \brief   Implementation of TATntuRaw.
*/

#include <bitset>
using namespace std;

#include "TString.h"

#include "TAMCntuEve.hxx"

/*!
  \class TAMCeveHit TAMCntuEve.hxx "TAMCntuEve.hxx"
  \brief Ntuplize ToF raw data - hit object. **
*/

ClassImp(TAMCeveHit);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAMCeveHit::TAMCeveHit()
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
TAMCeveHit::TAMCeveHit(Int_t i_id, Int_t i_chg, Int_t i_type, 
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

TAMCeveHit::~TAMCeveHit()
{}

//------------------------------------------+-----------------------------------
TVector3 TAMCeveHit::InitPos()
{
  return inpos;
}

TVector3 TAMCeveHit::FinalPos()
{
  return fipos;
}

TVector3 TAMCeveHit::InitP()
{
  return ip;
}

TVector3 TAMCeveHit::FinalP()
{
  return fp;
}

TVector3 TAMCeveHit::MotherInitP()
{
  return mothip;
}

TVector3 TAMCeveHit::MotherFinalP()
{
  return mothfp;
}

Double_t  TAMCeveHit::Time()
{
  return time;
}

Double_t  TAMCeveHit::Tof()
{
  return tof;
}

Double_t  TAMCeveHit::Trlen()
{
  return trlen;
}

Double_t  TAMCeveHit::Mass()
{
  return mass;
}

Int_t  TAMCeveHit::FlukaID()
{
  return id;
}

Int_t  TAMCeveHit::Type()
{
  return type;
}

Int_t  TAMCeveHit::Dead()
{
  return dead;
}

Int_t  TAMCeveHit::Reg()
{
  return reg;
}

Int_t  TAMCeveHit::Bar()
{
  return bar;
}

Int_t  TAMCeveHit::MotherID()
{
  return mothid;
}

Int_t  TAMCeveHit::Chg()
{
  return chg;
}

// VM 17/11/13
Int_t  TAMCeveHit::PileUp()
{
  return pileup;
}


//------------------------------------------+-----------------------------------
void TAMCeveHit::SetInitPos(TVector3 pos)
{
  inpos = pos;
  return;
}

void TAMCeveHit::SetFinalPos(TVector3 pos)
{
  fipos = pos;
  return;
}

void TAMCeveHit::SetInitP(TVector3 mom)
{
  ip = mom;
  return;
}

void TAMCeveHit::SetMotherInitP(TVector3 mom)
{
  mothip = mom;
  return;
}


void TAMCeveHit::SetMotherFinalP(TVector3 mom)
{
  mothfp = mom;
  return;
}


void TAMCeveHit::SetMass(double amass)
{
  mass =amass;
  return;
}

void TAMCeveHit::SetFlukaID(int aid)
{
  id =aid;
  return;
}

void TAMCeveHit::SetType(int atyp)
{
  type = atyp;
  return;
}

void TAMCeveHit::SetDead(int adead)
{
  dead =adead;
  return;
}

void TAMCeveHit::SetReg(int areg)
{
  reg =areg;
  return;
}

void TAMCeveHit::SetBar(int abar)
{
  bar =abar;
  return;
}

void TAMCeveHit::SetMotherID(int aid)
{
  mothid =aid;
  return;
}

void TAMCeveHit::SetChg(int achg)
{
  chg = achg;
  return;
}

// VM 17/11/13
void TAMCeveHit::SetPileUp(int ipup)
{
  pileup = ipup;
  return;
}

//##############################################################################

/*!
  \class TAMCntuEve TAMCntuEve.hxx "TAMCntuEve.hxx"
  \brief Ntuplize ToF raw data - container. **
*/

ClassImp(TAMCntuEve);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAMCntuEve::TAMCntuEve()
 : TAGdata(),
   fListOfHits(0x0)
{
    SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAMCntuEve::~TAMCntuEve()
{
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
//! new hit
TAMCeveHit* TAMCntuEve::NewHit(  Int_t i_id, Int_t i_chg, Int_t i_type,
                                      Int_t i_reg, Int_t i_bar, Int_t i_dead,
                                      Double_t i_mass, Int_t i_moth, 
                                      Double_t i_time,
                                      Double_t i_tof, Double_t i_trlen,
                                      TVector3 i_ipos, TVector3 i_fpos, 
                                      TVector3 i_ip,TVector3 i_fp,
                                      TVector3 i_mothip,
                                      TVector3 i_mothfp,Int_t i_pileup ) {

    TClonesArray &hitCollection = *fListOfHits;
    TAMCeveHit* hit = new( hitCollection[hitCollection.GetEntriesFast()] ) TAMCeveHit( 
                                            i_id,i_chg,i_type,i_reg,i_bar,i_dead,i_mass,i_moth,
                                            i_time,i_tof,i_trlen,i_ipos,i_fpos,i_ip,i_fp,i_mothip,i_mothfp,i_pileup );
    return hit;

};

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAMCntuEve::SetupClones()   {
  if (!fListOfHits) fListOfHits = new TClonesArray("TAMCeveHit", 500);
}

//------------------------------------------+-----------------------------------
//! Clear.
void TAMCntuEve::Clear(Option_t*)
{
   fListOfHits->Clear("C");
}

//------------------------------------------+-----------------------------------
//! return n hits
Int_t TAMCntuEve::GetHitsN() const
{
   return fListOfHits->GetEntries();
}

//------------------------------------------+-----------------------------------
//! Access \a i 'th hit

TAMCeveHit* TAMCntuEve::Hit(Int_t i)
{
   return (TAMCeveHit*) ((*fListOfHits)[i]);;
}

//------------------------------------------+-----------------------------------
//! Read-only access \a i 'th hit
const TAMCeveHit* TAMCntuEve::Hit(Int_t i) const
{
   return (const TAMCeveHit*) ((*fListOfHits)[i]);;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAMCntuEve::ToStream(ostream& os, Option_t* option) const
{
  // os << "TAMCntuEve" 
  //    // << Form("  nhit=%3d", nhit) 
  //    << endl;

  // // if (nhit == 0) return;
  
  // os << "ind slat stat  adct  adcb  tdct  tdcb" << endl;
  
  // // for (Int_t i = 0; i < nhit; i++) {
  // //   const TAMCeveHit* p_hit = Hit(i);
  // //   os << Form("%3d %4lf", i, p_hit->mass)
  // //      << endl;
  // // }

  // return;
}


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
 : TAGobject(),
   fFlukaId(-1),
   fCharge(0.),
   fType(-1),
   fRegion(-1),
   fBaryon(-1),
   fDead(-1),
   fMass(0.),
   fMotherId(-1),
   fTime(0.),
   fTof(0.),
   fTrkLength(0.),
   fInitPos(0,0,0),
   fFinalPos(0,0,0),
   fInitMom(0,0,0),
   fFinalMom(0,0,0),
   fMotherInitMom(0,0,0),
   fMotherFinalMom(0,0,0),
   fPileUp(-1)
{
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
 : TAGobject(),
   fFlukaId(i_id),
   fCharge(i_chg),
   fType(i_type),
   fRegion(i_reg),
   fBaryon(i_bar),
   fDead(i_dead),
   fMass(i_mass),
   fMotherId(i_moth),
   fTime(i_time),
   fTof(i_tof),
   fTrkLength(i_trlen),
   fPileUp(i_pileup)
{
  fInitPos.SetXYZ(i_ipos.X(),i_ipos.Y(),i_ipos.Z());
  fFinalPos.SetXYZ(i_fpos.X(),i_fpos.Y(),i_fpos.Z());
  fInitMom.SetXYZ(i_ip.X(),i_ip.Y(),i_ip.Z());
  fFinalMom.SetXYZ(i_fp.X(),i_fp.Y(),i_fp.Z());
  fMotherInitMom.SetXYZ(i_mothip.X(),i_mothip.Y(),i_mothip.Z());
  fMotherFinalMom.SetXYZ(i_mothfp.X(),i_mothfp.Y(),i_mothfp.Z());
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAMCeveHit::~TAMCeveHit()
{
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

TAMCeveHit* TAMCntuEve::GetHit(Int_t i)
{
   return (TAMCeveHit*) ((*fListOfHits)[i]);;
}

//------------------------------------------+-----------------------------------
//! Read-only access \a i 'th hit
const TAMCeveHit* TAMCntuEve::GetHit(Int_t i) const
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


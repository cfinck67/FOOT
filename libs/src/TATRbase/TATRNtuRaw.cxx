/*!
  \file
  \version $Id: TATRntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATRntuRaw.
*/

#include "TString.h"

#include "TATRntuRaw.hxx"

/*!
  \class TATRntuRaw TATRntuRaw.hxx "TATRntuRaw.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TATRntuHit);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRntuHit::TATRntuHit()
 : TAGobject(),
   fChannel(0),
   fCharge(0.),
   fTime(0.)
{
   
}

//------------------------------------------+-----------------------------------
//! Constructor
TATRntuHit::TATRntuHit(Int_t channel, Double_t charge, Double_t time)
 : TAGobject(),
   fChannel(channel),
   fCharge(charge),
   fTime(time)
{
   
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRntuHit::~TATRntuHit()
{
}

//------------------------------------------+-----------------------------------
void TATRntuHit::Clear(Option_t* /*option*/)
{
   fMCindex.Set(0);
   fMcTrackId.Set(0);
}

//------------------------------------------+-----------------------------------
void TATRntuHit:: AddMcTrackId(Int_t trackId, Int_t mcId)
{
   fMCindex.Set(fMCindex.GetSize()+1);
   fMCindex[fMCindex.GetSize()-1]   = mcId;
   
   fMcTrackId.Set(fMcTrackId.GetSize()+1);
   fMcTrackId[fMcTrackId.GetSize()-1] = trackId;
}


//##############################################################################

ClassImp(TATRntuRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRntuRaw::TATRntuRaw()
: TAGdata(),
 fListOfHits(0)
{
   SetupClones();
}


//------------------------------------------+-----------------------------------
//! Destructor.

TATRntuRaw::~TATRntuRaw()
{
  delete fListOfHits;
}

//------------------------------------------+-----------------------------------
Int_t TATRntuRaw::GetHitsN() const
{
   return fListOfHits->GetEntries();
}

//______________________________________________________________________________
//
TATRntuHit* TATRntuRaw::NewHit(int channel, double charge, double time)
{
   TClonesArray &pixelArray = *fListOfHits;
   
   TATRntuHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATRntuHit(channel, charge, time);
   
   return hit;
   
}

//------------------------------------------+-----------------------------------
//! Access \a i 'th hit

TATRntuHit* TATRntuRaw::Hit(Int_t i)
{
   return (TATRntuHit*) ((*fListOfHits)[i]);;
}

//------------------------------------------+-----------------------------------
//! Read-only access \a i 'th hit

const TATRntuHit* TATRntuRaw::Hit(Int_t i) const
{
   return (const TATRntuHit*) ((*fListOfHits)[i]);;
}


//------------------------------------------+-----------------------------------
//! Setup clones.

void TATRntuRaw::SetupClones()
{
  if (!fListOfHits) fListOfHits = new TClonesArray("TATRntuHit");
   
  return;
}


//------------------------------------------+-----------------------------------
//! Clear event.

void TATRntuRaw::Clear(Option_t*)
{
  TAGdata::Clear();
  if (fListOfHits) fListOfHits->Clear("C");

  return;
}



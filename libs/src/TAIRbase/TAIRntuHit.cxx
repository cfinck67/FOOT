/*!
 \file
 \version $Id: TAIRntuHit.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
 \brief   Implementation of TAIRntuHit.
 */

#include "TAIRntuHit.hxx"

/*!
 \class TAIRntuHit TAIRntuHit.hxx "TAIRntuHit.hxx"
 \brief Mapping and Geometry parameters for CA detectors. **
 */

ClassImp(TAIRhit);

//------------------------------------------+-----------------------------------
//! Destructor.

TAIRhit::~TAIRhit()
{}

//------------------------------------------+-----------------------------------
//! Default constructor.

TAIRhit::TAIRhit()
 : TObject(),
   fTime(999999.),
   fCharge(0.),
   fChannelId(0),
   fType(0),
   fMcTrackCount(0)

{
}

//------------------------------------------+-----------------------------------
TAIRhit::TAIRhit(int cha, double charge, double time, int typ)
 : TObject(),
   fTime(time),
   fCharge(charge),
   fChannelId(cha),
   fType(typ),
   fMcTrackCount(0)
{
}

//______________________________________________________________________________
//
void TAIRhit:: AddMcTrackId(Int_t trackId)
{
   fMcTrackId[fMcTrackCount++] = trackId;
}

//##############################################################################

ClassImp(TAIRntuHit);

TString TAIRntuHit::fgkBranchName   = "irrh.";

//------------------------------------------+-----------------------------------
//! Default constructor.

TAIRntuHit::TAIRntuHit()
 : TAGdata(),
   fListOfHits(0x0)
{
   SetupClones();
}


//------------------------------------------+-----------------------------------
//! Destructor.

TAIRntuHit::~TAIRntuHit()
{
   delete fListOfHits;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAIRntuHit::SetupClones()
{
   if (!fListOfHits) fListOfHits = new TClonesArray("TAIRhit");
}

//------------------------------------------+-----------------------------------
Int_t TAIRntuHit::GetHitsN() const
{
   return fListOfHits->GetEntries();
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAIRhit* TAIRntuHit::GetHit(Int_t id)
{
   return (TAIRhit*)fListOfHits->At(id);
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAIRhit* TAIRntuHit::GetHit(Int_t id) const
{
   return (TAIRhit*)fListOfHits->At(id);
   
}

//______________________________________________________________________________
//
TAIRhit* TAIRntuHit::NewHit(int channel, double charge, double time, int type)
{
   TClonesArray &pixelArray = *fListOfHits;

   TAIRhit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TAIRhit(channel, charge, time, type);
   return hit;

}

//------------------------------------------+-----------------------------------
//! Clear event.

void TAIRntuHit::Clear(Option_t*)
{
   fListOfHits->Delete();
   
   return;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.

void TAIRntuHit::ToStream(ostream& os, Option_t* option) const
{
   os << "TAIRntuHit " << GetName()
   << endl;
   return;
}

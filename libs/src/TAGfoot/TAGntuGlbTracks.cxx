/*!
  \file
  \version $Id: TAGntuGlbTracks.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAGntuGlbTracks.
*/

#include <string.h>

#include <fstream>
#include <bitset>
using namespace std;
#include <algorithm>

#include "TString.h"

#include "TAGntuGlbTracks.hxx"

/*!
  \class TAGntuGlbTracks TAGntuGlbTracks.hxx "TAGntuGlbTracks.hxx"
  \brief Mapping and Geometry parameters for IR detectors. **
*/

ClassImp(TAGntuGlbTrk);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuGlbTrk::TAGntuGlbTrk() :
  vtxs(0), atthit(0) {
  fPath.clear();
  fMass = -1000;
  fMom = -1000;
  fCharge = -1000;
  fCharge_test = -1000;
  fToff = -1000;
  fTofr = -1000;
  fId = -1000;
  fTgtDir.SetXYZ(0,0,0);
  fTgtPos.SetXYZ(0,0,0);

  fMatPos.SetXYZ(0,0,0);

  fTofPos.SetXYZ(0,0,0);
  fTofDir.SetXYZ(0,0,0);

  fVtxTrkID = -1;
  //  fMatVtx = NULL;

   SetupClones();
}

//------------------------------------------+-----------------------------------
//! constructor.
TAGntuGlbTrk::TAGntuGlbTrk(vector<TVector3> apath) :
  vtxs(0), atthit(0)  {
  fPath = apath;
  
  fMass = -1000;
  fMom = -1000;
  fCharge = -1000;
  fCharge_test = -1000;
  fToff = -1000;
  fTofr = -1000;
  fId = -1000;
  
  fTgtDir.SetXYZ(0,0,0);
  fTgtPos.SetXYZ(0,0,0);

  fMatPos.SetXYZ(0,0,0);

  fTofPos.SetXYZ(0,0,0);
  fTofDir.SetXYZ(0,0,0);

  fVtxTrkID = -1;
  //  fMatVtx = NULL;

   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuGlbTrk::~TAGntuGlbTrk()
{
  delete vtxs;
  delete atthit;
}

//------------------------------------------+-----------------------------------
//! return a Track for a given sensor
TAVTvertex* TAGntuGlbTrk::GetVertex()
{
  if (vtxs->GetEntries() == 1) {
    return (TAVTvertex*)vtxs->At(0);
  }
  else {
    cout<<" More than 1 vertext associated to one track!!!!!"<<endl;
    return 0x0;
  }
}

//______________________________________________________________________________
//  
void TAGntuGlbTrk::AttachHit(TATntuRawHit& hit)
{
   TClonesArray &hitArray = *atthit;
   new(hitArray[hitArray.GetEntriesFast()]) TATntuRawHit(hit);
   return;
}


//------------------------------------------+-----------------------------------
//! return a Track for a given sensor
TATntuRawHit* TAGntuGlbTrk::GetHit(Int_t iHit)
{
  if (iHit >=0 || iHit < atthit->GetEntries())
	  return (TATntuRawHit*)atthit->At(iHit);
   else
	  return 0x0;
}

//______________________________________________________________________________
//  
void TAGntuGlbTrk::NewVertex(TAVTvertex& vtx)
{
   TClonesArray &vtArray = *vtxs;
   new(vtArray[vtArray.GetEntriesFast()]) TAVTvertex(vtx);
   return;
}


void TAGntuGlbTrk::SetupClones()
{
  if (!vtxs) vtxs = new TClonesArray("TAVTvertex");
  if (!atthit) atthit = new TClonesArray("TATntuRawHit");
  return;
}


//------------------------------------------+-----------------------------------
//! Clear event.

void TAGntuGlbTrk::Clear(Option_t*)
{

  vtxs->Delete();
  atthit->Delete();

  return;
}

//##############################################################################

ClassImp(TAGntuGlbTracks);

//------------------------------------------+-----------------------------------
//! Default constructor.

TAGntuGlbTracks::TAGntuGlbTracks() :
  nglbtrk(0), 
  glbtrk(0) 
{

   SetupClones();
}


//------------------------------------------+-----------------------------------
//! Destructor.

TAGntuGlbTracks::~TAGntuGlbTracks() {
  delete glbtrk;
}

//------------------------------------------+-----------------------------------
//! Setup clones.

void TAGntuGlbTracks::SetupClones()
{
  if (!glbtrk) glbtrk = new TClonesArray("TAGntuGlbTrk");
  return;
}


//------------------------------------------+-----------------------------------
//! Clear event.

void TAGntuGlbTracks::Clear(Option_t*)
{
  TAGdata::Clear();

  nglbtrk = 0;
  glbtrk->Delete();

  return;
}


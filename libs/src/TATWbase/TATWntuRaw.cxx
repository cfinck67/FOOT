/*!
  \class TATWntuRaw TATWntuRaw.hxx "TATWntuRaw.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TATWntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TString.h"
#include "TClonesArray.h"

#include "TATWntuRaw.hxx"

ClassImp(TATWntuHit) // Description of Single Detector TATWntuHit 

//______________________________________________________________________________
//  build a hit from a rawHit
TATWntuHit::TATWntuHit( TATWrawHit* hit )
   : TAGobject(),
    m_layer(0),
    m_bar(0),
    m_de(0),
    m_time(0),
    m_coordinate(0),
    m_z(0)
{
}

//______________________________________________________________________________
//
TATWntuHit::TATWntuHit(const TATWntuHit& aHit)
:  TAGobject(aHit),
   m_layer(aHit.m_layer),
   m_bar(aHit.m_bar),
   m_de(aHit.m_de),
   m_time(aHit.m_time),
   m_coordinate(aHit.m_coordinate),
   m_z(aHit.m_z)
{
   
}

//______________________________________________________________________________
// Build the hit from its sensor, line and column// constructor of a Pixel with column and line 
TATWntuHit::TATWntuHit ( int aView, int aBar, Double_t aDe, Double_t aTime, Double_t pos)
: TAGobject(),
  m_layer(aView),
  m_bar(aBar),
  m_de(aDe),
  m_time(aTime),
  m_coordinate(pos),
  m_z(0)
{
}

//______________________________________________________________________________
//
void TATWntuHit::AddMcTrackId(Int_t trackId, Int_t mcId)
{
   m_MCindex.Set(m_MCindex.GetSize()+1);
   m_MCindex[m_MCindex.GetSize()-1]   = mcId;
   
   m_McTrackId.Set(m_McTrackId.GetSize()+1);
   m_McTrackId[m_McTrackId.GetSize()-1] = trackId;
}

//______________________________________________________________________________
//
void TATWntuHit::Clear(Option_t* /*option*/)
{
   m_MCindex.Set(0);
   m_McTrackId.Set(0);
}

//##############################################################################

ClassImp(TATWntuRaw);
TString TATWntuRaw::fgkBranchName   = "twrh.";


//------------------------------------------+-----------------------------------
//! 
TATWntuRaw::TATWntuRaw() 
: TAGdata(),
    m_listOfHits(0x0)
{
   m_twGeo = (TATWparGeo*) gTAGroot->FindParaDsc(TATWparGeo::GetDefParaName(), "TATWparGeo")->Object();
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TATWntuRaw::~TATWntuRaw()
{
    delete m_listOfHits;
}

//______________________________________________________________________________
//  standard 
TATWntuHit* TATWntuRaw::NewHit( int layer, int bar, double energyLoss, double atime, double pos ) {

    if ( layer >= 0  && layer < m_twGeo->GetNLayers() ) {

        TClonesArray &pixelArray = *GetListOfHits(layer);
        TATWntuHit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATWntuHit( layer, bar, energyLoss, atime, pos);
       
        return hit;
    } else {
       Error("NewHit()", "Required layer not allowed: %d\n", layer);
       return 0x0;
    }
   
    return 0x0;  // never happens, but compiler doesn't complain
}

//------------------------------------------+-----------------------------------
//! return number of hits for a given sensor.  
int TATWntuRaw::GetHitN( int layer ) {

    if ( layer >= 0  && layer < m_twGeo->GetNLayers()) {
        TClonesArray*list = GetListOfHits(layer);
        return list->GetEntries();
    } else  {
       Error("GetHitN()", "Required layer not allowed: %d\n", layer);
       return 0x0;
    }
   
    return 0x0;
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TATWntuHit* TATWntuRaw::GetHit(  int layer, int hitID ) {

    if ( layer < 0  || layer > m_twGeo->GetNLayers()) {
       Error("GetHitN()", "Required layer not allowed: %d\n", layer);
       return 0x0;
    }
    if ( hitID < 0 || hitID >= GetHitN(layer) ) {
       Error("GetHitN()", "number of hit %d required is wrong. Max num %d", hitID, GetHitN(layer));
       return 0x0;
    }

    TClonesArray* list = GetListOfHits( layer );
    return (TATWntuHit*)list->At( hitID );
}

//------------------------------------------+-----------------------------------
TClonesArray* TATWntuRaw::GetListOfHits( int layer ) {

   if ( layer >= 0  && layer < m_twGeo->GetNLayers() ) {
	  TClonesArray* list = (TClonesArray*)m_listOfHits->At(layer);
	  return list;
   } else {
      Error("GetHitN()", "Required layer not allowed: %d required is wrong. Max num %d\n", layer,  m_twGeo->GetNLayers());
      return 0x0;
   }
}

//------------------------------------------+-----------------------------------
//! Setup clones. Crate and initialise the list of pixels
void TATWntuRaw::SetupClones()   {

    if (m_listOfHits) return;
    
    m_listOfHits = new TObjArray();

    for ( int i = 0; i < m_twGeo->GetNLayers(); ++i ) {
        TClonesArray* arr = new TClonesArray("TATWntuHit", 500);
        arr->SetOwner(true);
        m_listOfHits->AddAt(arr, i);
    }
    m_listOfHits->SetOwner(true);
}


//------------------------------------------+-----------------------------------
//! Clear event.
void TATWntuRaw::Clear(Option_t*) {

    for ( int i = 0; i < m_twGeo->GetNLayers(); ++i ) {
        TClonesArray* list = GetListOfHits(i);
        list->Delete();   
    }
}

//------------------------------------------+-----------------------------------
//! ostream insertion.
void TATWntuRaw::ToStream(ostream& os, Option_t* option) const
{
   // for (Int_t i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
	  
	  // os << "TATWntuRaw " << GetName()
	  // << Form("  nPixels=%3d", GetPixelsN(i))
	  // << endl;
	  
	  // //TODO properly
	  // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  // for (Int_t j = 0; j < GetPixelsN(i); j++) {  // all by default
		 // const TATWntuHit*  pixel = GetPixel(i,j, "all");
		 // if (pixel)
			// os << Form("%4d", pixel->GetPixelIndex());
		 // os << endl;
	  // }
   // }
}

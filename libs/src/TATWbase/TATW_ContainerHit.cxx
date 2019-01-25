/*!
  \file
  \version $Id: TATW_ContainerHit.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TATW_ContainerHit.
*/


/*!
  \class TATW_ContainerHit TATW_ContainerHit.hxx "TATW_ContainerHit.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TATW_Hit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TString.h"
#include "TClonesArray.h"

#include "TATW_ContainerHit.hxx"

#include "TATWdatRaw.hxx"



//##############################################################################

ClassImp(TATW_ContainerHit);
TString TATW_ContainerHit::fgkBranchName   = "twrh.";



//------------------------------------------+-----------------------------------
//! 
TATW_ContainerHit::TATW_ContainerHit() 
: TAGdata(),
    m_listOfHits(0x0)
{
   m_twGeo = (TATWparGeo*) gTAGroot->FindParaDsc(TATWparGeo::GetDefParaName(), "TATWparGeo")->Object();
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TATW_ContainerHit::~TATW_ContainerHit()
{
    delete m_listOfHits;
}

//______________________________________________________________________________
//  standard 
TATW_Hit* TATW_ContainerHit::NewHit( int layer, int bar, double energyLoss, double atime, double pos ) {

    if ( layer >= 0  && layer < m_twGeo->GetNLayers() ) {

        TClonesArray &pixelArray = *GetListOfHits(layer);
        TATW_Hit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TATW_Hit( layer, bar, energyLoss, atime, pos);
       
        return hit;
    } else {
       Error("NewHit()", "Required layer not allowed: %d\n", layer);
       return 0x0;
    }
   
    return 0x0;  // never happens, but compiler doesn't complain
}

//______________________________________________________________________________
//  
TATW_Hit* TATW_ContainerHit::NewHit( TATWrawHit* hit ) {

    // To be checked !!!!!!!!!!!!!!
    // if ( hit->GetLayer() >= 0  && hit->GetLayer() < m_twGeo->GetNLayers() ) {      

        // TClonesArray &pixelArray = *GetListOfHits( hit->GetLayer() );
        // TATW_Hit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TATW_Hit( hit->GetLayer(), hit );
        // return pixel;

    // } else {
    //     cout << "ERROR >> TATW_ContainerHit::NewHit(rawHit)   -->  required layer not allowed: " << hit->GetLayer() << endl;
    //     exit(0);
    // }   
    // return NULL;  // never happens, but compiler doesn't complain
   
   return 0x0;
}


//------------------------------------------+-----------------------------------
//! return number of hits for a given sensor.  
int TATW_ContainerHit::GetHitN( int layer ) {

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
TATW_Hit* TATW_ContainerHit::GetHit(  int layer, int hitID ) {

    if ( layer < 0  || layer > m_twGeo->GetNLayers()) {
       Error("GetHitN()", "Required layer not allowed: %d\n", layer);
       return 0x0;
    }
    if ( hitID < 0 || hitID >= GetHitN(layer) ) {
       Error("GetHitN()", "number of hit %d required is wrong. Max num %d", hitID, GetHitN(layer));
       return 0x0;
    }

    TClonesArray* list = GetListOfHits( layer );
    return (TATW_Hit*)list->At( hitID );
}

//------------------------------------------+-----------------------------------
TClonesArray* TATW_ContainerHit::GetListOfHits( int layer ) {

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
void TATW_ContainerHit::SetupClones()   {

    if (m_listOfHits) return;
    
    m_listOfHits = new TObjArray();

    for ( int i = 0; i < m_twGeo->GetNLayers(); ++i ) {
        TClonesArray* arr = new TClonesArray("TATW_Hit", 500);
        arr->SetOwner(true);
        m_listOfHits->AddAt(arr, i);
    }
    m_listOfHits->SetOwner(true);
}


//------------------------------------------+-----------------------------------
//! Clear event.
void TATW_ContainerHit::Clear(Option_t*) {

    for ( int i = 0; i < m_twGeo->GetNLayers(); ++i ) {
        TClonesArray* list = GetListOfHits(i);
        list->Delete();   
    }
}

//------------------------------------------+-----------------------------------
//! ostream insertion.
void TATW_ContainerHit::ToStream(ostream& os, Option_t* option) const
{
   // for (Int_t i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
	  
	  // os << "TATW_ContainerHit " << GetName()
	  // << Form("  nPixels=%3d", GetPixelsN(i))
	  // << endl;
	  
	  // //TODO properly
	  // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  // for (Int_t j = 0; j < GetPixelsN(i); j++) {  // all by default
		 // const TATW_Hit*  pixel = GetPixel(i,j, "all");
		 // if (pixel)
			// os << Form("%4d", pixel->GetPixelIndex());
		 // os << endl;
	  // }
   // }
}






























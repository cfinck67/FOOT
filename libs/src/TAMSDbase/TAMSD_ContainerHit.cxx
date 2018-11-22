/*!
  \file
  \version $Id: TAMSD_ContainerHit.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAMSD_ContainerHit.
*/


/*!
  \class TAMSD_ContainerHit TAMSD_ContainerHit.hxx "TAMSD_ContainerHit.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TAMSD_Hit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TString.h"
#include "TClonesArray.h"

#include "TAMSD_ContainerHit.hxx"

#include "TAMSDdatRaw.hxx"



//##############################################################################

ClassImp(TAMSD_ContainerHit);
// TString TAMSD_ContainerHit::fgkBranchName   = "vtrh.";





//------------------------------------------+-----------------------------------
//! 
TAMSD_ContainerHit::TAMSD_ContainerHit() 
: TAGdata(),
    m_listOfHits(0x0)
{
    cout << "TAMSD_ContainerHit::TAMSD_ContainerHit()" << endl;
    m_msdGeo = (TAMSDparGeo*) gTAGroot->FindParaDsc("msdGeo", "TAMSDparGeo")->Object();
    SetupClones();
}





//------------------------------------------+-----------------------------------
//! Destructor.
TAMSD_ContainerHit::~TAMSD_ContainerHit() {
    delete m_listOfHits;
}





//______________________________________________________________________________
//  standard 
TAMSD_Hit* TAMSD_ContainerHit::NewHit( int sensor, int strip, double energyLoss, double atime, int ntupID, int parentID ) {

    // if ( layer >= 0  && layer < m_msdGeo->GetNLayers() ) {
    if ( sensor >= 0  && sensor < m_msdGeo->GetNSensors() ) {

        // check on aorigin
        TClonesArray &pixelArray = *GetListOfHits(sensor);
        // FillPixelList( iSensor, aorigin, pixelArray.GetEntriesFast() ); 
        // TAMSD_Hit* strip = new(pixelArray[pixelArray.GetEntriesFast()]) TAMSD_Hit( sensor, bar, energyLoss, atime, ntupID, parentID );
        TAMSD_Hit* hit = new(pixelArray[pixelArray.GetEntriesFast()]) TAMSD_Hit( sensor, strip, energyLoss, atime, ntupID, parentID );
        return hit;

    } else {
        cout << "ERROR >> TAMSD_ContainerHit::NewHit   -->  required sensor not allowed: " << sensor << endl;
        exit(0);
    }
    return NULL;  // never happens, but compiler doesn't complain
}




//______________________________________________________________________________
//  
TAMSD_Hit* TAMSD_ContainerHit::NewHit( TAMSDrawHit* hit ) {

    // To be checked !!!!!!!!!!!!!!
    // if ( hit->GetLayer() >= 0  && hit->GetLayer() < m_msdGeo->GetNLayers() ) {      

        // TClonesArray &pixelArray = *GetListOfHits( hit->GetLayer() );
        // TAMSD_Hit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAMSD_Hit( hit->GetLayer(), hit );
        // return pixel;

    // } else {
    //     cout << "ERROR >> TAMSD_ContainerHit::NewHit(rawHit)   -->  required layer not allowed: " << hit->GetLayer() << endl;
    //     exit(0);
    // }   
    // return NULL;  // never happens, but compiler doesn't complain
}







//------------------------------------------+-----------------------------------
//! return number of hits for a given sensor.  
int TAMSD_ContainerHit::GetHitN( int sensor ) {

    if ( sensor >= 0  && sensor < m_msdGeo->GetNSensors()) {
        TClonesArray*list = GetListOfHits(sensor);
        return list->GetEntries();
    } else  {
        cout << "ERROR >> TAMSD_ContainerHit::GetPixelsN   -->  required sensor not allowed: " << sensor << endl;
        exit(0);
    }
    return NULL;  // never happens, but compiler doesn't complain
}







//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAMSD_Hit* TAMSD_ContainerHit::GetHit( int sensor, int hitID ) {

    if ( sensor < 0  || sensor > m_msdGeo->GetNSensors()) {
        cout << "ERROR >> TAMSD_ContainerHit::GetHit  -->  number of sensor "<<sensor<<" required is wrong. Max num  " << m_msdGeo->GetNSensors() << endl;
        exit(0);
    }
    if ( hitID < 0 || hitID >= GetHitN(sensor) ) {  
        cout << "ERROR >> TAMSD_ContainerHit::GetHit  -->  number of hit "<<hitID<<" required is wrong. Max num " << GetHitN(sensor) << endl;
        exit(0);
    }

    TClonesArray* list = GetListOfHits( sensor );
    return (TAMSD_Hit*)list->At( hitID );
}





//------------------------------------------+-----------------------------------
TClonesArray* TAMSD_ContainerHit::GetListOfHits( int sensor ) {

   if ( sensor >= 0  && sensor < m_msdGeo->GetNSensors() ) {
	  TClonesArray* list = (TClonesArray*)m_listOfHits->At(sensor);
	  return list;
   } else {
        cout << "ERROR >> TAMSD_ContainerHit::GetListOfHits  -->  number of sensor "<<sensor<<" required is wrong. Max num " << m_msdGeo->GetNSensors() << endl;
        exit(0);
   }   
}





//------------------------------------------+-----------------------------------
//! Setup clones. Crate and initialise the list of pixels
void TAMSD_ContainerHit::SetupClones()   {

    if (m_listOfHits) return;
    
    m_listOfHits = new TObjArray();

    for ( int i = 0; i < m_msdGeo->GetNSensors(); ++i ) {
        TClonesArray* arr = new TClonesArray("TAMSD_Hit", 500);
        arr->SetOwner(true);
        m_listOfHits->AddAt(arr, i);
    }
    m_listOfHits->SetOwner(true);
}





//------------------------------------------+-----------------------------------
//! Clear event.
void TAMSD_ContainerHit::Clear(Option_t*) {

    for ( int i = 0; i < m_msdGeo->GetNSensors(); ++i ) {
        TClonesArray* list = GetListOfHits(i);
        list->Delete();   
        list->Clear();
        // list->Clear("C");
    }

}





/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAMSD_ContainerHit::ToStream(ostream& os, Option_t* option) const
{
   // for (Int_t i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
	  
	  // os << "TAMSD_ContainerHit " << GetName()
	  // << Form("  nPixels=%3d", GetPixelsN(i))
	  // << endl;
	  
	  // //TODO properly
	  // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  // for (Int_t j = 0; j < GetPixelsN(i); j++) {  // all by default
		 // const TAMSD_Hit*  pixel = GetPixel(i,j, "all");
		 // if (pixel)
			// os << Form("%4d", pixel->GetPixelIndex());
		 // os << endl;
	  // }
   // }
}






























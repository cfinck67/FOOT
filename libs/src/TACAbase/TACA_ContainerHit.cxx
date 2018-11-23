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

#include "TACA_ContainerHit.hxx"

#include "TACAdatRaw.hxx"



//##############################################################################

ClassImp(TACA_ContainerHit);
// TString TATW_ContainerHit::fgkBranchName   = "vtrh.";



//------------------------------------------+-----------------------------------
//! 
TACA_ContainerHit::TACA_ContainerHit() 
: TAGdata(),
    m_listOfHits(0x0)
{
    cout << "TACA_ContainerHit::TACA_ContainerHit()" << endl;
    m_caGeo = (TACAparGeo*) gTAGroot->FindParaDsc("caGeo", "TACAparGeo")->Object();
    SetupClones();
}



//------------------------------------------+-----------------------------------
//! Destructor.
TACA_ContainerHit::~TACA_ContainerHit() {
    delete m_listOfHits;
}





//______________________________________________________________________________
//  standard 
TACA_Hit* TACA_ContainerHit::NewHit( int id_BGO, double enLoss, double time ) {

    if ( id_BGO >= 0  && id_BGO < m_caGeo->GetNBGO() ) {

        // check on aorigin
        TClonesArray &bgoArray = *GetListOfHits();     
        // FillPixelList( iSensor, aorigin, pixelArray.GetEntriesFast() ); 
        TACA_Hit* bgo = new(bgoArray[bgoArray.GetEntriesFast()]) TACA_Hit( id_BGO, enLoss, time );
        return bgo;

    } else {
        cout << "ERROR >> TACA_ContainerHit::NewHit   -->  required BGO crystal not allowed: " << id_BGO << endl;
        exit(0);
    }
    return NULL;  // never happens, but compiler doesn't complain
}




//______________________________________________________________________________
//  
TACA_Hit* TACA_ContainerHit::NewHit( TACArawHit* hit ) {

    // To be checked !!!!!!!!!!!!!!
    // if ( hit->GetLayer() >= 0  && hit->GetLayer() < m_twGeo->GetNLayers() ) {      

        // TClonesArray &bgoArray = *GetListOfHits();
        // TACA_Hit* bgo = new(bgoArray[bgoArray.GetEntriesFast()]) TACA_Hit( hit->GetLayer(), hit );
        // return bgo;

    // } else {
    //     cout << "ERROR >> TACA_ContainerHit::NewHit(rawHit)   -->  required layer not allowed: " << hit->GetLayer() << endl;
    //     exit(0);
    // }   
    // return NULL;  // never happens, but compiler doesn't complain
}





//------------------------------------------+-----------------------------------
//! return number of hits in the calorimeter  
int TACA_ContainerHit::GetHitN() {

    TClonesArray*list = GetListOfHits();
    return list->GetEntries();

}





//------------------------------------------+-----------------------------------
//! return a hit BGO
TACA_Hit* TACA_ContainerHit::GetHitBGO( int hitID ) {

    if ( hitID < 0 || hitID >= GetHitN() ) {  
        cout << "ERROR >> TACA_ContainerHit::GetHit  -->  number of hit "<<hitID<<" required is wrong. Max num " << GetHitN() << endl;
        exit(0);
    }

    TClonesArray* list = GetListOfHits();
    return (TACA_Hit*)list->At( hitID );

}




//------------------------------------------+-----------------------------------
TClonesArray* TACA_ContainerHit::GetListOfHits() {

    TClonesArray* list = (TClonesArray*) m_listOfHits->At(0);    // ->At(layer);
    return list;
  
}





//------------------------------------------+-----------------------------------
//! Setup clones. Crate and initialise the list of hit BGO
void TACA_ContainerHit::SetupClones()   {

    if (m_listOfHits) return;
    
    m_listOfHits = new TObjArray();

    TClonesArray* arr = new TClonesArray("TACA_Hit", 500);
    arr->SetOwner(true);
    m_listOfHits->AddAt(arr,0);
    
    m_listOfHits->SetOwner(true);

}






//------------------------------------------+-----------------------------------
//! Clear event.
void TACA_ContainerHit::Clear(Option_t*) {

    TClonesArray* list = GetListOfHits();
    list->Delete();   
    list->Clear();
    // list->Clear("C");

}





/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TACA_ContainerHit::ToStream(ostream& os, Option_t* option) const
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














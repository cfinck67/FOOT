/*!
  \file
  \version $Id: TAVTntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAVTntuRaw.
*/


/*!
  \class TAVTntuRaw TAVTntuRaw.hxx "TAVTntuRaw.hxx"
  \brief Container class for VTX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TAVTntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////


#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"

#include "TAVTdatRaw.hxx"



//##############################################################################

ClassImp(TAVTntuRaw);

// TString TAVTntuRaw::fgkBranchName   = "vtrh.";



//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTntuRaw::TAVTntuRaw() 
: TAGdata(),
  m_listOfPixels(0x0)
{
    cout << "TAVTntuRaw::TAVTntuRaw()" << endl;
    m_vtxGeo = (TAVTparGeo*) gTAGroot->FindParaDsc("vtGeo", "TAVTparGeo")->Object();
    SetupClones();
}



//------------------------------------------+-----------------------------------
//! Destructor.
TAVTntuRaw::~TAVTntuRaw() {
    delete m_listOfPixels;
    m_mcHitList.clear();
    m_mcClusterList.clear();
    m_noiseList.clear();
    m_pileUpList.clear();
}





//______________________________________________________________________________
//  Deprecated, to be private
TAVTntuHit* TAVTntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aLine, Int_t aColumn){
   if (iSensor >= 0  && iSensor < m_vtxGeo->GetNSensors()) {      
      TClonesArray &pixelArray = *GetListOfPixels(iSensor);
      TAVTntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAVTntuHit(iSensor, value, aLine, aColumn, "deprecated");
      return pixel;
   } else {
      Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
      return 0x0;
   }
}




//______________________________________________________________________________
//  standard 
TAVTntuHit* TAVTntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aLine, Int_t aColumn, string aorigin) {

   if ( iSensor >= 0  && iSensor < m_vtxGeo->GetNSensors() ) {

        // check on aorigin
        TClonesArray &pixelArray = *GetListOfPixels(iSensor);
        FillPixelList( iSensor, aorigin, pixelArray.GetEntriesFast() ); 
        TAVTntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAVTntuHit(iSensor, value, aLine, aColumn, aorigin);
        return pixel;

    } else {
        Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
        return 0x0;
    }
}




//______________________________________________________________________________
// Add MC original hits
TAVTntuHit* TAVTntuRaw::NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, int amcID, int agenPartID) {

    if ( aorigin != "mc_hit" ) {
        cout << "ERROR >> TAVTntuRaw::NewPixel  -->  developer error, input command not allowed " << aorigin << endl, exit(0);
    }

    if ( GlobalPar::GetPar()->Debug() > 1 )  cout << "TAVTntuRaw::NewPixel  " << iSensor << "  " << agenPartID << endl;

    TAVTntuHit* pixel = NewPixel( iSensor, value, aLine, aColumn, aorigin );

    // pixel->SetOrigin( aorigin );
    if ( aorigin == "mc_hit" ) {
        pixel->SetMCid( amcID );
        pixel->SetGenPartID( agenPartID );
    }
    return pixel;
}



//______________________________________________________________________________
// Add MC original hits
TAVTntuHit* TAVTntuRaw::NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, TAVTntuHit* originatingHit ) {

    if ( aorigin != "mc_cluster" )  {
        cout << "ERROR >> TAVTntuRaw::NewPixel  -->  developer error, input command not allowed " << aorigin << endl, exit(0);
    }

    TAVTntuHit* pixel = NewPixel( iSensor, value, aLine, aColumn, aorigin );

    if ( aorigin == "mc_cluster" ) {
        pixel->SetOriginalMC_Hit( originatingHit );

    }
    
    return pixel;
}



//______________________________________________________________________________
//  
TAVTntuHit* TAVTntuRaw::NewPixel( int iSensor, TAVTrawHit* rawPixel ) {

   if ( iSensor >= 0  && iSensor < m_vtxGeo->GetNSensors()) {      
   // if (iSensor >= 0  || iSensor < m_vtxGeo->GetNSensors()) {      
      TClonesArray &pixelArray = *GetListOfPixels(iSensor);
      TAVTntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAVTntuHit(iSensor, rawPixel);
      return pixel;
   } else {
      Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
      return 0x0;
   }   
}




//______________________________________________________________________________
void TAVTntuRaw::FillPixelList(int iSensor, string command, int id ) {

    if ( command == "mc_hit" ) {
        m_mcHitList[iSensor].push_back(id);
    }
    else if ( command == "mc_cluster" ) {
        m_mcClusterList[iSensor].push_back(id);
    }
    else if ( command == "noise" ) {
        m_noiseList[iSensor].push_back(id);
    }
    else if ( command == "pileup" ) {
        m_pileUpList[iSensor].push_back(id);
    }
    else {
        cout << "ERROR >> TAVTntuRaw::FillPixelList  -->  input command string not allowed" << endl;
        exit(0);
    }

}




// private!!!!  // all by default 
//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.  
Int_t TAVTntuRaw::GetPixelsN(Int_t iSensor) const {

   // if (iSensor >= 0  || iSensor < TAVTparMap::GetSensorsN()) {
   if (iSensor >= 0  && iSensor < m_vtxGeo->GetNSensors()) {
      TClonesArray*list = GetListOfPixels(iSensor);
      return list->GetEntries();
   } else  {
      Error("GetPixelsN()", "Wrong sensor number %d\n", iSensor);
      return -1;
   }
}




//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
int TAVTntuRaw::GetPixelsN( int iSensor, string command ) {

    // if ( iSensor >= 0  || iSensor < m_vtxGeo->GetNSensors() ) {
    if ( iSensor < 0 || iSensor >= m_vtxGeo->GetNSensors() ) {
        cout << "ERROR >> TAVTntuRaw::GetPixelsN  -->  number of sensor required is wrong" << iSensor<< endl;
        exit(0);
    }


    if ( command == "mc_hit" ) {
        return m_mcHitList[iSensor].size();
    }
    else if ( command == "mc_cluster" ) {
        return m_mcClusterList[iSensor].size();
    }
    else if ( command == "noise" ) {
        return m_noiseList[iSensor].size();
    }
    else if ( command == "pileup" ) {
        return m_pileUpList[iSensor].size();
    }
    else if ( command == "forPhys" ) {
        return m_mcClusterList[iSensor].size() + m_pileUpList[iSensor].size() + m_noiseList[iSensor].size();
    }
    else if ( command == "all" ) {
        TClonesArray*list = GetListOfPixels(iSensor);
        return list->GetEntries();
    }
    else  {
        cout << "ERROR >> TAVTntuRaw::GetPixelsN  -->  input command string not allowed" << endl;
        exit(0);
    }
    return -1;
}





//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAVTntuHit* TAVTntuRaw::GetPixel( int iSensor, int iPixel, string command ) {

    if (  iSensor < 0 || iSensor >= m_vtxGeo->GetNSensors() ) {
        cout << "ERROR >> TAVTntuRaw::GetPixel  -->  number of sensor required is wrong" << iSensor << endl;
        exit(0);
    }
    if ( iPixel < 0 || iPixel >= GetPixelsN(iSensor) ) {    // wrong check by Frank
        cout << "ERROR >> TAVTntuRaw::GetPixel  -->  number of pixel "<<iPixel<<" required is wrong " << GetPixelsN(iSensor) << endl;
        exit(0);
    }

    TClonesArray* list = GetListOfPixels( iSensor );
    if ( command == "mc_hit" ) {
        return (TAVTntuHit*)list->At( m_mcHitList[iSensor].at(iPixel) );
    }
    else if ( command == "mc_cluster" ) {
        return (TAVTntuHit*)list->At( m_mcClusterList[iSensor].at(iPixel) );
    }
    else if ( command == "noise" ) {
        return (TAVTntuHit*)list->At( m_noiseList[iSensor].at(iPixel) );
    }
    else if ( command == "pileup" ) {
        return (TAVTntuHit*)list->At( m_pileUpList[iSensor].at(iPixel) );
    }
    else if ( command == "forPhys" ) {
        if ( iPixel >= 0 && iPixel < m_mcHitList[iSensor].size() )
            return (TAVTntuHit*)list->At( m_mcHitList[iSensor].at(iPixel) );

        else if ( iPixel >= m_mcHitList[iSensor].size() && (iPixel < m_mcHitList[iSensor].size() + m_pileUpList[iSensor].size()) )
            return (TAVTntuHit*)list->At( m_pileUpList[iSensor].at(iPixel - m_mcHitList[iSensor].size()) );

        else if ( (iPixel >= m_mcHitList[iSensor].size() + m_pileUpList[iSensor].size()) && (iPixel < m_mcHitList[iSensor].size() + m_pileUpList[iSensor].size() + m_noiseList[iSensor].size()) )
            return (TAVTntuHit*)list->At( m_noiseList[iSensor].at( iPixel - m_mcHitList[iSensor].size() - m_pileUpList[iSensor].size() ) );
    }
    else if ( command == "all" ) {
        return (TAVTntuHit*)list->At( iPixel );
    }
    else {
        cout << "ERROR >> TAVTntuRaw::GetPixel  -->  command "<<command<<" not allowed " << endl;
        exit(0);
    }
    return NULL;  // never happens, but compiler doesn't complain
}




//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAVTntuHit* TAVTntuRaw::GetPixel(Int_t iSensor, Int_t iPixel, string command) const {
   // if (iPixel >=0 || iPixel < GetPixelsN(iSensor)) {
   //    TClonesArray* list = GetListOfPixels(iSensor);
   //    return (TAVTntuHit*)list->At(iPixel);
   // } else {
   //    Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
   //    return 0x0;
   // }  
   return GetPixel( iSensor, iPixel, command );
}





//------------------------------------------+-----------------------------------
TClonesArray* TAVTntuRaw::GetListOfPixels(int iSensor)  {

   if (iSensor >= 0  && iSensor < m_vtxGeo->GetNSensors()) {
	  TClonesArray* list = (TClonesArray*)m_listOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}





//------------------------------------------+-----------------------------------
TClonesArray* TAVTntuRaw::GetListOfPixels(int iSensor) const {

   if (iSensor >= 0  && iSensor < m_vtxGeo->GetNSensors()) {
	  TClonesArray* list = (TClonesArray*)m_listOfPixels->At(iSensor);
	  return list;
   } else {
	  Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}





//------------------------------------------+-----------------------------------
//! Setup clones. Crate and initialise the list of pixels
void TAVTntuRaw::SetupClones() {

    if (m_listOfPixels) return;
    m_listOfPixels = new TObjArray();

    for (int i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
        TClonesArray* arr = new TClonesArray("TAVTntuHit", 500);
        arr->SetOwner(true);
        m_listOfPixels->AddAt(arr, i);
    }
    m_listOfPixels->SetOwner(true);
}





//------------------------------------------+-----------------------------------
//! Clear event.
void TAVTntuRaw::Clear(Option_t*) {

    for (Int_t i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
        TClonesArray* list = GetListOfPixels(i);
        list->Delete();   
        list->Clear();
        // list->Clear("C");
    }

    m_mcHitList.clear();
    m_mcClusterList.clear();
    m_noiseList.clear();
    m_pileUpList.clear();

}





/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTntuRaw::ToStream(ostream& os, Option_t* option) const
{
   // for (Int_t i = 0; i < m_vtxGeo->GetNSensors(); ++i) {
	  
	  // os << "TAVTntuRaw " << GetName()
	  // << Form("  nPixels=%3d", GetPixelsN(i))
	  // << endl;
	  
	  // //TODO properly
	  // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  // for (Int_t j = 0; j < GetPixelsN(i); j++) {  // all by default
		 // const TAVTntuHit*  pixel = GetPixel(i,j, "all");
		 // if (pixel)
			// os << Form("%4d", pixel->GetPixelIndex());
		 // os << endl;
	  // }
   // }
}






























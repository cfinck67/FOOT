/*!
  \file
  \version $Id: TAITntuRaw.cxx,v 1.12 2003/06/09 18:41:17 mueller Exp $
  \brief   Implementation of TAITntuRaw.
*/


/*!
  \class TAITntuRaw TAITntuRaw.hxx "TAITntuRaw.hxx"
  \brief Container class for ITX ntu hit **
*/

////////////////////////////////////////////////////////////
// Class Description of TAITntuHit                        //
//                                                        //
//                                                        //
////////////////////////////////////////////////////////////

#include "TString.h"
#include "TClonesArray.h"


#include "TAITntuRaw.hxx"
#include "TAITdatRaw.hxx"



//##############################################################################

ClassImp(TAITntuRaw);

// TString TAITntuRaw::fgkBranchName   = "itrh.";



//------------------------------------------+-----------------------------------
//! Default constructor.
TAITntuRaw::TAITntuRaw() 
: TAGdata(),
  m_listOfPixels(0x0)
{
    cout << "TAITntuRaw::TAITntuRaw()" << endl;
    m_itxGeo = (TAITparGeo*) gTAGroot->FindParaDsc("itGeo", "TAITparGeo")->Object();
    SetupClones();
}



//------------------------------------------+-----------------------------------
//! Destructor.
TAITntuRaw::~TAITntuRaw()  {
    delete m_listOfPixels;
    m_mcHitList.clear();
    m_mcClusterList.clear();
    m_noiseList.clear();
    m_pileUpList.clear();
}






// //______________________________________________________________________________
// //    Deprecated, to be private
// TAITntuHit* TAITntuRaw::NewPixel(Int_t iSensor, Double_t value, Int_t aLine, Int_t aColumn) {

//    if (iSensor >= 0  && iSensor < m_itxGeo->GetNSensors()) {      
//       TClonesArray &pixelArray = *GetListOfPixels(iSensor);
//       TAITntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(iSensor, value, aLine, aColumn);
//       return pixel;
//    } else {
//       Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
//       return 0x0;
//    }
// }





//______________________________________________________________________________
//  standard 
TAITntuHit* TAITntuRaw::NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin) {

   if ( iSensor >= 0  && iSensor < m_itxGeo->GetNSensors() ) {

        // check on aorigin
        TClonesArray &pixelArray = *GetListOfPixels(iSensor);
        FillPixelList( iSensor, aorigin, pixelArray.GetEntriesFast() ); 
        TAITntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(iSensor, value, aLine, aColumn, aorigin);
        return pixel;

    } else {
        Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
        return 0x0;
    }
}




//______________________________________________________________________________
// Add MC original hits
TAITntuHit* TAITntuRaw::NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, int amcID, int agenPartID) {

    if ( aorigin != "mc_hit" ) {
        cout << "ERROR >> TAITntuRaw::NewPixel  -->  developer error, input command not allowed " << aorigin << endl, exit(0);
    }

    if ( GlobalPar::GetPar()->Debug() > 1 )  cout << "TAITntuRaw::NewPixel  " << iSensor << "  " << agenPartID << endl;

    TAITntuHit* pixel = NewPixel( iSensor, value, aLine, aColumn, aorigin );

    // pixel->SetOrigin( aorigin );
    if ( aorigin == "mc_hit" ) {
        pixel->SetMCid( amcID );
        pixel->SetGenPartID( agenPartID );
    }
    return pixel;
}



//______________________________________________________________________________
// Add MC original hits
TAITntuHit* TAITntuRaw::NewPixel(int iSensor, double value, int aLine, int aColumn, string aorigin, TAITntuHit* originatingHit ) {

    if ( aorigin != "mc_cluster" )  {
        cout << "ERROR >> TAITntuRaw::NewPixel  -->  developer error, input command not allowed " << aorigin << endl, exit(0);
    }

    TAITntuHit* pixel = NewPixel( iSensor, value, aLine, aColumn, aorigin );

    if ( aorigin == "mc_cluster" ) {
        pixel->SetOriginalMC_Hit( originatingHit );

    }
    
    return pixel;
}






//______________________________________________________________________________
//  
TAITntuHit* TAITntuRaw::NewPixel(Int_t iSensor, TAITrawHit* rawPixel)   {

   if (iSensor >= 0  && iSensor < m_itxGeo->GetNSensors()) {      
      TClonesArray &pixelArray = *GetListOfPixels(iSensor);
      TAITntuHit* pixel = new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(iSensor, rawPixel);
      return pixel;
   } else {
      Error("NewPixel()", "Wrong sensor number %d\n", iSensor);
      return 0x0;
   }   
}




//______________________________________________________________________________
void TAITntuRaw::FillPixelList(int iSensor, string command, int id ) {

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
        cout << "ERROR >> TAITntuRaw::FillPixelList  -->  input command string not allowed" << endl;
        exit(0);
    }

}




// private!!!!  // all by default 
//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
Int_t TAITntuRaw::GetPixelsN(Int_t iSensor) const {

   if (iSensor >= 0  && iSensor < m_itxGeo->GetNSensors()) {
	  TClonesArray*list = GetListOfPixels(iSensor);
	  return list->GetEntries();
   } else  {
	  Error("GetPixelsN()", "Wrong sensor number %d\n", iSensor);
	  return -1;
   }
}





//------------------------------------------+-----------------------------------
//! return number of pixels for a given sensor.
int TAITntuRaw::GetPixelsN( int iSensor, string command ) {

    if ( iSensor < 0 || iSensor >= m_itxGeo->GetNSensors() ) {
        cout << "ERROR >> TAITntuRaw::GetPixelsN  -->  number of sensor required is wrong" << iSensor<< endl;
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
        cout << "ERROR >> TAITntuRaw::GetPixelsN  -->  input command string not allowed" << endl;
        exit(0);
    }
    return -1;
}








//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
TAITntuHit* TAITntuRaw::GetPixel( int iSensor, int iPixel, string command ) {

    if (  iSensor < 0 || iSensor >= m_itxGeo->GetNSensors() ) {
        cout << "ERROR >> TAITntuRaw::GetPixel  -->  number of sensor required is wrong" << iSensor << endl;
        exit(0);
    }
    if ( iPixel < 0 || iPixel >= GetPixelsN(iSensor) ) {    // wrong check by Frank
        cout << "ERROR >> TAITntuRaw::GetPixel  -->  number of pixel "<<iPixel<<" required is wrong " << GetPixelsN(iSensor) << endl;
        exit(0);
    }

    TClonesArray* list = GetListOfPixels( iSensor );
    if ( command == "mc_hit" ) {
        return (TAITntuHit*)list->At( m_mcHitList[iSensor].at(iPixel) );
    }
    else if ( command == "mc_cluster" ) {
        return (TAITntuHit*)list->At( m_mcClusterList[iSensor].at(iPixel) );
    }
    else if ( command == "noise" ) {
        return (TAITntuHit*)list->At( m_noiseList[iSensor].at(iPixel) );
    }
    else if ( command == "pileup" ) {
        return (TAITntuHit*)list->At( m_pileUpList[iSensor].at(iPixel) );
    }
    else if ( command == "forPhys" ) {
        if ( iPixel >= 0 && iPixel < m_mcHitList[iSensor].size() )
            return (TAITntuHit*)list->At( m_mcHitList[iSensor].at(iPixel) );

        else if ( iPixel >= m_mcHitList[iSensor].size() && (iPixel < m_mcHitList[iSensor].size() + m_pileUpList[iSensor].size()) )
            return (TAITntuHit*)list->At( m_pileUpList[iSensor].at(iPixel - m_mcHitList[iSensor].size()) );

        else if ( (iPixel >= m_mcHitList[iSensor].size() + m_pileUpList[iSensor].size()) && (iPixel < m_mcHitList[iSensor].size() + m_pileUpList[iSensor].size() + m_noiseList[iSensor].size()) )
            return (TAITntuHit*)list->At( m_noiseList[iSensor].at( iPixel - m_mcHitList[iSensor].size() - m_pileUpList[iSensor].size() ) );
    }
    else if ( command == "all" ) {
        return (TAITntuHit*)list->At( iPixel );
    }
    else {
        cout << "ERROR >> TAITntuRaw::GetPixel  -->  command "<<command<<" not allowed " << endl;
        exit(0);
    }
    return NULL;  // never happens, but compiler doesn't complain
}








//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAITntuHit* TAITntuRaw::GetPixel( int iSensor, int iPixel, string command ) const  {

   // if (iPixel >=0 && iPixel < GetPixelsN(iSensor)) {
   //    TClonesArray* list = GetListOfPixels(iSensor);
   //    return (TAITntuHit*)list->At(iPixel);
   // } else {
   //    Error("GetPixel()", "Wrong sensor number %d\n", iSensor);
   //    return 0x0;
   // }
     return GetPixel( iSensor, iPixel, command ); 
}






//------------------------------------------+-----------------------------------
TClonesArray* TAITntuRaw::GetListOfPixels(int iSensor)    {

    if (iSensor >= 0  && iSensor < m_itxGeo->GetNSensors()) {
        TClonesArray* list = (TClonesArray*)m_listOfPixels->At(iSensor);
        return list;
    } else {
        Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
        return 0x0;
    }   
}





//------------------------------------------+-----------------------------------
TClonesArray* TAITntuRaw::GetListOfPixels(int iSensor) const  {

    if (iSensor >= 0  && iSensor < m_itxGeo->GetNSensors()) {
        TClonesArray* list = (TClonesArray*)m_listOfPixels->At(iSensor);
        return list;
    } else {
        Error("GetListOfPixels()", "Wrong sensor number %d\n", iSensor);
        return 0x0;
    }   
}






//------------------------------------------+-----------------------------------
//! Setup clones.
void TAITntuRaw::SetupClones()  {
  
   if (m_listOfPixels) return;
   m_listOfPixels = new TObjArray( m_itxGeo->GetNSensors() );
   
   for (Int_t i = 0; i < m_itxGeo->GetNSensors(); ++i) {
      TClonesArray* arr = new TClonesArray("TAITntuHit", 500);
      arr->SetOwner(true);
      m_listOfPixels->AddAt(arr, i);
   }
   m_listOfPixels->SetOwner(true);
}



//------------------------------------------+-----------------------------------
//! Clear event.
void TAITntuRaw::Clear(Option_t*)   {

    for (Int_t i = 0; i < m_itxGeo->GetNSensors(); ++i) {
        TClonesArray* list = GetListOfPixels(i);
        list->Delete();
        list->Clear();
    }   

    m_mcHitList.clear();
    m_mcClusterList.clear();
    m_noiseList.clear();
    m_pileUpList.clear();
}




/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAITntuRaw::ToStream(ostream& os, Option_t* option) const
{
   // for (Int_t i = 0; i < m_itxGeo->GetNSensors(); ++i) {
	  
	  // os << "TAITntuRaw " << GetName()
	  // << Form("  nPixels=%3d", GetPixelsN(i))
	  // << endl;
	  
	  // //TODO properly
	  // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
	  // for (Int_t j = 0; j < GetPixelsN(i); j++) {
		 // const TAITntuHit*  pixel = GetPixel(i,j);
		 // if (pixel)
			// os << Form("%4d", pixel->GetPixelIndex());
		 // os << endl;
	  // }
   // }
}


















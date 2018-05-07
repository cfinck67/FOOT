



#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTparMap.hxx"



#include "TAVTntuHit.hxx"



vector<string> m_originAllowed = { "mc_cluster", "mc_hit", "pileup", "noise", "data" };


ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 



//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit(Int_t aSensorNumber, TAVTrawHit* pixel)
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fFound(kFALSE),
  fDebugLevel(0)
{
   // constructor of a TAVTntuHit from a base pixel
   
    fPixelLine    = pixel->GetLineNumber();
    fPixelColumn  = pixel->GetColumnNumber();
    fPixelIndex   = pixel->GetIndex();

    fRawValue     = pixel->GetValue();
    fPulseHeight  = fRawValue;   

    m_origins = "data";
    m_layer = aSensorNumber;
    Initialise();

    if(fDebugLevel > 1)
        printf("TAVTntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}




//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue, string aorigin )
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue),
  fFound(kFALSE),
  fDebugLevel(0)
{
    m_origins = aorigin;
    m_layer = aSensorNumber;
    Initialise();

    fPulseHeight    = fRawValue; 

    if(fDebugLevel>1)
        printf("TAVTntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
   
}




//______________________________________________________________________________
//  
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn, string aorigin )
: TObject(),
  fSensorNumber(aSensorNumber),
  fMCid(-1),
  fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue),
  fFound(kFALSE),
  fDebugLevel(0)
{
    // constructor of a Pixel with column and line 
    m_origins = aorigin;
    m_layer = aSensorNumber;
    Initialise();

    fPulseHeight    = fRawValue; 

    if(fDebugLevel > 1) 
        printf("TAVTntuHit: pixel %d from plane %d with value %f built\n", fPixelIndex, fSensorNumber, fRawValue);
}



//______________________________________________________________________________
//  
void TAVTntuHit::Initialise() {

    fPosition.SetXYZ(0, 0, 0);
    fSize.SetXYZ(0, 0, 0);

    fMCPos.SetXYZ(0, 0, 0);
    fMCP.SetXYZ(0, 0, 0);

    m_mcID = -1;
    m_genPartIndex = -1;
    m_clusterSeed = NULL;
    m_genPartPointer = NULL;

    // if ( find( m_originAllowed.begin(), m_originAllowed.end(), m_originAllowed ) == m_originAllowed.end() )  {
    //  cout << 
    //  exit(0);
    // }

    m_geometry = (TAVTparGeo*) gTAGroot->FindParaDsc("vtGeo", "TAVTparGeo")->Object();

    // set center position
    SetPosition( m_geometry->GetPixelPos_Local( fSensorNumber, fPixelColumn, fPixelLine ) );
    // SetPosition( m_geometry->GetPosition( m_layer, fPixelColumn, fPixelLine ) );

    // m_originAllowed = { "MC_cluster", "MC_hit", "MC_pileup", "MC_noise", "data" };

}



//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(TAVTntuHit &aPixel)
{
   return Distance(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(TAVTntuHit &aPixel)
{
   return DistanceU(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(0);
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(TAVTntuHit &aPixel)
{
   return DistanceV(aPixel.GetPosition());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result(1);
}




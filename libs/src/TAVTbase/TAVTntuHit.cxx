
#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTntuHit.hxx"

ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 


//______________________________________________________________________________
//  build a hit from a rawHit
TAVTntuHit::TAVTntuHit(Int_t aSensorNumber, TAVTrawHit* pixel)
: TObject(),
  fSensorId(aSensorNumber),
  fMCid(-1),
  fFound(false)
{
   // constructor of a TAVTntuHit from a base pixel
   
    fPixelLine    = pixel->GetLineNumber();
    fPixelColumn  = pixel->GetColumnNumber();
    fPixelIndex   = pixel->GetIndex();

    fRawValue     = pixel->GetValue();
    fPulseHeight  = fRawValue;   

    Initialise();
    fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );
}

//______________________________________________________________________________
//  build the hit from the index
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TObject(),
  fSensorId(aSensorNumber),
  fMCid(-1),
  fPixelIndex(aPixelIndex),
  fPixelLine(0),
  fPixelColumn(0),
  fRawValue(aValue),
  fFound(false)
{
    Initialise();

    fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );

    fPulseHeight = fRawValue;
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TObject(),
  fSensorId(aSensorNumber),
  fMCid(-1),
  fPixelIndex(0),
  fPixelLine(aLine),
  fPixelColumn(aColumn),
  fRawValue(aValue),
  fFound(false)
{
    Initialise();

    fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );

    fPulseHeight = fRawValue;
}

//______________________________________________________________________________
//
void TAVTntuHit::Clear(Option_t* /*option*/)
{
}

//______________________________________________________________________________
//
Bool_t TAVTntuHit::IsEqual(const TObject* hit) const
{
   return ((fSensorId    == ((TAVTntuHit*)hit)->fSensorId)    &&
           (fPixelLine   == ((TAVTntuHit*)hit)->fPixelLine)   &&
           (fPixelColumn == ((TAVTntuHit*)hit)->fPixelColumn)
           );
}

//______________________________________________________________________________
//  
void TAVTntuHit::Initialise()
{
    fPosition.SetXYZ(0, 0, 0);
    fMCPos.SetXYZ(0, 0, 0);
    fMCP.SetXYZ(0, 0, 0);

    // take the detector geometry
    fGeometry = (TAVTparGeo*) gTAGroot->FindParaDsc(TAVTparGeo::GetDefVtxParaName(), "TAVTparGeo")->Object();

    // set center position
    if ( GlobalPar::GetPar()->Debug() > 1 )
       Info("Initialise()", "line =  %d col = %d", fPixelLine, fPixelColumn);
   
    SetPosition( fGeometry->GetPixelPos_detectorFrame(fSensorId, fPixelColumn, fPixelLine ) );
}

//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCPosition_sensorFrame()
{
    TVector3 glob = fMCPos;
    fGeometry->Detector2Sensor_frame( fSensorId, &glob );
    return glob; 
}

//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCPosition_footFrame()
{
    TVector3 glob = fMCPos;
    fGeometry->Local2Global( &glob ); 
    return glob; 
}

//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCMomentum_footFrame()
{
    TVector3 globP = fMCP;
    fGeometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(TAVTntuHit &aPixel)
{
   return Distance(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::Distance(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.Perp();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(TAVTntuHit &aPixel)
{
   return DistanceU(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceU(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.x();
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(TAVTntuHit &aPixel)
{
   return DistanceV(aPixel.GetPixelPosition_detectorFrame());
}

//______________________________________________________________________________
//  
Double_t TAVTntuHit::DistanceV(const TVector3& aPosition)
{
   TVector3 result(fPosition);
   result -= aPosition; 
   return result.y();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////


























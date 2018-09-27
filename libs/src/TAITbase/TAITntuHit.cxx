
#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAITntuHit.hxx"

ClassImp(TAITntuHit) // Description of Single Detector TAITntuHit 


//______________________________________________________________________________
//  build the hit from the index
TAITntuHit::TAITntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAVTbaseNtuHit(aSensorNumber, aPixelIndex, aValue)
{
   Initialise();
   fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAITntuHit::TAITntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAVTbaseNtuHit(aSensorNumber, aValue, aLine, aColumn)
{
   Initialise();
   fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );
}
                 
//______________________________________________________________________________
//
void TAITntuHit::Initialise()
{
   fPosition.SetXYZ(0, 0, 0);
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   // take the detector geometry
   fGeometry = (TAITparGeo*) gTAGroot->FindParaDsc(TAITparGeo::GetDefParaName(), "TAITparGeo")->Object();
   
   // set center position
   if ( fDebugLevel> 1 )
      Info("Initialise()", "line =  %d col = %d", fPixelLine, fPixelColumn);
   
   SetPosition( fGeometry->GetPixelPos_detectorFrame(fSensorId, fPixelColumn, fPixelLine ) );
}

//______________________________________________________________________________
//
Bool_t TAITntuHit::IsEqual(const TObject* hit) const
{
   return ((fSensorId    == ((TAITntuHit*)hit)->fSensorId)    &&
           (fPixelLine   == ((TAITntuHit*)hit)->fPixelLine)   &&
           (fPixelColumn == ((TAITntuHit*)hit)->fPixelColumn)
           );
}

//______________________________________________________________________________
TVector3 TAITntuHit::GetMCPosition_sensorFrame()
{
    TVector3 glob = fMCPos;
    fGeometry->Detector2Sensor_frame( fSensorId, &glob );
    return glob; 
}

//______________________________________________________________________________
TVector3 TAITntuHit::GetMCPosition_footFrame()
{
    TVector3 glob = fMCPos;
    fGeometry->Local2Global( &glob ); 
    return glob; 
}

//______________________________________________________________________________
TVector3 TAITntuHit::GetMCMomentum_footFrame()
{
    TVector3 globP = fMCP;
    fGeometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
}

////______________________________________________________________________________
////  
//Double_t TAITntuHit::Distance(TAITntuHit &aPixel)
//{
//   return Distance(aPixel.GetPixelPosition_detectorFrame());
//}
//
////______________________________________________________________________________
////  
//Double_t TAITntuHit::DistanceU(TAITntuHit &aPixel)
//{
//   return DistanceU(aPixel.GetPixelPosition_detectorFrame());
//}
//
////______________________________________________________________________________
////  
//Double_t TAITntuHit::DistanceV(TAITntuHit &aPixel)
//{
//   return DistanceV(aPixel.GetPixelPosition_detectorFrame());
//}


























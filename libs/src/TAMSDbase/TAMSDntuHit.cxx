
#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAMSDntuHit.hxx"

ClassImp(TAMSDntuHit) // Description of Single Detector TAMSDntuHit 


//______________________________________________________________________________
//  build the hit from the index
TAMSDntuHit::TAMSDntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAVTbaseNtuHit(aSensorNumber, aPixelIndex, aValue)
{
   Initialise();
   fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAMSDntuHit::TAMSDntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAVTbaseNtuHit(aSensorNumber, aValue, aLine, aColumn)
{
   Initialise();
   fLayer = fGeometry->GetLayerFromSensorID( aSensorNumber );
}
                 
//______________________________________________________________________________
//
void TAMSDntuHit::Initialise()
{
   fPosition.SetXYZ(0, 0, 0);
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   // take the detector geometry
   fGeometry = (TAMSDparGeo*) gTAGroot->FindParaDsc(TAMSDparGeo::GetDefParaName(), "TAMSDparGeo")->Object();
   
   // set center position
   if ( GlobalPar::GetPar()->Debug() > 1 )
      Info("Initialise()", "line =  %d col = %d", fPixelLine, fPixelColumn);
   
   SetPosition( fGeometry->GetPixelPos_detectorFrame(fSensorId, fPixelColumn, fPixelLine ) );
}

//______________________________________________________________________________
//
Bool_t TAMSDntuHit::IsEqual(const TObject* hit) const
{
   return ((fSensorId    == ((TAMSDntuHit*)hit)->fSensorId)    &&
           (fPixelLine   == ((TAMSDntuHit*)hit)->fPixelLine)   &&
           (fPixelColumn == ((TAMSDntuHit*)hit)->fPixelColumn)
           );
}

//______________________________________________________________________________
TVector3 TAMSDntuHit::GetMCPosition_sensorFrame()
{
    TVector3 glob = fMCPos;
    fGeometry->Detector2Sensor_frame( fSensorId, &glob );
    return glob; 
}

//______________________________________________________________________________
TVector3 TAMSDntuHit::GetMCPosition_footFrame()
{
    TVector3 glob = fMCPos;
    fGeometry->Local2Global( &glob ); 
    return glob; 
}

//______________________________________________________________________________
TVector3 TAMSDntuHit::GetMCMomentum_footFrame()
{
    TVector3 globP = fMCP;
    fGeometry->Local2Global_RotationOnly( &globP ); 
    return globP; 
}

////______________________________________________________________________________
////  
//Double_t TAMSDntuHit::Distance(TAMSDntuHit &aPixel)
//{
//   return Distance(aPixel.GetPixelPosition_detectorFrame());
//}
//
////______________________________________________________________________________
////  
//Double_t TAMSDntuHit::DistanceU(TAMSDntuHit &aPixel)
//{
//   return DistanceU(aPixel.GetPixelPosition_detectorFrame());
//}
//
////______________________________________________________________________________
////  
//Double_t TAMSDntuHit::DistanceV(TAMSDntuHit &aPixel)
//{
//   return DistanceV(aPixel.GetPixelPosition_detectorFrame());
//}


























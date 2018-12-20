
#include "TString.h"
#include "TClonesArray.h"

#include "TAVTntuRaw.hxx"
#include "TAVTntuHit.hxx"

#include "TAGgeoTrafo.hxx"

ClassImp(TAVTntuHit) // Description of Single Detector TAVTntuHit 


//______________________________________________________________________________
//  build the hit from the index
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAVTbaseNtuHit(aSensorNumber, aPixelIndex, aValue)
{
   Initialise();
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAVTntuHit::TAVTntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAVTbaseNtuHit(aSensorNumber, aValue, aLine, aColumn)
{
   Initialise();
}
                 
//______________________________________________________________________________
//
void TAVTntuHit::Initialise()
{
   fPosition.SetXYZ(0, 0, 0);
   fMCPos.SetXYZ(0, 0, 0);
   fMCP.SetXYZ(0, 0, 0);
   
   // take the detector geometry
   fGeometry = (TAVTparGeo*) gTAGroot->FindParaDsc(TAVTparGeo::GetDefParaName(), "TAVTparGeo")->Object();
   
   // set center position
   if ( fDebugLevel> 1 )
      Info("Initialise()", "line =  %d col = %d", fPixelLine, fPixelColumn);
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
TVector3 TAVTntuHit::GetMCPosition_footFrame()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

   TVector3 glob = fMCPos;
   glob = geoTrafo->FromVTLocalToGlobal(glob);
   
   return glob;
}
//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCMomentum_footFrame()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   TVector3 globP = fMCP;
   globP = geoTrafo->VecFromVTLocalToGlobal(globP) ;
   
   return globP;
}


//______________________________________________________________________________
//! Set MC truth position
void TAVTntuHit::SetMCPosition(TVector3 a_pos)
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

   a_pos = geoTrafo->FromGlobalToVTLocal(a_pos );
   fMCPos = a_pos;
}


//______________________________________________________________________________
//! Set MC truth mo
void TAVTntuHit::SetMCMomentum(TVector3 a_mom)
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   a_mom = geoTrafo->VecFromGlobalToVTLocal(a_mom );
   fMCP = a_mom;
}

//______________________________________________________________________________
TVector3 TAVTntuHit::GetMCPosition_sensorFrame()
{
   TVector3 glob = fMCPos;
   glob = fGeometry->Detector2Sensor( fSensorId, glob );
   return glob;
}

























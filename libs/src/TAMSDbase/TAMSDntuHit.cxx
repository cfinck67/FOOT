
#include "TString.h"
#include "TClonesArray.h"

#include "TAGgeoTrafo.hxx"
#include "GlobalPar.hxx"

#include "TAMSDntuHit.hxx"

ClassImp(TAMSDntuHit) // Description of Single Detector TAMSDntuHit 


//______________________________________________________________________________
//  build the hit from the index
TAMSDntuHit::TAMSDntuHit( Int_t aSensorNumber, const Int_t aPixelIndex, Double_t aValue)
: TAVTbaseNtuHit(aSensorNumber, aPixelIndex, aValue)
{
   Initialise();
}

//______________________________________________________________________________
// Build the pixel from its sensor, line and column// constructor of a Pixel with column and line 
TAMSDntuHit::TAMSDntuHit( Int_t aSensorNumber, Double_t aValue, Int_t aLine, Int_t aColumn )
: TAVTbaseNtuHit(aSensorNumber, aValue, aLine, aColumn)
{
   Initialise();
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
   glob = fGeometry->Detector2Sensor( fSensorId, glob );
   return glob;
}

//______________________________________________________________________________
TVector3 TAMSDntuHit::GetMCPosition_footFrame()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   TVector3 glob = fMCPos;
   glob = geoTrafo->FromMSDLocalToGlobal(glob);
   
   return glob;
}
//______________________________________________________________________________
TVector3 TAMSDntuHit::GetMCMomentum_footFrame()
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   TVector3 globP = fMCP;
   globP = geoTrafo->VecFromMSDLocalToGlobal(globP);
   
   return globP;
}


//______________________________________________________________________________
//! Set MC truth position
void TAMSDntuHit::SetMCPosition(TVector3 a_pos)
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   a_pos = geoTrafo->FromGlobalToMSDLocal(a_pos );
   fMCPos = a_pos;
}


//______________________________________________________________________________
//! Set MC truth mo
void TAMSDntuHit::SetMCMomentum(TVector3 a_mom)
{
   TAGgeoTrafo* geoTrafo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   a_mom = geoTrafo->VecFromGlobalToMSDLocal(a_mom );
   fMCP = a_mom;
}





















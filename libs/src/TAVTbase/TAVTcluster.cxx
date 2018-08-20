////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAVTcluster                       //
//                                                        //
////////////////////////////////////////////////////////////

#include "TAVTparGeo.hxx"
#include "TAVTcluster.hxx"
#include "TAVTntuTrack.hxx"

ClassImp(TAVTcluster) // Description of a cluster

//______________________________________________________________________________
//  
TAVTcluster::TAVTcluster()
:  TObject(),
   fPosition(new TVector3(0., 0., 0.)),
   fPosError(new TVector3(0., 0., 0.)),
   fPositionG(new TVector3(0., 0., 0.)),
   fDebugLevel(0),
   fNumber(0),
   fPlaneNumber(10),
   fFound(kFALSE),
	fFoundXZ(kFALSE),
	fFoundYZ(kFALSE),
   fClusterPulseSum(-99.),
   fClusterAreaPulseSum(-99.),
   fSNneighbour(-99.),
   fStripsInClusterArea(-99.),
   fPhSeed(0),  
   fIndexSeed(0)
{
   // TAVTcluster constructor
   SetupClones();
}

//______________________________________________________________________________
//
void TAVTcluster::SetupClones()
{
   fListOfPixels = new TClonesArray("TAVTntuHit");
   fListOfPixels->SetOwner(true);
}

//______________________________________________________________________________
//  
TAVTcluster::TAVTcluster(const TAVTcluster& cluster)
:  TObject(),
   fPosition(new TVector3(*cluster.fPosition)),
   fPosError(new TVector3(*cluster.fPosError)),
   fPositionG(new TVector3(*cluster.fPositionG)),
   fDebugLevel(cluster.fDebugLevel),
   fNumber(cluster.fNumber),
   fPlaneNumber(cluster.fPlaneNumber),
   fFound(cluster.fFound),
   fFoundXZ(cluster.fFoundXZ),
   fFoundYZ(cluster.fFoundYZ),
   fClusterPulseSum(cluster.fClusterPulseSum),
   fClusterAreaPulseSum(cluster.fClusterAreaPulseSum),
   fSNneighbour(cluster.fSNneighbour),
   fStripsInClusterArea(cluster.fStripsInClusterArea),
   fPhSeed(cluster.fPhSeed),  
   fIndexSeed(cluster.fIndexSeed)
{
   // TAVTcluster constructor
   fListOfPixels = (TClonesArray*)cluster.fListOfPixels->Clone();
}

//______________________________________________________________________________
//  
TAVTcluster::~TAVTcluster()
{ 
   // TAVTcluster default destructor 
   
   delete fPosition;
   delete fPositionG;
   delete fPosError;
   delete fListOfPixels;
}

//______________________________________________________________________________
//  
void TAVTcluster::SetPosition(TVector3* pos)
{
   fPosition->SetXYZ(pos->Px(), pos->Py(), pos->Pz());
}

//______________________________________________________________________________
//  
void TAVTcluster::SetPosError(TVector3* pos)
{
   fPosError->SetXYZ(pos->Px(), pos->Py(), pos->Pz());
}

//______________________________________________________________________________
//  
void TAVTcluster::SetPositionG(TVector3* posGlo)
{
   fPositionG->SetXYZ(posGlo->Px(), posGlo->Py(), posGlo->Pz());
}

//______________________________________________________________________________
// 
TAVTntuHit* TAVTcluster::GetPixel(Int_t idx)                    
{ 
   if (idx >=0 && idx < fListOfPixels->GetEntries())
	  return (TAVTntuHit*)fListOfPixels->At(idx); 
   else
	  return 0x0;
}

//______________________________________________________________________________
//  
Float_t TAVTcluster::GetClusterPulseSum()
{
   if (fClusterPulseSum == -99.) {
	  fClusterPulseSum = 0.;
	  for (Int_t i = 0;  i < fListOfPixels->GetEntries(); ++i) {
		 TAVTntuHit* pixel = (TAVTntuHit*)fListOfPixels->At(i);
		 fClusterPulseSum += pixel->GetPulseHeight();
	  }
	  return fClusterPulseSum;
   } else
	  return  fClusterPulseSum; 
}


//______________________________________________________________________________
//  
Float_t TAVTcluster::GetPixelDistanceU(Int_t index) const
{
   TAVTntuHit* pixelSeed = (TAVTntuHit*)fListOfPixels->At(0);
   if (index >= 0 && index < fListOfPixels->GetEntries()) {
	  TAVTntuHit* aNeighbour = (TAVTntuHit*)fListOfPixels->At(index);
	  return pixelSeed->DistanceU(aNeighbour->GetPosition());
   } else {
	  return -1;
   }   
}

//______________________________________________________________________________
//  
Float_t TAVTcluster::GetPixelDistanceV(Int_t index) const
{
   TAVTntuHit* pixelSeed = (TAVTntuHit*)fListOfPixels->At(0);
   if (index >= 0 && index < fListOfPixels->GetEntries()) {
	  TAVTntuHit* aNeighbour = (TAVTntuHit*)fListOfPixels->At(index);
	  return pixelSeed->DistanceV(aNeighbour->GetPosition());
   } else {
	  return -1;
   }   
}

//______________________________________________________________________________
//  
Float_t TAVTcluster::GetSeedU() const 
{ 
   TAVTntuHit* pixelSeed = (TAVTntuHit*)fListOfPixels->At(0);
   return pixelSeed->GetPosition().Px();
}

//______________________________________________________________________________
//  
Float_t TAVTcluster::GetSeedV() const 
{ 
   TAVTntuHit* pixelSeed = (TAVTntuHit*)fListOfPixels->At(0);
   return pixelSeed->GetPosition().Py();
}

//______________________________________________________________________________
//  
Float_t TAVTcluster::Distance( TAVTcluster *aClus) {
   // Return the distance between this clusters and the pointed cluster
   // regardless of the plane
   
   TVector3 clusPosition( aClus->GetPositionG() );
   
   // Now compute the distance beetween the two hits
   clusPosition -= (GetPositionG());
   
   // Insure that z position is 0 for 2D length computation
   clusPosition.SetXYZ( clusPosition[0], clusPosition[1], 0.);
   
   return clusPosition.Mag();
}


//______________________________________________________________________________
//  
Float_t TAVTcluster::Distance( TAVTtrack *aTrack) {
   // Return the distance between this cluster and the pointed track impact in the plane
   //
   
   TVector3 impactPosition( aTrack->Intersection( GetPositionG()[2]) );
   impactPosition -= GetPositionG();
   // Insure that z position is 0 for 2D length computation
   impactPosition.SetXYZ(impactPosition(0), impactPosition(1), 0.);
   
   return impactPosition.Mag();
}

//______________________________________________________________________________
//  
void TAVTcluster::AddPixel(TAVTntuHit* pixel)
{
   TClonesArray &pixelArray = *fListOfPixels;
   new(pixelArray[pixelArray.GetEntriesFast()]) TAVTntuHit(*pixel);
}

//______________________________________________________________________________
//  
void TAVTcluster::ResetPixels()
{
   fListOfPixels->Delete();
}


////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAVTcluster                       //
//                                                        //
////////////////////////////////////////////////////////////

#include "TAVTparGeo.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuTrack.hxx"

ClassImp(TAVTcluster) // Description of a cluster

//______________________________________________________________________________
//  
TAVTcluster::TAVTcluster()
:  TObject(),
   fPosition(new TVector3(0., 0., 0.)),
   fPosError(new TVector3(0., 0., 0.)),
   fPositionG(new TVector3(0., 0., 0.)),
   fListOfPixels(new TClonesArray("TAVTntuHit")),
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



//##############################################################################

ClassImp(TAVTntuCluster);

TString TAVTntuCluster::fgkBranchName   = "vtclus.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTntuCluster::TAVTntuCluster() 
: TAGdata(),
  fGeometry(0x0),
  fListOfClusters(0x0)
{
   fGeometry = (TAVTparGeo*) gTAGroot->FindParaDsc("vtGeo", "TAVTparGeo")->Object();
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTntuCluster::~TAVTntuCluster() 
{
   delete fListOfClusters;
}

//------------------------------------------+-----------------------------------
//! return number of clusters
Int_t TAVTntuCluster::GetClustersN(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < fGeometry->GetNSensors()) {
	  TClonesArray*list = GetListOfClusters(iSensor);
	  return list->GetEntries();   
   } else 
	  return -1;   
}

//------------------------------------------+-----------------------------------
//! return number of clusters
TClonesArray* TAVTntuCluster::GetListOfClusters(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < fGeometry->GetNSensors()) {
	  TClonesArray* list = (TClonesArray*)fListOfClusters->At(iSensor);
	  return list;
   } else 
	  return 0x0;   
}

//------------------------------------------+-----------------------------------
//! return number of clusters
TClonesArray* TAVTntuCluster::GetListOfClusters(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < fGeometry->GetNSensors()) {
	  TClonesArray* list = (TClonesArray*)fListOfClusters->At(iSensor);
	  return list;
	  
   } else 
	  return 0x0;   
}

//------------------------------------------+-----------------------------------
//! return a cluster
TAVTcluster* TAVTntuCluster::GetCluster(Int_t iSensor, Int_t iCluster)
{
   if (iCluster >=0 || iCluster < GetClustersN(iSensor)) {
	  TClonesArray* list = GetListOfClusters(iSensor);
	  return (TAVTcluster*)list->At(iCluster);
   } else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAVTcluster* TAVTntuCluster::GetCluster(Int_t iSensor, Int_t iCluster) const
{
   if (iCluster >=0 || iCluster < GetClustersN(iSensor)) {
	  TClonesArray* list = GetListOfClusters(iSensor);
	  return (TAVTcluster*)list->At(iCluster);
   } else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! Setup clones.
void TAVTntuCluster::SetupClones()
{
   if (fListOfClusters) return;
   fListOfClusters    = new TObjArray();
   for (Int_t i = 0; i < fGeometry->GetNSensors(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAVTcluster");
	  arr->SetOwner(true);
	  fListOfClusters->AddAt(arr, i);
   }
   fListOfClusters->SetOwner(true);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAVTntuCluster::Clear(Option_t*)
{
   for (Int_t i = 0; i < fGeometry->GetNSensors(); ++i) {
	  TClonesArray* list = GetListOfClusters(i);
	  list->Delete();
   }   
}

//______________________________________________________________________________
//  
TAVTcluster* TAVTntuCluster::NewCluster(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < fGeometry->GetNSensors()) {
	  TClonesArray &clusterArray = *GetListOfClusters(iSensor);
	  TAVTcluster* cluster = new(clusterArray[clusterArray.GetEntriesFast()]) TAVTcluster();
	  return cluster;
   } else {
	  cout << Form("Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//______________________________________________________________________________
//  
TAVTcluster* TAVTntuCluster::NewCluster(TAVTcluster* clus, Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < fGeometry->GetNSensors()) {
	  TClonesArray &clusterArray = *GetListOfClusters(iSensor);
	  TAVTcluster* cluster = new(clusterArray[clusterArray.GetEntriesFast()]) TAVTcluster(*clus);
	  return cluster;
   } else {
	  cout << Form("Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTntuCluster::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < fGeometry->GetNSensors(); ++i) {
   os << "TAVTntuCluster " << GetName()
   << Form("  nClus=%3d", GetClustersN(i))
   << endl;
   
   //TODO properly
   //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
   for (Int_t j = 0; j < GetClustersN(i); j++) {
	  const TAVTcluster*  cluster = GetCluster(i,j);
	  if (cluster)
		 os << Form("%4d", cluster->GetNumber());
	  os << endl;
	  
   }
   }
}


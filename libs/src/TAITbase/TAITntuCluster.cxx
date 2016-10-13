////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAITcluster                       //
//                                                        //
////////////////////////////////////////////////////////////

#include "TAITparMap.hxx"
#include "TAITntuCluster.hxx"
#include "TAITntuTrack.hxx"

ClassImp(TAITcluster) // Description of a cluster

//______________________________________________________________________________
//  
TAITcluster::TAITcluster()
:  TObject(),
   fPosition(new TVector3(0., 0., 0.)),
   fPosError(new TVector3(0., 0., 0.)),
   fPositionG(new TVector3(0., 0., 0.)),
   fListOfPixels(new TClonesArray("TAITntuHit")),
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
   // TAITcluster constructor
   fListOfPixels->SetOwner(true);
}

//______________________________________________________________________________
//  
TAITcluster::TAITcluster(const TAITcluster& cluster)
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
   // TAITcluster constructor
   fListOfPixels = (TClonesArray*)cluster.fListOfPixels->Clone();
}

//______________________________________________________________________________
//  
TAITcluster::~TAITcluster()
{ 
   // TAITcluster default destructor 
   
   delete fPosition;
   delete fPositionG;
   delete fPosError;
   delete fListOfPixels;
}

//______________________________________________________________________________
//  
void TAITcluster::SetPosition(TVector3* pos)
{
   fPosition->SetXYZ(pos->Px(), pos->Py(), pos->Pz());
}

//______________________________________________________________________________
//  
void TAITcluster::SetPosError(TVector3* pos)
{
   fPosError->SetXYZ(pos->Px(), pos->Py(), pos->Pz());
}

//______________________________________________________________________________
//  
void TAITcluster::SetPositionG(TVector3* posGlo)
{
   fPositionG->SetXYZ(posGlo->Px(), posGlo->Py(), posGlo->Pz());
}

//______________________________________________________________________________
// 
TAITntuHit* TAITcluster::GetPixel(Int_t idx)                    
{ 
   if (idx >=0 && idx < fListOfPixels->GetEntries())
	  return (TAITntuHit*)fListOfPixels->At(idx); 
   else
	  return 0x0;
}

//______________________________________________________________________________
//  
Float_t TAITcluster::GetClusterPulseSum()
{
   if (fClusterPulseSum == -99.) {
	  fClusterPulseSum = 0.;
	  for (Int_t i = 0;  i < fListOfPixels->GetEntries(); ++i) {
		 TAITntuHit* pixel = (TAITntuHit*)fListOfPixels->At(i);
		 fClusterPulseSum += pixel->GetPulseHeight();
	  }
	  return fClusterPulseSum;
   } else
	  return  fClusterPulseSum; 
}


//______________________________________________________________________________
//  
Float_t TAITcluster::GetPixelDistanceU(Int_t index) const
{
   TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
   if (index >= 0 && index < fListOfPixels->GetEntries()) {
	  TAITntuHit* aNeighbour = (TAITntuHit*)fListOfPixels->At(index);
	  return pixelSeed->DistanceU(aNeighbour->GetPosition());
   } else {
	  return -1;
   }   
}

//______________________________________________________________________________
//  
Float_t TAITcluster::GetPixelDistanceV(Int_t index) const
{
   TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
   if (index >= 0 && index < fListOfPixels->GetEntries()) {
	  TAITntuHit* aNeighbour = (TAITntuHit*)fListOfPixels->At(index);
	  return pixelSeed->DistanceV(aNeighbour->GetPosition());
   } else {
	  return -1;
   }   
}

//______________________________________________________________________________
//  
Float_t TAITcluster::GetSeedU() const 
{ 
   TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
   return pixelSeed->GetPosition().Px();
}

//______________________________________________________________________________
//  
Float_t TAITcluster::GetSeedV() const 
{ 
   TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
   return pixelSeed->GetPosition().Py();
}

//______________________________________________________________________________
//  
Float_t TAITcluster::Distance( TAITcluster *aClus) {
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
Float_t TAITcluster::Distance( TAITtrack *aTrack) {
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
void TAITcluster::AddPixel(TAITntuHit* pixel)
{
   TClonesArray &pixelArray = *fListOfPixels;
   new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(*pixel);
}

//______________________________________________________________________________
//  
void TAITcluster::ResetPixels()
{
   fListOfPixels->Delete();
}



//##############################################################################

ClassImp(TAITntuCluster);

TString TAITntuCluster::fgkBranchName   = "vtclus.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITntuCluster::TAITntuCluster() 
: TAGdata(),
  fListOfClusters(0x0)
{
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITntuCluster::~TAITntuCluster() 
{
   delete fListOfClusters;
}

//------------------------------------------+-----------------------------------
//! return number of clusters
Int_t TAITntuCluster::GetClustersN(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {
	  TClonesArray*list = GetListOfClusters(iSensor);
	  return list->GetEntries();   
   } else 
	  return -1;   
}

//------------------------------------------+-----------------------------------
//! return number of clusters
TClonesArray* TAITntuCluster::GetListOfClusters(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfClusters->At(iSensor);
	  return list;
   } else 
	  return 0x0;   
}

//------------------------------------------+-----------------------------------
//! return number of clusters
TClonesArray* TAITntuCluster::GetListOfClusters(Int_t iSensor) const
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {
	  TClonesArray* list = (TClonesArray*)fListOfClusters->At(iSensor);
	  return list;
	  
   } else 
	  return 0x0;   
}

//------------------------------------------+-----------------------------------
//! return a cluster
TAITcluster* TAITntuCluster::GetCluster(Int_t iSensor, Int_t iCluster)
{
   if (iCluster >=0 || iCluster < GetClustersN(iSensor)) {
	  TClonesArray* list = GetListOfClusters(iSensor);
	  return (TAITcluster*)list->At(iCluster);
   } else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAITcluster* TAITntuCluster::GetCluster(Int_t iSensor, Int_t iCluster) const
{
   if (iCluster >=0 || iCluster < GetClustersN(iSensor)) {
	  TClonesArray* list = GetListOfClusters(iSensor);
	  return (TAITcluster*)list->At(iCluster);
   } else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! Setup clones.
void TAITntuCluster::SetupClones()
{
   if (fListOfClusters) return;
   fListOfClusters    = new TObjArray();
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAITcluster");
	  arr->SetOwner(true);
	  fListOfClusters->AddAt(arr, i);
   }
   fListOfClusters->SetOwner(true);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAITntuCluster::Clear(Option_t*)
{
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
	  TClonesArray* list = GetListOfClusters(i);
	  list->Delete();
   }   
}

//______________________________________________________________________________
//  
TAITcluster* TAITntuCluster::NewCluster(Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {	  
	  TClonesArray &clusterArray = *GetListOfClusters(iSensor);
	  TAITcluster* cluster = new(clusterArray[clusterArray.GetEntriesFast()]) TAITcluster();
	  return cluster;
   } else {
	  cout << Form("Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

//______________________________________________________________________________
//  
TAITcluster* TAITntuCluster::NewCluster(TAITcluster* clus, Int_t iSensor)
{
   if (iSensor >= 0  || iSensor < TAITparMap::GetSensorsN()) {	  
	  TClonesArray &clusterArray = *GetListOfClusters(iSensor);
	  TAITcluster* cluster = new(clusterArray[clusterArray.GetEntriesFast()]) TAITcluster(*clus);
	  return cluster;
   } else {
	  cout << Form("Wrong sensor number %d\n", iSensor);
	  return 0x0;
   }   
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAITntuCluster::ToStream(ostream& os, Option_t* option) const
{
   for (Int_t i = 0; i < TAITparMap::GetSensorsN(); ++i) {
   os << "TAITntuCluster " << GetName()
   << Form("  nClus=%3d", GetClustersN(i))
   << endl;
   
   //TODO properly
   //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
   for (Int_t j = 0; j < GetClustersN(i); j++) {
	  const TAITcluster*  cluster = GetCluster(i,j);
	  if (cluster)
		 os << Form("%4d", cluster->GetNumber());
	  os << endl;
	  
   }
   }
}


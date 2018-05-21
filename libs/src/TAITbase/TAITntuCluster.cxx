////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAITcluster                       //
//                                                        //
////////////////////////////////////////////////////////////


#include "TAITntuCluster.hxx"
#include "TAITntuTrack.hxx"

ClassImp(TAITcluster) // Description of a cluster




//______________________________________________________________________________
//  
TAITcluster::TAITcluster()
:  TObject(),
   fPosition(TVector3(0., 0., 0.)),
   fPosError(TVector3(0., 0., 0.)),
   fListOfPixels(new TClonesArray("TAITntuHit")),
   fDebugLevel(0),
   m_clusterID(0),
   m_sensorID(0),
   fClusterPulseSum(-99.)
   // fClusterAreaPulseSum(-99.),
   // fSNneighbour(-99.),
   // fStripsInClusterArea(-99.),
   // fPhSeed(0),  
   // fIndexSeed(0)
{
   // TAITcluster constructor
   // cout << "\tTAITcluster::TAITcluster " << endl;

   fListOfPixels->SetOwner(true);
   m_geometry = (TAITparGeo*) gTAGroot->FindParaDsc("itGeo", "TAITparGeo")->Object();
}





//______________________________________________________________________________
//  
TAITcluster::TAITcluster(const TAITcluster& cluster)
:  TObject(),
   fPosition(cluster.fPosition),
   fPosError(cluster.fPosError),
   fDebugLevel(cluster.fDebugLevel),
   m_clusterID(cluster.m_clusterID),
   m_sensorID(cluster.m_sensorID),
   fClusterPulseSum(cluster.fClusterPulseSum)
   // fClusterAreaPulseSum(cluster.fClusterAreaPulseSum),
   // fSNneighbour(cluster.fSNneighbour),
   // fStripsInClusterArea(cluster.fStripsInClusterArea),
   // fPhSeed(cluster.fPhSeed),  
   // fIndexSeed(cluster.fIndexSeed)
{
   // TAITcluster constructor
   fListOfPixels = (TClonesArray*)cluster.fListOfPixels->Clone();
   m_geometry = (TAITparGeo*) gTAGroot->FindParaDsc("itGeo", "TAITparGeo")->Object();
}





//______________________________________________________________________________
//  
TAITcluster::~TAITcluster()   { 
   // delete fListOfPixels;
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

////////////////////// CORREGGI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//______________________________________________________________________________
//  
Float_t TAITcluster::GetPixelDistanceU(Int_t index) const
{
   TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
   if (index >= 0 && index < fListOfPixels->GetEntries()) {
	  TAITntuHit* aNeighbour = (TAITntuHit*)fListOfPixels->At(index);
	  return pixelSeed->DistanceU(aNeighbour->GetPixelPosition_detectorFrame());
   } else {
	  return -1;
   }   
}


////////////////////// CORREGGI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//______________________________________________________________________________
//  
Float_t TAITcluster::GetPixelDistanceV(Int_t index) const
{
   TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
   if (index >= 0 && index < fListOfPixels->GetEntries()) {
	  TAITntuHit* aNeighbour = (TAITntuHit*)fListOfPixels->At(index);
	  return pixelSeed->DistanceV(aNeighbour->GetPixelPosition_detectorFrame());
   } else {
	  return -1;
   }   
}

// //______________________________________________________________________________
// //  
// Float_t TAITcluster::GetSeedU() const 
// { 
//    TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
//    return pixelSeed->GetPosition().Px();
// }

// //______________________________________________________________________________
// //  
// Float_t TAITcluster::GetSeedV() const 
// { 
//    TAITntuHit* pixelSeed = (TAITntuHit*)fListOfPixels->At(0);
//    return pixelSeed->GetPosition().Py();
// }

//______________________________________________________________________________
//  
Float_t TAITcluster::Distance( TAITcluster *aClus) {
   // Return the distance between this clusters and the pointed cluster
   // regardless of the plane
   
   TVector3 clusPosition( aClus->GetPosition_detectorFrame() );
   
   // Now compute the distance beetween the two hits
   clusPosition -= (GetPosition_detectorFrame());
   
   // Insure that z position is 0 for 2D length computation
   clusPosition.SetXYZ( clusPosition[0], clusPosition[1], 0.);
   
   return clusPosition.Mag();
}


// //______________________________________________________________________________
// //  
// Float_t TAITcluster::Distance( TAITtrack *aTrack) {
//    // Return the distance between this cluster and the pointed track impact in the plane
//    //
   
//    TVector3 impactPosition( aTrack->Intersection( GetPixelPosition_detectorFrame()[2]) );
//    impactPosition -= GetPixelPosition_detectorFrame();
//    // Insure that z position is 0 for 2D length computation
//    impactPosition.SetXYZ(impactPosition(0), impactPosition(1), 0.);
   
//    return impactPosition.Mag();
// }

//______________________________________________________________________________
//  
void TAITcluster::AddPixel(TAITntuHit* pixel)
{
   TClonesArray &pixelArray = *fListOfPixels;
   new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(*pixel);
}


//______________________________________________________________________________
TVector3 TAITcluster::GetPosition_sensorFrame()  { 
   TVector3 globPos = fPosition;
   m_geometry->Detector2Sensor_frame( m_sensorID, &globPos ); 
   return globPos; 
}


//______________________________________________________________________________
TVector3 TAITcluster::GetPosition_footFrame()  { 
   TVector3 globPos = fPosition;
   m_geometry->Local2Global( &globPos ); 
   return globPos; 
}



//______________________________________________________________________________
//  
void TAITcluster::ResetPixels()
{
   fListOfPixels->Delete();
   // fListOfPixels->Clear();
}

















//##############################################################################

ClassImp(TAITntuCluster);

TString TAITntuCluster::fgkBranchName   = "itclus.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITntuCluster::TAITntuCluster() 
: TAGdata(),
  fListOfClusters(0x0)
{
   cout << "TAITntuCluster::TAITntuCluster()" << endl;
   m_geometry = (TAITparGeo*) gTAGroot->FindParaDsc("itGeo", "TAITparGeo")->Object();
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
   if (iSensor >= 0  && iSensor < m_geometry->GetNSensors()) {
	  TClonesArray*list = GetListOfClusters(iSensor);
	  return list->GetEntries();   
   } else 
	  return -1;   
}

//------------------------------------------+-----------------------------------
//! return number of clusters
TClonesArray* TAITntuCluster::GetListOfClusters(Int_t iSensor)
{
   if (iSensor >= 0  && iSensor < m_geometry->GetNSensors()) {
	  TClonesArray* list = (TClonesArray*)fListOfClusters->At(iSensor);
	  return list;
   } else 
	  return 0x0;   
}

//------------------------------------------+-----------------------------------
//! return number of clusters
TClonesArray* TAITntuCluster::GetListOfClusters(Int_t iSensor) const
{
   if (iSensor >= 0  && iSensor < m_geometry->GetNSensors()) {
	  TClonesArray* list = (TClonesArray*)fListOfClusters->At(iSensor);
	  return list;
	  
   } else 
	  return 0x0;   
}

//------------------------------------------+-----------------------------------
//! return a cluster
TAITcluster* TAITntuCluster::GetCluster(Int_t iSensor, Int_t iCluster)
{
   if (iCluster >=0 && iCluster < GetClustersN(iSensor)) {
	  TClonesArray* list = GetListOfClusters(iSensor);
	  return (TAITcluster*)list->At(iCluster);
   } else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAITcluster* TAITntuCluster::GetCluster(Int_t iSensor, Int_t iCluster) const
{
   if (iCluster >=0 && iCluster < GetClustersN(iSensor)) {
	  TClonesArray* list = GetListOfClusters(iSensor);
	  return (TAITcluster*)list->At(iCluster);
   } else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! Setup clones.
void TAITntuCluster::SetupClones() {

   if (fListOfClusters) return;
   fListOfClusters    = new TObjArray( m_geometry->GetNSensors() );

   for (Int_t i = 0; i < m_geometry->GetNSensors(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAITcluster", 500);
	  arr->SetOwner(true);
	  fListOfClusters->AddAt(arr, i);
   }
   fListOfClusters->SetOwner(true);
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAITntuCluster::Clear(Option_t*)
{
   for (Int_t i = 0; i < m_geometry->GetNSensors(); ++i) {
	  TClonesArray* list = GetListOfClusters(i);
	  list->Delete();
   }   
}

//______________________________________________________________________________
//  
TAITcluster* TAITntuCluster::NewCluster(Int_t iSensor)
{
   if (iSensor >= 0  && iSensor < m_geometry->GetNSensors()) {	  
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
   if (iSensor >= 0  && iSensor < m_geometry->GetNSensors()) {	  
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
   // for (Int_t i = 0; i < m_geometry->GetNSensors(); ++i) {
   // os << "TAITntuCluster " << GetName()
   // << Form("  nClus=%3d", GetClustersN(i))
   // << endl;
   
   // //TODO properly
   // //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
   // for (Int_t j = 0; j < GetClustersN(i); j++) {
	  // const TAITcluster*  cluster = GetCluster(i,j);
	  // if (cluster)
		 // os << Form("%4d", cluster->GetClusterID());
	  // os << endl;
	  
   // }
   // }
}


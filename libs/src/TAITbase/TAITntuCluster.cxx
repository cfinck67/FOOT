////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAITcluster                       //
//                                                        //
////////////////////////////////////////////////////////////

#include "TAITparGeo.hxx"
#include "TAITntuCluster.hxx"

ClassImp(TAITcluster) // Description of a cluster




//______________________________________________________________________________
//  
TAITcluster::TAITcluster()
:  TAVTcluster()
{
   SetupClones();
}


//______________________________________________________________________________
//  
TAITcluster::TAITcluster(const TAITcluster& cluster)
:  TAVTcluster(cluster)
{
   // TAITcluster constructor
}

//______________________________________________________________________________
//  
TAITcluster::~TAITcluster()
{
}

//______________________________________________________________________________
//
void TAITcluster::SetupClones()
{
   fListOfPixels = new TClonesArray("TAITntuHit");
   fListOfPixels->SetOwner(true);
}


//##############################################################################

ClassImp(TAITntuCluster);

TString TAITntuCluster::fgkItBranchName   = "itclus.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAITntuCluster::TAITntuCluster() 
: TAVTntuCluster()
{
   SetupClones();
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITntuCluster::~TAITntuCluster() 
{
}


//------------------------------------------+-----------------------------------
//! Setup clones.
void TAITntuCluster::SetupClones()
{
   fGeometry = (TAITparGeo*) gTAGroot->FindParaDsc(TAITparGeo::GetDefItParaName(), "TAITparGeo")->Object();

   if (fListOfClusters) return;
   fListOfClusters    = new TObjArray( fGeometry->GetNSensors() );

   for (Int_t i = 0; i < fGeometry->GetNSensors(); ++i) {
	  TClonesArray* arr = new TClonesArray("TAITcluster", 500);
	  arr->SetOwner(true);
	  fListOfClusters->AddAt(arr, i);
   }
   fListOfClusters->SetOwner(true);
}



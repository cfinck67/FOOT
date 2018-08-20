////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAITcluster                       //
//                                                        //
////////////////////////////////////////////////////////////

#include "TAITcluster.hxx"
#include "TAITntuHit.hxx"

ClassImp(TAITcluster) // Description of a cluster

//______________________________________________________________________________
//  
TAITcluster::TAITcluster()
:  TAVTbaseCluster()
{
   // TAITcluster constructor
   SetupClones();
}

//______________________________________________________________________________
//
void TAITcluster::SetupClones()
{
   fListOfPixels = new TClonesArray("TAITntuHit");
   fListOfPixels->SetOwner(true);
}

//______________________________________________________________________________
//  
TAITcluster::TAITcluster(const TAITcluster& cluster)
:  TAVTbaseCluster(cluster)
{
}

//______________________________________________________________________________
//  
TAITcluster::~TAITcluster()
{ 
   // TAITcluster default destructor
}


//______________________________________________________________________________
//  
void TAITcluster::AddPixel(TAITntuHit* pixel)
{
   TClonesArray &pixelArray = *fListOfPixels;
   new(pixelArray[pixelArray.GetEntriesFast()]) TAITntuHit(*pixel);
}




////////////////////////////////////////////////////////////
//                                                        //
// Class Description of TAMSDcluster                       //
//                                                        //
////////////////////////////////////////////////////////////

#include "TAMSDcluster.hxx"
#include "TAMSDntuHit.hxx"

ClassImp(TAMSDcluster) // Description of a cluster

//______________________________________________________________________________
//  
TAMSDcluster::TAMSDcluster()
:  TAVTbaseCluster()
{
   // TAMSDcluster constructor
   SetupClones();
}

//______________________________________________________________________________
//
void TAMSDcluster::SetupClones()
{
   fListOfPixels = new TClonesArray("TAMSDntuHit");
   fListOfPixels->SetOwner(true);
}

//______________________________________________________________________________
//  
TAMSDcluster::TAMSDcluster(const TAMSDcluster& cluster)
:  TAVTbaseCluster(cluster)
{
}

//______________________________________________________________________________
//  
TAMSDcluster::~TAMSDcluster()
{ 
   // TAMSDcluster default destructor
}


//______________________________________________________________________________
//  
void TAMSDcluster::AddPixel(TAMSDntuHit* pixel)
{
   TClonesArray &pixelArray = *fListOfPixels;
   new(pixelArray[pixelArray.GetEntriesFast()]) TAMSDntuHit(*pixel);
}




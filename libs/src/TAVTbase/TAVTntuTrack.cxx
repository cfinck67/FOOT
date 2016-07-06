//////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                          //
// Class Description of TAVTline                                                            // 
//                                                                                          //
// a Line object is defined by its                                                          //
//                  origin     = (x_0,y_0,z_0),                                             //
//                  slope      = (dx/dz,dy/dz,1),                                           //
// and              length.                                                                 //
// Points on the line at r_i are given as a function                                        //
// of the parameter beta. beta has no dimension.                                            //
// r_i(beta) = origin_i + beta * direction_i                                                //
// If one wants the pair (x,y) as a function of z along (0,0,dz) then beta = z/dz           //
//    and r_1(beta) = x_0 + z * dx/dz                                                       //
//        r_2(beta) = y_0 + z * dy/dz                                                       //
//        r_3(beta) = z_0 + z * 1                                                           //  
// In case one needs pair (x,y,z) as a function of l along (dx,dy,dz) then beta' = l/dl     //
//    and r_1(beta') = x_0 + l * dx/dl                                                      //
//        r_2(beta') = y_0 + l * dy/dl                                                      //
//        r_3(beta') = z_0 + l * dz/dl                                                      //
// with the relation beta^2 = beta'^2 * (1-x^2/l^2-y^2/l^2) / (1-dx^2/dl^2-dy^2/dl^2)       //
//                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////

#include "TMath.h"
#include "TAGgeoTrafo.hxx" 
#include "TAVTparGeo.hxx"
#include "TAVTntuTrack.hxx"


ClassImp(TAVTline) // Description of a Line

//______________________________________________________________________________
//  
TAVTline::TAVTline() 
: fOrigin(new TVector3()),
  fSlope(new TVector3()),
  fLength(0.0),
  fTheta(0.0),
  fPhi(0.0)
{
}

//______________________________________________________________________________
//  

TAVTline::TAVTline(TVector3 &aOrigin, TVector3 &aSlope, Float_t aLength)
{
   fOrigin    = new TVector3(aOrigin);
   fSlope     = new TVector3(aSlope);
   fLength    = aLength;
   TVector3 direction = fSlope->Unit();
   fTheta = direction.Theta()*TMath::RadToDeg();
   
   TVector3 origin = fOrigin->Unit();
   fPhi   = origin.Phi()*TMath::RadToDeg();
}

//______________________________________________________________________________
//  

TAVTline::TAVTline(const TAVTline& aLine)
:  TObject(aLine),
   fOrigin(new TVector3(*aLine.fOrigin)),
   fSlope(new TVector3(*aLine.fSlope)),
   fLength(aLine.fLength),
   fTheta(aLine.fTheta),
   fPhi(aLine.fPhi)
{
}

//______________________________________________________________________________
//  

TAVTline& TAVTline::operator=(const TAVTline& aLine)
{
   if (this == &aLine) return *this;

   fOrigin = new TVector3(*aLine.fOrigin);
   fSlope  = new TVector3(*aLine.fSlope);
   fLength = aLine.fLength;
   fTheta  = aLine.fTheta;
   fPhi    = aLine.fPhi;
   
   return *this;
}

//______________________________________________________________________________
//  
TAVTline::~TAVTline()
{
   delete  fOrigin;
   delete  fSlope;
}

//______________________________________________________________________________
//  
void TAVTline::SetValue(const TVector3& aOrigin, const TVector3& aSlope, const Float_t aLength)
{
   *fOrigin = aOrigin;
   *fSlope  = aSlope;
   fLength  = aLength;
   TVector3 direction = fSlope->Unit();
   fTheta = direction.Theta()*TMath::RadToDeg();
   
   TVector3 origin = fOrigin->Unit();
   fPhi   = origin.Phi()*TMath::RadToDeg();
}
//______________________________________________________________________________
//  
void TAVTline::Zero(){
   fOrigin->SetXYZ(0., 0., 0.);
   fSlope->SetXYZ(0., 0., 0.);
   fLength = 0;
   fTheta  = 0;
   fPhi    = 0;
}

//______________________________________________________________________________
// 
TVector3 TAVTline::GetPoint(Float_t beta)
{
   TVector3 result;
   result = (*fSlope) * beta;
   TVector3 offset = *fOrigin;
   offset(2) = 0.;
   return result += offset; 
}


//______________________________________________________________________________
//  
Float_t TAVTline::Distance(TVector3& p)
{
   // compute distance between a point M(x,y,z) and a
   // line defined by (origin, slope)
   // x = slopeX*z + originX
   // y = slopeY*z + originY
   // The distance is defined in a plane normal to the line
   //
   Float_t dX2 =  p.X()-fSlope->X()*p.Z()-fOrigin->X();
   dX2 *= dX2;
   Float_t dY2 =  p.Y()-fSlope->Y()*p.Z()-fOrigin->Y();
   dY2 *= dY2;
   Float_t prod2 = fSlope->Y()*(p.X()-fOrigin->X()) - fSlope->X()*(p.Y()-fOrigin->Y());
   prod2 *= prod2;
   
   Float_t distance = dX2 +dY2 + prod2;
   distance = TMath::Sqrt(distance);

   return distance;
}

//#################################################################

  //////////////////////////////////////////////////////////////////
  // Class Description of TAVTtrack                               //
  //                                                              //
  // A particle track from e.g. accelerator passing the tracker   //
  // The track is measured by the tracker with its silicon        //
  // reference planes                                             //
  // The track is e.g. a straight line                            //
  // The line is found by a fit to the hits in the silicon planes // 
  //                                                              //
  //////////////////////////////////////////////////////////////////


ClassImp(TAVTtrack) // Description of a Track

//______________________________________________________________________________
//  
TAVTtrack::TAVTtrack()
:  TObject(),
   fPileup(kFALSE),
   fType(0),
   fTrackNumber(-1),
   fLineTrajectory(new TAVTline()),
   fListOfClusters(new TClonesArray("TAVTcluster")),
   fShareCluster(0),
   fChiSquare(0.),
   fChiSquareU(0.),
   fChiSquareV(0.),
   fVertexZ(0.),
   fValidity(false),
   fImpactParameter(false),
   fChargeProba(new TArrayF(6)),
   fChargeWithMaxProba(0),
   fChargeMaxProba(0.),
   fChargeProbaNorm(new TArrayF(6)),
   fChargeWithMaxProbaNorm(0),
   fChargeMaxProbaNorm(0.),
   fDebugLevel(0)
{
   fListOfClusters->SetOwner(true);
}

//______________________________________________________________________________
//  
TAVTtrack::TAVTtrack(const TAVTtrack& aTrack)
:  TObject(aTrack),
   fPileup(aTrack.fPileup),
   fType(aTrack.GetType()),
   fTrackNumber(aTrack.GetNumber()),
   fLineTrajectory(new TAVTline(aTrack.GetTrackLine())),
   fShareCluster(aTrack.GetShareCluster()),
   fChiSquare(aTrack.GetChi2()),
   fChiSquareU(aTrack.GetChi2U()),
   fChiSquareV(aTrack.GetChi2V()),
   fVertexZ(aTrack.fVertexZ),
   fValidity(aTrack.fValidity),
   fImpactParameter(aTrack.fImpactParameter),
   fChargeProba(new TArrayF(*aTrack.fChargeProba)),
   fChargeWithMaxProba(aTrack.fChargeWithMaxProba),
   fChargeMaxProba(aTrack.fChargeMaxProba),
   fChargeProbaNorm(new TArrayF(*aTrack.fChargeProbaNorm)),
   fChargeWithMaxProbaNorm(aTrack.fChargeWithMaxProbaNorm),
   fChargeMaxProbaNorm(aTrack.fChargeMaxProbaNorm),
   fDebugLevel(aTrack.fDebugLevel)
{
   fListOfClusters = (TClonesArray*)aTrack.fListOfClusters->Clone();
}

//______________________________________________________________________________
//  
TAVTtrack::~TAVTtrack()
{
   delete fLineTrajectory;
   delete fListOfClusters;
   delete fChargeProba;
   delete fChargeProbaNorm;
}

//____________________________________________________________________________
//  
void TAVTtrack::Reset()
{
  fLineTrajectory->Zero();
  fPileup          = kFALSE;  
}

//______________________________________________________________________________
//  
void TAVTtrack::SetLineValue(const TVector3& aOrigin, const TVector3& aSlope, const Float_t aLength)
{
   fLineTrajectory->SetValue(aOrigin, aSlope, aLength);
}

//______________________________________________________________________________
//  
TVector3 TAVTtrack::Intersection(Float_t posZ) const
{
  // calculates the Intersection of the Track with the plane in
  // the coordinate system of the tracker.

  TVector3 result(GetTrackLine().GetOrigin());  // track origin in xyz tracker coordinates
  result(2) = 0.;
  result += GetTrackLine().GetSlopeZ() * posZ; // intersection in xyz frame at z_plane 
  return  result;
}

//______________________________________________________________________________
//  
Float_t TAVTtrack::Distance(TAVTtrack* track, Float_t z) const
{
   // calculates the distance with a track
   TVector3 pos1 = Intersection(z);
   TVector3 pos2 = track->Intersection(z);
   TVector3 pos0 = pos1-pos2;
   Float_t rho0  = pos0.Mag();
   
   return rho0;
}

//______________________________________________________________________________
//  
TVector2 TAVTtrack::DistanceXY(TAVTtrack* track, Float_t z) const
{
   // calculates the distance in X-Y with a track
   TVector3 pos1 = Intersection(z);
   TVector3 pos2 = track->Intersection(z);
   TVector3 pos0 = pos1-pos2;
   
   return TVector2(pos0.X(), pos0.Y());
}

//______________________________________________________________________________
//  
TVector2 TAVTtrack::DistanceMin(TAVTtrack* track, Float_t zMin, Float_t zMax, Float_t eps) const
{
   // calculates the minimum distance with a track
   Float_t z = 0.;
   Float_t rhoMin = Distance(track, zMin);
   Float_t rhoMax = Distance(track, zMax);
   Float_t rho  = 0.;
   Float_t rhoh = 0.;
   Float_t h    = eps;
   
   while (TMath::Abs(zMax - zMin) > eps) {
	  z    = (zMax + zMin)/2.;
	  rho  = Distance(track, z);
	  rhoh = Distance(track, z+h);
	  if ((rho - rhoh) < 0) {
		 zMax = z;
		 rhoMax = rho;
	  } else {
		 zMin = z;
		 rhoMin = rho;
	  }
   }
   return TVector2(z, rho);;
}

// __________________________________________________________________________
//
void TAVTtrack::MakeChiSquare(Float_t dhs)
{
   // Computes the chi2 of the fit track using dhs as the error
   Float_t tdU, tdV;
   Float_t tUtrack, tVtrack, err;
   Int_t   ndfU, ndfV;
   err = dhs;
   ndfU = ndfV = -2; // indeed, two parameters are fit per dimemsions (4 in all)
   fChiSquare = fChiSquareU = fChiSquareV = 0.0;
   if( GetClustersN() <=2 ) return; // return 0. for chisquare if there is less than 2 hits
   
   for (Int_t ht = 0; ht < GetClustersN(); ht++){
	  Float_t posZ = GetCluster(ht)->GetPosition().Pz();
	  tUtrack = Intersection(posZ).Px();
	  if (dhs == 0.)
		 dhs = GetCluster(ht)->GetPosError().Px();
	  tdU = (GetCluster(ht)->GetPositionG().Px() - tUtrack)/(dhs);
	  ndfU++;
	  fChiSquareU += tdU * tdU;
	  dhs = err;
	  
	  // only for pixel detector
	  tVtrack = Intersection(posZ).Py();
	  if (dhs == 0.)
		 dhs = GetCluster(ht)->GetPosError().Py();
	  tdV = (GetCluster(ht)->GetPositionG().Py() - tVtrack)/(dhs);
	  ndfV++;
	  fChiSquareV += tdV * tdV;
   
   }
   
   fChiSquare = (fChiSquareU+fChiSquareV) / Double_t(2*GetClustersN()-4);
   fChiSquareU /= ndfU;
   fChiSquareV /= ndfV;
   
   if(fDebugLevel) 
	  printf("TAVTtrack::MakeChiSquare chi2u=%f, ndfu=%d, chi2v=%f, ndfv=%d, chi2=%f, ndf=%d, resol=%f\n", 
			 fChiSquareU, ndfU, fChiSquareV, ndfV, fChiSquare, 2*GetClustersN()-4, dhs);
}

// __________________________________________________________________________
//
void TAVTtrack::SetChargeProba(const TArrayF* proba)
{
   const Float_t* array = proba->GetArray();
   fChargeProba->Set(proba->GetSize(), array);
}

// __________________________________________________________________________
//
void TAVTtrack::SetChargeProbaNorm(const TArrayF* proba)
{
   const Float_t* array = proba->GetArray();
   fChargeProbaNorm->Set(proba->GetSize(), array);
}

// __________________________________________________________________________
//
void TAVTtrack::AddCluster(TAVTcluster* cluster)
{
   TClonesArray &clusterArray = *fListOfClusters;
   new(clusterArray[clusterArray.GetEntriesFast()]) TAVTcluster(*cluster);
   
}

//##############################################################################

ClassImp(TAVTntuTrack);

TString TAVTntuTrack::fgkBranchName   = "vtTrack.";

//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTntuTrack::TAVTntuTrack() 
: TAGdata(),
  fListOfTracks(new TClonesArray("TAVTtrack")),
  fBeamPosition(),
  fPileup(false)
{
   fListOfTracks->SetOwner(true); 
   fBeamPosition.SetXYZ(0, 0, 0);
}
//------------------------------------------+-----------------------------------
//! Destructor.
TAVTntuTrack::~TAVTntuTrack() 
{
   delete fListOfTracks;
}

//------------------------------------------+-----------------------------------
//! return number of tracks
Int_t TAVTntuTrack::GetTracksN() const
{
   return fListOfTracks->GetEntries();
}

//------------------------------------------+-----------------------------------
//! return a Track for a given sensor
TAVTtrack* TAVTntuTrack::GetTrack(Int_t iTrack)
{
   if (iTrack >=0 || iTrack < GetTracksN())
	  return (TAVTtrack*)fListOfTracks->At(iTrack);
   else
	  return 0x0;
}

//------------------------------------------+-----------------------------------
//! return a pixel for a given sensor
const TAVTtrack* TAVTntuTrack::GetTrack(Int_t iTrack) const
{
   if (iTrack >=0 || iTrack < GetTracksN())
	  return (TAVTtrack*)fListOfTracks->At(iTrack);
   else
	  return 0x0;
}


//------------------------------------------+-----------------------------------
//! Setup clones.
void TAVTntuTrack::SetupClones()
{
   if (!fListOfTracks) {
	  fListOfTracks = new TClonesArray("TAVTtrack");
	  fListOfTracks->SetOwner(true);
   }
}

//------------------------------------------+-----------------------------------
//! Clear event.
void TAVTntuTrack::Clear(Option_t*)
{
   fListOfTracks->Delete();
}

//______________________________________________________________________________
//  
TAVTtrack* TAVTntuTrack::NewTrack()
{
   TClonesArray &trackArray = *fListOfTracks;
   TAVTtrack* track = new(trackArray[trackArray.GetEntriesFast()]) TAVTtrack();
   return track;
}

//______________________________________________________________________________
//  
TAVTtrack* TAVTntuTrack::NewTrack(TAVTtrack& trk)
{
   TClonesArray &trackArray = *fListOfTracks;
   TAVTtrack* track = new(trackArray[trackArray.GetEntriesFast()]) TAVTtrack(trk);
   return track;
}

/*------------------------------------------+---------------------------------*/
//! ostream insertion.
void TAVTntuTrack::ToStream(ostream& os, Option_t* option) const
{
	  
   os << "TAVTntuTrack " << GetName()
   << Form("  nPixels=%3d", GetTracksN())
   << endl;
   
   //TODO properly
   //os << "slat stat    adct    adcb    tdct    tdcb" << endl;
   for (Int_t j = 0; j < GetTracksN(); j++) {
	  const TAVTtrack*  track = GetTrack(j);
	  if (track)
		 os << Form("%4d", track->GetNumber());
	  os << endl;
   }
}


/*!
 \file
 \version $Id: TAITactBaseNtuTrack.cxx,v 1.9 2003/06/22 10:35:48 mueller Exp $
 \brief   Implementation of TAITactBaseNtuTrack.
 */
#include "TClonesArray.h"
#include "TMath.h"
#include "TF1.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TGraphErrors.h"
#include "TMath.h"
#include "TVector3.h"
#include "TVector2.h"

//BM
#include "TABMntuTrack.hxx"

// First
#include "TAGgeoTrafo.hxx"

//TAG
#include "TAGroot.hxx"

#include "TAITparGeo.hxx"
#include "TAITparConf.hxx"
#include "TAITparMap.hxx"
#include "TAITparCal.hxx"
#include "TAITntuTrack.hxx"
#include "TAITntuCluster.hxx"
#include "TAITactBaseNtuTrack.hxx"

/*!
 \class TAITactBaseNtuTrack 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAITactBaseNtuTrack);

Bool_t TAITactBaseNtuTrack::fgRefit = true;
//------------------------------------------+-----------------------------------
//! Default constructor.
TAITactBaseNtuTrack::TAITactBaseNtuTrack(const char* name, 
										 TAGdataDsc* pNtuClus,TAGdataDsc* pNtuTrack, TAGparaDsc* pConfig,  
										 TAGparaDsc* pGeoMap, TAGparaDsc* pCalib, TAGdataDsc* pBMntuTrack)
: TAGaction(name, "TAITactNtuTrack - Base NTuplize Tracker"),
  fpNtuTrack(pNtuTrack),
  fpNtuClus(pNtuClus),
  fpConfig(pConfig),
  fpGeoMap(pGeoMap),
  fpCalib(pCalib),
  fpBMntuTrack(pBMntuTrack),
  fTracksMaximum(100),
  fRequiredClusters(3),
  fSearchClusDistance(100),
  fGraphU(new TGraphErrors()),
  fGraphV(new TGraphErrors()),
  fBmTrackOk(false),
  fBmTrack(0x0),
  fBmTrackPos(0,0,0)
{
   AddDataIn(pNtuClus,   "TAITntuCluster");
   AddDataOut(pNtuTrack, "TAITntuTrack");
   AddPara(pGeoMap, "TAITparGeo");
   AddPara(pConfig, "TAITparConf");
   if (pCalib)
	  AddPara(pCalib, "TAITparCal");
   
   TAITparConf* config = (TAITparConf*) fpConfig->Object();
   fRequiredClusters   = config->GetAnalysisPar().PlanesForTrackMinimum;
   fSearchClusDistance = config->GetAnalysisPar().SearchHitDistance;
   SetGeoTrafo(TAGgeoTrafo::GetDefaultActName().Data());

}

//------------------------------------------+-----------------------------------
//! Destructor.
TAITactBaseNtuTrack::~TAITactBaseNtuTrack()
{   
   delete fGraphU;
   delete fGraphV;
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAITactBaseNtuTrack::CreateHistogram()
{
   DeleteHistogram();   
   TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();
   TAITparGeo* pGeoMap  = (TAITparGeo*) fpGeoMap->Object();
   
   fpHisPixelTot = new TH1F("vtTrackedClusPixTot", "Total # pixels per tracked clusters", 100, -0.5, 99.5);
   AddHistogram(fpHisPixelTot);
   
   fpHiVtxTgResX = new TH1F("vtTgResX", "Resolution at target in X", 500, -200, 200);
   AddHistogram(fpHiVtxTgResX);
   
   fpHiVtxTgResY = new TH1F("vtTgResY", "Resolution at target in Y", 500, -200, 200);
   AddHistogram(fpHiVtxTgResY);
   
   for (Int_t i = 0; i < pConfig->GetSensorsN(); ++i) {
	  if (TAITparConf::IsMapHistOn()) {
		 fpHisTrackMap[i] = new TH2F(Form("vtTrackMap%d", i+1), Form("Vertex - Tracks map for sensor %d", i+1), 
									 100, -pGeoMap->GetPitchV()*pGeoMap->GetPixelsNv()/2., pGeoMap->GetPitchV()*pGeoMap->GetPixelsNv()/2., 
									 100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2.);
		 fpHisTrackMap[i]->SetMarkerStyle(24);
		 fpHisTrackMap[i]->SetMarkerSize(1.);
		 fpHisTrackMap[i]->SetMarkerColor(3);
		 fpHisTrackMap[i]->SetStats(kFALSE);
		 AddHistogram(fpHisTrackMap[i]);
	  }
	  
	  fpHisPixel[i] = new TH1F(Form("vtTrackedClusPix%d", i+1), Form("# pixels per tracked clusters of sensor %d", i+1), 100, -0.5, 99.5);
	  AddHistogram(fpHisPixel[i]);
	  
	  fpHisResX[i] = new TH1F(Form("vtResX%d", i+1), Form("ResidualX of sensor %d", i+1), 400, -100, 100);
	  AddHistogram(fpHisResX[i]);
	  fpHisResY[i] = new TH1F(Form("vtResY%d", i+1), Form("ResidualY of sensor %d", i+1), 400, -100, 100);
	  AddHistogram(fpHisResY[i]);
   }
   
   fpHisResTotX = new TH1F("vtResTotX", "Total ResidualX", 400, -100, 100);
   fpHisResTotY = new TH1F("vtResTotY", "Total ResidualY", 400, -100, 100);
   AddHistogram(fpHisResTotX);
   AddHistogram(fpHisResTotY);
   
   fpHisChi2TotX = new TH1F("vtChi2TotX", "Total Chi2 X", 200, 0, 20);
   fpHisChi2TotY = new TH1F("vtChi2TotY", "Total Chi2 Y", 200, 0, 20);
   AddHistogram(fpHisChi2TotX);
   AddHistogram(fpHisChi2TotY);
   
   fpHisTrackEvt = new TH1F("vtTrackEvt", "Number of tracks per event", 21, 0, 20);
   AddHistogram(fpHisTrackEvt);
   
   fpHisTrackClus = new TH1F("vtTrackClus", "Number of clusters per track", 9, -0.5, 8.5);
   AddHistogram(fpHisTrackClus);
   
   fpHisClusSensor = new TH1F("vtClusSensor", "Number of tracked clusters per sensor", 9, -0.5, 8.5);
   AddHistogram(fpHisClusSensor);
   
   fpHisTheta = new TH1F("vtTrackTheta", "Track polar distribution", 500, 0, 90);
   AddHistogram(fpHisTheta);
   
   fpHisPhi = new TH1F("vtTrackPhi", "Track azimutal distribution (#theta > 5)", 500, -180, 180);
   AddHistogram(fpHisPhi);
   
   fpHisBeamProf = new TH2F("vtBeamProf", "Vertex Beam Profile", 
							100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., 
							100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2.);
   fpHisBeamProf->SetStats(kFALSE);
   AddHistogram(fpHisBeamProf);
   
   fpHisBmBeamProf = new TH2F("bmBeamProf", "BM Beam Profile", 
							  100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., 
							  100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2.);
   fpHisBmBeamProf->SetStats(kFALSE);
   AddHistogram(fpHisBmBeamProf);
   
   fpHisVtxResX = new TH1F("vtBmResX", "Vertex position resisdualX BM/VT",  500, -5000, 5000);
   AddHistogram(fpHisVtxResX);
   
   fpHisVtxResY = new TH1F("vtBmResY", "Vertex position resisdualY BM/VT",  500, -5000, 5000);   
   AddHistogram(fpHisVtxResY);
   
   fpHisBmChi2 = new TH1F("bmChi2", "BM chi2 of tracks", 200, 0, 1000);
   AddHistogram(fpHisBmChi2);
   
   fpHisClusLeft = new TH1F("vtClusLeft", "Clusters left per sensor", 8, 1, 8);
   AddHistogram(fpHisClusLeft);
   
   fpHisClusLeftPix = new TH2F("vtClusLeftPix", "Number of pixels for left clusters vs sensor", 
							   10, 0, 9, 50, 1, 50);
   fpHisClusLeftPix->SetStats(kFALSE);
   AddHistogram(fpHisClusLeftPix);
   
   SetValidHistogram(kTRUE);
   return;
}

//_____________________________________________________________________________
//  
Bool_t TAITactBaseNtuTrack::Action()
{
   
   // BM tracks
   if (fpBMntuTrack) 
	  CheckBM();
   
   
   // VTX
   TAITntuTrack* pNtuTrack = (TAITntuTrack*) fpNtuTrack->Object();
   pNtuTrack->Clear();
   
   // looking straight
   FindStraightTracks();
   
   // looking inclined line
   if (!FindTiltedTracks()){
	  if (ValidHistogram())
		 FillHistogramm();
	  fpNtuTrack->SetBit(kValid);
	  return true;
   }
   
   if (FindVertices())
	  FindTiltedTracks();
   
   if(FootDebugLevel(1)) {
	  printf(" %d tracks found\n", pNtuTrack->GetTracksN());
	  for (Int_t i = 0; i < pNtuTrack->GetTracksN(); ++i) {
		 TAITtrack* track = pNtuTrack->GetTrack(i);
		 printf("   with # clusters %d\n", track->GetClustersN());
	  }
   }
   
   if (ValidHistogram())
	  FillHistogramm();
   
   // Set charge probability
   SetChargeProba();
   
   fpNtuTrack->SetBit(kValid);
   return true;
}


//_____________________________________________________________________________
//  
void TAITactBaseNtuTrack::CheckBM()
{   
   // BM info
   TAITparConf*  pConfig  = (TAITparConf*) fpConfig->Object();
   
   Float_t zDiff = 0;
   
   if (fpFirstGeo) {
	  TVector3 bmPos = fpFirstGeo->GetBMCenter();
	  TVector3 vtPos = fpFirstGeo->GetVTCenter();
	  zDiff  = vtPos.Z() - bmPos.Z();	  
   }
   
   TABMntuTrack* pBMtrack = 0x0;
   fBmTrackOk    = false;
   pBMtrack = (TABMntuTrack*) fpBMntuTrack->Object();
   if (pBMtrack->GetTracksN() > 0)
	  fBmTrack = pBMtrack->Track(0);
   
   if (fBmTrack) {
	  fBmTrackPos  = fBmTrack->PointAtLocalZ(zDiff);
	  fBmTrackPos *= TAGgeoTrafo::CmToMu();
	  Float_t chi2 = fBmTrack->GetChi2();
	  if (ValidHistogram())
		 fpHisBmChi2->Fill(chi2);
	  
	  if (chi2 < pConfig->GetAnalysisPar().BmTrackChi2Limit && chi2 > 0) 
		 fBmTrackOk = true;
   }   
}

//_____________________________________________________________________________
//  
void TAITactBaseNtuTrack::SetChargeProba()
{
   if (fpCalib == 0x0) return;
   TAITntuTrack* ntuTrack = (TAITntuTrack*) fpNtuTrack->Object();
   TAITparCal*    pCalib  = (TAITparCal*) fpCalib->Object();
   
   Int_t nTrack = ntuTrack->GetTracksN();
   if (nTrack == 0) return;
   
   for (Int_t iTrack = 0; iTrack < nTrack; ++iTrack) {
	  
	  Int_t nPixelsTot = 0;
	  Float_t nPixelsMean;
	  TAITtrack* track = ntuTrack->GetTrack(iTrack);
	  Int_t nClus = track->GetClustersN();
	  
	  for (Int_t i=0;i<nClus;i++) {
		 TAITcluster* clus = track->GetCluster(i);
		 nPixelsTot += clus->GetPixelsN();
	  }
	  nPixelsMean = nPixelsTot/nClus;
	  const TArrayF* proba =  pCalib->GetChargeProba(nPixelsMean);
	  track->SetChargeProba(proba);
	  track->SetChargeMaxProba(pCalib->GetChargeMaxProba());
	  track->SetChargeWithMaxProba(pCalib->GetChargeWithMaxProba());
      
      proba =  pCalib->GetChargeProbaNorm(nPixelsMean);
	  track->SetChargeProbaNorm(proba);
	  track->SetChargeMaxProbaNorm(pCalib->GetChargeMaxProbaNorm());
	  track->SetChargeWithMaxProbaNorm(pCalib->GetChargeWithMaxProbaNorm());
   }   
}

//_____________________________________________________________________________
//  
Bool_t TAITactBaseNtuTrack::FindStraightTracks()
{
   Double_t minDistance  = 1.e9;
   Double_t aDistance;
   
   TVector3 lineOrigin;    // origin in the tracker system
   TVector3 lineSlope;     // slope along z-axis in tracker system
   TVector3 pos ;
   
   // init
   lineOrigin.SetXYZ(0.,0.,0.);
   lineSlope.SetXYZ(0.,0.,1.);
   
   TAITntuCluster* pNtuClus  = (TAITntuCluster*) fpNtuClus->Object();
   TAITntuTrack*   pNtuTrack = (TAITntuTrack*)   fpNtuTrack->Object();
   TAITparGeo*     pGeoMap   = (TAITparGeo*)     fpGeoMap->Object();
   TAITparConf*    pConfig   = (TAITparConf*)    fpConfig->Object();
   
   TList array;
   array.SetOwner(false);
   array.Clear();
   
   Int_t nPlane = pGeoMap->GetSensorsN()-1;
   Int_t curPlane = nPlane;
   
   while (curPlane >= fRequiredClusters-1) {
	  // Get the last reference plane
	  curPlane = nPlane--;
	  TClonesArray* list = pNtuClus->GetListOfClusters(curPlane);
	  Int_t nClusters = pNtuClus->GetClustersN(curPlane);
	  if ( nClusters == 0) continue;
	  
	  // Loop on all clusters of the first plane
	  // and try to make a track out of each one
	  for( Int_t iLastClus = 0; iLastClus < nClusters; ++iLastClus) { // loop on cluster of last plane, 
		 
		 if( pNtuTrack->GetTracksN() >= pConfig->GetAnalysisPar().TracksMaximum ) break; // if max track number reach, stop
		 
		 TAITcluster* cluster = (TAITcluster*)list->At( iLastClus );
		 if (cluster->GetFound()) continue;
		 TAITtrack*   track   = new TAITtrack();
		 array.Clear();
		 track->AddCluster(cluster);
		 array.Add(cluster);
		 
		 lineOrigin.SetXYZ(cluster->GetPositionU(), cluster->GetPositionV(), 0); // parallel lines
		 lineOrigin = pGeoMap->Local2Global(curPlane, lineOrigin);
		 lineSlope.SetXYZ(0, 0, 1);
		 
		 track->SetLineValue(lineOrigin, lineSlope);
		 
		 // Loop on all planes to find a matching cluster in them
		 for( Int_t iPlane = curPlane-1; iPlane >= 0; --iPlane) { // loop on planes
			TClonesArray* list1 = pNtuClus->GetListOfClusters(iPlane);
			Int_t nClusters1 = pNtuClus->GetClustersN(iPlane);
			if (nClusters1 == 0) continue; //empty planes
			
			// loop on all clusters of this plane and keep the nearest one
			minDistance = fSearchClusDistance;
			TAITcluster* bestCluster = 0x0;
			
			for( Int_t iClus = 0; iClus < nClusters1; ++iClus ) { // loop on plane clusters
			   TAITcluster* aCluster = (TAITcluster*)list1->At( iClus );
			   
			   if( aCluster->GetFound()) continue; // skip cluster already found
			   
			   aDistance = aCluster->Distance(track);
			   
			   if( aDistance < minDistance ) {
				  minDistance = aDistance;
				  bestCluster = aCluster;
			   }
			} // end loop on plane clusters
			
			// if a cluster has been found, add the cluster
			if( bestCluster ) {	
			   bestCluster->SetFound();
			   cluster->SetFound();
			   track->AddCluster(bestCluster);
			   array.Add(bestCluster);
			   if (fgRefit)
				  UpdateParam(track);
			}
			
		 } // end loop on planes
		 
		 // Apply cuts
		 if (AppyCuts(track)) {
			track->SetNumber(pNtuTrack->GetTracksN());
			track->MakeChiSquare();
			track->SetType(0);
			//			cout<<"RecAn:: "<<track->GetTrackLine().GetTheta()*TMath::DegToRad()<<" RecX:: "<<track->GetTrackLine().GetSlopeZ().Unit().X()<<" RecY:: "<<track->GetTrackLine().GetSlopeZ().Unit().Y()<<endl;
			pNtuTrack->NewTrack(*track);
			TVector3 orig(0,0,0);
			pNtuTrack->SetBeamPosition(orig);
			
			if (ValidHistogram()) {
			   FillHistogramm(track);
			}
			
			delete track;
		 } else { // reset clusters
			for (Int_t i = 0; i < array.GetEntries(); ++i) {
			   TAITcluster*  cluster1 = (TAITcluster*)array.At(i);
			   cluster1->SetFound(false);
			}
			delete track;
			array.Clear();
		 }
		 
	  } // end loop on first clusters
	  
   } // while
   
   
   return true;
}

//_____________________________________________________________________________
//  
Bool_t TAITactBaseNtuTrack::AppyCuts(TAITtrack* track)
{
   Bool_t valid = false;  
   
   TAITparConf* pConfig = (TAITparConf*) fpConfig->Object();
   if (track->GetClustersN() >= pConfig->GetAnalysisPar().PlanesForTrackMinimum )
  	  valid = true;
   
   return valid;
}

//_____________________________________________________________________________
//  
void TAITactBaseNtuTrack::UpdateParam(TAITtrack* track)
{
   TVector3           lineOrigin;  // origin in the tracker system
   TVector3           lineSlope;   // slope along z-axis in tracker system
   
   // init
   lineOrigin.SetXYZ(0.,0.,0.);
   lineSlope.SetXYZ(0.,0.,1.);
   
   TAITparGeo* pGeoMap = (TAITparGeo*) fpGeoMap->Object();
   
   Int_t nClusters = track->GetClustersN();
   
   if( nClusters == 2) {
	  TAITcluster* cluster0 = track->GetCluster(0);
	  TAITcluster* cluster1 = track->GetCluster(1);
	  
	  if(FootDebugLevel(1))
		 printf("TAITactNtuTrack::Analyse track with %d cluster taking origin from it and slope 0\n", track->GetClustersN());
	  Double_t x0 = cluster0->GetPositionG()[0];
	  Double_t y0 = cluster0->GetPositionG()[1];
	  
	  Double_t x1 = cluster1->GetPositionG()[0];
	  Double_t y1 = cluster1->GetPositionG()[1];
	  
	  Double_t z0 = pGeoMap->GetSensorPar(cluster0->GetPlaneNumber()).Position[2];
	  Double_t z1 = pGeoMap->GetSensorPar(cluster1->GetPlaneNumber()).Position[2];
	  
	  if(FootDebugLevel(1)) 
		 printf( "TAITactNtuTrack::Analyze cluster[pl %d]=(%.2f, %.2f, %.2f) cluster[pl %d]=(%.2f, %.2f, %.2f)\n",
				cluster0->GetPlaneNumber(), x0, y0, z0, 
				cluster1->GetPlaneNumber(), x1, y1, z1);
	  
	  if( z1-z0 != 0.) {
		 lineOrigin(0) = (z1*x0-z0*x1)/(z1-z0);
		 lineOrigin(1) = (z1*y0-z0*y1)/(z1-z0);
		 lineSlope(0)  = (x1-x0)/(z1-z0);
		 lineSlope(1)  = (y1-y0)/(z1-z0);
	  }
   } else {
	  Double_t x, dx;
	  Double_t y, dy;
	  Double_t z, dz;
	  
	  fGraphU->Set(nClusters);
	  fGraphV->Set(nClusters);
	  
	  for (Int_t i = 0; i < nClusters; ++i) {
		 TAITcluster* cluster = track->GetCluster(i);
		 x  = cluster->GetPositionG()(0);
		 y  = cluster->GetPositionG()(1);
		 z  = cluster->GetPositionG()(2);
		 dx = cluster->GetPosError()(0);
		 dy = cluster->GetPosError()(1);
		 dz = cluster->GetPosError()(2);
		 fGraphU->SetPoint(i, z, x);
		 fGraphU->SetPointError(i, dz, dx);
		 
		 fGraphV->SetPoint(i, z, y);
		 fGraphV->SetPointError(i, dz, dy);
	  }
	  
	  fGraphU->Fit("pol1", "Q");
	  TF1* polyU = fGraphU->GetFunction("pol1");
	  (lineOrigin)(0) = polyU->GetParameter(0);
	  (lineSlope)(0)  = polyU->GetParameter(1);
	  fGraphV->Fit("pol1", "Q");
	  TF1* polyV = fGraphV->GetFunction("pol1");
	  (lineOrigin)(1) = polyV->GetParameter(0);
	  (lineSlope)(1)  = polyV->GetParameter(1);
   }
   
   track->SetLineValue(lineOrigin, lineSlope);
}

//_____________________________________________________________________________
//  
void TAITactBaseNtuTrack::FillHistogramm(TAITtrack* track)
{   
   TAITline line = track->GetTrackLine();
   fpHisTheta->Fill(line.GetTheta());
   
   if (line.GetTheta() > 5)
	  fpHisPhi->Fill(line.GetPhi());
   
   TAITparGeo* pGeoMap = (TAITparGeo*)fpGeoMap->Object();
   
   fpHisTrackClus->Fill(track->GetClustersN());
   for (Int_t i = 0; i < track->GetClustersN(); ++i) {
	  TAITcluster * cluster = track->GetCluster(i);
	  cluster->SetFound();
	  Int_t idx = cluster->GetPlaneNumber();
	  fpHisPixelTot->Fill(cluster->GetPixelsN());
	  fpHisPixel[idx]->Fill(cluster->GetPixelsN());
	  
	  Float_t posZ       = cluster->GetPositionG()[2];
	  TVector3 impact    = track->Intersection(posZ);
	  TVector3 impactLoc = pGeoMap->Global2Local(idx, impact);
	  if (TAITparConf::IsMapHistOn()) 
		 fpHisTrackMap[idx]->Fill(impactLoc[0], impactLoc[1]);
	  fpHisResTotX->Fill(impact[0]-cluster->GetPositionG()[0]);
	  fpHisResTotY->Fill(impact[1]-cluster->GetPositionG()[1]);
	  fpHisResX[idx]->Fill(impact[0]-cluster->GetPositionG()[0]);
	  fpHisResY[idx]->Fill(impact[1]-cluster->GetPositionG()[1]);
	  fpHisClusSensor->Fill(idx+1);
   }
   fpHisChi2TotX->Fill(track->GetChi2U());
   fpHisChi2TotY->Fill(track->GetChi2V());
   
   TVector3 origin = track->GetTrackLine().GetOrigin();
   fpHisBeamProf->Fill(origin.X(), origin.Y());
}

//_____________________________________________________________________________
//  
void TAITactBaseNtuTrack::FillHistogramm()
{
   TAITntuCluster* pNtuClus  = (TAITntuCluster*) fpNtuClus->Object();
   TAITntuTrack*   pNtuTrack = (TAITntuTrack*)   fpNtuTrack->Object();
   
   fpHisTrackEvt->Fill(pNtuTrack->GetTracksN());
   if (pNtuTrack->GetTracksN() == 0)
	  fpHisClusSensor->Fill(0);
   
   for (Int_t iPlane = 0; iPlane < TAITparMap::GetSensorsN(); ++iPlane) {
	  
	  TClonesArray* list = pNtuClus->GetListOfClusters(iPlane);
	  Int_t nClusters = pNtuClus->GetClustersN(iPlane);
	  if ( nClusters == 0) continue;
	  
	  Int_t left = 0;
	  for( Int_t iClus = 0; iClus < nClusters; ++iClus) { 
		 
		 TAITcluster* cluster = (TAITcluster*)list->At(iClus);
		 if (!cluster->GetFound()) {
			fpHisClusLeftPix->Fill(iPlane+1, cluster->GetPixelsN());
			left++;
		 }
	  }
	  static Float_t mean[8];
	  Int_t evtNumber = gTAGroot->CurrentEventNumber();
	  mean[iPlane] = evtNumber/(evtNumber+1.)*mean[iPlane] + 1./(evtNumber+1.)*(left/nClusters);
	  fpHisClusLeft->SetBinContent(iPlane+1, mean[iPlane]*100);
   }   
}

//_____________________________________________________________________________
//  
void TAITactBaseNtuTrack::FillBmHistogramm(TVector3 bmTrackPos)
{   
   TAITntuTrack* pNtuTrack = (TAITntuTrack*) fpNtuTrack->Object();
   bmTrackPos  = fpFirstGeo->FromBMLocalToGlobal(bmTrackPos);
   fpHisBmBeamProf->Fill(bmTrackPos.X(), bmTrackPos.Y());
   
   for (Int_t i = 0; i < pNtuTrack->GetTracksN(); ++i) {
	  TAITtrack* track  = pNtuTrack->GetTrack(i);
	  TVector3   origin = track->GetTrackLine().GetOrigin();
	  
	  origin  = fpFirstGeo->FromVTLocalToGlobal(origin);
	  TVector3 res = origin - bmTrackPos;
	  fpHisVtxResX->Fill(res.X());
	  fpHisVtxResY->Fill(res.Y());
   }   
}


//_____________________________________________________________________________
//  
TAITcluster* TAITactBaseNtuTrack::NearestCluster(TAITtrack *aTrack, Int_t iSensor)
{   
   // For a given track, return the index of the nearest hit of a given plane
   TAITntuCluster* pNtuClus = (TAITntuCluster*) fpNtuClus->Object();

   Double_t minDist = 1.e9;
   Double_t distance;
   TAITcluster *tCluster;
   
   TAITcluster *tNearest = 0x0;
   for (Int_t ht = 0; ht < pNtuClus->GetClustersN(iSensor); ++ht) { // loop on hits
	  tCluster = pNtuClus->GetCluster(iSensor, ht);
	  distance = tCluster->Distance( aTrack) ;
	  if( distance < minDist) {
		 minDist = distance;
		 tNearest = tCluster;
	  }
   } //end loop on hits
   
   return tNearest;
}

//_____________________________________________________________________________
//  
TAITtrack* TAITactBaseNtuTrack::NearestTrack(TAITcluster *aCluster) {
   
   // For a given track, return the index of the nearest hit of a given plane
   Double_t minDist = 1.e9;
   Double_t distance;
   TAITtrack *tTrack;
   
   TAITtrack *tNearest = 0x0;
   TAITntuTrack* pNtuTrack = (TAITntuTrack*) fpNtuTrack->Object();

   for( Int_t it = 0; it < pNtuTrack->GetTracksN(); ++it) { // loop on tracks
	  tTrack = pNtuTrack->GetTrack(it);
	  distance = aCluster->Distance( tTrack);
	  if( distance < minDist) {
		 minDist = distance;
		 tNearest = tTrack;
	  }
   } // end loop on tracks
   
   return tNearest;
}

//______________________________________________________________________________
//  
void TAITactBaseNtuTrack::SetGeoTrafo(TString name)
{
   fpFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(name.Data());
   if (!fpFirstGeo)
	  printf("No GeoTrafo action %s available yet\n", name.Data());
   else 
	  printf("GeoTrafo action %s found\n", name.Data());
}

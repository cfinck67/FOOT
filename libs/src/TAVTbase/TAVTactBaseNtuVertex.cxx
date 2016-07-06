/*!
 \file
 \brief   Implementation of TAVTactBaseNtuVertex.
 */
#include "TClonesArray.h"
#include "TMath.h"
#include "TH2F.h"

//TAG
#include "TAGroot.hxx"
#include "TAGgeoTrafo.hxx"
//BM
#include "TABMntuTrack.hxx"

// VTX
#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuTrack.hxx"
#include "TAVTntuCluster.hxx"
#include "TAVTntuVertex.hxx"
#include "TAVTactBaseNtuVertex.hxx"


/*!
 \class TAVTactBaseNtuVertex 
 \brief NTuplizer for vertex raw hits. **
 */

ClassImp(TAVTactBaseNtuVertex);
Float_t TAVTactBaseNtuVertex::fgAngleLimitDiffusion = 1.23;
Bool_t  TAVTactBaseNtuVertex::fgAngleDiffusionFlag  = false;
//------------------------------------------+-----------------------------------
//! Default constructor.
TAVTactBaseNtuVertex::TAVTactBaseNtuVertex(const char* name, 
										 TAGdataDsc* pNtuTrack,TAGdataDsc* pNtuVertex,
										 TAGparaDsc* pConfig,  TAGparaDsc* pGeoMap, TAGdataDsc* pBmTrack)
: TAGaction(name, "TAVTactBaseNtuVertex"),
  fpNtuTrack(pNtuTrack),
  fpNtuVertex(pNtuVertex),
  fpConfig(pConfig),
  fpGeoMap(pGeoMap),
  fpBMntuTrack(pBmTrack),
  fDebugLevel(0),
  fCheckPileUp(false),
  fCheckBmMatching(true)
{
    AddDataIn(pNtuTrack,   "TAVTntuTrack");
    AddDataOut(pNtuVertex, "TAVTntuVertex");
    AddPara(pGeoMap, "TAVTparGeo");
    AddPara(pConfig, "TAVTparConf");
   
   TAVTparConf* config = (TAVTparConf*) fpConfig->Object();
   fSearchClusDistance = config->GetAnalysisPar().SearchHitDistance;
   fVtxPos.SetXYZ(0,0,0);
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAVTactBaseNtuVertex::~TAVTactBaseNtuVertex()
{   
}

//------------------------------------------+-----------------------------------
//! Setup all histograms.
void TAVTactBaseNtuVertex::CreateHistogram()
{
   DeleteHistogram();   
   TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMap->Object();
   
   fpHisPosZ = new TH1F("vtVtxPosZ", "Vertex position at Z", 100, -pGeoMap->GetTargetWidth()/2., pGeoMap->GetTargetWidth()/2.);
   AddHistogram(fpHisPosZ);
   
   if (fCheckBmMatching) {
	  fpHisBmMatchX = new TH1F("vtBmMatchX", "Residual vertexing - BM in X", 500, -pGeoMap->GetTargetWidth()/2., pGeoMap->GetTargetWidth()/2.);
	  AddHistogram(fpHisBmMatchX);
   
	  fpHisBmMatchY = new TH1F("vtBmMatchY", "Residual vertexing - BM in Y", 500, -pGeoMap->GetTargetWidth()/2., pGeoMap->GetTargetWidth()/2.);
	  AddHistogram(fpHisBmMatchY);
   }
   
   if (TAVTparConf::IsMapHistOn()) {
	  fpHisPosXY = new TH2F( "vtVtxPosXY", "Vertex position at XY", 
							100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNv()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNv()/2., 
							100, -pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2., pGeoMap->GetPitchU()*pGeoMap->GetPixelsNu()/2.);
	  fpHisPosXY->SetMarkerColor(3);
	  fpHisPosXY->SetStats(kFALSE);
	  AddHistogram(fpHisPosXY);
   }
   
   SetValidHistogram(kTRUE);
}

//_______________________________________________________________________________
//!Action
Bool_t TAVTactBaseNtuVertex::Action()
{
   Bool_t ok = true;
  
   TAVTntuTrack* pNtuTrack = (TAVTntuTrack*) fpNtuTrack->Object();
   Bool_t check = CheckPileUp();
   Bool_t diffusion = false;

   Int_t nTrack = pNtuTrack->GetTracksN();

   if( nTrack == 0) {
        fpNtuVertex->SetBit(kValid);
        return true;
   }

   if (nTrack == 1) {
	  diffusion = SearchDiffusion(0);
	  if(!diffusion)
		 SetNotValidVertex(0);
	  else {
		 SetValidVertex();
		 if (fDebugLevel)
			cout<<"diffusion "<<endl;
	  }
   } else if (nTrack >= 2 && check) { 
	  for(Int_t q =0; q< nTrack; ++q) 
		 SetNotValidVertex(q);
   } else
	  ok = ComputeVertex();

   if(ok)
	  fpNtuVertex->SetBit(kValid);
   
   if (fCheckBmMatching)
	  CheckBmMatching();

    return ok;
}


//-------------------------------------------------------------------------------------
//!Check Pile Up
Bool_t TAVTactBaseNtuVertex::CheckPileUp()
{
    if (fCheckPileUp) {//CheckPile Up
	   ComputePileUp();
	   TAVTntuTrack* pNtuTrack = (TAVTntuTrack*) fpNtuTrack->Object();
	   return pNtuTrack->IsPileUp();
    } else
        return false;
}

//-------------------------------------------------------------------------------------
//!Check BM matching
Bool_t TAVTactBaseNtuVertex::CheckBmMatching()
{
   if (!fpBMntuTrack || !fpNtuVertex) return false;
   
   TABMntuTrack* pBMtrack    = (TABMntuTrack*) fpBMntuTrack->Object();
   TAVTntuVertex* pNtuVertex = (TAVTntuVertex*)fpNtuVertex->Object();

   Float_t min = 999999;

   TABMntuTrackTr* bmTrack = pBMtrack->Track(0);
   if (!bmTrack) return false;
   
   TAGgeoTrafo* pFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());

   if (!pFirstGeo) return false;
	   
   TVector3 bestRes;
   Int_t    bestIdx = -1;
   Bool_t isGood = false;
   for (Int_t i = 0; i < pNtuVertex->GetVertexN(); ++i) {
	  TAVTvertex* vtx      = pNtuVertex->GetVertex(i);
	  TVector3 vtxPosition = vtx->GetVertexPosition();
	  vtxPosition  = pFirstGeo->FromVTLocalToGlobal(vtxPosition*TAGgeoTrafo::MuToCm());
	  
	  TVector3 bmPosition = pFirstGeo->FromGlobalToBMLocal(vtxPosition);
	  vtxPosition *= TAGgeoTrafo::CmToMu();

	  bmPosition  = bmTrack->PointAtLocalZ(bmPosition.Z());
	  bmPosition  = pFirstGeo->FromBMLocalToGlobal(bmPosition);
	  bmPosition *= TAGgeoTrafo::CmToMu();
	  
	  TVector3 res = vtxPosition - bmPosition;
	  if (res.Perp() < min) {
		 min = res.Perp();
		 bestRes = res;
		 isGood = true;
		 bestIdx = i;
	  }
   }
   
   if (!isGood) return false;

   if (fCheckBmMatching) {
	  if (bestIdx != -1) {
		 TAVTvertex* vtx  = pNtuVertex->GetVertex(bestIdx);
		 vtx->SetBmMatched();
	  }
	  
	  if (ValidHistogram()) {
		 fpHisBmMatchX->Fill(bestRes.X());
		 fpHisBmMatchY->Fill(bestRes.Y());
	  }
   }
   
   return true;
}

//------------------------------------------------------------
//!Search diffusion
Bool_t TAVTactBaseNtuVertex::SearchDiffusion(Int_t idTk)
{
   Bool_t result = false;
      
   // retrieve trafo
   TAGgeoTrafo* pFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   //Track from bm
   if (!fpBMntuTrack)
	  return false;
   TABMntuTrack* pBMtrack  = (TABMntuTrack*) fpBMntuTrack->Object();
   if (!pBMtrack) return false;
   TABMntuTrackTr* bmTrack = pBMtrack->Track(0);
   if (!bmTrack) return false;
   
   TVector3 slope = bmTrack->GetDirection();
   TVector3 direction = slope.Unit();
   direction = pFirstGeo->VecFromBMLocalToGlobal(direction);
   Double_t angleBm = direction.Theta()*TMath::RadToDeg();
   
   //Track from vtx
   TAVTntuTrack* pNtuTrack = (TAVTntuTrack*) fpNtuTrack->Object();
   TAVTtrack* track0       = pNtuTrack->GetTrack(idTk);
   TAVTline line1          = track0->GetTrackLine();
   
   direction = line1.GetSlopeZ().Unit();
   direction = pFirstGeo->VecFromVTLocalToGlobal(direction);
   Double_t angleTk = direction.Theta()*TMath::RadToDeg();
   
   if(TMath::Abs(angleBm-angleTk) < fgAngleLimitDiffusion && fgAngleDiffusionFlag) {
	  result = false;//No diffusion
   } else {
	  if (fDebugLevel)
		 cout<<"Relative angle "<<TMath::Abs(angleBm-angleTk)<<endl;
	  ComputeInteractionVertex(bmTrack, line1);
	  result = true;
   }
   
   return result;
}

//--------------------------------------------------------------
//!Compute the point interaction of diffusion
void TAVTactBaseNtuVertex::ComputeInteractionVertex(TABMntuTrackTr* lbm, TAVTline lvtx)
{
   // retrieve trafo
   TAGgeoTrafo* pFirstGeo = (TAGgeoTrafo*)gTAGroot->FindAction(TAGgeoTrafo::GetDefaultActName().Data());
   
   //prendiamo il punto A della retta del bm
   Double_t z = 0;
   Double_t DZ = 1;
   TVector3 Apoint (lbm->PointAtLocalZ(z).X(), lbm->PointAtLocalZ(z).Y(),z); //coordinate del punto A appartenente alla retta del bm in Z = 0
   Apoint  = pFirstGeo->FromBMLocalToGlobal(Apoint);
   Apoint *= TAGgeoTrafo::CmToMu();
   TVector3 Bpoint (lvtx.GetPoint(z).X(),lvtx.GetPoint(z).Y(),z); //coordinate del punto B appartenente alla retta del vtx in Z = 0
   Bpoint  = pFirstGeo->FromVTLocalToGlobal(Bpoint*TAGgeoTrafo::MuToCm());
   Bpoint *= TAGgeoTrafo::CmToMu();
   
   TVector3 AmB = Apoint-Bpoint;
   
   TVector3 pSlopebm(lbm->ux/lbm->uz, lbm->uy/lbm->uz, 1);
   TVector3 pDirbm  = pFirstGeo->VecFromBMLocalToGlobal(pSlopebm*DZ); //director parameter of bm line
   TVector3 pDirvtx = pFirstGeo->VecFromVTLocalToGlobal(lvtx.GetSlopeZ()*DZ); //director parameter of vtx line
   
   Double_t etaBm = pDirbm*pDirbm;
   Double_t eta = pDirvtx*pDirvtx;
   Double_t mix = pDirbm*pDirvtx;
   Double_t Apar = AmB*pDirbm;
   Double_t Bpar = AmB*pDirvtx;
   
   
   Double_t q = (Apar*mix-etaBm*Bpar)/(mix*mix - eta*etaBm);
   Double_t p = (-Apar+q*mix)/etaBm;
   
   TVector3 P = Apoint + pDirbm*p;
   TVector3 Q = Bpoint + pDirvtx*q;
   
   fVtxPos = (P+Q)*0.5;
   // Again in local frame of VTX
   fVtxPos = pFirstGeo->FromGlobalToVTLocal(fVtxPos*TAGgeoTrafo::MuToCm());
   fVtxPos *= TAGgeoTrafo::CmToMu();
   
   if (fDebugLevel)
	  fVtxPos.Print();
}

//------------------------------------------------------------------------------------
//!Set vertex not valid
Bool_t TAVTactBaseNtuVertex::SetNotValidVertex(Int_t idTk)
{
   TAVTntuVertex* pNtuVertex = (TAVTntuVertex*)fpNtuVertex->Object();
   //Crete a vertex
   TVector3 vtxPos(0,0,0);
   TAVTvertex* vtx = new TAVTvertex();
   vtx->SetVertexValidity(0);
   
   //Attacchiamo la traccia idTk al vertice 
   TAVTntuTrack* pNtuTrack = (TAVTntuTrack*) fpNtuTrack->Object();
   TAVTtrack*    track0    = pNtuTrack->GetTrack(idTk);
   
   vtxPos = track0->Intersection(0);//Target center
   vtx->SetVertexPosition(vtxPos);
   track0->SetValidity(-1);
   track0->SetIP(0);
   track0->SetVertexZ(vtxPos.Z());
   vtx->AddTrack(track0);
   pNtuVertex->NewVertex(*vtx);
   
   delete vtx;
   
   return true;
}

//-----------------------------------------------------------------
//!SetValid Vertex

void TAVTactBaseNtuVertex::SetValidVertex()
{
   TAVTntuVertex* ntuVertex = (TAVTntuVertex*)fpNtuVertex->Object();
   
   TAVTvertex* vtx = new TAVTvertex();
   vtx->SetVertexValidity(1);
   vtx->SetVertexPosition(fVtxPos);
   
   
   TAVTntuTrack* ntuTrack = (TAVTntuTrack*)fpNtuTrack->Object();
   Int_t nTracks = ntuTrack->GetTracksN();
   for(Int_t q = 0; q<nTracks; ++q ){
	  TAVTtrack* track0 = ntuTrack->GetTrack(q);
	  track0->SetValidity(1);
	  track0->SetVertexZ(fVtxPos.Z());
	  vtx->AddTrack(track0);
   }
   
   ntuVertex->NewVertex(*vtx);
   
   delete vtx;
}

//--------------------------------------------
//!compute pileup
void TAVTactBaseNtuVertex::ComputePileUp()
{
   
   TAVTntuTrack* pNtuTrack = (TAVTntuTrack*) fpNtuTrack->Object();
   Int_t nTracks = pNtuTrack->GetTracksN();
   Bool_t pileup = false;
   pNtuTrack->SetPileUp(false);
   for (Int_t i = 0; i < nTracks; ++i) {
	  for (Int_t j = i+1; j < nTracks; ++j) {
		 TAVTtrack* track0 = pNtuTrack->GetTrack(i);
		 TAVTtrack* track1 = pNtuTrack->GetTrack(j);
		 TVector2 pos = track0->DistanceMin(track1);
		 if (TMath::Abs(pos.X()) > TAVTparGeo::GetTargetWidth()/2. || pos.Y() > fSearchClusDistance) {
			track0->SetPileUp();
			track1->SetPileUp();
			pileup = true;
		 }
	  }
   }
   
   if (pileup) 
	  pNtuTrack->SetPileUp();
}

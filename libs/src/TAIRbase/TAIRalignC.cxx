/*!
 // C.A. Reidel & Ch. Finck
 
 \file
 \brief   Implementation of TAIRalignC.
 */
#include "TMath.h"
#include "TVector3.h"
#include "TH1F.h"
#include "TF1.h"
#include "TList.h"
#include "TStyle.h"
#include "TArrayD.h"
#include "TGraphErrors.h"
#include "TObjArray.h"
#include "TCanvas.h"

#include "TAGparGeo.hxx"
#include "TAGactTreeReader.hxx"
#include "TAGdataDsc.hxx"
#include "TAGgeoTrafo.hxx"

#include "TAVTparGeo.hxx"
#include "TAVTparConf.hxx"
#include "TAVTntuCluster.hxx"

#include "TAMSDparGeo.hxx"
#include "TAMSDparConf.hxx"
#include "TAMSDntuCluster.hxx"

#include "TAIRalignC.hxx"

#include "TAIRparDiff.hxx"
#include "TAIRntuAlignC.hxx"


/*!
 \class TAIRalignC
 \brief Alignment class for vertex tracks. **
 */

ClassImp(TAIRalignC);

TAIRalignC* TAIRalignC::fgInstance = 0x0;
      Int_t TAIRalignC::fgkPreciseIt = 5;
//__________________________________________________________
TAIRalignC* TAIRalignC::Instance(const TString name, Bool_t flagVtx, Bool_t flagIt, Bool_t flagMsd, Int_t weight)
{
   if (fgInstance == 0x0)
      fgInstance = new TAIRalignC(name, flagVtx, flagIt, flagMsd, weight);
   
   return fgInstance;
}

//------------------------------------------+-----------------------------------
//! Default constructor.
TAIRalignC::TAIRalignC(const TString name, Bool_t flagVtx, Bool_t flagIt, Bool_t flagMsd, Int_t weight)
 : TObject(),
   fFileName(name),
   fFlagVtx(flagVtx),
   fFlagIt(flagIt),
   fFlagMsd(flagMsd),
   fEbeamInit(0),
   fpcInit(0),
   fBetaInit(0),
   fAbeam(0),
   fZbeam(0),
   fEbeam(0),
   fpc(0),
   fBeta(0),
   fWeighted(weight),
   fPreciseIt(1),
   fCut1(20),
   fCut2(4),
   fCutFactor(0),
   fHitPlanes(-1),
   fEvents1(0),
   fSumWeightQ(0),
   fFixPlaneRef1(false),
   fFixPlaneRef2(false),
   fPlaneRef1(-1),
   fPlaneRef2(-1),
   fResidualX(new TGraphErrors()),
   fResidualY(new TGraphErrors())
{
   
   const TString confFileVtx = "./config/TAVTdetector.cfg";
   const TString confFileMsd = "./config/TAMSDdetector.cfg";
   
   Int_t devsNtot = 0;
   
   fAGRoot        = new TAGroot();
   fClusterArray  = new TObjArray();
   fClusterArray->SetOwner(false);
   
   fInfile        = new TAGactTreeReader("inFile");
   
   fpGeoMapG    = new TAGparaDsc("gGeo", new TAGparGeo());
   TAGparGeo* geomapG   = (TAGparGeo*) fpGeoMapG->Object();
   geomapG->InitGeo();

   fpDiff         = new TAIRparDiff(fpGeoMapG);

   // VTX
   if (fFlagVtx) {
      fpGeoMapVtx    = new TAGparaDsc(TAVTparGeo::GetDefParaName(), new TAVTparGeo());
      TAVTparGeo* geomapVtx   = (TAVTparGeo*) fpGeoMapVtx->Object();
      geomapVtx->InitGeo();
      
      fpConfigVtx    = new TAGparaDsc("vtConf", new TAVTparConf());
      TAVTparConf* parConfVtx = (TAVTparConf*) fpConfigVtx->Object();
      parConfVtx->FromFile(confFileVtx.Data());
      
      devsNtot += parConfVtx->GetSensorsN();

      fpNtuClusVtx   = new TAGdataDsc("vtClus", new TAVTntuCluster());
      fInfile->SetupBranch(fpNtuClusVtx, TAVTntuCluster::GetBranchName());
   }
   
   // MSD
   if (fFlagMsd) {
      fpGeoMapMsd    = new TAGparaDsc(TAMSDparGeo::GetDefParaName(), new TAMSDparGeo());
      TAMSDparGeo* geomapMsd   = (TAMSDparGeo*) fpGeoMapMsd->Object();
      geomapMsd->InitGeo();
   
      fpConfigMsd    = new TAGparaDsc("msdConf", new TAMSDparConf());
      TAMSDparConf* parConfMsd = (TAMSDparConf*) fpConfigMsd->Object();
      parConfMsd->FromFile(confFileMsd.Data());
      
      devsNtot += parConfMsd->GetSensorsN();
      
      fpNtuClusMsd  = new TAGdataDsc("msdClus", new TAMSDntuCluster());
      fInfile->SetupBranch(fpNtuClusMsd, TAMSDntuCluster::GetBranchName());
   }

   if (devsNtot <= 2) {
      Error("TAIRalignC", "No enough sensors to align");
      exit(0);
   }

   fDevStatus     = new Int_t[devsNtot];

   FillStatus();
   
   InitParameters();
   
   FillPosition();
   
   fAlign = new TAIRntuAlignC(&fSecArray, fDevStatus);
}

//------------------------------------------+-----------------------------------
//! Destructor.
TAIRalignC::~TAIRalignC()
{
   for(Int_t i = 0; i < fSecArray.GetSize(); i++){
      delete[] fWeightQ;
      delete[] fZposition;
      delete[] fSigmaAlfaDist;
      delete[] fSigmaMeasQfinal;
      delete[] fPosUClusters;
      delete[] fPosVClusters;
      delete[] fErrUClusters;
      delete[] fErrVClusters;
      delete[] fTiltW;
      delete[] fAlignmentU;
      delete[] fAlignmentV;
      delete[] fStatus;
      delete[] fDevStatus;
   }
   delete fResidualX;
   delete fResidualY;
   delete fClusterArray;
}

//______________________________________________________________________________
//
// Fill array of clusters
void TAIRalignC::FillClusterArray()
{
   fClusterArray->Clear();
   
   if (fFlagVtx) {
      TAVTntuCluster* pNtuClus  = (TAVTntuCluster*) fpNtuClusVtx->Object();
      TAVTparConf* pConfigVtx = (TAVTparConf*) fpConfigVtx->Object();
      
      for (Int_t iPlane = 0; iPlane < pConfigVtx->GetSensorsN(); ++iPlane) {
         if (pConfigVtx->GetStatus(iPlane) == -1) continue;
         TClonesArray* list = pNtuClus->GetListOfClusters(iPlane);
         fClusterArray->AddLast(list);
      }
   }
   
   if (fFlagMsd) {
      TAMSDntuCluster* pNtuClus  = (TAMSDntuCluster*) fpNtuClusMsd->Object();
      TAMSDparConf* pConfigMsd = (TAMSDparConf*) fpConfigMsd->Object();
      
      for (Int_t iPlane = 0; iPlane < pConfigMsd->GetSensorsN(); ++iPlane) {
         if (pConfigMsd->GetStatus(iPlane) == -1) continue;
         TClonesArray* list = pNtuClus->GetListOfClusters(iPlane);
         fClusterArray->AddLast(list);
      }
   }

}

//______________________________________________________________________________
//
// Create all the residuals histograms
void TAIRalignC::InitParameters()
{
   fWeightQ         = new Double_t[fSecArray.GetSize()];
   fZposition       = new Double_t[fSecArray.GetSize()];
   fSigmaAlfaDist   = new Double_t[fSecArray.GetSize()];
   fSigmaMeasQfinal = new Double_t[fSecArray.GetSize()];
   fPosUClusters    = new Double_t[fSecArray.GetSize()];
   fPosVClusters    = new Double_t[fSecArray.GetSize()];
   fErrUClusters    = new Double_t[fSecArray.GetSize()];
   fErrVClusters    = new Double_t[fSecArray.GetSize()];
   fTiltW           = new Float_t[fSecArray.GetSize()];
   fAlignmentU      = new Float_t[fSecArray.GetSize()];
   fAlignmentV      = new Float_t[fSecArray.GetSize()];
   fStatus          = new Bool_t[fSecArray.GetSize()];
   
   for(Int_t i = 0; i < fSecArray.GetSize(); i++){
      fWeightQ[i]         = 0;
      fZposition[i]       = 0;
      fSigmaAlfaDist[i]   = 0;
      fSigmaMeasQfinal[i] = 0;
      fPosUClusters[i]    = 0;
      fPosVClusters[i]    = 0;
      fErrUClusters[i]    = 0;
      fErrVClusters[i]    = 0;
      fTiltW[i]           = 0;
      fAlignmentU[i]      = 0;
      fAlignmentV[i]      = 0;
      fStatus[i]          = false;
   }
}

//______________________________________________________________________________
//
// Create all the residuals histograms
void TAIRalignC::FillStatus()
{
   TAVTbaseParConf* parConf = 0x0;
   fOffsetMsd = 0;

   if (fFlagVtx) {
      parConf = (TAVTparConf*) fpConfigVtx->Object();
      FillStatus(parConf);
      fOffsetMsd += fSecArray.GetSize();
   }
   
   if (fFlagMsd) {
      parConf = (TAMSDparConf*) fpConfigMsd->Object();
      FillStatus(parConf, fOffsetMsd);
   }
}

//______________________________________________________________________________
//
// Fill Status
void TAIRalignC::FillStatus(TAVTbaseParConf* parConf, Int_t offset)
{
   for (Int_t i = 0; i < parConf->GetSensorsN(); i++) {
      if (parConf->GetStatus(i) != -1) {
         fSecArray.Set(fSecArray.GetSize()+1);
         fSecArray.AddAt(i, fSecArray.GetSize()-1);
      }
      if (parConf->GetStatus(i) == 0) {
         fFixPlaneRef1 = true;
         fPlaneRef1 = fSecArray.GetSize()-1;
      }
      
      if (parConf->GetStatus(i) == 1) {
         fFixPlaneRef2 = true;
         fPlaneRef2 = fSecArray.GetSize()-1;
      }
      
      Int_t iSensor = fSecArray.GetSize()-1;
      fDevStatus[fSecArray.GetSize()-1] = parConf->GetStatus(iSensor);
   }
}

//______________________________________________________________________________
//
// Fill Position
void TAIRalignC::FillPosition()
{
   TAVTbaseParGeo*  parGeo = 0x0;
   Int_t offset = 0;
   if (fFlagVtx) {
      parGeo  = (TAVTparGeo*)  fpGeoMapVtx->Object();
      FillPosition(parGeo);
   }
   
   if (fFlagMsd) {
      parGeo  = (TAMSDparGeo*)  fpGeoMapMsd->Object();
      FillPosition(parGeo, fOffsetMsd);
   }
}

//______________________________________________________________________________
//
// Fill Position
void TAIRalignC::FillPosition(TAVTbaseParGeo* parGeo, Int_t offset)
{
   for (Int_t i = 0; i < parGeo->GetNSensors(); i++){
      TVector3 posSens = parGeo->GetSensorPosition(i);
      parGeo->Local2Global(&posSens);
      fZposition[i+offset] = posSens.Z()*TAGgeoTrafo::CmToMm();
   }
}


//______________________________________________________________________________
//
// Create all the residuals histograms
void TAIRalignC::CreateHistogram()
{   
   Int_t iPlane = 0;
   for (Int_t i = 0; i < fSecArray.GetSize(); i++) {
      iPlane = fSecArray[i];
      
      fpResXC[i] = new TH1F(Form("ResX%dC", i+1), Form("ResidualX of sensor %d", iPlane+1), 400, -200, 200);
      fpResYC[i] = new TH1F(Form("ResY%dC", i+1), Form("ResidualY of sensor %d", iPlane+1), 400, -200, 200);
   }
   
   fpResTotXC = new TH1F("TotResXC", "Total residualX", 400, -200, 200);
   fpResTotYC = new TH1F("TotResYC", "Total residualY", 400, -200, 200);
   
   return;
}
//_____________________________________________________________________________
//
void TAIRalignC::LoopEvent(Int_t nEvts)
{
   if ((fFixPlaneRef1 == false)||(fFixPlaneRef2 == false)) {
      Error("LoopEvent()", "In config:: Wrong references planes in the configuration file: 1 plane must be set to 0 (reference 0),  1 plane must be set to 1 (reference 1) and the others to 2");
      return;
   }
   
   fAGRoot->AddRequiredItem(fInfile);
   if (fInfile->Open(fFileName.Data())) return;
   
   CreateHistogram();
   
   fAGRoot->BeginEventLoop();
   fAGRoot->Print();
   
   if (nEvts > fInfile->NEvents() || nEvts == 1) nEvts = fInfile->NEvents();
   printf("Number of total entries: %d \n", fInfile->NEvents());
   printf("Number of events: %d \n", nEvts);
   
   if(!DefineWeights()) return;
   
   fAlign->DefineConstant(fWeightQ, fZposition);
   fEvents1 = 0;
   fInfile->Reset();
   
   Float_t limitShift = 5;
   Float_t limitTilt  = 0.1*TMath::DegToRad();
   Bool_t roughAlign = true;
   
   for (Int_t i = 0; i < nEvts; ++i ) {
      if(i % 10000 == 0) {
         std::cout << "Loaded Event:: " << i << std::endl;
      }
      if (!fAGRoot->NextEvent()) break;
      
      FillClusterArray();
      if(!Align(roughAlign)) continue;
   }
   
   fAlign->Constraint(fPlaneRef1, fPlaneRef2);
   fAlign->Minimize();
   UpdateAlignmentParams();
   fAlign->Reset();
   printf("Number of events for iteration 1: %d / %d \n", fEvents1, nEvts);
   
   roughAlign = false;
   fCutFactor = fCut1;
   for (Int_t j = 0; j < fPreciseIt; j++){
      fInfile->Reset();
      fEvents1 = 0;
      for (Int_t i = 0; i < nEvts; ++i ) {
         if(i % 10000 == 0) {
            std::cout << "Loaded Event:: " << i << std::endl;
         }
         if (!fAGRoot->NextEvent()) break;
         FillClusterArray();
         if(!Align(roughAlign)) continue;
      }
      
      fAlign->Constraint(fPlaneRef1, fPlaneRef2);
      fAlign->Minimize();
      UpdateAlignmentParams();
      fAlign->Reset();
      printf("Number of events for iteration %d: %d / %d \n", j+2, fEvents1, nEvts);
      fCutFactor = fCut2;
      Bool_t status = true;
      for(Int_t i = 0; i < fSecArray.GetSize(); i++)
         status &= ((fAlign->GetOffsetU()[i]*TAGgeoTrafo::MmToMu() < limitShift) && (fAlign->GetOffsetV()[i]*TAGgeoTrafo::MmToMu() < limitShift)) && (fAlign->GetTiltW()[i]<limitTilt);
      if (!status)
         fPreciseIt++;
      if (fPreciseIt >= fgkPreciseIt)
         break;
   }
   
//   for (Int_t i = 0; i < fSecArray.GetSize(); i++ ) {
//      UpdateTransfo(i);
//   }
   
  // UpdateGeoMaps();
   
  
   
   // Now display the filled histos
   printf("------------------------------------------------------------\n");
   printf("--------------------- Alignment RESULT --------------------\n");
   printf("------------------------------------------------------------\n");
   
   for(Int_t i = 0; i < fSecArray.GetSize(); i++) {
      Int_t iPlane = fSecArray[i];
      fStatus[i] = ((fAlign->GetOffsetU()[i]*TAGgeoTrafo::MmToMu() < limitShift) && (fAlign->GetOffsetV()[i]*TAGgeoTrafo::MmToMu() < limitShift)) && (fAlign->GetTiltW()[i]<limitTilt);
      printf("----------------------------------------------\n");
      printf("Sensor: %d AlignmentU: %5.1f AlignmentV: %5.1f TiltW: %6.3f status %d\n", iPlane+1,
             fAlignmentU[i]*TAGgeoTrafo::MmToMu(),
             fAlignmentV[i]*TAGgeoTrafo::MmToMu(),
             fTiltW[i]*TMath::RadToDeg(),
             fStatus[i]);
   }
   
   printf("\n----------------------------------------------\n");
   printf("Restart to produce residual plots with %d evts\n", nEvts);
   
   fInfile->Reset();
   for (Int_t i = 0; i < nEvts; ++i ) {
      if(i % 10000 == 0)
         std::cout << " Loaded Event:: " << i << std::endl;
      if (!fAGRoot->NextEvent()) break;
      FillClusterArray();
      if(!FillHistograms()) continue;
   }
   
   TCanvas* canvas1 = new TCanvas("canvas1", "ResidualX");
   canvas1->Divide(fSecArray.GetSize()/2,3);
   gStyle->SetOptStat(1111);
   
   TCanvas* canvas2 = new TCanvas("canvas2", "ResidualY");
   canvas2->Divide(fSecArray.GetSize()/2,3);
   gStyle->SetOptStat(1111);
   
   TCanvas* canvas3 = new TCanvas("canvas3", "ResidualTotX");
   gStyle->SetOptStat(1111);
   
   TCanvas* canvas4 = new TCanvas("canvas4", "ResidualTotY");
   gStyle->SetOptStat(1111);
   
   
   for(Int_t i = 0; i < fSecArray.GetSize(); i++) {
      canvas1->cd(i+1);
      fpResXC[i]->Draw();
      
      canvas2->cd(i+1);
      fpResYC[i]->Draw();
   }
   
   canvas3->cd();
   fpResTotXC->Draw();
   
   canvas4->cd();
   fpResTotYC->Draw();
   
}

//______________________________________________________________________________
//
// Alignment with all the events which fired all the planes
Bool_t TAIRalignC::Align(Bool_t rough)
{
   TAVTntuCluster* pNtuClus  = (TAVTntuCluster*) fpNtuClusVtx->Object();

   fSlopeU = 0;
   fSlopeV = 0;
   fNewSlopeU = 0;
   fNewSlopeV = 0;
   
   Int_t iPlane = 0;
   Int_t nCluster = 0;
   fHitPlanes = 0;
   
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      fPosUClusters[i] = 999999;
      fPosVClusters[i] = 999999;
      Int_t nValidCluster = 0;
      Int_t aCluster = 0;
      TClonesArray* list = (TClonesArray*)fClusterArray->At(i);
      nCluster = list->GetEntries();

      if (nCluster < 1) return false;
      for (Int_t j = 0; j < nCluster; j++){
         TAVTbaseCluster* cluster = (TAVTbaseCluster*)list->At(j);
         if (cluster->IsValid() != true) continue;
         nValidCluster++;
         if (nValidCluster > 1) return false;
         aCluster = cluster->GetNumber();
      }
      if (nValidCluster < 1) return false;
      fHitPlanes ++;
      TAVTbaseCluster* cluster = (TAVTbaseCluster*)list->At(aCluster);
      if (rough) {
         FillClusPosRough(i, cluster);
      } else {
         if (!FillClusPosPrecise(i, cluster)) return false;
      }
   }
   
   if (fHitPlanes < fSecArray.GetSize()) return false;
   if (!(fAlign->Accumulate(fPosUClusters, fPosVClusters))) return false;
   fEvents1++;
   fAlign->Sum();
   
   return true;
}

//______________________________________________________________________________
//
// Fill rough position of cluster
Bool_t TAIRalignC::FillClusPosRough(Int_t i, TAVTbaseCluster* cluster)
{
   fPosUClusters[i] = cluster->GetPositionG()[0]*TAGgeoTrafo::CmToMm();
   fPosVClusters[i] = cluster->GetPositionG()[1]*TAGgeoTrafo::CmToMm();
   
   return true;
}

//______________________________________________________________________________
//
// Fill precise position of cluster, rejecting the ones which are too much scattered for
// a precise alignment
Bool_t TAIRalignC::FillClusPosPrecise(Int_t i, TAVTbaseCluster* cluster)
{
   TAVTbaseParGeo* pGeoMap   =  0x0;
   
   if(cluster->InheritsFrom("TAVTcluster"))
      pGeoMap = (TAVTparGeo*) fpGeoMapVtx->Object();
   else if (cluster->InheritsFrom("TAMSDcluster"))
      pGeoMap = (TAMSDparGeo*) fpGeoMapMsd->Object();
   else {
      Error("FillClusPosPrecise()","Unknown detector");
      exit(0);
   }

   fPosUClusters[i] = cluster->GetPositionG()[0]*TAGgeoTrafo::CmToMm() + (cluster->GetPositionG()[1]*TAGgeoTrafo::CmToMm() * (-fTiltW[i])) - fAlignmentU[i];
   fPosVClusters[i] = cluster->GetPositionG()[1]*TAGgeoTrafo::CmToMm() - (cluster->GetPositionG()[0]*TAGgeoTrafo::CmToMm() * (-fTiltW[i])) - fAlignmentV[i];
   
   if (i != 0){
      fNewSlopeU = (fPosUClusters[i]-fPosUClusters[i-1])/(fZposition[i]-fZposition[i-1]);
      fNewSlopeV = (fPosVClusters[i]-fPosVClusters[i-1])/(fZposition[i]-fZposition[i-1]);
   }
   
   if ((i != 0) && (i !=1)){
      if (fCutFactor*fSigmaAlfaDist[i]*((fZposition[i]-fZposition[i-1])/TMath::Sqrt(12)*TAGgeoTrafo::MmToMu()) < pGeoMap->GetPitchX()/TMath::Sqrt(12)*TAGgeoTrafo::CmToMu()){
         if ((TMath::Abs(fNewSlopeU - fSlopeU) > pGeoMap->GetPitchX()/TMath::Sqrt(12)*fCutFactor*TAGgeoTrafo::CmToMu()) ||
             (TMath::Abs(fNewSlopeV - fSlopeV) > pGeoMap->GetPitchX()/TMath::Sqrt(12)*fCutFactor*TAGgeoTrafo::CmToMu())) return false;
      } else {
         if ((TMath::Abs(fNewSlopeU - fSlopeU) > fSigmaAlfaDist[i]*fCutFactor) || (TMath::Abs(fNewSlopeV - fSlopeV) > fSigmaAlfaDist[i]*fCutFactor)) return false;
      }
   }
   
   fSlopeU = fNewSlopeU;
   fSlopeV = fNewSlopeV;
   
   return true;
}


//______________________________________________________________________________
//
// Filling the histograms with all the events when all the planes are fired
Bool_t TAIRalignC::FillHistograms()
{
   Int_t iPlane = 0;
   Int_t nCluster = 0;
   Double_t intersectionU = 0;
   Double_t intersectionV = 0;
   fHitPlanes = 0;
   Int_t aCluster[50] = {0};
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      fPosUClusters[i] = 999999;
      fPosVClusters[i] = 999999;
      fErrUClusters[i] = 0;
      fErrVClusters[i] = 0;
      Int_t nValidCluster = 0;
      
      iPlane = fSecArray[i];
      TClonesArray* list = (TClonesArray*)fClusterArray->At(iPlane);
      nCluster = list->GetEntries();
      
      if (nCluster < 1) return false;
      for (Int_t j = 0; j < nCluster; j++){
         TAVTbaseCluster* cluster = (TAVTbaseCluster*)list->At(j);
         if (cluster->IsValid() != true) continue;
         nValidCluster++;
         if (nValidCluster > 1) return false;
         aCluster[i] = cluster->GetNumber();
      }
      if (nValidCluster < 1) return false;
      fHitPlanes ++;
      TAVTbaseCluster* cluster = (TAVTbaseCluster*)list->At(aCluster[i]);
      
      fPosUClusters[i] = cluster->GetPositionG()[0]*TAGgeoTrafo::CmToMm() + (cluster->GetPositionG()[1]*TAGgeoTrafo::CmToMm() * (-fTiltW[i])) - fAlignmentU[i];
      fPosVClusters[i] = cluster->GetPositionG()[1]*TAGgeoTrafo::CmToMm() - (cluster->GetPositionG()[0]*TAGgeoTrafo::CmToMm() * (-fTiltW[i])) - fAlignmentV[i];
      TVector3 posG (fPosUClusters[i]*TAGgeoTrafo::MmToCm(), fPosVClusters[i]*TAGgeoTrafo::MmToCm(), fZposition[i]*TAGgeoTrafo::MmToCm());
      TVector3 pos (fPosUClusters[i]*TAGgeoTrafo::MmToCm(), fPosVClusters[i]*TAGgeoTrafo::MmToCm(), 0.);
      cluster->SetPositionG(&posG);
      cluster->SetPosition(&pos);
      fErrUClusters[i] = cluster->GetPosError()(0)*TAGgeoTrafo::CmToMm();
      fErrVClusters[i] = cluster->GetPosError()(1)*TAGgeoTrafo::CmToMm();
      cluster->ComputeSize();
   }
   
   if (fHitPlanes < fSecArray.GetSize()) return false;
   
   fResidualX->Set(fHitPlanes);
   fResidualY->Set(fHitPlanes);
   
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      if (fPosUClusters[i] == 999999 || fPosVClusters[i] == 999999) return false;
      fResidualX->SetPoint(i, fZposition[i]*TAGgeoTrafo::MmToMu(), fPosUClusters[i]*TAGgeoTrafo::MmToMu());
      fResidualX->SetPointError(i, 1, fErrUClusters[i]*TAGgeoTrafo::MmToMu()*(1/TMath::Sqrt(fWeightQ[i])));
      fResidualY->SetPoint(i, fZposition[i]*TAGgeoTrafo::MmToMu(), fPosVClusters[i]*TAGgeoTrafo::MmToMu());
      fResidualY->SetPointError(i, 1, fErrVClusters[i]*TAGgeoTrafo::MmToMu()*(1/TMath::Sqrt(fWeightQ[i])));
   }
   
   fResidualX->Fit("pol1", "Q");
   TF1* polyU = fResidualX->GetFunction("pol1");
   fResidualY->Fit("pol1", "Q");
   TF1* polyV = fResidualY->GetFunction("pol1");
   
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      if (i == 0) continue;
      if (fPosUClusters[i] == 999999 || fPosVClusters[i] == 999999) return false;
      intersectionU = polyU->GetParameter(0) + polyU->GetParameter(1)*fZposition[i]*TAGgeoTrafo::MmToMu();
      intersectionV = polyV->GetParameter(0) + polyV->GetParameter(1)*fZposition[i]*TAGgeoTrafo::MmToMu();
      if ((TMath::Abs(intersectionU - fPosUClusters[i]*TAGgeoTrafo::MmToMu()) > TMath::Sqrt(fSigmaMeasQfinal[i]*1e6)*2) ||
          (TMath::Abs(intersectionV - fPosVClusters[i]*TAGgeoTrafo::MmToMu()) > TMath::Sqrt(fSigmaMeasQfinal[i]*1e6)*2))return false;
   }
   
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      intersectionU = polyU->GetParameter(0) + polyU->GetParameter(1)*fZposition[i]*TAGgeoTrafo::MmToMu();
      intersectionV = polyV->GetParameter(0) + polyV->GetParameter(1)*fZposition[i]*TAGgeoTrafo::MmToMu();
      fpResXC[i]->Fill(intersectionU - fPosUClusters[i]*TAGgeoTrafo::MmToMu());
      fpResYC[i]->Fill(intersectionV - fPosVClusters[i]*TAGgeoTrafo::MmToMu());
      fpResTotXC->Fill(intersectionU - fPosUClusters[i]*TAGgeoTrafo::MmToMu());
      fpResTotYC->Fill(intersectionV - fPosVClusters[i]*TAGgeoTrafo::MmToMu());
   }
   
   return true;
}

//______________________________________________________________________________
//
// The final vector containing the alignment parameters are calculated
void TAIRalignC::UpdateAlignmentParams()
{
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      Int_t iSensor = fSecArray[i];
      
      printf("Sensor: %d OffsetU: %5.1f OffsetV: %5.1f TiltW: %6.3f \n", iSensor+1,
             fAlign->GetOffsetU()[i]*TAGgeoTrafo::MmToMu(),
             fAlign->GetOffsetV()[i]*TAGgeoTrafo::MmToMu(),
             fAlign->GetTiltW()[i]*TMath::RadToDeg());
      
      fAlignmentU[i] += fAlign->GetOffsetU()[i];
      fAlignmentV[i] += fAlign->GetOffsetV()[i];
      fTiltW[i]      += fAlign->GetTiltW()[i];
   }
   return;
}

//______________________________________________________________________________
//
// Definition of the weights of the measurements function of the scattering
// due to the silicon and the air. By default, all the weights are equals.
//In both cases the weight = 1/sigma and are normalized
// such Sum(weightsQ) = 1.
// These calculation are made thanks to the formulas implemented in Scattman
Bool_t TAIRalignC::DefineWeights()
{
   // tmp solution meanwhile to find something better
   if (fWeighted == -1) {
      for (Int_t i = 0; i < fSecArray.GetSize(); i++){
         fWeightQ[i]         = 1./fSecArray.GetSize();
         fSigmaMeasQfinal[i] = 2.5e-5;
         fSigmaAlfaDist[i]   = 5.0e-5;
      }
      return true;
   }
   
   TAGparGeo* pGeoMap = (TAGparGeo*) fpGeoMapG->Object();
   TAVTparGeo* vtGeoMap = (TAVTparGeo*) fpGeoMapVtx->Object();
   
   fEbeamInit   = pGeoMap->GetBeamPar().Energy*TAGgeoTrafo::GevToMev();
   fZbeam       = pGeoMap->GetBeamPar().AtomicNumber;
   fAbeam       = pGeoMap->GetBeamPar().AtomicMass;
   
   fpcInit      = fpDiff->PCCalc(fEbeamInit, fAbeam);
   fBetaInit    = fpDiff->BetaCalc(fEbeamInit);
   
   fEbeam  = fEbeamInit;
   fpc     = fpcInit;
   fBeta   = fBetaInit;
   
   Double_t sigmaMeas   =  5.0;
   Float_t  thickSi     =  vtGeoMap->GetTotalSize()[2] * TAGgeoTrafo::MuToMm();
   
   Double_t LrSi   = fpDiff->GetRadLength("Si");   // [g/cm^2] Radiation length for silicon
   Double_t rhoSi  = fpDiff->GetDensity("Si");     // [g/cm^3] density silicon
   Double_t LrAir  = fpDiff->GetRadLength("Air");  // [g/cm^2] Radiation length for air
   Double_t rhoAir = fpDiff->GetDensity("Air");    // [g/cm^3] density air
   
   Float_t wepl = 0;
   Float_t weplSi = fpDiff->WEPLCalc("Si", thickSi);
   
   Double_t km = 0; Double_t factor1 = 0; Double_t factor2 = 0;
   Double_t sigmaAlfaMeasQ = 0;
   Double_t sigmaMeasQ = sigmaMeas * sigmaMeas * 1e-6;
   
   Int_t iSensor = 0;
   
   Float_t LrSum = 0;
   Float_t rhodSum = 0;
   Float_t logtermSum = 0;
   Float_t nonLogtermSumQ = 0;
   Float_t previousTermSumQ = 0;
   Float_t previousDistanceSum = 0;
   
   TVector3 posSens = vtGeoMap->GetSensorPosition(iSensor);
   vtGeoMap->Local2Global(&posSens);
   
   wepl = fpDiff->WEPLCalc("Air", TMath::Abs(pGeoMap->GetBeamPar().Position[2]-posSens[2])*TAGgeoTrafo::CmToMm());

   fEbeam   = fpDiff->EnergyCalc(fEbeam, fAbeam, fZbeam, wepl);
   fpc      = fpDiff->PCCalc(fEbeam, fAbeam);
   fBeta    = fpDiff->BetaCalc(fEbeam);
   if(fEbeam == 0){
      Error("DefineWeights()","Remaining energy in air is 0...");
      return false;
   }
   
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      iSensor = fSecArray[i];
      Double_t sigmaAlfaScattSi = 0;
      Double_t sigmaAlfaScattAir = 0;
      
      if (i > 0){
         Float_t distance = TMath::Abs(fZposition[i] - fZposition[i-1] - thickSi);
         sigmaAlfaScattSi = 14100.0/( fBeta * fpc ) * fZbeam * TMath::Sqrt(0.1*thickSi*rhoSi/LrSi) * (1+log10(0.1*thickSi*rhoSi/LrSi)/9.0);
         LrSum           += LrSi*0.1*thickSi*rhoSi;
         rhodSum         +=  0.1*thickSi*rhoSi;
         logtermSum       = 1+log10 ( rhodSum / (LrSum/rhodSum) )/9.0;
         nonLogtermSumQ  += (sigmaAlfaScattSi / (1+log10 ( 0.1*thickSi*rhoSi / LrSi )/9.0))*(sigmaAlfaScattSi / (1+log10 ( 0.1*thickSi*rhoSi / LrSi )/9.0));
         sigmaAlfaScattSi = TMath::Sqrt(logtermSum * logtermSum * nonLogtermSumQ - previousTermSumQ);
         factor1          = sigmaAlfaMeasQ + sigmaAlfaScattSi * sigmaAlfaScattSi;
         factor2          = sigmaAlfaMeasQ / factor1;
         sigmaMeasQ       = sigmaMeasQ + 1e-6 * km * km * factor2 * sigmaAlfaScattSi * sigmaAlfaScattSi;
         sigmaAlfaMeasQ   = factor1;
         km               = km * factor2 + thickSi; // see scattman formalismus paper
         previousTermSumQ = previousTermSumQ + sigmaAlfaScattSi*sigmaAlfaScattSi;
         fEbeam           = fpDiff->EnergyCalc(fEbeam, fAbeam, fZbeam, weplSi*0.1);
         fpc              = fpDiff->PCCalc(fEbeam, fAbeam);
         fBeta            = fpDiff->BetaCalc(fEbeam);
         
         if(fEbeam == 0){
            Error("DefineWeights()","Remaining energy is 0...");
            return false;
         }
         
         Int_t nLayers = 2;
         for (Int_t iLayer = 0; iLayer < nLayers; iLayer++){
            sigmaAlfaScattAir  =  14100.0/( fBeta * fpc ) * fZbeam * TMath::Sqrt( 0.1*distance/nLayers*rhoAir / LrAir ) *  (1+log10 ( 0.1*distance/nLayers*rhoAir / LrAir )/9.0);
            LrSum             += LrAir*0.1*distance/nLayers*rhoAir;
            rhodSum           +=  0.1*distance/nLayers*rhoAir;
            logtermSum         = 1+log10 ( rhodSum / (LrSum/rhodSum) )/9.0;
            nonLogtermSumQ    +=  (sigmaAlfaScattAir / (1+log10 ( 0.1*distance/nLayers*rhoAir / LrAir )/9.0))*(sigmaAlfaScattAir / (1+log10 ( 0.1*distance/nLayers*rhoAir / LrAir )/9.0));
            sigmaAlfaScattAir  = TMath::Sqrt(logtermSum * logtermSum * nonLogtermSumQ - previousTermSumQ);
            factor1            = sigmaAlfaMeasQ + sigmaAlfaScattAir * sigmaAlfaScattAir ;
            factor2            = sigmaAlfaMeasQ / factor1;
            sigmaMeasQ         = sigmaMeasQ + 1e-6 * km * km * factor2 * sigmaAlfaScattAir * sigmaAlfaScattAir;
            sigmaAlfaMeasQ     = factor1;
            km                 = km * factor2 + distance/nLayers ; // see scattman formalismus paper
            previousTermSumQ   = previousTermSumQ + sigmaAlfaScattAir*sigmaAlfaScattAir;
            wepl               = fpDiff->WEPLCalc("Air", distance/nLayers);
            fEbeam             = fpDiff->EnergyCalc(fEbeam, fAbeam, fZbeam, wepl*0.1);
            fpc                = fpDiff->PCCalc(fEbeam, fAbeam);
            fBeta              = fpDiff->BetaCalc(fEbeam);
            
            if(fEbeam == 0){
               Error("DefineWeights()","Remaining energy is 0...");
               return false;
            }
         }
      }
      
      fSigmaMeasQfinal[i] = sigmaMeasQ + 1e-6 * km * km * sigmaAlfaMeasQ;
      if (i == 0) fSigmaAlfaDist[i] = TMath::Sqrt(previousTermSumQ*1e-6);
      if (i > 0) fSigmaAlfaDist[i] = TMath::Sqrt(previousTermSumQ*1e-6 - previousDistanceSum);
      previousDistanceSum += fSigmaAlfaDist[i] * fSigmaAlfaDist[i];
      if (i == 0 && fSigmaMeasQfinal[i] == 0){
         Error("DefineWeights()","Can't divide by 0.....");
         return false;
      }
      
      fWeightQ[i] = 1/(fSigmaMeasQfinal[i]);
      if (fWeighted != 1) fWeightQ[i] = 1;
      fSumWeightQ += fWeightQ[i];
   }
   
   for (Int_t i = 0; i < fSecArray.GetSize(); i++){
      fWeightQ[i] = fWeightQ[i] / fSumWeightQ;
   }
   
   return true;
}
//______________________________________________________________________________
//
// Update transfo for every loop when it changes the alignment parameters
void TAIRalignC::UpdateTransfo(Int_t idx)
{
   TAVTparGeo* pGeoMap  = (TAVTparGeo*) fpGeoMapVtx->Object();
   Int_t       iPlane   = fSecArray[idx];
   pGeoMap->GetAlignmentU(iPlane) = fAlignmentU[idx]*TAGgeoTrafo::MmToMu();
   pGeoMap->GetAlignmentV(iPlane) = fAlignmentV[idx]*TAGgeoTrafo::MmToMu();
   pGeoMap->GetTiltW(iPlane)      = -fTiltW[idx];
   
   return;
}

//______________________________________________________________________________
//
// Modification of the geomap file with the final results of alignment
void TAIRalignC::UpdateGeoMaps()
{
   TAVTparGeo* pGeoMap  = (TAVTparGeo*)fpGeoMapVtx->Object();
   fstream  configFileOld;
   fstream  configFileNew;
   Char_t configFileName[1000];
   sprintf(configFileName,"%s", pGeoMap->GetFileName().Data());
   configFileOld.open(configFileName, ios::in);
   sprintf(configFileName,"%s_new", pGeoMap->GetFileName().Data());
   configFileNew.open(configFileName, ios::out);
   Char_t tmp[255];
   
   printf("\nCreating new file %s with updated alignment parameters\n", configFileName);
   
   TString key;
   while (!configFileOld.eof()) {
      configFileOld.getline(tmp, 255);
      key = tmp;
      configFileNew << tmp;
      configFileNew << "\n";
      if (key.Contains("Inputs:")) {
         Int_t pos = key.First(':');
         TString sIdx = key(pos+1, key.Length());
         Int_t idx = (Int_t)sIdx.Atoi()-1;
         for (Int_t i = 0; i < fSecArray.GetSize(); ++i) {
            if (idx == fSecArray[i]){
               UpdateGeoMapsUVW(configFileOld, configFileNew, idx);
               break;
            }
         }
      }
   }
   configFileOld.close();
   
   return;
}

//______________________________________________________________________________
//
// Modification of the geomap file with the final results of alignment for each plane
void TAIRalignC::UpdateGeoMapsUVW(fstream &fileIn, fstream &fileOut, Int_t idx)
{
   Char_t tmp[255];
   TString key;
   TAVTparGeo* pGeoMap  = (TAVTparGeo*)fpGeoMapVtx->Object();
   
   Float_t alignU = pGeoMap->GetAlignmentU(idx);
   Float_t alignV = pGeoMap->GetAlignmentV(idx);
   Float_t tiltW  = pGeoMap->GetTiltW(idx);
   
   while (!fileIn.eof()) {
      fileIn.getline(tmp, 255);
      key = tmp;
      if (key.Contains("AlignementU:")) {
         key = "AlignementU:";
         TString sIdx;
         sIdx = Form("   %8.2f\n", alignU);
         TString line = key+sIdx;
         fileOut << line.Data();
         fileIn.getline(tmp, 255);
         key = "AlignementV:";
         sIdx = Form("   %8.2f\n", alignV);
         line = key + sIdx;
         fileOut << line.Data();
         fileIn.getline(tmp, 255);
         key = "AlignementTilt:";
         sIdx = Form("   %6.3f\n", tiltW*TMath::RadToDeg());
         line = key + sIdx;
         fileOut << line.Data();
         break;
      } else {
         fileOut << tmp;
         fileOut << "\n";
      }
   } 
   
   return;
}


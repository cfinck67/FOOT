
#include "TAFOeventDisplayMC.hxx"
#include "TEveGeoNode.h"
#include "TEveManager.h"
#include "TEveBrowser.h"
#include "TEveWindow.h"
#include "TGeoManager.h"

#include "GlobalPar.hxx"
#include "TAGgeoTrafo.hxx"
#include "TASTntuRaw.hxx"
#include "TABMntuRaw.hxx"
#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"
#include "TATWntuRaw.hxx"
#include "TACAntuRaw.hxx"

ClassImp(TAFOeventDisplay)

//__________________________________________________________
TAFOeventDisplay* TAFOeventDisplayMC::Instance(Int_t type, const TString name)
{
   if (fgInstance == 0x0)
      fgInstance = new TAFOeventDisplayMC(type, name);
   
   return fgInstance;
}


//__________________________________________________________
TAFOeventDisplayMC::TAFOeventDisplayMC(Int_t type, const TString expName)
 : TAFOeventDisplay(type, expName),
   fActNtuRawVtx(0x0),
   fActNtuRawIt(0x0),
   fActNtuRawMsd(0x0),
   fActNtuRawTw(0x0),
   fActNtuRawCa(0x0),
   fTree(0x0),
   fActEvtFile(0x0),
   fCaMcDisplay(new TAGpointDisplay("Cal MC hit")),
   fTwMcDisplay(new TAGpointDisplay("ToF MC hit")),
   fMsdMcDisplay(new TAGpointDisplay("MSD MC hit")),
   fItMcDisplay(new TAGpointDisplay("IT MC hit")),
   fVtMcDisplay(new TAGpointDisplay("VTX MC hit")),
   fBmMcDisplay(new TAGpointDisplay("STC MC hit")),
   fStMcDisplay(new TAGpointDisplay("STC MC hit"))
{
   fEvtStruct = new EVENT_STRUCT;
}

//__________________________________________________________
TAFOeventDisplayMC::~TAFOeventDisplayMC()
{
   // default destructor
   if (fActEvtFile) delete fActEvtFile;
   
   delete fCaMcDisplay;
   delete fTwMcDisplay;
   delete fMsdMcDisplay;
   delete fItMcDisplay;
   delete fVtMcDisplay;
   delete fBmMcDisplay;
   delete fStMcDisplay;
}

//__________________________________________________________
Bool_t TAFOeventDisplayMC::GetEntry(Int_t entry)
{
   if (!fTree->GetEntry(entry)) return false;
   
   return true;
}

//__________________________________________________________
void TAFOeventDisplayMC::CreateRawAction()
{
   ReadParFiles();

   fpNtuMcEve = new TAGdataDsc("eveMc", new TAMCntuEve());
   fActNtuMcEve = new TAMCactNtuEve("eveActNtuMc", fpNtuMcEve, fEvtStruct);

   
   if (GlobalPar::GetPar()->IncludeST()) {
      fpNtuRawSt = new TAGdataDsc("stRaw", new TASTntuRaw());
      fActNtuRawSt = new TASTactNtuMC("stActNtu", fpNtuRawSt, fEvtStruct);
      fActNtuRawSt->CreateHistogram();
      
      fpNtuMcSt   = new TAGdataDsc("stMc", new TAMCntuHit());
      fActNtuMcSt = new TAMCactNtuStc("stActNtuMc", fpNtuMcSt, fEvtStruct);
   }

   if (GlobalPar::GetPar()->IncludeBM()) {
      fpNtuRawBm = new TAGdataDsc("bmRaw", new TABMntuRaw());
      fActNtuRawBm = new TABMactNtuMC("bmActNtu", fpNtuRawBm, fpParConfBm, fpParGeoBm, fEvtStruct);
      fActNtuRawBm->CreateHistogram();
      
      fpNtuMcBm   = new TAGdataDsc("bmMc", new TAMCntuHit());
      fActNtuMcBm = new TAMCactNtuBm("bmActNtuMc", fpNtuMcBm, fEvtStruct);
   }

   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpNtuRawVtx = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fActNtuRawVtx = new TAVTactNtuMC("vtActNtu", fpNtuRawVtx, fpParGeoVtx, fEvtStruct);
      fActNtuRawVtx->CreateHistogram();
      
      fpNtuMcVt   = new TAGdataDsc("vtMc", new TAMCntuHit());
      fActNtuMcVt = new TAMCactNtuVtx("vtActNtuMc", fpNtuMcVt, fEvtStruct);
   }

   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fpNtuRawIt = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActNtuRawIt = new TAITactNtuMC("itActNtu", fpNtuRawIt, fpParGeoIt, fEvtStruct);
      fActNtuRawIt->CreateHistogram();
      
      fpNtuMcIt   = new TAGdataDsc("itMc", new TAMCntuHit());
      fActNtuMcIt = new TAMCactNtuItr("itActNtuMc", fpNtuMcIt, fEvtStruct);
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fpNtuRawMsd = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
      fActNtuRawMsd = new TAMSDactNtuMC("msdActNtu", fpNtuRawMsd, fpParGeoMsd, fEvtStruct);
      fActNtuRawMsd->CreateHistogram();
      
      fpNtuMcMsd   = new TAGdataDsc("msdMc", new TAMCntuHit());
      fActNtuMcMsd = new TAMCactNtuMsd("msdActNtuMc", fpNtuMcMsd, fEvtStruct);
   }
   
   if(GlobalPar::GetPar()->IncludeTW()) {
      fpNtuRawTw   = new TAGdataDsc("twRaw", new TATWntuRaw());
      fActNtuRawTw = new TATWactNtuMC("twActNtu", fpNtuRawTw, fEvtStruct);
      fActNtuRawTw->CreateHistogram();
      
      fpNtuMcTw   = new TAGdataDsc("twMc", new TAMCntuHit());
      fActNtuMcTw = new TAMCactNtuTof("twActNtuMc", fpNtuMcTw, fEvtStruct);
   }
   
   if(GlobalPar::GetPar()->IncludeCA()) {
      fpNtuRawCa   = new TAGdataDsc("caRaw", new TACAntuRaw());
      fActNtuRawCa = new TACAactNtuMC("caActNtu", fpNtuRawCa, fpParGeoCa, fEvtStruct);
      fActNtuRawCa->CreateHistogram();
      
      fpNtuMcCa   = new TAGdataDsc("caMc", new TAMCntuHit());
      fActNtuMcCa = new TAMCactNtuCal("caActNtuMc", fpNtuMcCa, fEvtStruct);
   }
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredItem()
{
   fTAGroot->AddRequiredItem("eveActNtuMc");

   if (GlobalPar::GetPar()->IncludeST())
      AddRequiredMcItemSt();
   
   if (GlobalPar::GetPar()->IncludeBM())
      AddRequiredMcItemBm();

   if (GlobalPar::GetPar()->IncludeVertex())
      AddRequiredMcItemVt();

   if (GlobalPar::GetPar()->IncludeInnerTracker())
      AddRequiredMcItemIt();

   if (GlobalPar::GetPar()->IncludeMSD())
      AddRequiredMcItemMs();
   
   if (GlobalPar::GetPar()->IncludeTW())
      AddRequiredMcItemTw();

   if (GlobalPar::GetPar()->IncludeCA())
      AddRequiredMcItemCa();
   
   TAFOeventDisplay::AddRequiredRecItem();
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemSt()
{
   fTAGroot->AddRequiredItem("stActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemBm()
{
   fTAGroot->AddRequiredItem("bmActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemVt()
{
   fTAGroot->AddRequiredItem("vtActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemIt()
{
   fTAGroot->AddRequiredItem("itActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemMs()
{
   fTAGroot->AddRequiredItem("msdActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemTw()
{
   fTAGroot->AddRequiredItem("twActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::AddRequiredMcItemCa()
{
   fTAGroot->AddRequiredItem("caActNtuMc");
}

//__________________________________________________________
void TAFOeventDisplayMC::OpenFile(const TString fileName)
{
   fActEvtFile = new TFile(fileName.Data());
   fTree = (TTree*)fActEvtFile->Get("EventTree");
   
   Evento *ev  = new Evento();
   ev->FindBranches(fTree, fEvtStruct);
}

//__________________________________________________________
void TAFOeventDisplayMC::AddElements()
{
   TAFOeventDisplay::AddElements();

   if (GlobalPar::GetPar()->IncludeCA()) {
      fCaMcDisplay->ResetPoints();
      gEve->AddElement(fCaMcDisplay);
   }
   
   if (GlobalPar::GetPar()->IncludeTW()) {
      fTwMcDisplay->ResetPoints();
      gEve->AddElement(fTwMcDisplay);
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()){
      fMsdMcDisplay->ResetPoints();
      gEve->AddElement(fMsdMcDisplay);
   }
   
   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fItMcDisplay->ResetPoints();
      gEve->AddElement(fItMcDisplay);
   }
   
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fVtMcDisplay->ResetPoints();
      gEve->AddElement(fVtMcDisplay);
   }
   
   if (GlobalPar::GetPar()->IncludeBM()) {
      fBmMcDisplay->ResetPoints();
      gEve->AddElement(fBmMcDisplay);
   }
   
   if (GlobalPar::GetPar()->IncludeST()) {
      fStMcDisplay->ResetPoints();
      gEve->AddElement(fStMcDisplay);
   }
}

//__________________________________________________________
void TAFOeventDisplayMC::ConnectElements()
{
   TAFOeventDisplay::ConnectElements();
   
   fCaMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateCaInfo(Int_t)");
   fTwMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateTwInfo(Int_t)");
   fMsdMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateMsInfo(Int_t)");
   fItMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateItInfo(Int_t)");
   fVtMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateVtInfo(Int_t)");
   fBmMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateBmInfo(Int_t)");
   fStMcDisplay->Connect("PointSelected(Int_t )", "TAFOeventDisplayMC", this, "UpdateStInfo(Int_t)");
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateStInfo(Int_t idx)
{
   UpdateMcInfo("st", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateBmInfo(Int_t idx)
{
   UpdateMcInfo("bm", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateVtInfo(Int_t idx)
{
   UpdateMcInfo("vt", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateItInfo(Int_t idx)
{
   UpdateMcInfo("it", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateMsInfo(Int_t idx)
{
   UpdateMcInfo("ms", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateTwInfo(Int_t idx)
{
   UpdateMcInfo("tw", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateCaInfo(Int_t idx)
{
   UpdateMcInfo("ca", idx);
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateMcInfo(TString prefix, Int_t idx)
{
   TAMChit* point = 0x0;
   TString name   = "";
   
   if (prefix == "st") {
      point = (TAMChit*)fStMcDisplay->GetPointId(idx);
      name = "STC";
   }

   if (prefix == "bm") {
      point = (TAMChit*)fBmMcDisplay->GetPointId(idx);
      name = "BM";
   }
   
   if (prefix == "vt") {
      point = (TAMChit*)fVtMcDisplay->GetPointId(idx);
      name = "VTX";
   }

   if (prefix == "it") {
      point = (TAMChit*)fItMcDisplay->GetPointId(idx);
      name = "ITR";
   }
   
   if (prefix == "ms") {
      point = (TAMChit*)fMsdMcDisplay->GetPointId(idx);
      name = "MSD";
   }
   
   if (prefix == "tw") {
      point = (TAMChit*)fTwMcDisplay->GetPointId(idx);
      name = "TOF";
   }

   if (prefix == "ca") {
      point = (TAMChit*)fCaMcDisplay->GetPointId(idx);
      name = "CAL";
   }
   
   if(point == 0x0) return;

   TVector3 pos = point->GetInPosition();
   TVector3 mom = point->GetInMomentum();
   
   fInfoView->AddLine( Form("%s sensor id: %d, Hit:\n", name.Data(), point->GetID()) );
   fInfoView->AddLine( Form("at position:   (%.3g %.3g %.3g) cm\n", pos[0], pos[1], pos[2]) );
   fInfoView->AddLine( Form("with momentum: (%.3g %.3g %.3g) GeV/c\n", mom[0], mom[1], mom[2]) );
   fInfoView->AddLine( Form("eLoss: %.3g MeV time: %.3g ns\n", point->GetDeltaE()*TAGgeoTrafo::GevToMev(), point->GetTof()*TAGgeoTrafo::SecToNs()) );
   
   Int_t trackId       = point->GetTrackId();
   TAMCntuEve* pNtuHit = (TAMCntuEve*) fpNtuMcEve->Object();
   TAMCeveTrack* track   = pNtuHit->GetHit(trackId);
   
   fInfoView->AddLine( Form("Generated from track # %d\n", trackId) );
   fInfoView->AddLine( Form("Charge: %d Mass: %.3g GeV/c2\n", track->GetCharge(), track->GetMass()) );
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateElements()
{
   if (GlobalPar::GetPar()->IncludeST())
      UpdateMcElements("st");
   
   if (GlobalPar::GetPar()->IncludeBM())
      UpdateMcElements("bm");
   
   if (GlobalPar::GetPar()->IncludeVertex())
      UpdateMcElements("vt");
   
   if (GlobalPar::GetPar()->IncludeInnerTracker())
      UpdateMcElements("it");
   
   if (GlobalPar::GetPar()->IncludeMSD())
      UpdateMcElements("ms");
   
   if (GlobalPar::GetPar()->IncludeTW())
      UpdateMcElements("tw");
   
   if (GlobalPar::GetPar()->IncludeCA())
      UpdateMcElements("ca");
   
   TAFOeventDisplay::UpdateElements();
}

//__________________________________________________________
void TAFOeventDisplayMC::UpdateMcElements(const TString prefix)
{
   
   if (!fgGUIFlag || (fgGUIFlag && fRefreshButton->IsOn())) {
      if (prefix == "ca")
         fCaMcDisplay->Reset();
      if (prefix == "tw")
         fTwMcDisplay->Reset();
      if (prefix == "ms")
         fMsdMcDisplay->Reset();
      if (prefix == "it")
         fItMcDisplay->Reset();
      if (prefix == "vt")
         fVtMcDisplay->Reset();
      if (prefix == "bm")
         fBmMcDisplay->Reset();
      if (prefix == "st")
         fStMcDisplay->Reset();
   }
   
   if (!fgDisplayFlag) // do not update event display
      return;
   
   Float_t  x = 0.,  y = 0.,  z = 0.;
   
   TAMCntuHit* pNtuHit = 0x0;
   
   if (prefix == "st")
      pNtuHit = (TAMCntuHit*) fpNtuMcSt->Object();
   
   if (prefix == "bm")
      pNtuHit = (TAMCntuHit*) fpNtuMcBm->Object();

   if (prefix == "vt")
      pNtuHit = (TAMCntuHit*) fpNtuMcVt->Object();

   if (prefix == "it")
      pNtuHit = (TAMCntuHit*) fpNtuMcIt->Object();
   
   if (prefix == "ms")
      pNtuHit = (TAMCntuHit*) fpNtuMcMsd->Object();

   if (prefix == "tw")
      pNtuHit = (TAMCntuHit*) fpNtuMcTw->Object();
   
   if (prefix == "ca")
      pNtuHit = (TAMCntuHit*) fpNtuMcCa->Object();
   
   Int_t nHits = pNtuHit->GetHitsN();
   if (nHits == 0) return;
   
   for (Int_t iHit = 0; iHit < nHits; ++iHit) {

      TAMChit *hit = pNtuHit->GetHit(iHit);
      TVector3 pos = hit->GetInPosition();
      
      x = pos(0);
      y = pos(1);
      z = pos(2);
      
      if (prefix == "st") {
         fStMcDisplay->AddPoint(x, y, z);
         fStMcDisplay->SetPointId(hit);
      }
      
      if (prefix == "bm") {
         fBmMcDisplay->AddPoint(x, y, z);
         fBmMcDisplay->SetPointId(hit);
      }

      if (prefix == "vt") {
         fVtMcDisplay->AddPoint(x, y, z);
         fVtMcDisplay->SetPointId(hit);
      }

      if (prefix == "it") {
         fItMcDisplay->AddPoint(x, y, z);
         fItMcDisplay->SetPointId(hit);
      }

      if (prefix == "ms") {
         fMsdMcDisplay->AddPoint(x, y, z);
         fMsdMcDisplay->SetPointId(hit);
      }

      if (prefix == "tw") {
         fTwMcDisplay->AddPoint(x, y, z);
         fTwMcDisplay->SetPointId(hit);
      }
      
      if (prefix == "ca") {
         fCaMcDisplay->AddPoint(x, y, z);
         fCaMcDisplay->SetPointId(hit);
      }

   } //end loop on hits
   
}



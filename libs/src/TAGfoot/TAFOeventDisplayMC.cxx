

#include "TAFOeventDisplayMC.hxx"

#include "GlobalPar.hxx"
#include "TABMntuRaw.hxx"
#include "TAVTntuRaw.hxx"
#include "TAITntuRaw.hxx"
#include "TAMSDntuRaw.hxx"
#include "TATW_ContainerHit.hxx"
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
   fActEvtFile(0x0)
{
   fEvtStruct = new EVENT_STRUCT;
}

//__________________________________________________________
TAFOeventDisplayMC::~TAFOeventDisplayMC()
{
   // default destructor
   if (fActEvtFile) delete fActEvtFile;
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

   if (GlobalPar::GetPar()->IncludeBM()) {
      fpNtuRawBm = new TAGdataDsc("bmRaw", new TABMntuRaw());
      fActNtuRawBm = new TABMactNtuMC("bmActNtu", fpNtuRawBm, fpParConfBm, fpParGeoBm, fEvtStruct);
      fActNtuRawBm->CreateHistogram();
   }

   
   if (GlobalPar::GetPar()->IncludeVertex()) {
      fpNtuRawVtx = new TAGdataDsc("vtRaw", new TAVTntuRaw());
      fActNtuRawVtx = new TAVTactNtuMC("vtActNtu", fpNtuRawVtx, fpParGeoVtx, fEvtStruct);
      fActNtuRawVtx->CreateHistogram();
   }

   if (GlobalPar::GetPar()->IncludeInnerTracker()) {
      fpNtuRawIt = new TAGdataDsc("itRaw", new TAITntuRaw());
      fActNtuRawIt = new TAITactNtuMC("itActNtu", fpNtuRawIt, fpParGeoIt, fEvtStruct);
      fActNtuRawIt->CreateHistogram();
   }
   
   if (GlobalPar::GetPar()->IncludeMSD()) {
      fpNtuRawMsd = new TAGdataDsc("msdRaw", new TAMSDntuRaw());
      fActNtuRawMsd = new TAMSDactNtuMC("msdActNtu", fpNtuRawMsd, fpParGeoMsd, fEvtStruct);
      fActNtuRawMsd->CreateHistogram();
   }
   
   if(GlobalPar::GetPar()->IncludeTW()) {
      new TAGdataDsc("containerPoint", new TATW_ContainerPoint());
      fpNtuRawTw   = new TAGdataDsc("containerHit", new TATW_ContainerHit());
      fActNtuRawTw = new TATWactNtuMC("twActNtu", fpNtuRawTw, fEvtStruct);
      fActNtuRawTw->CreateHistogram();
   }
   
   if(GlobalPar::GetPar()->IncludeCA()) {
      fpNtuRawCa   = new TAGdataDsc("caRaw", new TACAntuRaw());
      fActNtuRawCa = new TACAactNtuMC("caActNtu", fpNtuRawCa, fpParGeoCa, fEvtStruct);
      fActNtuRawCa->CreateHistogram();
   }

}

//__________________________________________________________
void TAFOeventDisplayMC::OpenFile(const TString fileName)
{
   fActEvtFile = new TFile(fileName.Data());
   fTree = (TTree*)fActEvtFile->Get("EventTree");
   
   Evento *ev  = new Evento();
   ev->FindBranches(fTree, fEvtStruct);}


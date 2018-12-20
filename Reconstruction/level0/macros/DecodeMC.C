// Macro to make local reconstruction
// Ch. Finck


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <TString.h>
#include <TStopwatch.h>

#include "GlobalPar.hxx"
#include "LocalRecoMC.h"

#endif


//void ReadVtxRawMC(TString name = "16O_C2H4_200_1.root")
//void ReadVtxRawMC(TString name = "p_80_vtx.root")
//void ReadVtxRawMC(TString name = "12C_80_vtx.root")
void DecodeMC(TString name = "12C_400_vtx.root")
{
   GlobalPar::Instance();
   GlobalPar::GetPar()->Print();
   
   Int_t pos = name.Last('.');
   TString nameOut = name(0, pos);
   nameOut.Append("_Out.root");
   
   LocalRecoMC* locRec = new LocalRecoMC(name, nameOut);
   
   // global setting
   //locRec->EnableTree();
   locRec->EnableHisto();
   
   // set detecteors
   // VTX
   if (GlobalPar::GetPar()->IncludeVertex())
      locRec->EnableVtxTrack();
   
   TStopwatch watch;
   watch.Start();
   
   locRec->BeforeEventLoop();
   locRec->LoopEvent(500);
   locRec->AfterEventLoop();

   watch.Print();
}



//header file

#ifndef TCGbaseRunAction_h
#define TCGbaseRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

#include "TAGroot.hxx"
#include "TString.h"

#include <TStopwatch.h>


class G4Run;
class TFile;
class TTree;

class TCGbaseRunAction : public G4UserRunAction
{
public:
   TCGbaseRunAction();
   virtual ~TCGbaseRunAction();
   
   void BeginOfRunAction(const G4Run* aRun);
   void EndOfRunAction(const G4Run* aRun);
   void FillAndClear();
   
   
   TFile* GetOutFile()                const { return fpOutFile;    }
   Int_t  GetEventsNToBeProcessed()   const { return fEventsNToBeProcessed;}
   
public:
   static void SetRootFileName(const char* name) { fgRootFileName = name; }
   static const char* GetRootFileName() { return fgRootFileName; }
   
protected:
   static TString       fgRootFileName;
   
protected:
   TFile*            fpOutFile;
   TTree*            fpTree;
   Int_t             fEventsNToBeProcessed;
   TStopwatch        fWatch;
   
protected:
   virtual void SetContainers()   { return; }
   virtual void ClearContainers() { return; }

};

#endif

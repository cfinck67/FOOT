//header file

#ifndef TCFOrunAction_h
#define TCFOrunAction_h 1

#include "G4UserRunAction.hh"

#include "globals.hh"

#include "Evento.hxx"
#include "TAGroot.hxx"
#include "TString.h"

#include <TStopwatch.h>

class G4Run;
class TFile;
class TTree;

class TCFOrunAction : public G4UserRunAction
{
    public:
    TCFOrunAction();
    ~TCFOrunAction();

    void BeginOfRunAction(const G4Run* aRun);
    void EndOfRunAction(const G4Run* aRun);
    void FillAndClear();

    TFile* GetOutFile()                const { return fpOutFile;    }
    Int_t  GetEventsNToBeProcessed()   const { return fEventsNToBeProcessed;}

    public:
    Evento*      GetEventMC()     const { return fpEventMC; }
    static void SetRootFileName(const char* name) { fgRootFileName = name; }
    static const char* GetRootFileName() { return fgRootFileName; }

    private:
    Evento*        fpEventMC;  // For data form Simulation
    static TString fgRootFileName;
    TFile*         fpOutFile;
    TTree*         fpTree;
    Int_t          fEventsNToBeProcessed;
    TStopwatch     fWatch;

    private:
    void           SetContainers();
    void           ClearContainers();

};

#endif

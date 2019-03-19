// Macro to display FOOT raw data
// Ch. Finck


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TString.h>
#include "TAFOeventDisplay.hxx"
#include "TAGbaseEventDisplay.hxx"
#endif

void DisplayFOOT(TString file = "./run_1004")
{
//   TAFOeventDisplay::SetTrackingAlgo('F');
   
   TAFOeventDisplay::Instance(0)->ShowDisplay(file);
}



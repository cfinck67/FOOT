// Macro to display FOOT raw data
// Ch. Finck


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TString.h>
#include "TAFOeventDisplay.hxx"
#include "TAGbaseEventDisplay.hxx"
#endif

//void DisplayFOOT(TString file = "12C_400_vtx.root")
void DisplayFOOT(TString file = "16O_C2H4_200_1.root")
{
//   TAFOeventDisplay::SetTrackingAlgo('F');
   
   TAFOeventDisplay::Instance(0)->ShowDisplay(file);
}



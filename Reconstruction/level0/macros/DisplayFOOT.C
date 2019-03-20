// Macro to display FOOT raw data
// Ch. Finck


#if !defined(__CINT__) || defined(__MAKECINT__)

#include <Riostream.h>
#include <TString.h>
#include "TAFOeventDisplay.hxx"
#include "TAGbaseEventDisplay.hxx"
#endif

void DisplayFOOT(TString file = "data_test.00001431.physics_foot.daq.RAW._lb0000._EB-RCD._0001.data")
//void DisplayFOOT(TString file = "./run_1005")
{
   //   TAFOeventDisplay::SetTrackingAlgo('F');
   //TAFOeventDisplay::EnableStdAlone();
   TAFOeventDisplay::Instance(0)->ShowDisplay(file);
}

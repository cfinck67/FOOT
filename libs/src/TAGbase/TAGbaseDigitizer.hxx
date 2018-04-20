#ifndef _TAGbaseDigitizer_HXX
#define _TAGbaseDigitizer_HXX

#include "TObject.h"

// --------------------------------------------------------------------------------------
class TAGbaseDigitizer : public TObject {
   
public:
   TAGbaseDigitizer();
   ~TAGbaseDigitizer();

protected:
   //parameters of the ADC
   static         Int_t    fgNadcThresholds;
   static         Double_t fgThresholds[];
      
protected:
   static void SetDefaultADC(Double_t value, Int_t index)
   {
      if (index < 0 || index >= fgNadcThresholds) {
         printf("index out of range %d of %d\n", index, fgNadcThresholds);
         return;
      } else {
         fgThresholds[index] = value;
      }
   }
   
   static void SetDefaultADC(Double_t* value, Int_t n)
   {
      fgNadcThresholds = n;
      for (Int_t i = 0; i < n; ++i)
         fgThresholds[i] = value[i];
   }
};
#endif


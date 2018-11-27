#ifndef _TAGparTools_HXX
#define _TAGparTools_HXX
/*!
  \file
  \brief   Declaration of TAGparTools.
 
  \author Ch. Finck
*/
/*------------------------------------------+---------------------------------*/

#include "Riostream.h"

#include "TObject.h"
#include "TString.h"
#include "TArrayC.h"
#include "TVector3.h"

#include "TAGpara.hxx"


//##############################################################################

class TAGparTools : public TAGpara {
      
protected:
   ifstream fFileStream; // parameter file
   TString  fFileName;   // name of the file
   Int_t    fDebugLevel; // debug level
   
public:
   TAGparTools();
   virtual ~TAGparTools();
   
   Bool_t  Open(const TString& name);
   Bool_t  Eof();
   Bool_t  Close();
   void    ReadItem(TString& item);
   void    ReadItem(Int_t &arg);
   void    ReadItem(Float_t &arg);
   void    ReadItem(TArrayC& array, const Char_t delimiter = ';');
   void    ReadItem(Float_t* coeff, Int_t size, const Char_t delimiter = ' ');
   void    ReadItem(TArrayC& arrayLine, TArrayC& arrayCol, const Char_t delimiter1 = ',', const Char_t delimiter2 = ';');
   void    ReadVector3(TVector3 &arg);
   void    ReadStrings(TString& aString);
   void    GetRange(const char* str, Int_t& begin, Int_t& end, Int_t& incr, Int_t& n);
   void    FillArray(TString& s, TArrayC& array);

   TString Normalize(const char* line);
   
   //! Get file name
   TString GetFileName() const { return fFileName; }
   
   ClassDef(TAGparTools,1)
};

#endif

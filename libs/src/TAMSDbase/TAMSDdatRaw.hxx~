#ifndef _TAVTdatRaw_HXX
#define _TAVTdatRaw_HXX

#include "TAGdata.hxx"
#include "TObjArray.h"
#include "TClonesArray.h"
#include "TString.h"

// --------------------------------------------------------------------------------------
/** TAVTrawHit simple class containers for raw hit (pixel)
 
 \author Ch. Finck
 */

class TAVTrawHit : public TObject {
   
protected:
   Int_t fInput;
   Int_t fValue;
   Int_t fIndex;
   Int_t fLineNumber;
   Int_t fColumnNumber;
         
public:
   TAVTrawHit();
   TAVTrawHit( Int_t input, Int_t value, Int_t index, Int_t columnNumber = 0 );
   virtual ~TAVTrawHit() {;}

   //! Comapre method
   Int_t      Compare(const TObject* obj) const;
   //! Get input type
   Int_t      GetInput()        const    { return fInput;        }
   //! Get value (pulse height)
   Int_t      GetValue()        const    { return fValue;        }
   //! Get line number
   Int_t      GetLineNumber()   const    { return fLineNumber;   }
   //! Get column number
   Int_t      GetColumnNumber() const    { return fColumnNumber; }
   //! Get index
   Int_t      GetIndex()        const    { return fIndex;        }
   //! Is Sortable
   Bool_t     IsSortable()      const    { return kTRUE;         }

   //! Set input type
   void     SetInput(Int_t input)        { fInput = input;      }
   //! Set value
   void     SetValue(Int_t value)        { fValue = value;      }
   //! Set line number
   void     SetLineNumber(Int_t line)    { fLineNumber = line;  }
   //! Set column number
   void     SetColumnNumber(Int_t col)   { fColumnNumber = col; }
   //! Set index
   void     SetIndex(Int_t index)        { fIndex = index;      }
   
   ClassDef(TAVTrawHit,1)
};

//######################################################################################
/** TAVTdatRaw class for raw data (VME & PXI)
 
 \author Ch. Finck
 */

class TAVTdatRaw : public TAGdata {
   
protected:
   TObjArray* fListOfPixels;

private:
   static TString fgkBranchName;    // Branch name in TTree

public:
   TAVTdatRaw();
   ~TAVTdatRaw();
   
   //! Get list of pixels
   TClonesArray*     GetPixels(Int_t iSensor)  const;
 
   //! Get number of pixels
   Int_t             GetPixelsN(Int_t iSensor)  const;      
   
   //! Get pixel
   TAVTrawHit*       GetPixel(Int_t iSensor, Int_t iPixel);        
   const TAVTrawHit* GetPixel(Int_t iSensor, Int_t iPixel)   const;
   
   //! Add pixel to list
   void              AddPixel(Int_t sensor, Int_t value, Int_t aLine, Int_t aColumn);
  
   //! Set up clones
   void             SetupClones();

   //! Clear pixel list
   virtual void     Clear(Option_t* opt="");
   
   //! To stream
   virtual void    ToStream(ostream& os=cout, Option_t* option="") const;

public:   
   static const Char_t* GetBranchName()   { return fgkBranchName.Data(); }

   ClassDef(TAVTdatRaw,2)
};

# endif

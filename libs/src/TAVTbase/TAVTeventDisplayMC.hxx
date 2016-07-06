

#ifndef _TAVTeventDisplayMC_HXX_
#define _TAVTeventDisplayMC_HXX_

#ifndef TAVTbaseEventDisplay_HXX
#include "TAVTbaseEventDisplay.hxx"
#endif

#ifndef ROOT_TTree
#include "TTree.h"
#endif

/** TAVTeventDisplayMC a class to work on the MC event display
 
 */
class TAVTeventDisplayMC : public TAVTbaseEventDisplay
{
   
private:
   static TAVTeventDisplayMC* fgInstance; // static instance of class
   
public:
   //! Instance of class
   static TAVTeventDisplayMC* Instance(const TString name = "MC_Evt1k_0000.root");  
   
public:   
   virtual ~TAVTeventDisplayMC();

private:
   TAVTeventDisplayMC(const TString name);
   
protected:
   
   //! loop events
   virtual Bool_t SetNextEvent();
   
   //! Draw histo for raw data
   virtual void DrawRawdata();
   
   //! Draw histo for rates
   virtual void DrawRate();
   
   //! Reset rate
   void ResetRate();
   
   //! Reset histograms
   void ResetHistogram();
   
   //! Create raw data action
   void CreateRawAction();
   
   //! Add required items
   void AddRequiredItem();
   
private:
   
   TAGaction* fActMCRaw; // action for MC raw data
   TTree*     fTree;     // tree for MC

   ClassDef(TAVTeventDisplayMC, 0); 
};

#endif

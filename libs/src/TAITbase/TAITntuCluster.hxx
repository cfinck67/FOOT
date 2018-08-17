#ifndef _TAITcluster_HXX
#define _TAITcluster_HXX

// ROOT classes
#include "TObject.h"
#include "TList.h"
#include "TVector3.h"
#include "TClonesArray.h"
#include "TObjArray.h"

#include "TAGdata.hxx"
#include "TAVTntuCluster.hxx"
#include "TAITntuRaw.hxx"


/** TAITcluster class, simple container class for tracks with the associated clusters                    
 \author Ch. Finck
 */

class TAITcluster : public TAVTcluster {
   
public:

   TAITcluster();
   TAITcluster(const TAITcluster& cluster);
   ~TAITcluster();
   
   virtual void       SetupClones();

   
   ClassDef(TAITcluster,1)                          // Describes TAITcluster
};



//##############################################################################

class TAITntuCluster : public TAVTntuCluster {
      
private:
   static TString    fgkItBranchName;    // Branch name in TTree

public:
   TAITntuCluster();
   virtual            ~TAITntuCluster();
   
   virtual void       SetupClones();

public:
   static const Char_t* GetItBranchName()   { return fgkItBranchName.Data();   }

   ClassDef(TAITntuCluster,1)
};

#endif


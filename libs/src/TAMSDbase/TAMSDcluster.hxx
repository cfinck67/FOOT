#ifndef _TAMSDcluster_HXX
#define _TAMSDcluster_HXX

// ROOT classes

#include "TAVTbaseCluster.hxx"


/** TAMSDcluster class, simple container class for tracks with the associated clusters                    
 
 \author Ch. Finck
 */

class TAMSDntuHit;

class TAMSDcluster : public TAVTbaseCluster {
   
   
public:
   TAMSDcluster(); 
   TAMSDcluster(const TAMSDcluster& cluster);
   ~TAMSDcluster();
   
   virtual void       SetupClones();

   //! Add pixel to the list
   void               AddPixel(TAMSDntuHit* pixel);
   
   ClassDef(TAMSDcluster,2)                          // Describes TAMSDcluster
};


#endif


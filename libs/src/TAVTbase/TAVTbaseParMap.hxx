#ifndef _TAVTbaseParMap_HXX
#define _TAVTbaseParMap_HXX
/*!
 \file
 \version $Id: TAVTbaseParMap.hxx,v 1.1 2001/11/05 23:13:57 mueller Exp $
 \brief   Declaration of TAVTbaseParMap.
 */
/*------------------------------------------+---------------------------------*/


#include <vector> 

#include "TAGparTools.hxx"

//##############################################################################

class TAVTbaseParMap : public TAGparTools {
   
public:
   TAVTbaseParMap();
   virtual ~TAVTbaseParMap();

   //! return plane id
   Int_t        GetPlaneId(Int_t sensorId);
   
   //! Read from file
   Bool_t       FromFile(const TString& name = "");

protected:
   Int_t              fSensorsN;
   std::vector<int>   fPlaneId;
   TString            fkDefaultMapName; // default detector mapping file
   
   ClassDef(TAVTbaseParMap,1)
  
};

#endif

#ifndef _TAITactNtuPostTrack_HXX
#define _TAITactNtuPostTrack_HXX
/*!
 \file
 \version $Id: TAITactNtuPostTrack.hxx,v 1.4 2003/06/09 18:17:14 mueller Exp $
 \brief   Declaration of TAITactNtuPostTrack.
 */
/*------------------------------------------+---------------------------------*/


#include "TAITactBaseNtuTrack.hxx"


class TAITactNtuPostTrack : public TAITactBaseNtuTrack {
   
public:
   
   explicit  TAITactNtuPostTrack(const char* name       = 0,
								 TAGdataDsc* p_ntuclus  = 0, 
								 TAGdataDsc* p_ntutrack = 0, 
								 TAGparaDsc* p_config   = 0,
								 TAGparaDsc* p_geomap   = 0,
								 TAGdataDsc* pNtuVertex = 0);
   
   virtual ~TAITactNtuPostTrack();
   
   //! Action
   Bool_t  Action();
   
   //! Apply cuts
   Bool_t  AppyCuts(TAITtrack* track);

private:
   TAGdataDsc* fpNtuVertex;		 // input data dsc

private:    
   
   Bool_t FindTiltedTracks();
   
   ClassDef(TAITactNtuPostTrack,0)
};

#endif

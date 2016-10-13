#ifndef _TAITactNtuTrackH_HXX
#define _TAITactNtuTrackH_HXX


#include "TAITactBaseNtuTrack.hxx"

class TH2F;
class TList;
class TAITactNtuTrackH : public TAITactBaseNtuTrack {
   
public:
   
   explicit  TAITactNtuTrackH(const char* name      = 0,
							 TAGdataDsc* p_ntuclus  = 0, 
							 TAGdataDsc* p_ntutrack = 0, 
							 TAGparaDsc* p_config   = 0,
							 TAGparaDsc* p_geomap   = 0,
							 TAGparaDsc* p_calib    = 0,
							 TAGdataDsc* p_bmtrack  = 0);
   virtual ~TAITactNtuTrackH();
	   
   //! Get list of clusters
   TList*  GetListOfVertices() const { return   fListOfVertices; }

private:
   TH2F*   fpHisHoughXZ;		// XZ histogram for Hough transform
   TH2F*   fpHisHoughYZ;		// YZ histogram for Hough transform
   TList*  fListOfVertices;     // list of tracks' intersection on target

private:    
   
   Bool_t FindTiltedTracks();
   Bool_t IsGoodCandidate(TAITtrack* track);
   Bool_t FindVertices();

   ClassDef(TAITactNtuTrackH,0)
};

#endif

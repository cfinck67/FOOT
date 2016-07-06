
  ////////////////////////////////////////////////////////////
  //                                                        //
  // Class  TAVTntuAlign                                    //
  // book keeping for alignment                             //
  //                                                        //
  ////////////////////////////////////////////////////////////

#include "TMath.h"

#include "TAVTntuAlign.hxx"

ClassImp(TAVTntuAlign) // Description of TAVTntuAlign 

//______________________________________________________________________________
//  
TAVTntuAlign::TAVTntuAlign()
: TAGdata()
{
  SetDebug( 0 );
  fPlaneNumber = 0;
}

//______________________________________________________________________________
//  
TAVTntuAlign::TAVTntuAlign( Int_t aPlaneNumber)
: TAGdata()
{
  fDebugAlign       = 0;
  fPlaneNumber      = aPlaneNumber;
  fEnough2D         = kFALSE;
  fAlignResolutionU = 0.0;
  fAlignMeanU       = 0.0;
  fAlignResolutionV = 0.0;
  fAlignMeanV       = 0.0;
  fAlignSum_du      = 0.0;
  fAlignSum_dudu    = 0.0;
  fAlignSum_duvt    = 0.0;
  fAlignSum_vt      = 0.0;
  fAlignSum_vtvt    = 0.0;
  fAlignCountu      = 0;
  fAlignSum_dv      = 0.0;
  fAlignSum_dvdv    = 0.0;
  fAlignSum_dvut    = 0.0;
  fAlignSum_ut      = 0.0;
  fAlignSum_utut    = 0.0;
  fAlignCountv      = 0;
  fAlignSum_uvt2    = 0.;
  fAlignSum_dvuduv  = 0.0;
  fAlignCount2D     = 0;
  fAlignTiltW       = 0.0;
  fAlignOffsetU     = 0.0;
  fAlignOffsetV     = 0.0; 
  fAlignTiltWError  = 0.0;
  fAlignOffsetUError= 0.0;
  fAlignOffsetVError= 0.0; 
  fAlignCorrelation = 0.0;
  fNumberOfTracks   = 300; // preliminary number 
  fBound            = 5000.; // preliminary max distance track-hit

   if(fDebugAlign) cout << endl << " -*-*- TAVTntuAlign User Constructor -*-*- " << endl;
   
  if(fDebugAlign) {
    cout << "  Plane " << fPlaneNumber;
    cout << "  Miminal number of tracks to accumulate " << fNumberOfTracks << endl;
    cout << "  Maxinal distance between track and hit " << fBound << endl;
  }

  if(fDebugAlign) cout << endl << " -*-*- TAVTntuAlign User Constructor DONE -*-*- " << endl;

}

//______________________________________________________________________________
//  
TAVTntuAlign::~TAVTntuAlign()
{ 
  // Default TAVTntuAlign destructor.
}

//______________________________________________________________________________
//  
void TAVTntuAlign::SetBounding(Double_t aBound)  
{ 
  // Change bounding to accept a track to hit distance
  // and update histograms axis
  fBound = aBound; 
  if (fDebugAlign)
	 cout <<"Bound set to "<<fBound<<" for plane "<<fPlaneNumber<<endl;
}


//______________________________________________________________________________
//  
void TAVTntuAlign::Accumulate2D(TVector3& aTrackPos, Double_t aDistanceU, Double_t aDistanceV)
{
  // method to correlate alignment in U and V
  //
  // First updates event by event measured and predicted positions to compute a chi square:
  // chi2 = M-matrix x Align-vector - Shift-vector,
  // where Shift-vector = {sum(uhit-utrack), sum(vhit-vtrack), sum[vtrack(uhit-utrack)-utrack(vhit-vtrack)]}
  // and   Align-vector = { Uoffser, Voffset, TiltZ}.
  // Once the number of events is enough (fNumberOfTracks),
  // does a line fit or simulatenous chi2 minimization to determine inverse-M.
  // So, Align-vector = inverse-M x Shift-vector.
  
  if( fDebugAlign>1) 
	 printf("   TAVTntuAlign: plane %d Accumulating in U and V with distance (%f, %f) bound %.1f\n", fPlaneNumber, aDistanceU, aDistanceV, fBound);

  // Accumaltion of clusters
   if ( TMath::Abs(aDistanceU) <= fBound && TMath::Abs(aDistanceV) <= fBound ) {

    fAlignSum_du     += aDistanceU;                    // summation of u_hit - u_track
    fAlignSum_dv     += aDistanceV;                    // summation of v_hit - v_track
    fAlignSum_ut     += aTrackPos[0]; 
    fAlignSum_vt     += aTrackPos[1]; 
    fAlignSum_utut   += aTrackPos[0] * aTrackPos[0];
    fAlignSum_vtvt   += aTrackPos[1] * aTrackPos[1];
    fAlignSum_uvt2   += aTrackPos[0] * aTrackPos[0] + aTrackPos[1] * aTrackPos[1];
    fAlignSum_dvuduv += aDistanceU   * aTrackPos(1) - aDistanceV   * aTrackPos[0];
    fAlignSum_dudu   += aDistanceU * aDistanceU;       // summation for Resolution estimate
    fAlignSum_dvdv   += aDistanceV * aDistanceV;       // summation for Resolution estimate
    fAlignSum_duvt   += aDistanceU * aTrackPos[1];     // summation of (u_hit - u_track) * v_track
    fAlignSum_dvut   += aDistanceV * aTrackPos[0];     // summation of (v_hit - v_track) * u_track
    fAlignCount2D++;

	 if (fDebugAlign)
		 printf("   TAVTntuAlign::Accumulate2D plane %d, dist=(%.1f, %.1f) / %.1f, t=(%.1f, %.1f), count2D=%d/%d\n", 
				fPlaneNumber, TMath::Abs(aDistanceU), TMath::Abs(aDistanceV), fBound, fAlignSum_ut, fAlignSum_vt, 
				fAlignCount2D, fNumberOfTracks);
  }
   
  // Compute parameters according to the linear chisquare fit
  if (fAlignCount2D > fNumberOfTracks) {

    Double_t tDenominator = fAlignCount2D * fAlignSum_vtvt - fAlignSum_vt * fAlignSum_vt;
    fAlignOffsetU         = (fAlignSum_du * fAlignSum_vtvt - fAlignSum_duvt * fAlignSum_vt) / tDenominator;
    fAlignTiltW           = (fAlignCount2D  * fAlignSum_duvt - fAlignSum_du   * fAlignSum_vt) / tDenominator;
    fAlignOffsetUError    = sqrt( fAlignSum_vtvt / tDenominator );
    fAlignTiltWError      = sqrt( fAlignCount2D / tDenominator );
    fAlignCorrelation     = fAlignSum_vt / ( tDenominator * fAlignOffsetUError * fAlignTiltWError );
    tDenominator          = fAlignCount2D * fAlignSum_utut - fAlignSum_ut * fAlignSum_ut;
    fAlignOffsetV         = (fAlignSum_dv * fAlignSum_utut - fAlignSum_dvut * fAlignSum_ut) / tDenominator;
    fAlignOffsetVError    = sqrt( fAlignSum_utut / tDenominator );

    fAlignOffset(0)    = fAlignOffsetU;
    fAlignOffset(1)    = fAlignOffsetV;

    fAlignMeanU        = fAlignSum_du/fAlignCount2D;
    fAlignResolutionU  = sqrt(fAlignSum_dudu/fAlignCount2D - fAlignMeanU * fAlignMeanU);
    fAlignMeanV        = fAlignSum_dv/fAlignCount2D;
    fAlignResolutionV  = sqrt(fAlignSum_dvdv/fAlignCount2D - fAlignMeanV * fAlignMeanV);
    
    fEnough2D = kTRUE;
    if (fDebugAlign) 
	   printf("   TAVTntuAlign::Accumulate2D plane %d uoffset=%.1f +/- %.1f Voffset=%.1f +/- %.1f, Uresidu=%.1f +/- %.1f, Vresidu=%.1f +/- %.1f\n", 
			  fPlaneNumber, fAlignOffsetU, fAlignOffsetUError, fAlignOffsetV, fAlignOffsetVError, fAlignMeanU, fAlignResolutionU, 
			  fAlignMeanV, fAlignResolutionV);
  } else
    fEnough2D = kFALSE;
}

//______________________________________________________________________________
//  
void TAVTntuAlign::Modified()
{
  // reset the summation variables
  // and update the max distance track-hit
  
  printf(" Alignment Accumulation restarts .... \n\n");
  
  fAlignSum_du      = 0.0;
  fAlignSum_dudu    = 0.0;
  fAlignSum_duvt    = 0.0;  
  fAlignSum_vt      = 0.0;
  fAlignSum_vtvt    = 0.0; 
  fAlignCountu      = 0;
  fAlignSum_dv      = 0.0; 
  fAlignSum_dvdv    = 0.0;
  fAlignSum_dvut    = 0.0;  
  fAlignSum_ut      = 0.0;
  fAlignSum_utut    = 0.0; 
  fAlignCountv      = 0;
  fAlignSum_dvuduv  = 0; 
  fAlignSum_uvt2    = 0; 
  fAlignCount2D     = 0; 
   
  Double_t tBound;
  tBound = TMath::Max( 50., 1.5*TMath::Max( TMath::Abs(fAlignResolutionU), TMath::Abs(fAlignResolutionV)) ); // 150. is a minimal value, changed to 50
  if( tBound < fBound ) {
    SetBounding(tBound);
  } 
}

//______________________________________________________________________________
//  
void TAVTntuAlign::ShowCorrection()
{
  printf(" Found Deviation in U Position [um] : %f\n",fAlignOffsetU);
  printf(" Found Deviation in V Position [um] : %f\n",fAlignOffsetV);
  printf(" Found Deviation in W-Tilt     [deg]: %f\n",fAlignTiltW*180/TMath::Pi());
  printf(" -> Estimated Resolution U %f um at Residual Mean %f um\n",fAlignResolutionU,fAlignMeanU);
  printf(" -> Estimated Resolution V %f um at Residual Mean %f um\n",fAlignResolutionV,fAlignMeanV);
  printf(" Estimated Offest-U Error      [um] : %f\n",fAlignOffsetUError);
  printf(" Estimated Offest-V Error      [um] : %f\n",fAlignOffsetVError);
  printf(" Estimated Tilt-W Error        [deg]: %f\n",fAlignTiltWError*180/TMath::Pi());  
  printf(" Correlation between du and vt      : %f\n",fAlignCorrelation);  
}

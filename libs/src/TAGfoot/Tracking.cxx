#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "Tracking.hxx"

using namespace std;

Tracking::Tracking() {
  
  /* Input from other files */
  track_id = 0; // from GetZ.h
  tracklet_z = 0;
  tracklet_xo = 0;
  tracklet_xs = 0;
  tracklet_yo = 0;
  tracklet_ys = 0;
  tracklet_time = 0;
  xpos_targ = 0;
  ypos_targ = 0;
  MOMENTUM_FACTOR = 0; 

  LBI =  90;
  LBJ =  19;
  LBK =  21;

  RBI =  85;
  RBJ =  19;
  RBK =  17;



}

Tracking::~Tracking() {}

void Tracking::Initialize(Int_t tid, Float_t trkl_z, Float_t trkl_xo, Float_t trkl_xs
			  , Float_t trkl_yo, Float_t trkl_ys, Float_t trkl_time
			  , Float_t x_t, Float_t y_t, Float_t momf) {
  
  /* Input from other files */ //TTB this should be the positions (o) and slopes (s) of the track in the music, also a initial target position?!
  track_id = tid; // from GetZ.h
  tracklet_z = trkl_z;
  tracklet_xo = trkl_xo;
  tracklet_xs = trkl_xs;
  tracklet_yo = trkl_yo;
  tracklet_ys = trkl_ys;
  tracklet_time = trkl_time;
  xpos_targ = x_t;
  ypos_targ = y_t;
  MOMENTUM_FACTOR = momf; 

  return;
}


//TTB also adds up total path, etc.
// Main routine 
Int_t Tracking::TrackMagnet(Int_t i_nevent) {

  cout<<"DEBUG: Called Tracking::TrackMagnet: Event:"<<i_nevent<<endl;

  /* Produced new variables */
  rigidity        = 0.0;   /* rigidity from tracking [Tm]                                  */
  rigx     = 0.0;
  rigy     = 0.0;
  rigz     = 0.0;
  rigx_lab = 0.0;
  rigy_lab = 0.0;
  rigz_lab = 0.0;

  trackinc = 0.0;
  backxtarg = 0.0; 
  IDreconst = -9999;
  IDloop    = -9999;

  tracking_result = RESULT_TRACK_START;
  magnet_theta    = -999.; 
  magnet_phi      = -999.; 
  delta_theta     = -999.; 
  delta_phi       = -999.; 
  beta_new        = -999.; 
  track_iters     = -999;  
  track_xtdiff    = -999.; 
  track_ytdiff    = -999.; 
  track_xt        = -999.;
  track_yt        = -999.;
  rig_raw         = -999.; 
  path_forward    = -999.; 
  path_backward   = -999.; 

  // Internal variables
  Int_t   i          = 0;                 
  Float_t pfit       = 0.0;
  Float_t x_tar      = 0.0;
  Float_t y_tar      = 0.0;
  Float_t z_tar      = 0.0;
  Float_t pchange    = 0.0;
  Float_t xfit       = 0.0;
  Float_t pulsfit    = 0.0;
  Float_t xdif[2]    = {0.0, 0.0};
  Float_t puls[2]    = {0.0, 0.0};
  UInt_t  tr_status  = 0;
  Int_t   iterations = 0;
  Float_t momxz      = 0.0;
  UInt_t  status     = 0;
  Float_t xtargback  = 0.0;
  Int_t   bigswi     = 0;
  Float_t xtdiff     = 0.0;
  Float_t ytdiff     = 0.0;
  Float_t pfit_ini   = 0.0;
  Int_t   maytay     = 0;
  Int_t   result     = 0;
//  Float_t gamma      = 0.0;
  Float_t dthe_ini   = 0.0;
  Float_t dphi_ini   = 0.0;
  Int_t iloopmax = 0;
  
  /* The measurement geometry */
  part my_in;           // Particle description
  part my_back;          // Backup for iterations

  // These variables are used as input for RungeKutta() in the end
  Float_t theta   = 0.0; // = xs
  Float_t phi     = 0.0; // = ys

  // used for check
  Float_t xtarget = xpos_targ;
  Float_t ytarget = ypos_targ;
  //  Float_t ztarget = 0.;
  // Pseudo statics

  /* Importante field flag to choose bewtwwn WS or fieldmap*/

  Int_t   field      = 1;        // 0 = Wood-Saxon, 1 = fieldmap
  Int_t   debug_flag = 0;        // 0 = shutup

  //if( i_nevent < 10 ) { cout << "DEBUG: I'm in TrackMagnet() now!" << endl; }
 
  //if( i_nevent < 50 ) { printf("xdc  = %f [cm]\nydc  = %f [cm]\nzdc  = %f [cm]\n", xdc, ydc, zdc); }
  //if( i_nevent < 50 ) { printf("xarf = %f [cm]\nyarf = %f [cm]\nzarf = %f [cm]\n", xtarget, ytarget, ztarget); }

  // Here we go!
  // ############################################
	
  tracking_result = RESULT_TRACK_START;

  // Check if we have a real tracklet with calibrated Z data
  if( track_id < 0 ) 
    {
      cout << "ERROR: track is not initialized!" << endl;
      result          = RESULT_TRACK_INIT;
      tracking_result = RESULT_TRACK_INIT;
      return result;
    }

	
  /* We have a new track to find */
  tr_status = NEW_TRACK;
  iterations = 0;
  
  /* Fill the variables with the track selected by GetZ() */
  theta = tracklet_xs;
  phi   = tracklet_ys;
  
  /* Do an estimated guess on the particle momentum */
  //  pfit = 1.0 + (PFIT_P1 * ( (tracklet_xo * 0.1) - PFIT_P2 * (theta * RAD2DEG - PFIT_P3) - PFIT_P4) );
  
  //  pfit = 1.0 + (PFIT_P1 * ( (tracklet_xo) - PFIT_P2 * (theta * RAD2DEG - PFIT_P3) - PFIT_P4) );
  
  /* tracklet_xo is already in cm!! */
  
  pfit = 1.0;
		      
  if( (pfit > PFIT_LOWER) && (pfit < PFIT_UPPER) ) /* Use all particles */ //TTB wtf, pfit is just set before to 1.0??? no use of "if"
    {
      /* initial value of momentum seems resonable */
      
      /* we start with a complete new fresh particle */
      init_part( &my_in );
      init_part( &my_back );
     
      //TTB we want obtain from initial MUSIC information coordinates in the ALADIN frame!!!!!!!!
      /* do the coordinate transformations */
      /* position of particle in MUSIC */
      //TTB do correct for some inaccuracies between assumed and real position,
      // some exchange between x and z coordinates (extra angle??)
      my_in.pos[2] = ALADIN_MUSIC_DIST -0.1*(0.00890*tracklet_xo);    /* z */
      my_in.pos[1] = 0.1*tracklet_yo-1.00;                                /* y */
      // added 27.04.05: the Music sits 10mm lower than the beam axis (ytarget=yaladin=0)

      my_in.pos[0] = 0.1*(0.99996*tracklet_xo) + MUSIC_X_SHIFT+0.091;        /* x */ //TTB do the shift in music frame

      /* new Geometry...hopefully with WT magic number! with changed sign*/
      
      //TTB rotations of MUSIC wrt to aladin
      /* angles of the particle in MUSIC */
      theta = (atan(theta) * RAD2DEG) + MUSIC_ROTATION- 0.0687;        /* theta */ //TTB some extra rotations???
      phi   = (phi * RAD2DEG); //why??? no atan???                                /* phi   */
      // ---------------------------------------------------------------------------------------
	
      /* initial momentum */
      my_in.smom = (pfit * MOMENTUM_FACTOR);//for the moment pfit always =1
      pfit_ini = (pfit * MOMENTUM_FACTOR);
	      
      /* target position (?target = [mm]) */ /*??????????? should also be [cm]! */
      //TTB wtf????
      x_tar = 0.1*(0.9922*xtarget) + (TARGET_X_SHIFT);                 /* x */
      y_tar = 0.1*ytarget;                                  /* y */
      z_tar = 0.1248*xtarget;	                                 /* z */

/* from here the two programs are identical!! */


      my_in.target[0] = x_tar;
      my_in.target[1] = y_tar;
      my_in.target[2] = z_tar;
      
      /* variables used for iterations */
      pchange = PCHANGE_INI;
      puls[0] = 0.;
      puls[1] = 0.;
      xdif[0] = 0.;
      xdif[1] = 0.;
      xfit    = 0.;
      pulsfit = 0.;
	 
      /* we store a copy of the particle here */
      copy_part( my_in , &my_back );
      dthe_ini=theta;
      dphi_ini=phi;
      /* we have a new track to find */
      tr_status = NEW_TRACK;
      iterations = 0;

      if( debug_flag > 0 )  {
	
	cout << "---------------------- Starting Values -------------------------------- " << endl;
	cout << my_in.pos[0] << " " << my_in.pos[1] << " " << my_in.pos[2] << endl;
	cout << my_in.smom  << " " << my_in.status  << " " << theta << " " << phi << " " << tracklet_z << endl;
	cout << " ************************************************************************" << endl;
      }
	      
      /* we have a maximum number of tries per track */
      for( maytay = 0; maytay < MAX_TRY; maytay++ )
	{
	  if( debug_flag > 0 )
	    {
	      printf("#********************************************************************\n");
	      printf("# Iteration: %d\n", maytay);
	      printf("# Status   : 0x%08x\n", tr_status);
	      if( tr_status & TRACK_NOCONV )     { printf("#          : TRACK_NOCONV\n"); }
	      if( tr_status & TRACK_HITALADIN )  { printf("#          : TRACK_HITALADIN\n"); }
	      if( tr_status & TRACK_PASSALADIN ) { printf("#          : TRACK_PASSALADIN\n"); }
	      if( tr_status & TARGET_HIT )       { printf("#          : TARGET_HIT\n"); }
	      if( tr_status & TARGET_MISS )      { printf("#          : TARGET_MISS\n"); }
	      if( tr_status & TARGET_BOTHSIDES ) { printf("#          : TARGET_BOTHSIDES\n"); }
	      if( tr_status & TARGET_SAMESIDE )  { printf("#          : TARGET_SAMESIDE\n"); }
	      if( tr_status & TARGET_APPROACH )  { printf("#          : TARGET_APPROACH\n"); }	      
	      if( tr_status & TARGET_DEPART )    { printf("#          : TARGET_DEPART\n"); }
	      printf("# bigswi   : %d\n", bigswi);
	      printf("# smom     : %10.4f\n", my_in.smom);
	      printf("# stot     : %10.4f\n", my_in.spathfw);
	      printf("# xdif[]   : %10.4f %10.4f\n", xdif[0], xdif[1]);
	      printf("# puls[]   : %10.4f %10.4f\n", puls[0], puls[1]);
	      printf("# X Y Z    : %10.4f %10.4f %10.4f\n", my_in.pos[0], my_in.pos[1], my_in.pos[2]);
	      printf("# PX PY PZ : %10.4f %10.4f %10.4f\n", my_in.mom[0], my_in.mom[1], my_in.mom[2]);
	      printf("# theta phi: %10.4f %10.4f\n", theta, phi);
	    }
	  
	  /* End the loop if we either hit the target or ALADIN magnet */
	  if( tr_status & TARGET_HIT ) { break; }
	  if( tr_status & TRACK_HITALADIN ) { break; }
	  
	  /* we need momentum components px,py,pz for tracking */
	  my_in.mom[1] = - my_in.smom * sin( dphi_ini * DEG2RAD );                         /* py */
	  momxz      = sqrt( my_in.smom * my_in.smom - my_in.mom[1] * my_in.mom[1] );     /* projection in x-z */
	  my_in.mom[0] = - momxz * sin( dthe_ini * DEG2RAD );                          /* px */ 
	  my_in.mom[2] = - sqrt( momxz * momxz - my_in.mom[0] * my_in.mom[0] );         /* pz */

	  /*
	  cout << " ===============================================================================" << endl;
	  cout << my_in.pos[0] << " " << my_in.pos[1] << " " << my_in.pos[2] << endl;
	  cout << my_in.mom[0] << " " << my_in.mom[1] << " " << my_in.mom[2] << endl;
	  cout << tracklet_z << endl;
	  cout << " ===============================================================================" << endl;
	  */

	  /* restore the position inside MUSIC to initial values */
	  restore_position( &my_back, &my_in );

	  /* each iteration dumps the values */
	  if( debug_flag > 0 )
	    {
	      printf("#********************************************************************\n");
	      printf("# Iteration: %d\n", maytay);
	      printf("# Status   : 0x%08x\n", tr_status);
	      if( tr_status & TRACK_NOCONV )     { printf("#          : TRACK_NOCONV\n"); }
	      if( tr_status & TRACK_HITALADIN )  { printf("#          : TRACK_HITALADIN\n"); }
	      if( tr_status & TRACK_PASSALADIN ) { printf("#          : TRACK_PASSALADIN\n"); }
	      if( tr_status & TARGET_HIT )       { printf("#          : TARGET_HIT\n"); }
	      if( tr_status & TARGET_MISS )      { printf("#          : TARGET_MISS\n"); }
	      if( tr_status & TARGET_BOTHSIDES ) { printf("#          : TARGET_BOTHSIDES\n"); }
	      if( tr_status & TARGET_SAMESIDE )  { printf("#          : TARGET_SAMESIDE\n"); }
	      if( tr_status & TARGET_APPROACH )  { printf("#          : TARGET_APPROACH\n"); }	      
	      if( tr_status & TARGET_DEPART )    { printf("#          : TARGET_DEPART\n"); }
	      printf("# bigswi   : %d\n", bigswi);
	      printf("# smom     : %10.4f\n", my_in.smom);
	      printf("# stot     : %10.4f\n", my_in.spathfw);
	      printf("# xdif[]   : %10.4f %10.4f\n", xdif[0], xdif[1]);
	      printf("# puls[]   : %10.4f %10.4f\n", puls[0], puls[1]);
	      printf("# X Y Z    : %10.4f %10.4f %10.4f\n", my_in.pos[0], my_in.pos[1], my_in.pos[2]);
	      printf("# PX PY PZ : %10.4f %10.4f %10.4f\n", my_in.mom[0], my_in.mom[1], my_in.mom[2]);
	      printf("# theta phi: %10.4f %10.4f\n", theta, phi);
	    }
	  
	  /* call RK4() */
	  status = RungeKutta( &my_in, field, debug_flag, 0 );
	  /* translate the result to our needs */
	  if( (status & RK_BADTRACK)  ) { tr_status = ((tr_status & ~MASK_TRACK) | TRACK_HITALADIN);  }
	  if( (status & RK_GOODTRACK) ) { tr_status = ((tr_status & ~MASK_TRACK) | TRACK_PASSALADIN); }

	  if( debug_flag > 0 )
	    {
	      printf("#********************************************************************\n");
	      printf("# Iteration: %d\n", maytay);
	      printf("# Status   : 0x%08x\n", tr_status);
	      if( tr_status & TRACK_NOCONV )     { printf("#          : TRACK_NOCONV\n"); }
	      if( tr_status & TRACK_HITALADIN )  { printf("#          : TRACK_HITALADIN\n"); }
	      if( tr_status & TRACK_PASSALADIN ) { printf("#          : TRACK_PASSALADIN\n"); }
	      if( tr_status & TARGET_HIT )       { printf("#          : TARGET_HIT\n"); }
	      if( tr_status & TARGET_MISS )      { printf("#          : TARGET_MISS\n"); }
	      if( tr_status & TARGET_BOTHSIDES ) { printf("#          : TARGET_BOTHSIDES\n"); }
	      if( tr_status & TARGET_SAMESIDE )  { printf("#          : TARGET_SAMESIDE\n"); }
	      if( tr_status & TARGET_APPROACH )  { printf("#          : TARGET_APPROACH\n"); }	      
	      if( tr_status & TARGET_DEPART )    { printf("#          : TARGET_DEPART\n"); }
	      printf("# bigswi   : %d\n", bigswi);
	      printf("# smom     : %10.4f\n", my_in.smom);
	      printf("# stot     : %10.4f\n", my_in.spathfw);
	      printf("# xdif[]   : %10.4f %10.4f\n", xdif[0], xdif[1]);
	      printf("# puls[]   : %10.4f %10.4f\n", puls[0], puls[1]);
	      printf("# X Y Z    : %10.4f %10.4f %10.4f\n", my_in.pos[0], my_in.pos[1], my_in.pos[2]);
	      printf("# PX PY PZ : %10.4f %10.4f %10.4f\n", my_in.mom[0], my_in.mom[1], my_in.mom[2]);
	      printf("# theta phi: %10.4f %10.4f\n", theta, phi);
	    }
	  
	  
	  if( tr_status & TRACK_PASSALADIN )
	    {
	      /* particle passed ALADIN successfully */
	      xtargback = my_in.pos[0];
		
//	printf("%11.5f,%11.5f,%11.5f,%11.5f,%11.5f\n",my_in.pos[0],x_tar,xtargback-x_tar,pchange,my_in.smom) ;
	
/* from gere I cannot follow anymore the algorithm with respect to ours! */
      
	      if( (tr_status & MASK_ITER) != 0 )
//	      if(bigswi!=0 )
		{
		  /* this is not our first attempt in tracking, so compare against the previous one */
		  iterations++;
		  /* Store number of iterations */
		  tr_status = ((tr_status & ~MASK_ITER) | (iterations & MASK_ITER));
		  if( debug_flag > 0 ) { printf(" -> ITER > 0\n"); }
		  
		  xdif[1] = xtargback - x_tar;
		  puls[1] = my_in.smom;
		  
		  if( fabs(xdif[1]) < TARGET_X_RESOLUTION )
		    {
		      /* we made it into target with the current track */
		      tr_status = ((tr_status & ~MASK_TARGET) | TARGET_HIT);
		      if( debug_flag > 0 ) { printf(" -> TARGET_HIT\n"); }  
		      
		      xfit     = xdif[1];
		      pulsfit  = puls[1];
		      my_in.smom = puls[1];
		
		    }
		  else /*( fabs(xdif[1]) > TARGET_X_RESOLUTION ) */
		    {
		      /* not yet hit, so get an idea on how to change paramters for next iteration */
		      if( xdif[0] * xdif[1] > 0 )
			{
			  /* previous and current tracks are on the same side of target */
			  tr_status = ((tr_status & ~MASK_TARGET) | TARGET_SAMESIDE);
			  if( debug_flag > 0 ) { printf(" -> TARGET_SAMESIDE\n"); }			  			      

/* from here ....*/			  
			  if( fabs(xdif[1]) - fabs(xdif[0]) < 0 )
			    {
			      /* our tracks are approaching the target */
			      tr_status = ((tr_status & ~MASK_TARGET) | TARGET_APPROACH);
			      if( debug_flag > 0 ) { printf(" -> TARGET_APPROACH\n"); }  			      

			      /* added 7.12.07 the first and commented the second */
   			      /*my_in.smom =  puls[0] + (puls[1] - puls[0]) *
			      xdif[0] / (xdif[0] - xdif[1]); */
			      
  
			      xdif[0] = xdif[1];
			      puls[0] = puls[1];
			      my_in.smom = puls[0] * (1 + bigswi * pchange);
			    }
			  else
			    {
			      /* our tracks are moving away from target */
			      tr_status = ((tr_status & ~MASK_TARGET) | TARGET_DEPART);
			      if( debug_flag > 0 ) { printf(" -> TARGET_DEPART\n"); }		  			      
			      
			      bigswi = -bigswi;

			    }
/*to here ok */
			    
			} /* if( xdif[0] * xdif[1] < 0 ) */
		      else
			{
			  /* previous and current tracks are on different sides of target */
			  tr_status = ((tr_status & ~MASK_TARGET) | TARGET_BOTHSIDES);
			  if( debug_flag > 0 ) { printf(" -> TARGET_BOTHSIDES\n"); }		  			      
			  
			  pulsfit = puls[0] + (puls[1] - puls[0]) * xdif[0] / (xdif[0] - xdif[1]);
			  
			  //if( pchange < PCHANGE_INI )
			  //  {
			  //    /* we did already do tracking with smaller steps */
			  //    tr_status = ((tr_status & ~MASK_TARGET) | TARGET_HIT);
			  //    if( debug_flag > 0 ) { printf(" -> TARGET_HIT\n"); }		      
			  //    
			  //  }
			  //else
			   // {
			      /* do another iterations with smaller step size */
			      tr_status = ((tr_status & ~MASK_TARGET) | TARGET_MISS);
			      if( debug_flag > 0 ) { printf(" -> TARGET_MISS BETTER\n"); }     	      
			      
			      bigswi   = 0;
			      pchange /= 5.; 
			      /* WT: /=2. and the distribution of xdiff shoul change <-- TEST IT!
			      6.07.07 */
			      
			      my_in.smom = pulsfit;
			    //}
			  
			}/* if( xdif[0] * xdif[1] < 0 ) */
		    }/*( fabs(xdif[1]) > TARGET_X_RESOLUTION ) */
		}
	      else /*if( (tr_status & MASK_ITER) == 0 ) */
		{
		  /* our first attempt, check if we already succeeded; if not -> try again */
			/* cs 5.12.07 checked: here is passes only the first time */
		  iterations++;
		  /* Store number of iterations */
		  tr_status = ((tr_status & ~MASK_ITER) | (iterations & MASK_ITER));
		  if( debug_flag > 0 ) { printf(" -> ITER = 0\n"); }  			      
		  
		  xdif[0] = xtargback - x_tar;
		  puls[0] = my_in.smom;
		  
		  if( fabs(xdif[0]) < TARGET_X_RESOLUTION )
		    {
		      /* we made it into target with the first tracking attempt */
		      tr_status = ((tr_status & ~MASK_TARGET) | TARGET_HIT);
		      if( debug_flag > 0 ) { printf(" -> TARGET_HIT\n"); }
		      
		      xfit    = xdif[0];
		      pulsfit = puls[0];
		    }
		  else /*( fabs(xdif[0]) > TARGET_X_RESOLUTION ) */
		    {
		      /* not yet hit, so get an idea on how to change paramters for next iteration */
		      tr_status = ((tr_status & ~MASK_TARGET) | TARGET_MISS);
		      if( debug_flag > 0) { printf(" -> TARGET_MISS\n"); }			  			      
		      
		      if( xdif[0] < 0 ) /* cs 5.12.07 checked OK */
			{
			  bigswi = -1;
			}
		      else
			{
			  bigswi = 1;
			}
		      
		      my_in.smom = puls[0] * (1 + bigswi * pchange);
		    }
		  
		}/*if( (tr_status & MASK_ITER) == 0 ) */
	    }
	  else /*if(!(tr_status & TRACK_PASSALADIN ))*/
	    {
	      /* particle did hit the magnet */
	      tr_status = ((tr_status & ~MASK_TRACK) | TRACK_HITALADIN);		      
	    }
	    iloopmax = maytay;
	    	  
	} /* iteration loop */
      
      /* we left the iteration loop */
      
      //if( tr_status & (TARGET_HIT | TRACK_PASSALADIN) ) /* BAD IDEA... */

      if( tr_status & TARGET_HIT )
	{
	  restore_position( &my_back, &my_in );
	  /* we need momentum components px,py,pz for tracking */
	  my_in.mom[1] = - my_in.smom * sin( dphi_ini * DEG2RAD );                         /* py */
	  momxz      = sqrt( my_in.smom * my_in.smom - my_in.mom[1] * my_in.mom[1] );     /* projection in x-z */
	  my_in.mom[0] = - momxz * sin( dthe_ini * DEG2RAD );                          /* px */ 
	  my_in.mom[2] = - sqrt( momxz * momxz - my_in.mom[0] * my_in.mom[0] );         /* pz */
	  
	  /* Run once more through the final track to get valid path length */
	  //if( i_nevent < 100 ) 
	  //  {
	  //    RungeKutta( in, field, debug_flag, 10 );
	  //  }
	  //else
	  //  {
	  RungeKutta( &my_in, field, debug_flag, 0 );
	  //  }
	  
	  if( debug_flag > 0 ) {
	    printf("######################################################################\n");
	    printf("# Final conditions for track %d:\n", i_nevent);
	    printf("# X     = %11.5f\n", my_in.pos[0]);
	    printf("# Y     = %11.5f\n", my_in.pos[1]);
	    printf("# Z     = %11.5f\n", my_in.pos[2]);
	    printf("# theta = %11.5f\n", atan(my_in.mom[0] / my_in.mom[2])*RAD2DEG);
	    printf("# phi   = %11.5f\n", atan(my_in.mom[1] / my_in.mom[2])*RAD2DEG);
	    printf("# SMOM  = %11.5f\n", my_in.smom);
	    printf("# STOT  = %11.5f\n", my_in.spathfw);
	    printf("# RK4   =  0x%08x\n", status);
	    printf("######################################################################\n");
	    printf("\n\n");
	  }
  
	  /* change to laborarory frame with WT transformation so to have the angle correct*/
 
	  /* Here we must do the physics calculations */
	  /* Check position in target and TOF for valid values */
	  path_forward = my_in.spathfw * 100.;
	  path_backward = my_in.spathbk * 100;
	  
	  xtdiff     = my_in.pos[0] - x_tar;
	  ytdiff     = my_in.pos[1] - y_tar;
	 
	  if( (xtdiff > -1.0) && (xtdiff < 1.0) && (ytdiff > -10.0) && (ytdiff < 10.0)) // && ((tr_status & MASK_ITER) == 1) ) // BUG
	    {
	      /* particle hit the target */
	      
	      /* rigidity of particle */
	      rigidity = 0.;
	      for( i = 0; i < 3; i++ )
		{
		  rigidity = rigidity + my_in.mom[i] * my_in.mom[i];
		}
	      rigidity        = sqrt(rigidity);	      
	      rig_raw         = rigidity / MOMENTUM_FACTOR;
	      rigx = my_in.mom[0];
	      rigy = my_in.mom[1];
	      rigz = my_in.mom[2];
	      track_xt=my_in.pos[0];
	      track_yt=my_in.pos[1];
	      
	      

/*
 * 	cout << " ===============================================================================" << endl;
 * 	cout << my_in.pos[0] << " " << my_in.pos[1] << " " << my_in.pos[2] << endl;
 * 	cout << my_in.mom[0] << " " << my_in.mom[1] << " " << my_in.mom[2] << endl;
 * 	cout << tracklet_z << endl;
 * 	cout << " ===============================================================================" << endl;
 */		
	      track_xtdiff    = xtdiff;
	      track_ytdiff    = ytdiff;
	      IDreconst       = -100;
	      IDloop          = iloopmax;
	      result          = RESULT_TRACK_DONE;
	      tracking_result = RESULT_TRACK_DONE;
	      track_iters     = (tr_status & MASK_ITER);
	    }
	  else
	    {
	      /* particle missed target */
	      /* cout << "Z= " << tracklet_z << " missed target" << endl; */
	      for( i = 0; i < 3; i++ )
		{
		  rigidity = rigidity + my_in.mom[i] * my_in.mom[i];
		}
	      //rigidity        = -999.;
	      rigx = -999.;
	      rigy = -999.;
  	      rigz = -999.;
	      track_xtdiff    = -999.;
	      track_ytdiff    = -999.;
	      trackinc        = 1.;
	      IDreconst       = 10;
	      IDloop          = iloopmax;
	      track_xt        = -999.;
	      track_yt        = -999.;
	      
	      result          = RESULT_TRACK_MISSED;
	      tracking_result = RESULT_TRACK_MISSED;
	      track_iters     = (tr_status & MASK_ITER);
	    }
	}
      else if( tr_status & TRACK_HITALADIN )
	{
	  /* we did run into ALADIN */
 	  /*cout << "Z= " << tracklet_z << " did run into ALADIN " << endl; */
	      for( i = 0; i < 3; i++ )
		{
		  rigidity = rigidity + my_in.mom[i] * my_in.mom[i];
		}
	  //rigidity        = -999.;
	  trackinc        = 2.;
	  IDreconst       = 100;
          IDloop          = iloopmax;
	  
	  rigx = -999.;
	  rigy = -999.;
	  rigz = -999.;
	  track_xtdiff    = -999.;
	  track_ytdiff    = -999.;
          track_xt        = -999.;
	  track_yt        = -999.;
	      
	  
	  result          = RESULT_TRACK_ALADIN;
	  tracking_result = RESULT_TRACK_ALADIN;
	  track_iters     = (tr_status & MASK_ITER);
	}
      else if( ((tr_status & MASK_ITER) == MAX_TRY) )
	{
	  /* we did not converge */
 	  /* cout << "Z= " << tracklet_z << " did not converge" << endl;*/
	      for( i = 0; i < 3; i++ )
		{
		  rigidity = rigidity + my_in.mom[i] * my_in.mom[i];
		}
	  //rigidity        = -999.;
	  trackinc        = 3.;
	  IDreconst       = 1000;
          IDloop          = iloopmax;
	  
	  rigx = -999.;
	  rigy = -999.;
	  rigz = -999.;
	  track_xtdiff    = -999.;
	  track_ytdiff    = -999.;
          track_xt        = -999.;
	  track_yt        = -999.;

	  /*printf("HOHOHO: 0x%08x\n", tr_status);*/
	  
	  result          = RESULT_TRACK_NOCONV;
	  tracking_result = RESULT_TRACK_NOCONV;
	  track_iters     = (tr_status & MASK_ITER);
	}
      
    } /* if() for reasonable track condition */
  else
    {
      /* initial momentum is not reasonable */
      printf("# Error: initial momentum value is out of range (%7.2f | %7.2f | %7.2f)\n",
	     PFIT_LOWER, pfit, PFIT_UPPER);
      rigidity        = -999.;
      trackinc        = 4.;
      IDreconst       = -500;
      IDloop          = iloopmax;
      track_xt        = -999.;
      track_yt        = -999.;

      
      tracking_result = RESULT_TRACK_BAD;
      result          = RESULT_TRACK_BAD;
    }

	/* added 09.01.2008 transform the components of the momentum
	 in the target reference frame */

	  rigx_lab = -rigx*0.9922-rigz*0.1248;
	  rigy_lab = -rigy;
	  rigz_lab = rigx*0.1248-rigz*0.9922;
	 
	 
  // ############################################

  if( i_nevent < 500 ) 
    { 
      //printf("DEBUG: p = %+9.7f Tm\n", rigidity); 
/*
 *       printf("-> %7.5f %7.6f %7.3f (%8.6f %9.6f %9.6f) (%6E %6E) %3E %3E 0x%08x %4d %4d\n", 
 * 	     rigidity, gamma, mass, xtarget, ytarget, ztarget, magnet_theta, magnet_phi,
 * 	     xtdiff, ytdiff,
 * 	     tr_status, track_iters, tracking_result);
 */
    }

  //if( track_iters == 1 )
  //  {
  //    printf("BUGBUGBUG: Z = %f Z = %d xs = %f theta = %f\n", tracklet_z, gotZ, tracklet_xs, tracklet_theta);
  //  }

  return result;
}


/*___________________________________________________________________________*/

//TTB coordinates in ALADiN reference frame 

/* get_field() takes a particle (with position information) and delivers the fields. */
/* It does the necessary coordinate transformations.                                 */
Int_t Tracking::get_field(part *in)
{

//  cout<<"#### TT's evil brother is ALIVE #########"<<endl;

  //  mapper my_mapper1;
  mapper mp1;

  //  mapper my_mapper2;
  mapper mp2;

  Float_t xmap = 0.0;
  Float_t ymap = 0.0;
  Float_t zmap = 0.0;
  Float_t xzw  = 0.0;
  Float_t zzw  = 0.0;

  Float_t pbox1[3] = {0.0, 0.0, 0.0};
  Float_t pbox2[3] = {0.0, 0.0, 0.0};

  Int_t result = 0;
  /*
  mp1 = &my_mapper1;
  mp2 = &my_mapper2;
  */
  /* Coordinate transformations */
  
  /* added 09.01.2008 to account for the B=0 for y>19.6 */
  if(in->pos[1]>19.6 && in->pos[1]<22.2){
  	in->pos[1]=-in->pos[1];
  }
  
  
  xmap =  in->pos[2] * 10.0; /* change coordinate system, use [mm]     */
  ymap =  in->pos[1] * 10.0; /* this coordinate fits already, use [mm] */
  zmap = -in->pos[0] * 10.0; /* change coordinate system, use [mm]     */

  /* Box 1 coordinates */
  pbox1[0] = xmap + geom.off1[0];
  pbox1[1] = ymap + geom.off1[1];
  pbox1[2] = zmap + geom.off1[2];
 
  pbox1[0] = pbox1[0] - geom.box1[0][0];
  pbox1[2] = pbox1[2] - geom.box1[2][0];

  xzw      =  pbox1[0] * cos(-geom.alpha1) + pbox1[2] * sin(-geom.alpha1);
  zzw      = -pbox1[0] * sin(-geom.alpha1) + pbox1[2] * cos(-geom.alpha1);

  pbox1[0] = xzw + geom.box1[0][0];
  pbox1[2] = zzw + geom.box1[2][0];

  /* We have now coordinates in box 1 - need to go to [xx,xy,xz,yy,zz] */
  mp1.pos[0]   = (pbox1[0] + 1400.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xx coordinate */
  mp1.pos[1]   = (pbox1[0] + 1390.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xy coordinate */
  mp1.pos[2]   = (pbox1[0] + 1380.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xz coordinate */
  mp1.pos[3]   = (pbox1[1] +  190.0) * 0.1 / STEP_Y + 1.0 + 2.0; /* y coordinate  */
  mp1.pos[4]   = (pbox1[2] +  660.0) * 0.1 / STEP_Z + 1.0 + 0.0; /* z coordinate  */
  mp1.field[0] = 0.0;
  mp1.field[1] = 0.0;
  mp1.field[2] = 0.0;
  mp1.status   = 0;

  /* Now comes field1_int() */

  /* Box 2 coordinates */
  pbox2[0] = xmap + geom.off2[0];
  pbox2[1] = ymap + geom.off2[1];
  pbox2[2] = zmap + geom.off2[2];
 
  pbox2[0] = pbox2[0] - geom.box2[0][0];
  pbox2[2] = pbox2[2] - geom.box2[2][0];

  xzw      =  pbox2[0] * cos(-geom.alpha2) + pbox2[2] * sin(-geom.alpha2);
  zzw      = -pbox2[0] * sin(-geom.alpha2) + pbox2[2] * cos(-geom.alpha2);

  pbox2[0] = xzw + geom.box2[0][0];
  pbox2[2] = zzw + geom.box2[2][0];

  /* We have now coordinates in box 2 - need to go to [xx,xy,xz,yy,zz] */
  mp2.pos[0]   = (pbox2[0] + 1250.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xx coordinate */
  mp2.pos[1]   = (pbox2[0] + 1240.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xy coordinate */
  mp2.pos[2]   = (pbox2[0] + 1230.0) * 0.1 / STEP_X + 1.0 - 1.0; /* xz coordinate */
  mp2.pos[3]   = (pbox2[1] +  190.0) * 0.1 / STEP_Y + 1.0 + 2.0; /* y coordinate  */
  mp2.pos[4]   = (pbox2[2] -   60.0) * 0.1 / STEP_Z + 1.0 - 1.0; /* z coordinate  */
  mp2.field[0] = 0.0;
  mp2.field[1] = 0.0;
  mp2.field[2] = 0.0;
  mp2.status   = 0;

  /* Now comes field2_int() */

  /*
  printf("# get_field(): [%7.2f|%7.2f|%7.2f] (ALADIN)\n", in->pos[0], in->pos[1], in->pos[2]);
  printf("# get_field(): [%7.2f|%7.2f|%7.2f] (BOX1)\n", pbox1[0], pbox1[1], pbox1[2]);
  printf("# get_field(): [%7.2f|%7.2f|%7.2f] (BOX2)\n", pbox2[0], pbox2[1], pbox2[2]);
  printf("# get_field(): [%7.2f|%7.2f|%7.2f|%7.2f|%7.2f] (RAW1)\n", mp1.pos[0], mp1.pos[1], mp1.pos[2], mp1.pos[3], mp1.pos[4]);
  printf("# get_field(): [%7.2f|%7.2f|%7.2f|%7.2f|%7.2f] (RAW2)\n", mp2.pos[0], mp2.pos[1], mp2.pos[2], mp2.pos[3], mp2.pos[4]);
  */
  get_field1(&mp1);
  get_field2(&mp2);

/*
 *   printf("# get_field(): field1(%d) <-> field2(%d)\n", mp1.status, mp2.status);
 */

  /* Now we do the interpolation between the two measurement boxes, if necessary */
  if( mp1.status == 1)
    {
      /* First box delivered a measured value */
      xzw =  mp1.field[0] * cos(geom.alpha1) + mp1.field[2] * sin(geom.alpha1);
      zzw = -mp1.field[0] * sin(geom.alpha1) + mp1.field[2] * cos(geom.alpha1);
      
      mp1.field[0] = -zzw;
      mp1.field[2] =  xzw;
    }

  if( mp2.status == 1)
    {
      /* First box delivered a measured value */
      xzw =  mp2.field[0] * cos(geom.alpha2) + mp2.field[2] * sin(geom.alpha2);
      zzw = -mp2.field[0] * sin(geom.alpha2) + mp2.field[2] * cos(geom.alpha2);
      
      mp2.field[0] = -zzw;
      mp2.field[2] =  xzw;
    }
  
  if( (mp1.status == 1) && (mp2.status == 1) )
    {
      /* Both boxes delivered measured field values */
      in->field[0] = 10.0 * ((fabs(mp1.field[0]) > fabs(mp2.field[0]) ? mp1.field[0] : mp2.field[0]));
      in->field[1] = 10.0 * ((fabs(mp1.field[1]) > fabs(mp2.field[1]) ? mp1.field[1] : mp2.field[1]));
      in->field[2] = 10.0 * ((fabs(mp1.field[2]) > fabs(mp2.field[2]) ? mp1.field[2] : mp2.field[2]));

      in->field[0] = 5.0 * (mp1.field[0] + mp2.field[0]);
      in->field[1] = 5.0 * (mp1.field[1] + mp2.field[1]);
      in->field[2] = 5.0 * (mp1.field[2] + mp2.field[2]);
   }

  if( (mp1.status == 1) && (mp2.status == 0) )
    {
      /* Only left box delivered measured field values */
      in->field[0] = 10.0 * mp1.field[0];
      in->field[1] = 10.0 * mp1.field[1];
      in->field[2] = 10.0 * mp1.field[2];
    }

  if( (mp1.status == 0) && (mp2.status == 1) )
    {
      /* Only right box delivered measured field values */
      in->field[0] = 10.0 * mp2.field[0];
      in->field[1] = 10.0 * mp2.field[1];
      in->field[2] = 10.0 * mp2.field[2];
    }

  if( (mp1.status == 0) && (mp2.status == 0) )
    {
      /* Only left box delivered measured field values */
      in->field[0] = 0.0;
      in->field[1] = 0.0;
      in->field[2] = 0.0;
    }

  /* added 09.01.2008 to account for the B=0 for y>19.6 */
  if(in->pos[1]<-19.6 && in->pos[1]>-22.2){
  	in->pos[1]=-in->pos[1];
	in->field[0]=-in->field[0];
	in->field[2]=-in->field[2];
  }
  	
  result = 1 * mp1.status + 2 * mp2.status;

  return result;
}

void Tracking::get_field2(mapper *mp2)
{
  Int_t   ix1      = 0;
  Int_t   ix2      = 0;
  Int_t   iy1      = 0;
  Int_t   iy2      = 0;
  Int_t   iz1      = 0;
  Int_t   iz2      = 0;
  Float_t fx       = 0.0;
  Float_t fy       = 0.0;
  Float_t fz       = 0.0;
  Int_t   limx[21] = {91, 82, 73, 64, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 64, 67, 75, 91};
  Float_t g[6]     = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  /* Calculate for xx position */
  ix1 = (Int_t)(mp2->pos[0]);
  if( mp2->pos[0] < 0 ) { ix1--; }
  ix2 = ix1 + 1;

  iy1 = (Int_t)(mp2->pos[3]);
  if( mp2->pos[3] < 0 ) { iy1--; }
  iy2 = iy1 + 1;

  iz1 = (Int_t)(mp2->pos[4]);
  if( mp2->pos[4] < 0 ) { iz1--; }
  iz2 = iz1 + 1;

/*
 *   printf("# get_field2(): xx: (%d|%7.2f|%d) (%d|%7.2f|%d) (%d|%7.2f|%d) \n", 
 * 	 ix1, mp2->pos[0], ix2, iy1, mp2->pos[3], iy2, iz1, mp2->pos[4], iz2);
 */

  if( (ix2 > fm.ri-1) || (ix1 < 0) || (iz2 > fm.rk-1) || (iz1 < 0) ) 
    {
      /* Out of x and z boundaries */
      mp2->field[0] = 0.0;
      mp2->field[1] = 0.0;
      mp2->field[2] = 0.0;
      mp2->status   = 0;
      return;
    }

  if( (iy2 > fm.rj-1) || (iy2 < 0) )
    {
      /* at least one y delimiter is out of field map */
      if( (iy1 >= -3 + 2) && (iy2 <= 17 + 2) ) /* -3:17 */
	{
	  /* our point is just on the border line */
	  if( (ix1-1 < limx[iy1-2 + 3]) || (ix1-1 < limx[iy2-2 + 3]) )
	    {
	      if( (ix1-1 < limx[iy1-2 + 3]) && (ix1-1 >= limx[iy1-2 + 1 + 3]) )
		{
		  iy1++;
		  iy2++;
		}
	      else if( (ix1-1 < limx[iy2-2 + 3]) && (ix1-1 >= limx[iy2-2 - 1 + 3]) )
		{
		  iy1--;
		  iy2--;
		}
	      else
		{
		  mp2->field[0] = 0.0;
		  mp2->field[1] = 0.0;
		  mp2->field[2] = 0.0;
		  mp2->status   = 0;
		  return;
		}
	    }
	}
      else
	{
	  mp2->field[0] = 0.0;
	  mp2->field[1] = 0.0;
	  mp2->field[2] = 0.0;
	  mp2->status   = 0;
	  return;
	}
    }

  fx = mp2->pos[0] - ix1;
  fy = mp2->pos[3] - iy1;
  fz = mp2->pos[4] - iz1;

  if( (ix1<0) || (ix1>fm.ri-1) || (ix2<0) || (ix2>fm.ri-1) ||
      (iy1<0) || (iy1>fm.rj-1) || (iy2<0) || (iy2>fm.rj-1) || 
      (iz1<0) || (iz1>fm.rk-1) || (iz2<0) || (iz2>fm.rk-1) )
    {
      //      printf("# RMAPX ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
      //	     ix1, fm.ri-1, ix2, iy1, fm.rj-1, iy2, iz1, fm.rk-1, iz2 );
    }

  g[0] = fm.rbx[ix1][iy1][iz1] + fx * (fm.rbx[ix2][iy1][iz1] - fm.rbx[ix1][iy1][iz1]);
  g[1] = fm.rbx[ix1][iy2][iz1] + fx * (fm.rbx[ix2][iy2][iz1] - fm.rbx[ix1][iy2][iz1]);
  g[2] = fm.rbx[ix1][iy2][iz2] + fx * (fm.rbx[ix2][iy2][iz2] - fm.rbx[ix1][iy2][iz2]);
  g[3] = fm.rbx[ix1][iy1][iz2] + fx * (fm.rbx[ix2][iy1][iz2] - fm.rbx[ix1][iy1][iz2]);
  g[4] = g[0] + fy * (g[1] - g[0]);
  g[5] = g[3] + fy * (g[2] - g[3]);

  mp2->field[0] = g[4] + fz * (g[5] - g[4]);

  /* xx part done */

  /* Calculate for xy position */
  ix1 = (Int_t)(mp2->pos[1]);
  if( mp2->pos[1] < 0 ) { ix1--; }
  ix2 = ix1 + 1;

  if( (ix2 > fm.ri-1) || (ix1 < 0) ) 
    {
      mp2->field[0] = 0.0;
      mp2->field[1] = 0.0;
      mp2->field[2] = 0.0;
      mp2->status   = 0;
      return;
    }

  if( (iy1 >= -3 + 2) && (iy2 <= 17 + 2) )
    {
      if( (ix1-1 < limx[iy1-2 + 3]) || (ix1-1 < limx[iy2-2 + 3]) )
	{
	  if( (ix1-1 < limx[iy1-2 + 3]) && (ix1-1 >= limx[iy1-2 + 1 + 3]) )
	    {
	      iy1++;
	      iy2++;
	    }
	  else if( (ix1-1 < limx[iy2-2 + 3]) && (ix1-1 >= limx[iy2-2 - 1 + 3]) )
	    {
	      iy1--;
	      iy2--;
	    }
	  else
	    {
	      mp2->field[0] = 0.0;
	      mp2->field[1] = 0.0;
	      mp2->field[2] = 0.0;
	      mp2->status   = 0;
	      return;
	    }
	}
    }
  else
    {
      mp2->field[0] = 0.0;
      mp2->field[1] = 0.0;
      mp2->field[2] = 0.0;
      mp2->status   = 0;
      return;
    }

  fx = mp2->pos[1] - ix1;

  if( (ix1<0) || (ix1>fm.ri-1) || (ix2<0) || (ix2>fm.ri-1) ||
      (iy1<0) || (iy1>fm.rj-1) || (iy2<0) || (iy2>fm.rj-1) || 
      (iz1<0) || (iz1>fm.rk-1) || (iz2<0) || (iz2>fm.rk-1) )
    {
/*
 *       printf("# RMAPY ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
 * 	     ix1, fm.ri-1, ix2, iy1, fm.rj-1, iy2, iz1, fm.rk-1, iz2 );
 */
    }

  g[0] = fm.rby[ix1][iy1][iz1] + fx * (fm.rby[ix2][iy1][iz1] - fm.rby[ix1][iy1][iz1]);
  g[1] = fm.rby[ix1][iy2][iz1] + fx * (fm.rby[ix2][iy2][iz1] - fm.rby[ix1][iy2][iz1]);
  g[2] = fm.rby[ix1][iy2][iz2] + fx * (fm.rby[ix2][iy2][iz2] - fm.rby[ix1][iy2][iz2]);
  g[3] = fm.rby[ix1][iy1][iz2] + fx * (fm.rby[ix2][iy1][iz2] - fm.rby[ix1][iy1][iz2]);
  g[4] = g[0] + fy * (g[1] - g[0]);
  g[5] = g[3] + fy * (g[2] - g[3]);

  mp2->field[1] = g[4] + fz * (g[5] - g[4]);

  /* xy part done */

  /* Calculate for xz position */
  ix1 = (Int_t)(mp2->pos[2]);
  if( mp2->pos[2] < 0 ) { ix1--; }
  ix2 = ix1 + 1;

  if( (ix2 > fm.ri-1) || (ix1 < 0) ) 
    {
      mp2->field[0] = 0.0;
      mp2->field[1] = 0.0;
      mp2->field[2] = 0.0;
      mp2->status   = 0;
      return;
    }

  if( (iy1 >= -3 + 2) && (iy2 <= 17 + 2) )
    {
      if( (ix1-1 < limx[iy1-2 + 3]) || (ix1-1 < limx[iy2-2 + 3]) )
	{
	  if( (ix1-1 < limx[iy1-2 + 3]) && (ix1-1 >= limx[iy1-2 + 1 + 3]) )
	    {
	      iy1++;
	      iy2++;
	    }
	  else if( (ix1-1 < limx[iy2-2 + 3]) && (ix1-1 >= limx[iy2-2 - 1 + 3]) )
	    {
	      iy1--;
	      iy2--;
	    }
	  else
	    {
	      mp2->field[0] = 0.0;
	      mp2->field[1] = 0.0;
	      mp2->field[2] = 0.0;
	      mp2->status   = 0;
	      return;
	    }
	}
    }
  else
    {
      mp2->field[0] = 0.0;
      mp2->field[1] = 0.0;
      mp2->field[2] = 0.0;
      mp2->status   = 0;
      return;
    }

  fx = mp2->pos[2] - ix1;

  if( (ix1<0) || (ix1>fm.ri-1) || (ix2<0) || (ix2>fm.ri-1) ||
      (iy1<0) || (iy1>fm.rj-1) || (iy2<0) || (iy2>fm.rj-1) || 
      (iz1<0) || (iz1>fm.rk-1) || (iz2<0) || (iz2>fm.rk-1) )
    {
      //      printf("# RMAPZ ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
      //	     ix1, fm.ri-1, ix2, iy1, fm.rj-1, iy2, iz1, fm.rk-1, iz2 );
    }
  g[0] = fm.rbz[ix1][iy1][iz1] + fx * (fm.rbz[ix2][iy1][iz1] - fm.rbz[ix1][iy1][iz1]);
  g[1] = fm.rbz[ix1][iy2][iz1] + fx * (fm.rbz[ix2][iy2][iz1] - fm.rbz[ix1][iy2][iz1]);
  g[2] = fm.rbz[ix1][iy2][iz2] + fx * (fm.rbz[ix2][iy2][iz2] - fm.rbz[ix1][iy2][iz2]);
  g[3] = fm.rbz[ix1][iy1][iz2] + fx * (fm.rbz[ix2][iy1][iz2] - fm.rbz[ix1][iy1][iz2]);
  g[4] = g[0] + fy * (g[1] - g[0]);
  g[5] = g[3] + fy * (g[2] - g[3]);

  mp2->field[2] = g[4] + fz * (g[5] - g[4]);

  mp2->status = 1;

  /* xz part done */

  return;
}

void Tracking::get_field1(mapper *mp1)
{
  Int_t   ix1      = 0;
  Int_t   ix2      = 0;
  Int_t   iy1      = 0;
  Int_t   iy2      = 0;
  Int_t   iz1      = 0;
  Int_t   iz2      = 0;
  Int_t   store1   = 0;
  Int_t   store2   = 0;
  Float_t fx       = 0.0;
  Float_t fy       = 0.0;
  Float_t fz       = 0.0;
  Int_t   limx[21] = {91, 86, 77, 69, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 69, 77, 86, 91};
  Float_t g[6]     = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

  /* Calculate for xx position */
  ix1 = (Int_t)(mp1->pos[0]);
  if( mp1->pos[0] < 0 ) { ix1--; }
  ix2 = ix1 + 1;

  iy1 = (Int_t)(mp1->pos[3]);
  if( mp1->pos[3] < 0 ) { iy1--; }
  iy2 = iy1 + 1;

  iz1 = (Int_t)(mp1->pos[4]);
  if( mp1->pos[4] < 0 ) { iz1--; }
  iz2 = iz1 + 1;

  store1 = iy1;
  store2 = iy2;

  if( (ix2 > fm.li-1) || (ix1 < 0) || (iz2 > fm.lk-1) || (iz1 < 0) ) 
    {
      /* Out of x and z boundaries */
      mp1->field[0] = 0.0;
      mp1->field[1] = 0.0;
      mp1->field[2] = 0.0;
      mp1->status   = 0;
      return;
    }

  if( (iy2 > fm.lj-1) || (iy2 < 0) )
    {
      /* at least one y delimiter is out of field map */
      if( (iy1 >= -3 + 2) && (iy2 <= 17 + 2) ) /* -3:17 */
	{
	  /* our point is just on the border line */
	  if( (ix1-1 < limx[iy1-2 + 3]) || (ix1-1 < limx[iy2-2 + 3]) )
	    {
	      if( (ix1-1 < limx[iy1-2 + 3]) && (ix1-1 >= limx[iy1-2 + 1 + 3]) )
		{
		  iy1++;
		  iy2++;
		}
	      else if( (ix1-1 < limx[iy2-2 + 3]) && (ix1-1 >= limx[iy2-2 - 1 + 3]) )
		{
		  iy1--;
		  iy2--;
		}
	      else
		{
		  mp1->field[0] = 0.0;
		  mp1->field[1] = 0.0;
		  mp1->field[2] = 0.0;
		  mp1->status   = 0;
		  return;
		}
	    }
	}
      else
	{
	  mp1->field[0] = 0.0;
	  mp1->field[1] = 0.0;
	  mp1->field[2] = 0.0;
	  mp1->status   = 0;
	  return;
	}
    }

  fx = mp1->pos[0] - ix1;
  fy = mp1->pos[3] - iy1;
  fz = mp1->pos[4] - iz1;

  if( (ix1<0) || (ix1>fm.li-1) || (ix2<0) || (ix2>fm.li-1) ||
      (iy1<0) || (iy1>fm.lj-1) || (iy2<0) || (iy2>fm.lj-1) || 
      (iz1<0) || (iz1>fm.lk-1) || (iz2<0) || (iz2>fm.lk-1) )
    {
      //      printf("# LMAPX ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
      //	     ix1, fm.li-1, ix2, iy1, fm.lj-1, iy2, iz1, fm.lk-1, iz2 );
    }

  g[0] = fm.lbx[ix1][iy1][iz1] + fx * (fm.lbx[ix2][iy1][iz1] - fm.lbx[ix1][iy1][iz1]);
  g[1] = fm.lbx[ix1][iy2][iz1] + fx * (fm.lbx[ix2][iy2][iz1] - fm.lbx[ix1][iy2][iz1]);
  g[2] = fm.lbx[ix1][iy2][iz2] + fx * (fm.lbx[ix2][iy2][iz2] - fm.lbx[ix1][iy2][iz2]);
  g[3] = fm.lbx[ix1][iy1][iz2] + fx * (fm.lbx[ix2][iy1][iz2] - fm.lbx[ix1][iy1][iz2]);
  g[4] = g[0] + fy * (g[1] - g[0]);
  g[5] = g[3] + fy * (g[2] - g[3]);

  mp1->field[0] = g[4] + fz * (g[5] - g[4]);

  /* xx part done */

  /* Calculate for xy position */
  iy1 = store1;
  iy2 = store2;

  ix1 = (Int_t)(mp1->pos[1]);
  if( mp1->pos[1] < 0 ) { ix1--; }
  ix2 = ix1 + 1;

  if( (ix2 > fm.li-1) || (ix1 < 0) ) 
    {
      mp1->field[0] = 0.0;
      mp1->field[1] = 0.0;
      mp1->field[2] = 0.0;
      mp1->status   = 0;
      return;
    }

  if( (iy1 >= -3 + 2) && (iy2 <= 17 + 2) )
    {
      if( (ix1-1 < limx[iy1-2 + 3]) || (ix1-1 < limx[iy2-2 + 3]) )
	{
	  if( (ix1-1 < limx[iy1-2 + 3]) && (ix1-1 >= limx[iy1-2 + 1 + 3]) )
	    {
	      iy1++;
	      iy2++;
	    }
	  else if( (ix1-1 < limx[iy2-2 + 3]) && (ix1-1 >= limx[iy2 -2 - 1 + 3]) )
	    {
	      iy1--;
	      iy2--;
	    }
	  else
	    {
	      mp1->field[0] = 0.0;
	      mp1->field[1] = 0.0;
	      mp1->field[2] = 0.0;
	      mp1->status   = 0;
	      return;
	    }
	}
    }
  else
    {
      mp1->field[0] = 0.0;
      mp1->field[1] = 0.0;
      mp1->field[2] = 0.0;
      mp1->status   = 0;
      return;
    }

  fx = mp1->pos[1] - ix1;

  if( (ix1<0) || (ix1>fm.li-1) || (ix2<0) || (ix2>fm.li-1) ||
      (iy1<0) || (iy1>fm.lj-1) || (iy2<0) || (iy2>fm.lj-1) || 
      (iz1<0) || (iz1>fm.lk-1) || (iz2<0) || (iz2>fm.lk-1) )
    {
/*
 *       printf("# LMAPY ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
 * 	     ix1, fm.li-1, ix2, iy1, fm.lj-1, iy2, iz1, fm.lk-1, iz2 );
 */
    }

  g[0] = fm.lby[ix1][iy1][iz1] + fx * (fm.lby[ix2][iy1][iz1] - fm.lby[ix1][iy1][iz1]);
  g[1] = fm.lby[ix1][iy2][iz1] + fx * (fm.lby[ix2][iy2][iz1] - fm.lby[ix1][iy2][iz1]);
  g[2] = fm.lby[ix1][iy2][iz2] + fx * (fm.lby[ix2][iy2][iz2] - fm.lby[ix1][iy2][iz2]);
  g[3] = fm.lby[ix1][iy1][iz2] + fx * (fm.lby[ix2][iy1][iz2] - fm.lby[ix1][iy1][iz2]);
  g[4] = g[0] + fy * (g[1] - g[0]);
  g[5] = g[3] + fy * (g[2] - g[3]);

  mp1->field[1] = g[4] + fz * (g[5] - g[4]);

  /* xy part done */

  /* Calculate for xz position */
  iy1 = store1;
  iy2 = store2;

  ix1 = (Int_t)(mp1->pos[2]);
  if( mp1->pos[2] < 0 ) { ix1--; }
  ix2 = ix1 + 1;

  if( (ix2 > fm.li-1) || (ix1 < 0) ) 
    {
      mp1->field[0] = 0.0;
      mp1->field[1] = 0.0;
      mp1->field[2] = 0.0;
      mp1->status   = 0;
      return;
    }

  if( (iy1 >= -3 + 2) && (iy2 <= 17 + 2) )
    {
      if( (ix1-1 < limx[iy1-2 + 3]) || (ix1-1 < limx[iy2-2 + 3]) )
	{
	  if( (ix1-1 < limx[iy1-2 + 3]) && (ix1-1 >= limx[iy1-2 + 1 + 3]) )
	    {
	      iy1++;
	      iy2++;
	    }
	  else if( (ix1-1 < limx[iy2-2 + 3]) && (ix1-1 >= limx[iy2 -2- 1 + 3]) )
	    {
	      iy1--;
	      iy2--;
	    }
	  else
	    {
	      mp1->field[0] = 0.0;
	      mp1->field[1] = 0.0;
	      mp1->field[2] = 0.0;
	      mp1->status   = 0;
	      return;
	    }
	}
    }
  else
    {
      mp1->field[0] = 0.0;
      mp1->field[1] = 0.0;
      mp1->field[2] = 0.0;
      mp1->status   = 0;
      return;
    }

  fx = mp1->pos[2] - ix1;

  if( (ix1<0) || (ix1>fm.li-1) || (ix2<0) || (ix2>fm.li-1) ||
      (iy1<0) || (iy1>fm.lj-1) || (iy2<0) || (iy2>fm.lj-1) || 
      (iz1<0) || (iz1>fm.lk-1) || (iz2<0) || (iz2>fm.lk-1) )
    {
/*
 *       printf("# LMAPZ ARGL: [%2d (%2d) %2d] [%2d (%2d) %2d] [%2d (%2d) %2d]\n",
 * 	     ix1, fm.li-1, ix2, iy1, fm.lj-1, iy2, iz1, fm.lk-1, iz2 );
 */
    }

  g[0] = fm.lbz[ix1][iy1][iz1] + fx * (fm.lbz[ix2][iy1][iz1] - fm.lbz[ix1][iy1][iz1]);
  g[1] = fm.lbz[ix1][iy2][iz1] + fx * (fm.lbz[ix2][iy2][iz1] - fm.lbz[ix1][iy2][iz1]);
  g[2] = fm.lbz[ix1][iy2][iz2] + fx * (fm.lbz[ix2][iy2][iz2] - fm.lbz[ix1][iy2][iz2]);
  g[3] = fm.lbz[ix1][iy1][iz2] + fx * (fm.lbz[ix2][iy1][iz2] - fm.lbz[ix1][iy1][iz2]);
  g[4] = g[0] + fy * (g[1] - g[0]);
  g[5] = g[3] + fy * (g[2] - g[3]);

  mp1->field[2] = g[4] + fz * (g[5] - g[4]);

  mp1->status = 1;

  /* xz part done */

  return;
}



/* Fill the bx by bz fields from mapping files, do interpolation between different */
/* current settings if needed.                                                     */
/* Bug found: for the "right" box, k index was not corrected by -1                 */
Int_t Tracking::fill_field(Int_t current, TString dir)
{
  Int_t   match      = 0;
  char    filel[80];
  char    fileh[80];
  FILE    *infilel   = NULL;
  FILE    *infileh   = NULL;
  char    linel[255];
  char    lineh[255];
  Int_t   result     = 0;
  Int_t   i          = 0;

  /* File I/O variables */
  Int_t   fm_current[NUM_MAP_FILES] = {0,500,1100,1300,1500,1700,1900,2100,2300,2500};
  Int_t   fm_match        = 0;
  Int_t   fm_low          = 0;
  Int_t   fm_high         = 0;


  /* Variables read by file (low field) */
  Int_t   rdl_current = 0;
  Int_t   rdl_box     = 0;
  Int_t   rdl_i       = 0;
  Int_t   rdl_j       = 0;
  Int_t   rdl_k       = 0;
  Float_t rdl_bx      = 0.;
  Float_t rdl_by      = 0.;
  Float_t rdl_bz      = 0.;
  Float_t rdl_b       = 0.;
  /* Variables read by file (high field) */
  Int_t   rdh_current = 0;
  Int_t   rdh_box     = 0;
  Int_t   rdh_i       = 0;
  Int_t   rdh_j       = 0;
  Int_t   rdh_k       = 0;
  Float_t rdh_bx      = 0.;
  Float_t rdh_by      = 0.;
  Float_t rdh_bz      = 0.;
  Float_t rdh_b       = 0.;

  //cout << "DEBUG: We are in fill_field()." << endl;

  /* Check for reasonable setting of magnet current */
  if( (current < ALADIN_I_MIN) || (current > ALADIN_I_MAX) )
    {
      /* Bad... current is out of range */
      printf("# fill_field(): I_Aladin (%4d) is out of range [%d:%d]\n", current, ALADIN_I_MIN, ALADIN_I_MAX);
      result = -10;
      return result;
    }

  /* We are in range, so we can go on */
  printf("# fill_field(): I_Aladin (%4d) is in range [%d:%d].\n", current, ALADIN_I_MIN, ALADIN_I_MAX);
  
  /* We check for an existing field map */
  fm_match = -1;
  for( i = 0; i < NUM_MAP_FILES; i++) {
    if( current == fm_current[i] ) { fm_match = i; }
    if( fm_current[i] < current ) { fm_low = i; fm_high = i+1; }
  }
  
  printf("# match: %2d low: %2d high: %2d\n", fm_match, fm_low, fm_high);
  if( fm_match != -1 ) {
    /* We have found a matching measured field map */
    printf("# Matching field map found: %4d (%4d).\n", fm_current[fm_match], current);
    fm_low  = fm_current[fm_match];
    fm_high = fm_current[fm_match];
      
    sprintf(filel, "%s/fieldmap/ala_%04d.dat", dir.Data(), fm_low);
    sprintf(fileh, "%s/fieldmap/ala_%04d.dat", dir.Data(), fm_high);
    printf("# fill_field(): low  -> %s\n", filel);
    printf("# fill_field(): high -> %s\n", fileh);
      
    /* We have a matching file */
    match = 1;
    printf("# fill_field(): matching mapping available.\n");
      
    infilel = fopen(filel, "r");
    if( infilel == NULL ) {
      /* We couldn't open the input file */
      printf("# ERROR: fill_field() couldn't open %s.\n", filel);
      result = -10;
      return result;
    } else {
      /* We go ahead - read the stuff */
      while( (fgets(linel, 256, infilel) != NULL) ) { 
	/* We could read one line */
	if( linel[0] != '#' ) {
	  /* We have a normal data line, not a comment line */
	  //		  if( sscanf(linel, "%d %d %d %d %d %f %f %f %f", 
	  if( sscanf(linel, "%d %d %d %d %d %e %e %e %e", 
		     &rdl_current, &rdl_box, &rdl_k, &rdl_j, &rdl_i, &rdl_bx, &rdl_by, &rdl_bz, &rdl_b) == 9 ) {
	    /* Good input lines */
		      
	    /* Now sort it into the field map */
	    if( rdl_box == 0 ){
	      if( (rdl_i-1<0) || (rdl_i-1>LBI-1) || (rdl_j+2<0) || (rdl_j+2>LBJ-1) || (rdl_k<0) || (rdl_k>LBK-1)){
	      } else {
	      /* Left measurement box */
	      fm.lbx[rdl_i - 1][rdl_j + 2][rdl_k] = rdl_bx;
	      fm.lby[rdl_i - 1][rdl_j + 2][rdl_k] = rdl_by;
	      fm.lbz[rdl_i - 1][rdl_j + 2][rdl_k] = rdl_bz;
			  
	    //  cout << rdl_bx << " " << rdl_by<< " " << rdl_bz << endl;
			  				

	      }
	    } else if ( rdl_box == 1 ){
	      if( (rdl_i-1<0) || (rdl_i-1>RBI-1) || (rdl_j+2<0) || (rdl_j+2>RBJ-1) || (rdl_k-1<0) || (rdl_k-1>RBK-1)) {
	      } {
	      /* Right measurement box */
	      fm.rbx[rdl_i - 1][rdl_j + 2][rdl_k - 1] = rdl_bx; /* I'm not sure about the "-1" term here */
	      fm.rby[rdl_i - 1][rdl_j + 2][rdl_k - 1] = rdl_by;
	      fm.rbz[rdl_i - 1][rdl_j + 2][rdl_k - 1] = rdl_bz;
	  //    cout << rdl_bx << " " << rdl_by<< " " << rdl_bz << endl;


	      }
	    } else {
	      /* Error: non-existing measurement box */
	      printf("# Errror: fill_field(): non-existing box to fill.\n");
	      result = -1;
	    }
	  } else {
	    /* Bad input line */
	    printf("# ERROR: fill_field(): bad input line.\n");
	    result = -1;
	  }
	}
      } /* end of read loop */
    } /* end of non-interpolated case */
  }  else {
    /* We must interpolate between two currents */
    printf("# Interpolation between %4d (%4d) %4d.\n", fm_current[fm_low], current, fm_current[fm_high]);
    fm_low  = fm_current[fm_low];
    fm_high = fm_current[fm_high];
      
    sprintf(filel, "%s/fieldmap/ala_%04d.dat", dir.Data(), fm_low);
    sprintf(fileh, "%s/fieldmap/ala_%04d.dat", dir.Data(), fm_high);
    printf("# fill_field(): low  -> %s\n", filel);
    printf("# fill_field(): high -> %s\n", fileh);
      
    /* We must interpolate between two current settings */
    match = 0;
    printf("# fill_field(): interpolation necessary.\n");
      
    infilel = fopen(filel, "r");
    infileh = fopen(fileh, "r");
    if( (infilel == NULL) || (infileh == NULL) ) {
      /* We couldn't open the input file */
      printf("# ERROR: fill_field() couldn't open %s or %s.\n", filel, fileh);
      result = -10;
      return result;
    } else {
      /* We go ahead - read the stuff */
	  
      /* ################## */
	  
      while( (fgets(linel, 256, infilel) != NULL) && (fgets(lineh, 256, infileh) != NULL) ) { 
	/* We could read one line */
	if( (linel[0] != '#') && (lineh[0] != '#') ) {
	  /* We have a normal data line, not a comment line */
	  //		  if( (sscanf(linel, "%d %d %d %d %d %f %f %f %f", 
	  if( (sscanf(linel, "%d %d %d %d %d %g %g %g %g", 
		      &rdl_current, &rdl_box, &rdl_k, &rdl_j, &rdl_i, &rdl_bx, &rdl_by, &rdl_bz, &rdl_b) == 9) &&
		  //		      (sscanf(lineh, "%d %d %d %d %d %f %f %f %f", 
	      (sscanf(lineh, "%d %d %d %d %d %g %g %g %g", 
		      &rdh_current, &rdh_box, &rdh_k, &rdh_j, &rdh_i, &rdh_bx, &rdh_by, &rdh_bz, &rdh_b) == 9)
	      )
	    {
	      /* Good input line */
	      if( (rdl_box != rdh_box) || (rdl_i != rdh_i) || (rdl_j != rdh_j) || (rdl_k != rdh_k) )
		{
		  /*
		   * 			  printf("# WARNING! (%d|%d) (%d|%d) (%d|%d) (%d|%d)\n",
		   * 				 rdl_box, rdh_box, rdl_i, rdh_i, rdl_j, rdh_j, rdl_k, rdh_k);
		   * 			  printf("# low : %s", linel);
		   * 			  printf("# high: %s", lineh);
		   */
		    }
	      
	      if( rdl_box == 0 ) {
		
		if( (rdl_i-1<0) || (rdl_i-1>LBI-1) || (rdl_j+2<0) || (rdl_j+2>LBJ-1) || (rdl_k<0) || (rdl_k>LBK-1))
		  {
		    printf("# ARGL! IL %d (%d) %d (%d) %d (%d)\n", rdl_i-1, LBI-1, rdl_j+2, LBJ-1, rdl_k, LBK-1);
		  } else {
		  /* Left measurement box */
		  fm.lbx[rdl_i - 1][rdl_j + 2][rdl_k] = rdl_bx + (current - fm_low) * (rdh_bx - rdl_bx) / (fm_high - fm_low);
		  fm.lby[rdl_i - 1][rdl_j + 2][rdl_k] = rdl_by + (current - fm_low) * (rdh_by - rdl_by) / (fm_high - fm_low);
		  fm.lbz[rdl_i - 1][rdl_j + 2][rdl_k] = rdl_bz + (current - fm_low) * (rdh_bz - rdl_bz) / (fm_high - fm_low);
		  
		}
		
		  
	      } else if ( rdl_box == 1 ) {
		/* Right measurement box */
		if( (rdl_i-1<0) || (rdl_i-1>RBI-1) || (rdl_j+2<0) || (rdl_j+2>RBJ-1) || (rdl_k-1<0) || (rdl_k-1>RBK-1))
		  {
		    
		    printf("# ARGL! IR %d (%d) %d (%d) %d (%d)\n", rdl_i-1, RBI-1, rdl_j+2, RBJ-1, rdl_k-1, RBK-1);
		    
		  } else {

		  fm.rbx[rdl_i - 1][rdl_j + 2][rdl_k - 1] = rdl_bx + (current - fm_low) * (rdh_bx - rdl_bx) / (fm_high - fm_low);
		  fm.rby[rdl_i - 1][rdl_j + 2][rdl_k - 1] = rdl_by + (current - fm_low) * (rdh_by - rdl_by) / (fm_high - fm_low);
		  fm.rbz[rdl_i - 1][rdl_j + 2][rdl_k - 1] = rdl_bz + (current - fm_low) * (rdh_bz - rdl_bz) / (fm_high - fm_low);

		}
		
	      } else {
		/* Error: non-existing measurement box */
		printf("# Error: fill_field(): non-existing box to fill.\n");
		result = -1;
	      }
	      
	    }
	  else
	    {
	      /* Bad input line */
	      printf("# ERROR: fill_field(): bad input line.\n");
	      result = -1;
		}
	}
      } /* end of read loop */
    } /* end of interpolated area */
  }

  /* Clean up */
  if( infilel != NULL ) { fclose(infilel); }
  if( infileh != NULL ) { fclose(infileh); }

  if( result == 0 ) 
    { 
      fm.lstatus = 1; 
      fm.rstatus = 1;
      fm.li      = LBI;
      fm.lj      = LBJ;
      fm.lk      = LBK;
      fm.ri      = RBI;
      fm.rj      = RBJ;
      fm.rk      = RBK;
    }
  else
    { 
      fm.lstatus = -1; 
      fm.rstatus = -1;
      fm.li      = -1;
      fm.lj      = -1;
      fm.lk      = -1;
      fm.ri      = -1;
      fm.rj      = -1;
      fm.rk      = -1;
    }

  return result;
}

/* Clear the field map structure */
void Tracking::init_fieldmap()
{
  Int_t i = 0;
  Int_t j = 0;
  Int_t k = 0;

  //cout << "DEBUG: We are in init_fieldmap()." << endl;
  /* The monstrous field map */
  
  fm.li = LBI;
  fm.lj = LBJ;
  fm.lk = LBK;
  fm.lstatus = 0;
  fm.ri = RBI;
  fm.rj = RBJ;
  fm.rk = RBK;
  fm.lstatus = 0;

  /* Clear the field maps */
  for( i = 0; i < fm.li; i++ )
    {
      for( j = 0; j < fm.lj; j++ )
	{
	  for( k = 0; k < fm.lk; k++ )
	    {
	      fm.lbx[i][j][k] = 0.0;
	      fm.lby[i][j][k] = 0.0;
	      fm.lbz[i][j][k] = 0.0;
	    }
	}
    }

  for( i = 0; i < fm.ri; i++ )
    {
      for( j = 0; j < fm.rj; j++ )
	{
	  for( k = 0; k < fm.rk; k++ )
	    {
	      fm.rbx[i][j][k] = 0.0;
	      fm.rby[i][j][k] = 0.0;
	      fm.rbz[i][j][k] = 0.0;
	    }
	}
    }
  return;

}

/* Precalculates the geometry values for the coordinate transformation between ALADIN */
/* and box reference frames.                                                          */ 
void Tracking::init_geometry()
{
  Float_t cosa = 0.0;
  Float_t v1[2] = {0.0, 0.0};
  Float_t v2[2] = {0.0, 0.0};

  /* The measurement geometry */
  geom.box1[0][0] =  1233.00;
  geom.box1[1][0] =     0.00;
  geom.box1[2][0] =  -100.00;
  geom.box1[0][1] = -1232.24;
  geom.box1[1][1] =     0.00;
  geom.box1[2][1] =  -100.00;

  geom.box2[0][0] =  1233.00;
  geom.box2[1][0] =     0.00;
  geom.box2[2][0] =   100.00;
  geom.box2[0][1] = -1231.97;
  geom.box2[1][1] =     0.00;
  geom.box2[2][1] =   100.00;

  geom.mag1[0][0] =  1233.19;
  geom.mag1[1][0] =    -2.61;
  geom.mag1[2][0] =   -94.08;
  geom.mag1[0][1] = -1230.22;
  geom.mag1[1][1] =    -2.61;
  geom.mag1[2][1] =     0.92;

  geom.mag2[0][0] =  1232.59;
  geom.mag2[1][0] =    -2.10;
  geom.mag2[2][0] =   104.11;
  geom.mag2[0][1] = -1230.22;
  geom.mag2[1][1] =    -2.10;
  geom.mag2[2][1] =     0.92;

  geom.off1[0] = geom.box1[0][0] - geom.mag1[0][0];
  geom.off1[1] = geom.box1[1][0] - geom.mag1[1][0];
  geom.off1[2] = geom.box1[2][0] - geom.mag1[2][0];

  geom.off2[0] = geom.box2[0][0] - geom.mag2[0][0];
  geom.off2[1] = geom.box2[1][0] - geom.mag2[1][0];
  geom.off2[2] = geom.box2[2][0] - geom.mag2[2][0];

  /* Box 1 calculations */
  v1[0]   = geom.box1[0][1] - geom.box1[0][0];
  v1[1]   = geom.box1[2][1] - geom.box1[2][0];
  
  v2[0]   = geom.mag1[0][1] + geom.off1[0] - geom.box1[0][0];
  v2[1]   = geom.mag1[2][1] + geom.off1[2] - geom.box1[2][0];

  cosa  = v1[0] * v2[0] + v1[1] * v2[1];
  cosa /= sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
  cosa /= sqrt(v2[0] * v2[0] + v2[1] * v2[1]);

  geom.alpha1 = acos(cosa);

  /* Box 2 calculations */
  v1[0]   = geom.box2[0][1] - geom.box2[0][0];
  v1[1]   = geom.box2[2][1] - geom.box2[2][0];
  
  v2[0]   = geom.mag2[0][1] + geom.off2[0] - geom.box2[0][0];
  v2[1]   = geom.mag2[2][1] + geom.off2[2] - geom.box2[2][0];

  cosa  = v1[0] * v2[0] + v1[1] * v2[1];
  cosa /= sqrt(v1[0] * v1[0] + v1[1] * v1[1]);
  cosa /= sqrt(v2[0] * v2[0] + v2[1] * v2[1]);

  geom.alpha2 = -acos(cosa);

  /* added the "-" to make it identical with the fortran 6.12.07 */
  geom.status = 1;

  return;
}

/************************************************************************/
/* Tracks a particle backwards through the magnetic field.              */
/* Either a Wood-Saxon or the ALADIN fieldmap (not implemented yet) is  */
/* being used for the Lorentz equation.                                 */
/*                        in/out                 input      input       */
UInt_t Tracking::RungeKutta( part *in, 
			     Int_t field_flag, Int_t debug_flag, Int_t print_flag ) {
  Double_t dp[3];          /* impuls change in starting point  */
  Double_t dpm1[3];        /* impuls change in first midpoint  */
  Double_t dpm2[3];        /* impuls change in second midpoint */
  Double_t dpe[3];         /* impuls change in end point       */

  Double_t ds        = RK_STEPSIZE;//TTB this is the length with which we do the stepping
  Double_t ds2       = RK_STEPSIZE/2; //TTB half step length for getting the magnetic field in the middle
  
  Int_t    flag_rk2     = 0;     /* 0 => RK4, 1 => RK2               */ // BUGBUGBUG
  Int_t    track_status = 0;     /* set to 1 if particle hits ALADIN */
  Int_t    loop         = 0;
  Int_t    i            = 0;
  Int_t    point        = 0;
  Int_t    flag_fm      = (field_flag == 0 ? 0 : 1) ;     /* 0 => WoodSaxon, 1 => fieldmap    */
  
  bool debug = false;
  part back; //TTB this is for storing of the last particle position to UNDO the last step
  init_part(&back);
  in->spathfw    = 0.;           /* DAMNED */
  in->spathbk    = 0.;           /* DAMNED */
  
  track_status = RK_NEWTRACK; 

  //cout << "DEBUG: We are in RungeKutta()." << endl;

  /* Let's start the tracking. We sum up the total flight path.  */
  /* First tracklet is between MUSIC and TOF.                    */
  distTOF( in, debug_flag );

  /* Second tracklet is between MUSIC and entrance of the ALADIN */
  if( print_flag > 1 )
    {
      point++;
      dump_part( in, 0, point );
      point++;
    }
    
  beforeB( in, debug_flag, print_flag );

  if( print_flag > 1 )
    {
      /* printf("# RK4: before iterations\n"); */
      dump_part( in, 0, point );
      point++;
    }
  
//TTB our loop //////////////////////////
  for( loop = 0; loop < RK_LOOP; loop++) {
    
      /* Leave loop if we hit ALADIN */
      if( (track_status & RK_BADTRACK) || (track_status & RK_GOODTRACK) ) { 
	break; 
      }
      
      track_status = ((track_status & RK_MASKTRACK) | (loop & ~RK_MASKTRACK));

      /* store the position of the particle */
      copy_part( *in , &back );

      if(debug) {
	printf("bk: %f, %f, %f,%e %e %e\n",back.pos[0],back.pos[1],back.pos[2],
	       back.field[0],back.field[1],back.field[2]);
      }
      //tt	cout << in->mom[0] << " " << in->mom[1] << " " <<  
      //tt	in->mom[2] << " " << in->pos[0] << " " << in->pos[1] << " " << in->pos[2] << endl;
	     
      /* get field value at the starting point */
      if( flag_fm == 0 )
	{
	  alatest( in, debug_flag );
	}
      else
	{
	  //	  get_field(fm, in, geom);
	  get_field(in); //TTB get out field from the map!!!!!!!
	}
      

      /* solve the Maxwell equation */
      eqmaxwell( in, debug_flag );

      if(debug) {
	printf("%f, %f, %f,%e %e %e\n",in->pos[0],in->pos[1],in->pos[2],
	       in->field[0],in->field[1],in->field[2]);
      }

      /* proceed to first mid point */
      MovePart( in, ds2, debug_flag );
      get_dp( in, dp );

      if(debug) {      
	printf("%f, %f, %f,%e %e %e\n",in->pos[0],in->pos[1],in->pos[2],
	       in->field[0],in->field[1],in->field[2]);
      }

      if( in->status != 0 ) 
	{			
	  /* Ups. We did hit the magnet. */
	  track_status = ((track_status & ~RK_MASKTRACK) | RK_BADTRACK);
	}
      else
	{
	  /* we didn't crash the magnet :) */
	  
	  /* get field value at first midpoint */
	  if( flag_fm == 0 )
	    {
	      alatest( in, debug_flag );
	    }
	  else
	    {
	      //get_field(fm, in, geom);
	      get_field(in);
	    }

	  if(debug) {
	    printf("getf: %f, %f, %f,%e %e %e\n",in->pos[0],in->pos[1],in->pos[2],
		   in->field[0],in->field[1],in->field[2]);
	  }

	  /* solve Maxwell equations at first midpoint */
	  eqmaxwell( in, debug_flag );
	  get_dp( in, dpm1 );

	  if(debug) {
	    printf("eqma: %f, %f, %f,%e %e %e\n",in->pos[0],in->pos[1],in->pos[2],
		   in->field[0],in->field[1],in->field[2]);
	  }
	  
	  /* restore position of the particle */
	  restore_coords( &back, in );

	  if(debug) {
	    printf("fires: %f, %f, %f,%e %e %e\n",in->pos[0],in->pos[1],in->pos[2],
		   in->field[0],in->field[1],in->field[2]);
	  }

	  /* check if we want second or fourth order Runge Kutta */
	  if( flag_rk2 == 1 )
	    {
	      /* we have a Runge Kutta second order */
	      for( i = 0; i < 3; i++)
		{
		  dp[i] = dpm1[i];
		}
	    }
	  else
	    {
	      /* we do a Runge Kutta 4th order */
	      
	      /* proceed to second midpoint */
	      MovePart( in, ds2, debug_flag );
	      
	      /* get field value at second midpoint */
	      if( flag_fm == 0 )
		{
		  alatest( in, debug_flag );
		}
	      else
		{
		  //		  get_field(fm, in, geom);
		  get_field(in);
		}
	      
      
	      /* solve Maxwell equations at second midpoint */
	      eqmaxwell( in, debug_flag );
	      get_dp( in, dpm2 );
	      
	      /* restore position of particle */
	      restore_coords( &back, in );
	      
	      /* proceed to end point */
	      MovePart( in, ds, debug_flag );
	      
	      /* get field value at endpoint */
	      if( flag_fm == 0 )
		{
		  alatest( in, debug_flag );
		}
	      else
		{
		  //		  get_field(fm, in, geom);
		  get_field(in);
		}
	      
	      /* solve Maxwell equations */
	       
	      eqmaxwell( in, debug_flag );
	      get_dp( in, dpe );
	      
	      /* restore position of particle */
	      restore_coords( &back, in );
	      
	      /* Some calculations */
	      for( i = 0; i < 3; i++ )
		{
		  dp[i] = (dp[i] + dpe[i] + 2.0 * (dpm1[i] + dpm2[i])) / 6.0;
		}
	      
	    }
	  
	  /* Here we join after RK4 additional calculations */

	  /* proceed to endpoint */
	  put_dp( in, dp );

	  MovePart( in, ds, debug_flag );
	  if(debug) {
	    printf("move:: %f, %f, %f,%e %e %e\n",in->pos[0],in->pos[1],in->pos[2],
		   in->field[0],in->field[1],in->field[2]);
	  }

	  //tt	  cout << in->pos[0] << " && " << in->pos[1] << " && " << in->pos[2] << endl;
	  
	  if( debug_flag > 1 )
	    {
	      printf("# inB          : [%7.2f|%7.2f|%7.2f] -> <%6.2f>\n", in->pos[0], in->pos[1], in->pos[2], in->spathfw );
	    }

	  /* accumulate the flight path */
	  in->spathfw += ds;
	  in->spathbk += ds;
	  
	  if( print_flag > 1 )
	    {
	      /* printf("# after iteration\n"); */
	      dump_part( in, 0, point );
	      point++;
	    }
	  
	  if( in->pos[2] < RK_SMIN )
	    {
	      /* Finally. We made it out of the magnet */
	      track_status = ((track_status & ~RK_MASKTRACK) | RK_GOODTRACK);
	      
	      afterB( in, debug_flag, print_flag ); //TTB add distance between target and aladin
	      
	      if( print_flag > 1 )
		{
		  /* printf("# after RK4\n"); */
		  dump_part( in, 0, point );
		  point++;
		  printf("# RK4: ... done.\n");
		}
	    }
	}
    } /* end of loop */
	
  if( debug_flag > 0 ) { printf("# RK4: track_status = 0x%08x\n", track_status); }

  return track_status;

} //end of Runge Kutta method








void Tracking::restore_position( part *in, part *out )
{
  out->pos[0] = in->pos[0];
  out->pos[1] = in->pos[1];
  out->pos[2] = in->pos[2];
}

//TTB set "out" coord and mom to "in" coord and mom
void Tracking::restore_coords( part *in, part *out )
{
  for( Int_t i = 0; i < 3; i++ )
    {
      out->pos[i] = in->pos[i];
      out->mom[i] = in->mom[i];
    }
}

void Tracking::put_dp( part *in, double dp[] )
{
  in->dmom[0] = dp[0];
  in->dmom[1] = dp[1];
  in->dmom[2] = dp[2];
}

void Tracking::get_dp( part *in, double dp[] )
{
  dp[0] = in->dmom[0];
  dp[1] = in->dmom[1];
  dp[2] = in->dmom[2];
}

//TTB: move by distance "ds" (in 100um)

/************************************************************/
/* moves the particle a distance ds, renews pos and mom     */
/*                  in/out       input        input         */
void Tracking::MovePart( part *in, double ds, Int_t debug_flag )
{
  Int_t    i     = 0;
  Double_t pcorr = 0.0;
  Double_t pnorm = 0.0;

  for( i = 0; i < 3; i++ )
    {

      in->pos[i] += (in->mom[i] / in->smom) * ds * 100.0 + 0.5 * (in->dmom[i] / in->smom) * ds * ds * 100.0; //TTB: why only one time factor "100.0"???
      in->mom[i] += in->dmom[i] * ds;	//TTB: is later normalized (but is this the correct relative momentum change in the end???)
    }

  
  if((in->pos[0]< (-0.0795*in->pos[2]-69.5) || 
     in->pos[0]> (0.00795*in->pos[2] + 69.5)) ||
     (in->pos[1] <(-0.0418*in->pos[2]-25.) ||
      in->pos[1] > (0.0418*in->pos[2]+25.)))
    {    
      in->status = 1; //TTB set flag for out of boundaries
    }

  //TTB do momentum normalization
  pnorm = 0.0;
  for( i = 0; i < 3; i++ )
    {
      pnorm += in->mom[i] * in->mom[i];
    }
  pcorr = sqrt(pnorm) / in->smom;

  for( i = 0; i < 3; i++ )
    {
      in->mom[i] /= pcorr;
    }
}

//TTB calculates the change in momentum (direction) due to the field
/*******************************************************/
/* calculates dp from mom and field                    */
/*                    in/out           input           */
void Tracking::eqmaxwell( part *in, Int_t debug_flag )
{
  Int_t i    = 0;
  Int_t iunt = 0;
  Int_t iob  = 0;
	
  for( i = 0; i < 3; i++ )
    {
      iunt = i + 1;
      iob  = i + 2;
      if( iunt > 2 ) 
	{ 
	  iunt -= 3; 
	}
      if( iob > 2 )
	{
	  iob -= 3;
	}

      in->dmom[i] = - (in->mom[iunt] * in->field[iob] - in->mom[iob] * in->field[iunt]) / (in->smom * 10.0);	//TTB calculate force direction, normalized: as fraction of total momentum, why is here again a factor "10"??? smom momentum based on mm not cm???
    }
  if( debug_flag > 10 )
    {
      printf("# eqmaxwell: [%7.2f|%7.2f|%7.2f]\n", in->dmom[0], in->dmom[1], in->dmom[2] );
    }
}

//TTB "in" should be given in frame of target
/****************************************************************/
/* does linear motion behind the magnet                         */
/*             in/out           input             input         */          
void Tracking::afterB( part *in, Int_t debug_flag, Int_t print_flag )
{
  Int_t    i      = 0;
  Double_t dstot  = 0.;
  Double_t pos[3] = {0.,0.,0.};

  if( debug_flag > 1 )
    {
      printf("# afterB input : [%7.2f|%7.2f|%7.2f] -> <%6.2f>\n", in->pos[0], in->pos[1], in->pos[2], in->spathfw );
    }
  
  for( i = 0; i < 3; i++ )
    {
/*
 *       pos[i]      = in->pos[i];
 *       dstot      += pow((in->mom[i] / in->mom[2] * (in->pos[2] + in->target[2]) * 0.01), 2.0 );
 *       in->pos[i]  = in->pos[i] - (in->mom[i] / in->mom[2]) * (in->pos[2] + in->target[2]);
 *       pos[i]      = in->pos[i] - pos[i];
 */
      pos[i]      = in->pos[i];
      dstot      += pow((in->mom[i] / in->mom[2] * (in->pos[2] + ALADIN_TARGET_DIST) * 0.01), 2.0 );
      in->pos[i]  = in->pos[i] - (in->mom[i] / in->mom[2]) * (in->pos[2] + ALADIN_TARGET_DIST);
      pos[i]      = in->pos[i] - pos[i];
    }

  /* Calulate both angles */
  in->theta = atan(pos[0] / pos[2]);//TTB calculate angles at target from position deltas between magnet and target
  in->phi   = atan(pos[1] / pos[2]);

  if( print_flag > 2 )
    {
      /*
      printf("# dx = %7.2f dy = %7.2f dz = %7.2f theta = %7.2f smom = %7.2f\n", 
	     pos[0], pos[1], pos[2], atan(pos[0] / pos[2])*RAD2DEG, in->smom);
      */
      printf("%7.2f %7.2f\n",  atan(pos[0] / pos[2])*RAD2DEG, in->smom); 
    }

  dstot = sqrt(dstot);

  in->spathfw += dstot;
  in->spathbk += dstot;
  
  if( debug_flag > 1 )
    {
      printf("# afterB input : [%7.2f|%7.2f|%7.2f] -> <%6.2f>\n", in->pos[0], in->pos[1], in->pos[2], in->spathfw );
    }
}


//TTB "in" is given ALADIN coordinates  
/***********************************************************************/
/* generates a Wood-Saxon potential, returns field values in r_field[] */
/*                    in/out        input                              */
void Tracking::alatest( part *in, Int_t debug_flag )
{
  in->field[0] = 0.; /* x coordinate */
  in->field[2] = 0.; /* z coordinate */

//  if( (in->pos[2] < -ALADIN_Z_SIZE) || (in->pos[2] > ALADIN_Z_SIZE) )


   if( (in->pos[2] < -100000) || (in->pos[2] > 100000) )
   {
      /* we are already out of ALADIN */
      if( debug_flag > 10 )
	{
	  printf("# alatest: out of ALADIN!\n");
	}
      in->field[1] = 0.;
    }
  else
    {
      /* we are still within ALADIN */
//      in->field[1] = WOOD_SAX_AMP * ( 1.0 / (1.0 + exp((fabs(in->pos[2]) - 50.0) / 10.0)));
       in->field[1] = -9. * ( 1.0 / (1.0 + exp((fabs(in->pos[2]) - 70.0) / 10.0))); //TTB field is shifted wrt the centre of the ALADIN frame!!! why?????
   }
  
  if( debug_flag > 10 )
    {
      printf("# alatest: [%7.2f|%7.2f|%7.2f] -> <%7.2f|%7.2f|%7.2f>\n", 
	     in->pos[0], in->pos[1], in->pos[2], in->field[0], in->field[1], in->field[2] );
    }
}

/************************************************************/
/* copy one particle (backup)                               */
/*                     input              output            */
void Tracking::copy_part( part in, part *out) {

  for( Int_t i = 0; i < 3; i++)
    {
      out->pos[i]   = in.pos[i];
      out->mom[i]   = in.mom[i];
      out->field[i] = in.field[i];
      out->dmom[i]  = in.dmom[i];
      out->tof[i]   = in.tof[i];
    }
  out->spathfw    = in.spathfw;
  out->spathbk    = in.spathbk;
  
  out->smom     = in.smom;
  out->theta    = in.theta;
  out->phi      = in.phi;
  out->status   = in.status;

  return;
}

/*****************************************/
/* initialize particle                   */
/*                     in/out            */
void Tracking::init_part( part *in ) {

  for( Int_t i = 0; i < 3; i++)
    {
      in->pos[i]    = 0.;
      in->mom[i]    = 0.;
      in->target[i] = 0.;
      in->field[i]  = 0.;
      in->dmom[i]   = 0.;
      in->tof[i]    = 0.;
    }
  
  in->spathfw   = 0.;
  in->spathbk   = 0.;
  in->smom    = 0.;
  in->theta   = 0.;
  in->phi     = 0.;
  in->status  = 0;
  return;
}

//TTB in which coordinate system do we put in? MUSIC or aladin? should be ALADiN
/*************************************************************/
/* does linear motion before the magnet                      */
/*              in/out             input                     */
void Tracking::beforeB( part *in, Int_t debug_flag, Int_t print_flag ) //TTB before bending in aladin (backwards in time means from music)
{
  Int_t    i      = 0;
  Double_t dstot  = 0.;
  Double_t pos[3] = {0.,0.,0.};

  if( debug_flag > 1 )
    {
      printf("# beforeB: [%7.2f|%7.2f|%7.2f] -> <%7.2f>\n", in->pos[0], in->pos[1], in->pos[2], in->spathfw );
    }

  for( i = 0; i < 3; i++ )
    {
      pos[i]      = in->pos[i];//TTB backup given coordinates
      in->pos[i]  = in->pos[i] - (in->mom[i] / in->mom[2]) * MUSIC_MAPPER_DIST;
      dstot      += pow(((in->mom[i] / in->mom[2]) * (MUSIC_MAPPER_DIST * 0.01)), 2.0); //TTB why factor 0.01??? cm-> 100um?
      pos[i]      = in->pos[i] - pos[i];//TTB store here deltas of before and after
    }
  dstot = sqrt(dstot);//TTB this is the pathlength in 100um

  if( print_flag > 2 )
    {
      printf("%7.2f %7.2f",  atan(pos[0] / pos[2])*RAD2DEG, in->smom); 
    }
  
  in->spathfw += dstot;//add the pathlength
  in->spathbk += dstot;//add the pathlength
  
  if( debug_flag > 1 )
    {
      printf("# beforeB: [%7.2f|%7.2f|%7.2f] -> <%7.2f>\n", in->pos[0], in->pos[1], in->pos[2], in->spathfw );
    }
}

//TTB in which coordinate system do we put in? MUSIC ? should be MUSIC if no kink to TOF-Wall
/****************************************************************************/
/* calculates the distance to TOF (?)                                       */
/*                        I/O      input                                    */
void Tracking::distTOF( part *in, Int_t debug_flag )
{
  Int_t    i     = 0;
  Double_t dstot = 0.;

  if( debug_flag > 1 )
    {
      printf("# distTOF: [%7.2f|%7.2f|%7.2f] -> <%7.2f>\n", in->tof[0], in->tof[1], in->tof[2], in->spathfw );
    }

  for( i = 0; i < 3; i++ )
    {
      dstot      += pow(((in->mom[i] / in->mom[2]) * ((MUSIC_TOF_DIST+MUSIC_TOFFW) * 0.01)), 2.0 );
      in->tof[i]  = in->pos[i] + ((in->mom[i] / in->mom[2]) * MUSIC_TOF_DIST);
    }
  dstot = sqrt(dstot);

  in->spathfw += dstot;

  dstot = 0.;
  
  for( i = 0; i < 3; i++ )
    {
      dstot      += pow(((in->mom[i] / in->mom[2]) * ((MUSIC_TOF_DIST+MUSIC_TOFBK) * 0.01)), 2.0 );
    }
  dstot = sqrt(dstot);

  in->spathbk += dstot;

  if( debug_flag > 1 )
    {
      printf("# distTOF: [%7.2f|%7.2f|%7.2f] -> <%7.2f>\n", in->tof[0], in->tof[1], in->tof[2], in->spathfw );
    }
}

/***********************************************************/
/* Dump particle status. Either list mode or verbose mode  */
/* is possible.                                            */ 
void Tracking::dump_part( part *in, Int_t mode, Int_t point ) {
  if( mode == 0 )
    {
      printf("%6d %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f %11.5f  %11.5d\n", 
	     point, in->pos[0], in->pos[1], in->pos[2], 
	     in->mom[0], in->mom[1], in->mom[2],
	     in->field[0], in->field[1], in->field[2],
	     in->spathfw, in->smom, in->status); 
    }
  else
    {
      printf("##################################################\n");
      printf("# point = %6d\n", point);
      printf("# X     = %11.5f\n", in->pos[0]);
      printf("# Y     = %11.5f\n", in->pos[1]);
      printf("# Z     = %11.5f\n", in->pos[2]);
      printf("# PX    = %11.5f\n", in->mom[0]);
      printf("# PY    = %11.5f\n", in->mom[1]);
      printf("# PZ    = %11.5f\n", in->mom[2]);
      printf("# BX    = %11.5f\n", in->field[0]);
      printf("# BY    = %11.5f\n", in->field[1]);
      printf("# BZ    = %11.5f\n", in->field[2]);
      printf("# path  = %11.5f\n", in->spathfw);
      printf("# smom  = %11.5f\n", in->smom);
      printf("# stat  = %d\n", in->status);
      printf("##################################################\n");
    }
}

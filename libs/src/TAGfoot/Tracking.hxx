#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

#include "TObject.h"
#include "TString.h"

/* Return values for the Magnet() function */
#define RESULT_TRACK_DONE    0
#define RESULT_TRACK_MISSED -1
#define RESULT_TRACK_BAD    -2
#define RESULT_TRACK_ALADIN -3
#define RESULT_TRACK_NOCONV -4
#define RESULT_TRACK_INIT   -9
#define RESULT_TRACK_START  -10

  /* Parameters for pfit calculation. These are magic values from Titti. */
  /* To be honest: I don't know where they do come from, and if they are */
  /* suitable for us. In principle you can set pfit==1.0 and see what    */
  /* happens, as you will lose only some iterations...                   */
  static const Float_t PFIT_P1 =     -0.81898E-03;
  static const Float_t PFIT_P2 =    -31.612;
  static const Float_t PFIT_P3 =     69.786;
  static const Float_t PFIT_P4 =  -2203.4;
//TTB the gobal frame is the ALADIN frame!
  static const Float_t WOOD_SAX_AMP        =  -12.710;    /* amplitude for WS potential [Tm]                 *///TTB for wood saxon see how it is set to get the middle of magnetic fied and size of aladin
  static const Float_t MUSIC_MAPPER_DIST   =  110.000;    /* distance between MUSIC and MapperBox [cm]       */ //TTB middle of music to mapper box (=beginning of magentic field) //TTB always to middle of sensitive box
  static const Float_t MUSIC_TOF_DIST      =   94.06;     /* distance between MUSIC and TOF Flange[cm]       (was 114.820!)      */ //TTB middle of music to beginning of TOF-Wall
  static const Float_t MUSIC_TOFFW         =   12.10;     /* distance between frame MUSIC and TOF forward wall [cm] *///TTB beginning of TOF-Wall to surface of the paddle
  static const Float_t MUSIC_TOFBK         =   20.45;     /* distance between frame MUSIC and TOF backward wall [cm] */
  static const Float_t ALADIN_TARGET_DIST  =  227.420;    /* distance between ALADIN and target chamber [cm] z */ //TTB middle of aladin
  static const Float_t ALADIN_MUSIC_DIST   =  248.180;    /* distance between ALADIN and MUSIC [cm]*/
  static const Float_t ALADIN_THETA        =    7.170;    /* rotation angle of ALADIN [deg]                  */
  static const Float_t MUSIC_X_SHIFT       =    0.150;    /* offset of MUSIC from ALADIN Z axis [cm] */
  static const Float_t TARGET_X_SHIFT      =    24.34;    /* offset of target from ALADIN Z axis [cm]        */
  static const Float_t MUSIC_ROTATION      =    0.515;    /* rotation of MUSIC [deg]                         */ 
  static const Float_t PFIT_LOWER          =    0.550;    /* lower limit for accepted initial momentum [1]   */
  static const Float_t PFIT_UPPER          =    1.400;    /* upper limit for accepted initial momentum [1]   */
  static const Float_t TARGET_X_RESOLUTION =    0.020;    /* [mm]?? acceptance for back tracked particle [cm]       */ 
  static const Float_t PCHANGE_INI         =    0.015;    /* initial momentum change for iterations [1]     (was 0.15) */
  static const Float_t RK_SMAX             =    160.0;    /* used in RungeKutta() [cm]                  */
  static const Float_t RK_SMIN             =   -150.0;    /* used in RungeKutta() [cm]                  */
  static const Float_t RK_LOOP             =    10000;    /* was 10000 */ 
  static const Float_t RK_STEPSIZE         =    0.050;    /* stepsize for RungeKutta()                  */ /* was 0.05 */
  static const Float_t RAD2DEG             = 57.2957; /* transform from RAD to DEG                 */
  static const Float_t DEG2RAD             = .01745; /* transform from DEG to RAD                 */
//  static const Float_t PI = 3.141592654;
//look 
  /* Geometry parameters for measurement boxes */
  static const float STEP_X  = 3.0;
  static const float STEP_Y  = 3.0;
  static const float STEP_Z  = 4.0;




class Tracking {


 public:

  /* Experimental setup */
  static const Int_t   ALADIN_Z_SIZE       =       50;    /* ALADIN size restrictions [cm]                   */
  static const Int_t   MAX_TRY             =      100;    /* maximum number of iterations on one track  */ /* was 100 */

  /* In C we must use flags instead of GOTO. Goto is from hell. */
  static const Int_t   MASK_ITER           = 0x0000ffff;  /* number of iterations */
  static const Int_t   MASK_TRACK          = 0x00ff0000;  /* tracking status      */
  static const Int_t   MASK_TARGET         = 0xff000000;  /* target status        */

  static const Int_t   TRACK_NOCONV        = 0x00010000;  /* tracking did not converge - fatal */
  static const Int_t   TRACK_HITALADIN     = 0x00020000;  /* particle did hit ALADIN   - fatal */
  static const Int_t   TRACK_PASSALADIN    = 0x00040000;  /* particle did pass ALADIN  - good  */

  static const Int_t   TARGET_HIT          = 0x01000000;  /* particle did hit target   - good  */
  static const Int_t   TARGET_MISS         = 0x02000000;  /* particle did miss target  - bad   */
  static const Int_t   TARGET_BOTHSIDES    = 0x04000000;  /* left and right tracks     - good  */
  static const Int_t   TARGET_SAMESIDE     = 0x08000000;  /* tracks on same side       - bad   */
  static const Int_t   TARGET_APPROACH     = 0x10000000;  /* getting closer            - good  */
  static const Int_t   TARGET_DEPART       = 0x20000000;  /* moving away               - bad   */

  static const Int_t   NEW_TRACK           = 0x00000000;

  /* Particle structure. It stores all information needed for tracking. */
  /* One copy must be kept before doing RungeKutta(), as parts of the   */
  /* structure will be changed inside the tracking routines.            */
  
  /* Range of supported ALADIN currents */
  static const Int_t ALADIN_I_MIN  =    0; /* [A] */
  static const Int_t ALADIN_I_MAX  = 2500; /* [A] */
  static const Int_t NUM_MAP_FILES =   10;

  /* Status information for RungeKutta() */
  static const Int_t   RK_NEWTRACK         = 0x00000000; /* D[23:0]  = loop counter                     */
  static const Int_t   RK_BADTRACK         = 0x01000000;
  static const Int_t   RK_GOODTRACK        = 0x02000000;
  static const Int_t   RK_MASKTRACK        = 0xFF000000; /* D[31:24] = status                           */

  typedef struct {
    double pos[3];    /* [x|y|z] position              */ //TTB used in cm
    double mom[3];    /* [px|py|pz] momentum           */
    double field[3];  /* [Bx|By|Bz] magnetic field     */
    double dmom[3];   /* [dpx|dpy|dpz] momentum change */
    double tof[3];    /* [x|y|z] position in TOF       */
    double target[3]; /* [x|y|z] position of target    */
    double spathfw;     /* summed flight path up to forward wall  */
    double spathbk;     /* summed flight path up to backward wall */  
    double smom;      /* momentum                      */
    double theta;     /* angle in x-z-plane before B   */ //TTB "before B"?? before bending???
    double phi;       /* angle in x-y-plane before B   */
    int    status;
  } part;
  
  
  /* Special coordinate system for the two mapper boxes.           */
  /* pos[0] = xx, pos[1] = xy, pos[2] = xz, pos[3] = y, pos[4] = z */
  typedef struct {
    double pos[5];
    double field[3];
    int    status;
  } mapper;
  
//done in  xmap, ymap, zmap
//xx xy xz projections on coord system of the field box, y,z coord 
//y-axis remains the same, other axis change: x->z z->x

  /* stores the fieldmap delivered by the two measurement boxes. */
  typedef struct {
    int   li;
    int   lj;
    int   lk;
    float lbx[90][19][21];
    float lby[90][19][21];
    float lbz[90][19][21];
    int   lstatus;
    int   ri;
    int   rj;
    int   rk;
    float rbx[85][19][17];
    float rby[85][19][17];
    float rbz[85][19][17];
    int   rstatus;
  } fieldmap;
  
  /* used for transformation between measurement boxes and magnet */
  typedef struct {
    /* they were all float */
    double box1[3][2];
    double box2[3][2];
    double mag1[3][2];
    double mag2[3][2];
    double off1[3];
    double off2[3];
    double alpha1;
    double alpha2;
    int status;
  } geometry;

  Tracking();
  virtual ~Tracking();

  Int_t   track_id; // from GetZ.h
  Float_t tracklet_z;
  Float_t tracklet_xo;
  Float_t tracklet_xs;
  Float_t tracklet_yo;
  Float_t tracklet_ys;
  Float_t tracklet_time;
  Float_t xpos_targ;
  Float_t ypos_targ;
  Float_t MOMENTUM_FACTOR; 
  
  //GLOBAL VARIABLES (Float_t * xratio)

  /* Used functions */
  void Initialize(Int_t tid, Float_t trkl_z, Float_t trkl_xo, Float_t trkl_xs
		  , Float_t trkl_yo, Float_t trkl_ys, Float_t trkl_time
		  , Float_t x_t, Float_t y_t, Float_t momf);
  
  void   init_fieldmap();
  void   init_geometry();
  Int_t  fill_field(Int_t current, TString dir);
  Int_t  get_field(part *in);
  void   get_field1(mapper *mp1);
  void   get_field2(mapper *mp2);
  UInt_t RungeKutta( part *in,
		     Int_t field_flag, Int_t debug_flag, Int_t print_flag );
  void   restore_position( part *in, part *out );
  void   restore_coords( part *in, part *out );
  void   put_dp( part *in, double dp[] );
  void   get_dp( part *in, double dp[] );
  void   dump_part( part *in, Int_t mode, Int_t point );
  void   distTOF( part *in, Int_t debug_flag );
  void   beforeB( part *in, Int_t debug_flag, Int_t print_flag );
  void   init_part( part *in );
  void   copy_part( part in , part *out );
  void   alatest( part *in, Int_t debug_flag );
  void   afterB( part *in, Int_t debug_flag, Int_t print_flag );
  void   eqmaxwell( part *in, Int_t debug_flag );
  void   MovePart( part *in, double ds, Int_t debug_flag );

  Int_t  TrackMagnet(Int_t i_nevent); // Main routine

  /* Produced new variables */
  Float_t rigidity;   /* rigidity from tracking [Tm]                                  */
  Float_t rigx;
  Float_t rigy;
  Float_t rigz;
  Float_t rigx_lab;
  Float_t rigy_lab;
  Float_t rigz_lab;

  Float_t trackinc;
  Float_t backxtarg; /* Interpolated value of position at target */
  Int_t IDreconst;
  Int_t IDloop;

  Int_t   tracking_result;
  Float_t magnet_theta; /* x-z angle upstream ALADIN, from tracking [rad]               */
  Float_t magnet_phi; /* x-y angle upstream ALADIN, from tracking [rad]               */
  Float_t delta_theta; /* [rad] */
  Float_t delta_phi  ; /* [rad] */
  Float_t beta_new   ; /* recalibrated RICH velocity [v/c]                             */
  Int_t   track_iters ;  /* number of iterations to fit track [1]                        */
  Float_t track_xtdiff; /* difference between target position and tracked position [  ] */
  Float_t track_ytdiff; /* difference between target position and tracked position [  ] */
  Float_t track_xt    ;
  Float_t track_yt    ;
  Float_t rig_raw     ; /* dimensionless rigidity (1.0 = soll rigidity) [1]             */
  Float_t path_forward; /* Pathlength of particle up to forward wall*/
  Float_t path_backward; /* Pathlength of particle up to backward wall */

  fieldmap fm;
  geometry geom;

  /* Left box indizes */
  Int_t   LBI ;
  Int_t   LBJ ;
  Int_t   LBK ;
  /* Right box indizes */
  Int_t   RBI ;
  Int_t   RBJ ;
  Int_t   RBK ;

};

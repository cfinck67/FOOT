// ~ 25kb per event  (calculated for 1000 particles)

// - WATCHOUT: if EM particles (like delta-rays) produced entrance and exit coordinates might be wrong (=slightly off) TODO: solve this?
//    - find out: parent hadrons are ALWAYS tracked to end before EM kids? if so entrance is correct! -> in not the case -> we have to do the complicated way
//    - add a flag for hadron flgHad: activated if hadron entered in scoring region
//    - for entrance point: when entrance point set look if particle is hadron or not, and set flag
//        - for every hadronic particle entering check if flag set or not, if not set determine new entrance point and set flag 
//    - for exit point: if flag set: "hadron was here", then no scoring of EM anymore
// - WATCHOUT: there are some exiting positions of high energy photons which are wrong? Is this due to the way the EM particle tacks are saved?
// - TODO: set EMF PROD and transport cut of all materials as default = 1GeV

//TODO: check and add overflow message for the score routines? -> maybe better not, because errors are more easily visible


//CHECK: the positive x side should be always left (beam's eye view)!!!!!!!!!!!
#ifndef FOOT_GEOMETRY
#define FOOT_GEOMETRY


// coding conventions (ROOT):
// (currently not yet coherently followed)
// kMyVariable = constant
// gMyVariable = global variable
// fMyVariable = class data member

#include "TObject.h"
#include "TString.h"
#include <cmath>



//using namespace std;

const Int_t kVerbosity		=0;
//kFormat
//0= FLUKA (default)
//1= SimpleGeo
const Int_t kFormat		=0;

//detector configuration: 1=ONION, 0=ZIGGURAT
const Bool_t kON_OnionRing	=0;


//material configuration: 1=ON, 0=OFF
//TODO:
//const Bool_t kON_SetBeamMediaVaccum	=0;



/*  units : cm, degree */
const Double_t  PI		=3.141592653589793238462643383279502884197169399375;
const Double_t  FWHM2Sig	=2.354820;//conversion FWHM to sigma (Gaussian)
const Double_t  kAvoidOverlap1  =.0001;//avoid overlap
const Double_t  kAvoidOverlap2  =.00011;//avoid overlap nested

//************************************************************
/* PRIMARY */


//**********************************************************************************
//DEFAULT CONFIGURATION

const Int_t  PRIM_Type  =-2;   // -2=heavy ion 1=proton
const Int_t    PRIM_Z	=6;
const Int_t  PRIM_A	=12;//C-12
const Double_t PRIM_T		=0.4; //GeV/n (kinetic energy of primaries)
const TString  PRIM_DIST	="GAUSSIAN";//lateral beam distribution 
                                            // other option "ANNULAR"
const Double_t PRIM_RMAX	=0.3;//maximum radius (FWHM)
const Double_t  PRIM_dP		=0.0;//momentum spread (+=flat, -=Gaussian (FWHM) )
const Double_t  PRIM_DIV	=-1.25;//in mrad (+=flat, -=Gaussian, 10000.=iso

const Double_t  PRIM_Pos_X		=0.0;
const Double_t  PRIM_Pos_Y		=0.0;
const Double_t  PRIM_Pos_Z		=-10.0;
//set target, mi_P, and vuoto to air

//const TString  TG_MEDIA         ="CARBON";
const TString  TG_MEDIA         ="CARBONTG";
//const TString  TG_MEDIA         ="GOLD";
//const TString  TG_MEDIA         ="VACUUM";

//const TString  TG_MEDIA         ="VACUUM";
const TString  WO_MEDIUM	="VACUUM"; 
const TString  MI_MIMO_GAS	="AIR";

const TString TW_Bra_MEDIUM	="Brass";//Material in between two TOFWALL planes

const Double_t  PRIM_Pos_CX		=0.0;//cos y
const Double_t  PRIM_Pos_CY		=0.0;//cos x

//**********************************************************************************
//ALTERNATIVE CONFIGURATIONS

// const Int_t  PRIM_Type  =-2;//primary FLUKA particle type //DEFAULT
//const Int_t  PRIM_Z	=6;const Int_t  PRIM_A	=12;//C-12 //DEFAULT
//const Int_t  PRIM_Z	=3;const Int_t  PRIM_A	=7;//Li-7
// const Int_t  PRIM_Z	=2;const Int_t  PRIM_A	=4;//He-4
// const Int_t  PRIM_Z	=8;const Int_t  PRIM_A	=16;//O-16
// const Int_t  PRIM_Z	=79;const Int_t  PRIM_A	=197;//Au-197
// const Int_t  PRIM_Z	=50;const Int_t  PRIM_A	=119;//Sn-119
// const Double_t PRIM_T		=0.6; //GeV/n (kintic energy of primaries)



//const Double_t  PRIM_Pos_X		=0.0; //DEFAULT
// const Double_t  PRIM_Pos_Y		=0.0; //DEFAULT
// const Double_t  PRIM_Pos_Z		=-10.0; //DEFAULT
//const Double_t  PRIM_dP		=0.0;//Gaussian momentum spread (FWHM) //DEFAULT


//  const TString  TG_MEDIA        ="CARBON";//WARNING: a specific delta-ray threshold is automatically set for this material! //DEFAULT
//const TString  TG_MEDIA        ="GOLD";
//const TString  TG_MEDIA        ="TIN";

//const TString  WO_MEDIUM	="VACUUM"; //DEFAULT

//**********************************************************************************
//PHYSICS AND TRANSPORT SETTINGS
// threshold for produciton and transport of EM particles (GeV)
//const Double_t TRANS_THRES_EM   = 1.;//OFF
const Double_t TRANS_THRES_EM   = 0.001;//1MeV
//const Double_t TRANS_THRES_EM   = 0.0001;//100keV

const Double_t AL_THRES_EM   = 1.;//OFF




// CSDA range in | water   | SiO2    | air
// 100keV        |.143 mm  | 0.0752  | 134.7
//  50keV        |.0432mm  | 0.0230  | 40.8
// 10keV         |.00252mm | 0.00140 | 2.39
const Double_t MI_THRES_EM      =0.001;//1MeV
//const Double_t MI_THRES_EM      =0.0001;//100keV
//const Double_t MI_THRES_EM      =0.00005;//50keV
//const Double_t MI_THRES_EM      =0.00001;//10keV //DEFAULT FOR NOW BUT SLOW
//const Double_t MI_THRES_EM      =0.000001;//1keV
//const Double_t MI_THRES_EM   = 1.;//OFF //FOR FAST VERSION (factor ~2 wrt 10keV)


// threshold for SCORING tracks of EM particles (GeV)
//const Double_t SCO_THRES_EM   = 1000.;//OFF
const Double_t SCO_THRES_EM   = 0.001;//1MeV
// const Double_t SCO_THRES_EM   = 0.0001;//100keV
//const Double_t SCO_THRES_EM   = 0.00001;//10keV //WATCHOUT:number too low-> stupid bug (C->FORTRAN)




//************************************************************
//GENERAL GEOMETRY
/* angles */ 
//minus angles bend to the right

//const Double_t AL_ANGLE_Y 	=0.0;
//const Double_t MU_ANGLE_Y 	=0.0;
//const Double_t AL_ANGLE_Y 	=45.; 


//const Double_t AL_ANGLE_Y 	=XXX.; //to be optimized
//const Double_t MU_ANGLE_Y 	=AL_ANGLE_Y; //

//const Double_t AL_ANGLE_Y 	=7.170; //bend to the left! switched SPALADIN set-up 
//const Double_t MU_ANGLE_Y 	=AL_ANGLE_Y+0.515; //bend to the left! //switched SPALADIN set-up

const Double_t LA_ANGLE_Y 	=0.0; //default



//************************************************************
//current (in A) in the magnet
//positive current bends positively charged particles to the left


const Double_t AL_CURR 		=-6.77;  // this is the value that fix the mass peak of carbon in data
//const Double_t AL_CURR 		=-7.10;

//const Double_t AL_CURR 	=0.0;// No magnetic fied(Run no:451 - 470), beam goes directly to slat 38 tofwall; seems ok!



//standard with spaladin setup and hole at x=-11.5cm
//const Double_t AL_CURR 		=-11.3; //SPAL hole:0.0 C-12@400, O-16@400

//standards with spaladin setup and hole at -60.5 
//const Double_t AL_CURR 		=20.5; //SPAL hole:-60.5 C-12@400, O-16@400
//const Double_t AL_CURR 		=18.4; //SPAL hole:-60.5 Li-7@250
//const Double_t AL_CURR 		=13.9; //SPAL hole:-60.5 He-4@200



//maximum field of 1.6 Tesla (V.Serfling 1993) (max: 2.2Tm)
//at current of 25. -> max Tesla ~1.6 ok!


//************************************************************
// SPECIFIC GEOMETRY
//Interaction Region (IR)
const Double_t IR_X		=0.0;//middle of IR
const Double_t IR_Y		=0.0;//middle of IR
const Double_t IR_Z		=-10.0;//beginning of IR

const Double_t IR_HEIGHT 	=100.;
const Double_t IR_WIDTH  	=100.;
//const Double_t IR_THICK  	=70.;
const Double_t IR_THICK  	=144.; 

//const TString  IR_MEDIUM	="VACUUM"; 
const TString  IR_MEDIUM	="AIR"; //DEFAULT

//************************************************************
//TODO: make gas 80%-20% AR-CO2 mixture instead of air
/* parametri del rivelatore di start */
const Double_t ST_Z  		=0.0;
const Double_t ST_HEIGHT	=4.0;
const Double_t ST_WIDTH 	=4.0;
const Double_t ST_THICK 	=0.02;
//************************************************************
//old BM
/* parametri geometrici per la camera a drift di monitor del beam */
/*const Double_t BM_Z		=ST_Z+2.0;
const Double_t MYLARTHICK	=0.01;
const Double_t BM_HEIGHT	=6.0;
const Double_t BM_WIDTH   	=6.0;
const Double_t BM_LENGTH   	=12.0;
// ************************************************************
//  parametri dei fili 
const Double_t elle 		=0.5;
const Double_t acca 		=0.433012701;
const Double_t delta_chamber 	=0.3;
const Double_t DELTAZ    	=1.0;
const Double_t DELTAY    	=1.0;
const Double_t DELTAX    	=1.0;
const Int_t    NWIRELAYER 	=22;
const Int_t    NLAYER 		=4;
const Int_t    NSENSE 		=4;
const Double_t RSENSE 		=0.003;
const Double_t RFIELD 		=0.008;
const Double_t U_VIEW  		=1;
const Double_t V_VIEW  		=-1;
*/

// ************************************************************
/* NEW parametri geometrici per la camera a drift di monitor del beam */
/*   New BM*/
const Double_t BMN_W_THICK	=0.01;//mylar window (is also used to define wall in other directions!)
const Double_t BMN_U_VIEW  	=1;//wire along x (side view)
const Double_t BMN_V_VIEW  	=-1;//wire along y (top view)
const Double_t BMN_RSENSE 	=0.003;//radius of sensitive wires
const Double_t BMN_RFIELD 	=0.008;//radius of field wires


/* new chmb geom params  */
const Double_t BMN_HEIGHT	=11.2;
const Double_t BMN_WIDTH	=11.2;
const Double_t BMN_LENGTH	=21.0;
/*  parametri dei fili */
const Double_t BMN_STEP		=0.5; //distance between planes (z distance)
const Double_t BMN_PASSO	=0.8; //distance between wires (x or y distance)
const Double_t BMN_PLANE_D	=0.3; //distance between layer U and V (all wires in one direction)

/*  shift dei fili rispetto ai lati della camera */
//ALESSIO's version: const Double_t BMN_DELTA_Z	=2.8;
const Double_t BMN_DELTA_Z	=2.85;//measured distance to first layer of from (not necessarily accurate)
const Double_t BMN_DELTA_Y	=2.8;
const Double_t BMN_DELTA_X	=2.8;

const Int_t BMN_WIRELAYER_N	=21;//wires per plane  DEBUG::: 22
const Int_t BMN_LAYER_N		=6;// no. of planes/2 for x and y
const Int_t BMN_SENSE_N		=3;// no. cells??? DEBUG: 4!

// old const Int_t BMN_FILIPIANO_N	=21;//wires per plane
//const Int_t BMN_PIANI_N		=6;// no. of planes/2
//const Int_t BMN_CELLE_N		=3;// no. cells???

/* Euler angles that defines the BEAM monitor orientation ( degrees)*/
//Goldstein convention: psi, theta, psi
//#define EULER1MONNEW   0.
//#define EULER2MONNEW   0.
//#define EULER3MONNEW   0.


//************************************************************
/* parametri della target*/
const Double_t TG_HEIGHT  	=6.0;
const Double_t TG_WIDTH   	=6.0;
const Double_t TG_THICK   	=0.8;//target thickness
//const Double_t TG_THICK   	=0.05;// target thickness for gold


//************************************************************
/*parametri dei silici*/
//TODO: for starting, geometry is implemented with x<->y swapped
// 1.3M sensitive pads per plane (2 MIMOSAs), 18um=0.018mm distance between pads


const Double_t MI_DIST_TGT_MI	=0.3;//distance target (end) and mimosa (beginning of first plane)
const Double_t MI_DIST_PLANES	=0.3;//distance between mimosa planes (middle to middle)


const Int_t    MI_MIMO_N  	=8; //number of MIMOSA chips


const Double_t MI_GLAFI_HEIGHT 	=5.;
const Double_t MI_GLAFI_WIDTH  	=5.;
const Double_t MI_GLAFI_THICK  	=0.1;

//const TString  MI_MIMO_GAS	="AIR"; //DEFAULT
const TString  MI_MIMO_MEDIUM	="MI_SILIC";
const Double_t MI_MIMO_HEIGHT 	=1.06;
const Double_t MI_MIMO_WIDTH  	=2.12;
//const Double_t MI_MIMO_THICK  	=0.01;//80um old
const Double_t MI_MIMO_THICK  	=0.00504;//30um+14um+6.4um
const Double_t MI_MEOX_THICK	=0.00064;


const Double_t MI_SENS_HEIGHT 	=MI_MIMO_HEIGHT;
const Double_t MI_SENS_WIDTH  	=MI_MIMO_WIDTH;
const Double_t MI_SENS_THICK  	=0.0014;//14um
//const Double_t MI_SENS_THICK  	=0.002;//20um old

const Int_t  MI_SENS_BIN_HEIGHT =576; //number of bins
const Int_t  MI_SENS_BIN_WIDTH  =1152;
const Int_t  MI_SENS_BIN_THICK  =1;

//const Double_t MI_PAD_WIDTH      =0.0018;//cm =18um
const Double_t MI_PAD_WIDTH      =0.00184027777778;
const Double_t MI_PAD_HEIGHT     =MI_PAD_WIDTH;//cm
const Double_t MI_PAD_THICK      =MI_SENS_THICK;//cm


const Int_t  MI_SENS_BIN_UNIT   =-38;//FLUKA scoring unit


//Christian: the overlap is about 100microns=0.01cm -> 0.005cm 
//shift of MIMOSAs
const Double_t MI_MIMO_Y[] 	={+0.005-MI_MIMO_HEIGHT/2.
				 ,-0.005+MI_MIMO_HEIGHT/2.
				 ,+0.005-MI_MIMO_HEIGHT/2.
				 ,-0.005+MI_MIMO_HEIGHT/2.
				 ,+0.005-MI_MIMO_HEIGHT/2.
				 ,-0.005+MI_MIMO_HEIGHT/2.
				 ,+0.005-MI_MIMO_HEIGHT/2.
				 ,-0.005+MI_MIMO_HEIGHT/2.};
const Double_t MI_HOLE_HEIGHT 	=2.*MI_MIMO_HEIGHT-2.*0.005;// 100um overlap 

//old layout
//const Double_t MI_MIMO_Y[] 	={-0.175-MI_MIMO_HEIGHT/2.
//				 ,-0.250+MI_MIMO_HEIGHT/2.
//				 ,+0.150+MI_MIMO_HEIGHT/2.
//				 ,-0.075-MI_MIMO_HEIGHT/2.
//				 ,-0.150+MI_MIMO_HEIGHT/2.
//				 ,+0.050-MI_MIMO_HEIGHT/2.
//				 ,-0.025+MI_MIMO_HEIGHT/2.};

//const Double_t MI_HOLE_HEIGHT 	=2.*MI_MIMO_HEIGHT-0.075;//=2.045; 750um overlap
const Double_t MI_HOLE_WIDTH  	=2.2;
const Double_t MI_HOLE_THICK  	=MI_GLAFI_THICK;
const Double_t MI_HEIGHT 	=MI_GLAFI_HEIGHT+1.;
const Double_t MI_WIDTH  	=MI_GLAFI_WIDTH+1.;

// VM 14/11/2013 flags for pile-up simulation in the VTX detector
// if MI_PILE_UP=true extract number of vertices with Poisson distribution with
// mean value MI_MEAN_NVTX (from events saved in a queue)
const Bool_t MI_PILE_UP = true;
const Double_t MI_MEAN_NVTX = .758;
// MI_PILEUP_SKIP_EV if true do not save events used to store MI pile up infoes
// if false use previous events for pile-up simulation
const Bool_t MI_PILEUP_SKIP_EV = false;
// min/max elements in the queue
const Double_t MI_HIGH_QUEUE = 20;
const Double_t MI_LOW_QUEUE = 10;

//************************************************************
/*parametri dello KENTROS*/


//fiber strips
const Int_t    KE_FIB_PlaneNb=25;// fiber strips will be NFplans
const Double_t KE_FIB_THICK=0.2; //Height of the Fiber Strip
const Double_t KE_FIB_RAD_IN=33.;
const Double_t KE_FIB_RAD_OU=KE_FIB_RAD_IN+KE_FIB_THICK;

//barrel
const Int_t    KE_BAR_PlaneNb=30;// slices will be 2 * NBplans
const Int_t    KE_BAR_THICK=50.;// BH # Height of the Barrell
const Double_t KE_BAR_RAD_IN=KE_FIB_RAD_OU+kAvoidOverlap1;
const Double_t KE_BAR_RAD_OU=37.;

const Double_t KE_OFFSET= 6.7; //Ensure the KE_ACT, KE_Bigend cup, KE_Smallend cup positions w.r.t survey measurements 

//big endcap

const Int_t    KE_BEC_PlaneNb=30;
const Double_t KE_BEC_THICK=3.5; //Height of the Big EndCap

const Double_t KE_BEC_RAD_IN=14.0;
const Double_t KE_BEC_RAD_OU=37.+kAvoidOverlap1; //TODO: why (37.+kAvoidOverlap1)????? 

//ZBEmin=$(echo " $ZBmax + 0.0001" | bc -l)
//ZBEmax=$(echo " $ZBEmin + $BEH" | bc -l)


//small endcap
const Int_t    KE_SEC_PlaneNb=12;
const Double_t KE_SEC_THICK=3.5; //Height of the Small EndCap

const Double_t KE_SEC_RAD_IN=5.;
const Double_t KE_SEC_RAD_OU=15.; 

const TString  KE_DET_MEDIUM	="KE_PLA"; 

//wrong! const Int_t    KE_SegmentsTotalNb= FIB_PlaneNb+KE_BAR_PlaneNb+KE_BEC_PlaneNb+KE_SEC_PlaneNb;

//ZSEmin=$(echo " $ZBEmax + 0.0001" | bc -l)
//ZSEmax=$(echo " $ZSEmin + $SEH" | bc -l)


//box around
const Double_t KE_RADIUS_OU	=KE_BAR_RAD_OU+1.;
const Double_t KE_RADIUS_IN	=KE_FIB_RAD_IN-1.;
const Double_t KE_THICK_OU	=KE_BAR_THICK+KE_BEC_THICK+KE_SEC_THICK+2.;
const Double_t KE_THICK_IN	=KE_BAR_THICK+1.-1.;

//light guide
const Double_t KE_BAR_LIGHTGUIDE_LENGTH=10;
const Double_t KE_BEC_LIGHTGUIDE_LENGTH=10;
const Double_t KE_SEC_LIGHTGUIDE_LENGTH=10;

//************************************************************
/* parametri del ALADiN*/



//ALADiN itself
//container size & position (the magnetic field is placed in the middle of this box)
const Double_t AL_HEIGHT 	=420.;
const Double_t AL_WIDTH  	=420.;
//const Double_t AL_THICK  	=230.02;//+0.01-0.1.
const Double_t AL_THICK  	=176.02+53;// survey + flanges(design)


const Double_t AL_W_THICK   	=0.01;
//magnetic field
const Double_t AL_MF_HEIGHT 	=50.;
const Double_t AL_MF_WIDTH_IN 	=130.;
const Double_t AL_MF_WIDTH_OU 	=150.;
//not needed: const Double_t AL_MF_WIDTH  	=80.;
//const Double_t AL_MF_THICK  	=AL_THICK-2*AL_W_THICK;

//not needed: const Double_t AL_MF_Z     	=AL_Z+AL_W_THICK;

//collimator in front of ALADiN
//entrance window
//const Double_t AL_CO_W_THICK  =0.01;
const Double_t AL_CO_W_THICK   	=0.0125;
const Double_t AL_CO_W_RAD  	=7.5;

const Double_t AL_CO1_THICK 	=2.4;
//const Double_t AL_CO1_RAD_I 	=10.;
const Double_t AL_CO1_RAD_I 	=7.5;// Reduced the radius according to technical design
const Double_t AL_CO1_RAD_E 	=16.5;

const Double_t AL_CO2_THICK 	=4.0;
const Double_t AL_CO2_RAD_I 	=10.;
const Double_t AL_CO2_RAD_E 	=65.0;

const Double_t AL_CO3_THICK 	=2.4;
const Double_t AL_CO3_RAD_I 	=35.;
const Double_t AL_CO3_RAD_E 	=65.0;

const Double_t AL_CO4_THICK 	=1.2;
const Double_t AL_CO4_RAD_I 	=20.;
const Double_t AL_CO4_RAD_E 	=65.0;

//const Double_t AL_OFFSET	=0.0;
const Double_t AL_OFFSET	=0.0;	
					//Offset is added to X, Z points of Aladin center taking consider the rotation.




//distance: collimator box beginning to aladin middle
//const Double_t DIST_AL_CO_Z 	=25.+AL_THICK/2.
//           +AL_CO_W_THICK+AL_CO1_THICK+AL_CO2_THICK+AL_CO3_THICK+AL_CO4_THICK;//according to G4 simulation


const Double_t DIST_AL_CO_Z 	=22.0+AL_THICK/2.
           +AL_CO_W_THICK+AL_CO1_THICK+AL_CO2_THICK+AL_CO3_THICK+AL_CO4_THICK;


//container size & position (make a box that contains collimator + vacuum until the magnet zone)
const Double_t AL_CO_BOX_THICK 	=DIST_AL_CO_Z;
const Double_t AL_CO_BOX_RAD 	=65.0;


//************************************************************
/* parametri del MUSIC4*/

//in ALADiN reference frame // not used anymore

//container size & position
const Double_t MU_HEIGHT 	=150.;
const Double_t MU_WIDTH  	=350.;
const Double_t MU_THICK  	=243.;

//MUSIC4 body
const Double_t MU_BO_WIDTH_IN  	=283.5;
const Double_t MU_BO_WIDTH_OU  	=MU_WIDTH;



//entrance window thickness
const Double_t MU_W_THICK   	=0.01;

//TODO: correct thickness and material?
//MUSIC-TOF-Wall window: "white mylar": 200um
//probably old: Moehlenkamp 1996: Flugzeitwand durch eine 0.1 mm dicke Folie aus Edelstahl von der übrigen Detektorkammer getrennt und wurde in einer Stickstoffatmosphare unter Normaldruck betrieben.

//rear window thickness
const Double_t MU_WR_THICK   	=0.02;


//thickness of central cathode (material: kapton,Moehlenkamp 1996)
const Double_t MU_CC_WIDTH   	=0.1;//assume 1mm and kapton until we know better 


// Moehlenkamp 1996: Die Abmessungen von 1.8m entlang der Strahlachse 2m in der Biegeebene des Magneten x und 1m in vertikaler Richtung y bilden das aktive Volumen der MUSIC III.

//sensitive volume (positioned in the middle (x,y,z) of outer box)
const Double_t MU_SEN_HEIGHT 	=100.;
const Double_t MU_SEN_WIDTH  	=200.;
//const Double_t MU_SEN_THICK  	=180.;
const Double_t MU_SEN_THICK  	=200.;//enlarged to fit PC sensitive regions (see below)

const Double_t MU_SEN_MARGIN 	=2.; //material on sides (not entrance + exit) around active MUSIC volume


//proportional counters
const Int_t    MU_PRO_Nb	=8;
//const Double_t MU_PRO_Z[]	={-75.,-25.,25.,75.};
//wrt to the middle of the sensitive volume//according to C.Sfienti: 63cm middle to middle between the PCs
const Double_t MU_PRO_Z[]	={-94.5,-31.5,31.5,94.5};//wrt to the middle of the sensitive volume
const Double_t MU_PRO_HEIGHT	=96.;//wrt to the middle of the sensitive volume
const Double_t MU_PRO_WIDTH	=MU_SEN_WIDTH-kAvoidOverlap1;//wrt to the middle of the sensitive volume
const Double_t MU_PRO_THICK	=8.;//wrt to the middle of the sensitive volume



//enclosure
//TODO: implement enclosure
// const Double_t MU_ENC_THICK     =2.;
// const Double_t MU_ENC_V_X	=47.5;
// const Double_t MU_ENC_V_Y	=MU_HEIGHT;
// const Double_t MU_ENC_V_Z	=MU_ENC_THICK;

// const Double_t MU_ENC_H_X	=MU_ENC_THICK;
// const Double_t MU_ENC_H_Y	=MU_HEIGHT;

//const Double_t MU_ENC_H_ANGLE_Y	=std::atan( ((MU_BO_WIDTH_OU-MU_BO_WIDTH_IN)/2.) / (MU_THICK-MU_ENC_V_Z) ) ;
//const Double_t MU_ENC_H_ANGLE_Y	=0.0;

//TODO: fix: why zero for first compilation when creating ROOT dictionary??? Vincenzo do you understant why?
//WORKAROUND: for first compilation set MU_ENC_H_Z=1., then set MU_ENC_H_Z correctly and recompile
//const Double_t MU_ENC_H_Z	=(MU_THICK-MU_ENC_V_Z)/std::cos(MU_ENC_H_ANGLE_Y) - MU_ENC_H_X*std::tan(MU_ENC_H_ANGLE_Y);
//const Double_t MU_ENC_H_Z	=1.;
//const Double_t MU_ENC_H_Z	=1./std::cos(0.0);

//************************************************************
/* parametri del TOF Wall*/ 


/*

                --
	     /  / |
            /  /   |
            --     |
           |  |    |  fTof_Y = 1100.0*mm
           |  |    |
--->       |  |    |
beam (z)   |  |   /
           |  | /   fTof_X = 2400.0*mm
            --
                 
    fTof_Z = 95.0*mm 

  */


const Double_t TW_HEIGHT	=150.;//Y
const Double_t TW_WIDTH		=350.;//X
const Double_t TW_THICK		=57.5;//Z

//in MUSIC reference frame
//distance between end of MUSIC and beginning of TOF-Wall boxes
const Double_t DIST_MU_TW_Z       =.001;//default

// Brass sheet for tofwall

const Double_t TW_Brass_X	=TW_HEIGHT;//Y
const Double_t TW_Brass_Y	=TW_WIDTH;//X
const Double_t TW_Brass_Z	=0.05;// thickness of brass sheet. height and width is the same that of TW_Height and TW_Width



const Double_t TW_ShiftOfSlats_B_Z       =0.25;//brass sheet position is 0.25 cm apart from back side of frontwall
const Double_t TW_ShiftOfSlats_B_X       =0.0;
const Double_t TW_ShiftOfSlats_B_Y       =0.0;


 
// This is merely a constant value , no relation with music 
//distance between MUSIC middle and beginning of front and rear TOF-Wall

const Double_t TW_ShiftOfSlats_F_Z  = 0.0;  //according to TAT geo setup file
const Double_t TW_ShiftOfSlats_R_Z  = 0.0;  //according to TAT geo setup file


// Serfling 93
//Die beiden Szintillatorwande sind
// um eine halbe Stabbreite 12.5mm vertikal gegeneinander verschoben so dass keine
// Ritzen im Detektor entstehen Der Abstand zwischen den Wanden betragt ungefahr
// 50mm

//slat dimensions
const Double_t TW_Slat_X = 2.5;
const Double_t TW_Slat_Y = 110.0;
const Double_t TW_Slat_Z = 1.;

//slats of front and rear wall

const Double_t TW_SpaceBetweenSlats_X = 0.01;//between surfaces
const Double_t TW_SpaceBetweenModules_X = 0.01;//between surfaces
const Double_t TW_SpaceBetweenFrontWallAndScoring_Z = 0.5;//between surfaces

//offset from the middle of front for both front and rear wall
//position of slats relative to TOF-Wall Container
//for optimization Front Wall is set to middle and Back Wall is displaced by (TW_Slat_X+TW_SpaceBetweenSlats_X)/2.
const Double_t TW_ShiftOfSlats_F_X = 0.0; 
const Double_t TW_ShiftOfSlats_F_Y = 0.0;

const Double_t TW_ShiftOfSlats_R_X = 0.0;
const Double_t TW_ShiftOfSlats_R_Y = 0.0;

//const Double_t TW_ShiftOfSlats_Z = -5.0; OLD

//TODO: check for windows front and rear in SPALADIN
const Double_t TW_RearWindow_X = 200.0;
const Double_t TW_RearWindow_Y = 20.0;
const Double_t TW_RearWindow_Z = 0.010;
const Double_t TW_RearWindow_From_Rear = 5.0;
//Ti middle of end of container

const Double_t TW_EnclosureThickness = 2.0;

//Stainless Steel
const Double_t TW_EnclosureH_X = TW_EnclosureThickness;
const Double_t TW_EnclosureH_Y = TW_HEIGHT;
const Double_t TW_EnclosureH_Z = TW_THICK - TW_EnclosureThickness;

const Double_t TW_EnclosureV_X = (TW_WIDTH-TW_RearWindow_X)/2.;
const Double_t TW_EnclosureV_Y = TW_HEIGHT;
const Double_t TW_EnclosureV_Z = TW_EnclosureThickness;


//onion ring detector
//like this we have thickness 1cm and max radius 10cm
const Int_t    TW_Onion_Nb=10;
const Double_t TW_Onion_dR=1.;
const Double_t TW_Onion_Z=1.;//thickness in z

//ziggurat detector
//first plane: 6x6 with thickness 4 , secon plane 4x4 thickness 7
const Double_t TW_Zig_1_HEIGHT=4.;
const Double_t TW_Zig_1_WIDTH=4.;
const Double_t TW_Zig_1_THICK=7.;

const Double_t TW_Zig_2_HEIGHT=6.;
const Double_t TW_Zig_2_WIDTH=6.;
const Double_t TW_Zig_2_THICK=4.;


//const Double_t TW_SlatHole_X = 2.5;
//const Double_t TW_SlatHole_Y = 2.*53.1;
//const Double_t TW_SlatHole_Z = 1.;


/* NB MODULES */
// const Int_t TW_NbModFrontL = 4;//?value?
// const Int_t TW_NbModFrontR = 4;//?value?
// const Int_t TW_NbModBackL = 3;//?value?
// const Int_t TW_NbModBackR = 4;//?value?

//in beam's eye view the numbering of the slats is from left to right!

const Int_t TW_NbModFront = 12;
const Int_t TW_NbModBack = 12;


//TODO: what is the number of avaiable slats for the FIRST exp???
// if you change these numbers remember to change ASSIGNMAT in the inp-file and nrefTW* in usrini.f
const Int_t TW_NbSlatFront = TW_NbModFront*8; //96; //((TW_NbModFrontL*8+2)+(TW_NbModFrontR*8+3)+(3));//64+2+3+3=72
const Int_t TW_NbSlatBack  = TW_NbModBack*8; //96; //((TW_NbModBackL*8+2)+(TW_NbModBackR*8+3)+(3));//56+2+3+3=64

const Double_t TW_tofWallF_X = (Double_t)TW_NbSlatFront*(TW_Slat_X+TW_SpaceBetweenSlats_X)+(Double_t)TW_NbModFront*(TW_SpaceBetweenModules_X-TW_SpaceBetweenSlats_X)-TW_SpaceBetweenModules_X;
const Double_t TW_tofWallF_Y = 110.;
const Double_t TW_tofWallF_Z = 1.;

const Double_t TW_tofWallB_X = (Double_t)TW_NbSlatBack*(TW_Slat_X+TW_SpaceBetweenSlats_X)+(Double_t)TW_NbModBack*(TW_SpaceBetweenModules_X-TW_SpaceBetweenSlats_X)-TW_SpaceBetweenModules_X;
const Double_t TW_tofWallB_Y = 110.;
const Double_t TW_tofWallB_Z = 1.;

// VM 8/8/2013 added extra module before the central rear slats
const Int_t TW_NbModExtra = 1;
const Int_t TW_NbSlatExtra = 8*TW_NbModExtra;
// VM 28/2/2014 changed
const Double_t TW_OffsetZ_Extra = 2.; // 2 cm distnce from rear plane in Z'


const Double_t TW_tofWallE_X = (Double_t)TW_NbSlatExtra*(TW_Slat_X+TW_SpaceBetweenSlats_X)+(Double_t)TW_NbModExtra*(TW_SpaceBetweenModules_X-TW_SpaceBetweenSlats_X)-TW_SpaceBetweenModules_X;
const Double_t TW_tofWallE_Y = 110.;
const Double_t TW_tofWallE_Z = 1.;

const Double_t TW_FrontLeftEdge = TW_ShiftOfSlats_F_X+TW_tofWallF_X/2.;
//measured from middle SPALDIN, old value:81.8					
								     
const Double_t TW_BackLeftEdge  = TW_ShiftOfSlats_R_X+TW_tofWallB_X/2.; 
//measured from middle SPALDIN, old value:60.5;
								     
const Double_t TW_ScoringLayerLeftEdge  =std::max(TW_FrontLeftEdge,TW_BackLeftEdge);
const Double_t TW_ScoringLayerRightEdge =std::min(TW_FrontLeftEdge-TW_tofWallF_X,TW_BackLeftEdge-TW_tofWallB_X);
const Double_t TW_ScoringLayer_X = TW_ScoringLayerLeftEdge-TW_ScoringLayerRightEdge;
const Double_t TW_ScoringLayer_Y = TW_Slat_Y;

const Double_t TW_ScoringLayer_Z = 1.0;//TODO: sufficiently thick?
//const Double_t TW_ScoringLayer_Z = 0.1;//TODO: sufficiently thick?


//Thomas Gorbinet
//-> The Holes : they are both the same as far as I know : 34 mm height and 75 mm width 
//=> the width is equal to 3 scintillators since the hole is "drilled" in a module of 8 scintillators (12 modules per plane) //(slats number 3 4 5 when looking at the tof wall module, beam in your back)
//Assume that slats 3,4,5 in front plane
//const Int_t TW_Hole1ModNb=7;//module chosen from TG's TOF-Wall picture
//const Int_t TW_Hole2ModNb=9;//module chosen from TG's TOF-Wall picture 

//WARNING: the holes don't include any slat shifts (see above TW_ShiftOfSlats) 
//New setting
								     
const Int_t TW_Hole1ModNb=9;//module chosen so that hole is at the side 
//const Int_t TW_Hole2ModNb=7;//module chosen so that it is more or less in the middle


//hole closer to centre (is set to zero size)
const Double_t TW_Hole_CentralSlat = 53;
const Double_t TW_Hole1_X = 3*TW_Slat_X+3*TW_SpaceBetweenSlats_X;// 7.5;
const Double_t TW_Hole1_Y =3.4;// 3.4;
const Double_t TW_Hole1_Z = TW_Slat_Z;//z determined by intersection with Front and Back Walls
/* const Double_t TW_ShiftOfHole1_X=-(TW_FrontLeftEdge-(TW_SpaceBetweenSlats_X/2.+(((float)TW_Hole1ModNb-1.)*8.+4.)*(TW_Slat_X+TW_SpaceBetweenSlats_X)));//-7.;//?value? //TODO: correct positioning, current formula just approximate */
/* const Double_t TW_Hole1_X =0.001;// 7.5; */
/* const Double_t TW_Hole1_Y =0.001;// 3.4; */
/* const Double_t TW_Hole1_Z = TW_THICK;//z determined by intersection with Front and Back Walls */

//hole away from centre (also for veto)
//const Double_t TW_ShiftOfHole2_X=0.0;//default
const Double_t TW_ShiftOfHole2_X=-11.5;
//const Double_t TW_ShiftOfHole2_X=11.35;
//const Double_t TW_ShiftOfHole2_X=-(TW_FrontLeftEdge-(TW_SpaceBetweenSlats_X/2.+(((float)TW_Hole2ModNb-1.)*8.+4.)*(TW_Slat_X+TW_SpaceBetweenSlats_X)));//-65.;//?value? //TODO: correct positioning, current formula just approximate

const Double_t TW_ShiftOfOnion_X=-11.5;

const Double_t TW_Hole2_X = 0.001;
const Double_t TW_Hole2_Y = 0.001;
//const Double_t TW_Hole2_X = 7.5;//default
//const Double_t TW_Hole2_Y = 3.4;//default
const Double_t TW_Hole2_Z = TW_THICK;//z determined by intersection with Front and Back Walls





// const Double_t TW_tofWallFL_X = (TW_NbModFrontL*8.+2.)*TW_Slat_X; 
// const Double_t TW_tofWallFL_Y = 110.;
// const Double_t TW_tofWallFL_Z = 1.;
// 
// const Double_t TW_tofWallFR_X = (TW_NbModFrontR*8.+3.)*TW_Slat_X; 
// const Double_t TW_tofWallFR_Y = 110.;
// const Double_t TW_tofWallFR_Z = 1.;
// 
// const Double_t TW_tofWallFH_X = 3.*TW_Slat_X; 
// const Double_t TW_tofWallFH_Y = 110.;
// const Double_t TW_tofWallFH_Z = 1.;
// 
// const Double_t TW_tofWallBL_X = (TW_NbModBackL*8.+2.)*TW_Slat_X; 
// const Double_t TW_tofWallBL_Y = 110.;
// const Double_t TW_tofWallBL_Z = 1.;
// 
// const Double_t TW_tofWallBR_X = (TW_NbModBackR*8.+3.)*TW_Slat_X; 
// const Double_t TW_tofWallBR_Y = 110.;
// const Double_t TW_tofWallBR_Z = 1.;
// 
// const Double_t TW_tofWallBH_X = 3.*TW_Slat_X;
// const Double_t TW_tofWallBH_Y = 110.;
// const Double_t TW_tofWallBH_Z = 1.;

//const TString  TW_BadSlats="";

//const G4double kX_TOF   = -410.*mm;
//const G4double kZ_TOF   = 5890.*mm;

// MC calibration parameters (changed 16/5/13)
								     
const Double_t TW_InvSigVel = 1.E-9 / 15.7;
//UNIT:s/cm //inverse signal velocity (1/(16.8cm/ns)) BC408 

// VM 9/1/2014 lambda changed according to new fits from Poli     
//const Double_t TW_Lambda = 0.0143; 
const Double_t TW_Lambda = 0.019422;
//UNIT:1/cm //attenuation length 

const Double_t TW_Ene_peak = 116.235; // calibration peak on data					     
// birk constant for energy corrections (taking into account the density)
const static Double_t TW_Birk_const= 0.00650/1.032;
							     
//***********************************************************
/* parametri del LAND */ 

const Double_t LA_HEIGHT	=300.;
const Double_t LA_WIDTH		=301.;
const Double_t LA_THICK		=5.;
//const Double_t LA_THICK		=30.;//for plotting

//const Double_t DIST_TG_LA       =300.;

const Double_t LA_Z		=252.42+685.;
const Double_t LA_Y		=0.0;
const Double_t LA_X		=0.0;
//const Double_t LA_X		=98.0;//for plotting


//************************************************************
// maximum array lengths

//const int MAXTR = 200;
const int MAXTR = 2000;
const int MAXST   = 1000;
const int MAXMON  = 1000;
const int MAXTG   = 1000;
const int MAXMI = 1000;
const int MAXMISIG  = 2000;
const int MAXKE   = 1000;
const int MAXMU   = 1000;
const int MAXTW   = 1000;
const int MAXON   = 1000;

//*************************************************************
//Constants for signals 

const static Double_t gBMN_v_drift= 1./400.;//UNITS: cm/ns

//KENTROS calibration parameters//////////////////////////////////////////////////////////////////  
//Bruno: Our scintillator has refractive index 1.58 at the scintillation wavelength, 
//so the speed of light in the scintillator should be 19.0 cm/ns. 
//The attenuation length is 4 meters, so the attenuation is small for distances < 50 cm. 
//However to be more precise you can take it into account and put 4 meters. 
///We'll try to take into account the calibration measurements as soon as we can. 
const static Double_t gKE_InvSigVel=1.E-9/19.0;//UNIT:s/cm //inverse signal velocity (1/(19.0 cm/ns))
const static Double_t gKE_Lambda=1./400.;//UNIT:1/cm //attenuation length (1/400 1/cm) 


const static Double_t gMU_v_drift = 5.5*1.E6;//UNITS:cm/s drift speed for MUSIC operation conditions (from M. De Napoli)
//2.380cm/us=2.380cm/1.E3ns



#endif



///***********************************************************************************************
///*** EOF ***************************************************************************************
///***********************************************************************************************

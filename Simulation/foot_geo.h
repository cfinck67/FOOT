#ifndef FOOT_GEOMETRY
#define FOOT_GEOMETRY


const Int_t kVerbosity		=0;

/*  constants   */
const Double_t  PI		=3.141592653589793238462643383279502884197169399375;
const Double_t  ZERO            =0.0;
const Double_t  FWHM2Sig	=2.354820;//conversion FWHM to sigma (Gaussian)


//************************************************************
// PRIMARY BEAM
//************************************************************

const Int_t  PRIM_Type  =-2;
/*  C-12  */
//const Int_t  PRIM_Z	=6;
//const Int_t  PRIM_A	=12;
/*  O-16  */
const Int_t  PRIM_Z	=8;
const Int_t  PRIM_A	=16;

const Double_t  PRIM_T		=0.20094; //GeV/n (kinetic energy of primaries) -> HIT
const TString   PRIM_DIST	="GAUSSIAN";//lateral beam distribution
const Double_t  PRIM_RMAX	=0.48;//maximum radius (FWHM) -> HIT
const Double_t  PRIM_dP		=0.0;//momentum spread (+=flat, -=Gaussian (FWHM) )
const Double_t  PRIM_DIV	=0.0;//in mrad (+=flat, -=Gaussian, 10000.=isotropic)
const Double_t  PRIM_Pos_X	=0.0;
const Double_t  PRIM_Pos_Y	=0.0;
const Double_t  PRIM_Pos_Z	=-30.0;

const Double_t  PRIM_Pos_CX	=0.0;//cos y
const Double_t  PRIM_Pos_CY	=0.0;//cos x


//************************************************************
// PHYSICS AND TRANSPORT SETTINGS
//************************************************************

// threshold for produciton and transport of EM particles (GeV)
const Double_t TRANS_THRES_EM       = 0.0001;
const Double_t PROD_THRES_EM        = 0.0001;
const Double_t DELTA_THRES_EM       = 0.0001;
const Double_t PAIRBREM_THRES_EM    = 0.0001;

const Double_t CALO_THRES_EM    = 0.001;
const Double_t MAG_THRES_EM     = 1.;
const Double_t AL_THRES_EM      = 0.005;


//************************************************************
// MAGNETIC FIELD
//************************************************************

// MagField = true if magnetic field is on
//          = false if magnetic field is off
//const bool MagField            =true;
const bool MagField            =false;


//************************************************************
// GEOMETRY & MEDIA
//************************************************************

//************************************************************
// Blackbody and Air

const TString  BLK_MEDIUM       ="BLCKHOLE";
const TString  AIR_MEDIUM	="AIR"; 

//************************************************************
// Start Counter

const Double_t STC_X		=0.0;//middle of STC
const Double_t STC_Y		=0.0;//middle of STC
const Double_t STC_Z		=-29.0;//beginning of STC
		 
const Double_t STC_RAD    	=2.6;//STC radius
const Double_t STC_THICK  	=250.E-6;//STC thickness (along z)
		 
const TString  STC_MEDIUM	="EJ-232"; 

//************************************************************
// Beam Monitor

const Double_t BMN_X 		=0.0;//middle of beam monitor
const Double_t BMN_Y 		=0.0;//middle of beam monitor
const Double_t BMN_Z 		=-14.0;//middle of beam monitor

const Double_t BMN_SHI_LENGTH  	=23.0;//length (along z) of beam monitor shield
const Double_t BMN_SHI_THICK   	=1.5;//thickness of beam monitor shield

const Double_t BMN_WIDTH  	=11.2;//width of beam monitor (gas)
const Double_t BMN_HEIGHT 	=11.2;//height of beam monitor (gas) 
const Double_t BMN_LENGTH    	=21.0;//length of beam monitor (gas)

const Int_t    BMN_NLAY         =6;//no. of layers
const Int_t    BMN_NWIRELAY     =21;//no. of wires (field or sense) per layer
const Int_t    BMN_NSENSELAY    =3;//no. of sense wires (and also cells) per layers

const Double_t BMN_UVIEW  	=1;//wire along x (side view)  //not used
const Double_t BMN_VVIEW  	=-1;//wire along y (top view)  //not used

//ATTENZIONE:controllare i prossimi parametri
const Double_t BMN_RSENSE 	=0.0015;//radius of sensitive wires//ATTENZIONE:in first erano 0.003
const Double_t BMN_RFIELD 	=0.0045;//radius of field wires//ATTENZIONE:in first erano 0.008
const Double_t BMN_STEP		=0.5;//distance between wires (z distance)
const Double_t BMN_PASSO	=0.8;//distance between wires (x or y distance)
const Double_t BMN_LAYDIST   	=0.3;//distance between layers U and V

//ATTENZIONE: da controllare
const Double_t BMN_DELTAY	=2.8;//wires x shift from the beam monitor internal sides
const Double_t BMN_DELTAX	=2.8;//wires y shift from the beam monitor internal sides
//Alessio's version: const Double_t BMN_DELTA_Z	=2.8;
const Double_t BMN_DELTAZ	=2.85;//wires z shift from the beam monitor internal sides

const Double_t BMN_MYL_THICK	=0.01;//mylar windows thickness

const TString  BMN_SHI_MEDIUM   ="ALUMINUM";
const TString  BMN_MYL_MEDIUM   ="Mylar";
const TString  BMN_GAS_MEDIUM   ="Ar-CO2";
const TString  BMN_SWIRE_MEDIUM ="TUNGSTEN";
const TString  BMN_FWIRE_MEDIUM ="ALUMINUM";

//************************************************************
// Target

const Double_t TG_X 		=0.0;//middle of target
const Double_t TG_Y 		=0.0;//middle of target
const Double_t TG_Z 		=0.0;//middle of target

const Double_t TG_WIDTH   	=1.5;//x dimension
const Double_t TG_HEIGHT  	=1.5;//y dimension
const Double_t TG_THICK   	=0.2;//z dimension

const TString  TG_MEDIUM        ="Polyethy";

//************************************************************
// Vertex

const Double_t VTX_X 		=0.0;//middle of vertex
const Double_t VTX_Y 		=0.0;//middle of vertex
const Double_t VTX_Z 		=1.1;//middle of vertex
				     
const Double_t VTX_WIDTH   	=2.0;//x dimension
const Double_t VTX_HEIGHT  	=2.0;//y dimension
const Double_t VTX_THICK   	=0.005;//z dimension

const Int_t    VTX_NLAY         =3;//no. of layers
const Double_t VTX_LAYDIST   	=0.5;//distrance btw layers

const TString  VTX_MEDIUM       ="SILICON";

//************************************************************
// Inner Tracker

const Double_t ITR_X 		=0.0;//middle of vertex
const Double_t ITR_Y 		=0.0;//middle of vertex
const Double_t ITR_Z 		=14.5;//middle of vertex
				     
const Double_t ITR_WIDTH   	=6.0;//x dimension
const Double_t ITR_HEIGHT  	=6.0;//y dimension
const Double_t ITR_THICK   	=0.005;//z dimension

const Int_t    ITR_NLAY         =2;
const Double_t ITR_LAYDIST   	=1.0;

const TString  ITR_MEDIUM       ="SILICON";

//************************************************************
// Magnets (Sanelli config)

const Double_t MAG_X 		=0.0;//middle of magnet
const Double_t MAG_Y 		=0.0;//middle of magnet
const Double_t MAG_Z 		=8.7;//middle of magnet

const Double_t MAG_DIST		=12.0;//distance btw magnets
const Int_t    MAG_N		=2;//no. of magnets

const Double_t MAG_CV_OUTRAD	=7.4;//outer radius of magnets Al cover
const Double_t MAG_CV_INRAD 	=3.5;//inner radius of magnets Al cover
const Double_t MAG_CV_LENGTH 	=7.4;//length of magnets Al cover
const Double_t MAG_PM_OUTRAD 	=7.2;//outer radius of permanent magnets
const Double_t MAG_PM_INRAD 	=3.7;//inner radius of permanent magnets
const Double_t MAG_PM_LENGTH 	=7.0;//length of permanent magnets

const Double_t MAG_AIR_X 	=0.0;//middle of magnetic field region
const Double_t MAG_AIR_Y 	=0.0;//middle of magnetic field region
const Double_t MAG_AIR_Z 	=MAG_Z+MAG_DIST/2;//middle of magnetic field region

const Double_t MAG_AIR_WIDTH	=10.0;//width of magnetic field region
const Double_t MAG_AIR_HEIGHT 	=10.0;//height of magnetic field region
const Double_t MAG_AIR_LENGTH 	=60.0;//length of magnets Al cover

const TString  MAG_PM_MEDIUM    ="SmCo";
const TString  MAG_CV_MEDIUM    ="ALUMINUM";
const TString  MAG_AIR_MEDIUM   ="AIR";

//************************************************************
// Drift Chamber

const Double_t DCH_X 		=0.0;//middle of drift chamber
const Double_t DCH_Y 		=0.0;//middle of drift chamber
const Double_t DCH_Z 		=37.7;//middle of drift chamber

const Double_t DCH_SHI_LENGTH  	=23.0;//length (along z) of drift chamber shield
const Double_t DCH_SHI_THICK   	=1.5;//thickness of drift chamber shield

const Double_t DCH_WIDTH  	=20.0;//width of drift chamber (gas)
const Double_t DCH_HEIGHT 	=20.0;//height of drift chamber (gas) 
const Double_t DCH_LENGTH    	=21.0;//length of drift chamber (gas)

const Int_t    DCH_NLAY         =6;//no. of layers
const Int_t    DCH_NWIRELAY     =39;//no. of wires (field or sense) per layer
const Int_t    DCH_NSENSELAY    =6;//no. of sense wires (and also cells) per layers

const Double_t DCH_UVIEW  	=1;//wire along x (side view)//not used
const Double_t DCH_VVIEW  	=-1;//wire along y (top view)//not used

//ATTENZIONE:controllare i prossimi parametri
const Double_t DCH_RSENSE 	=0.0015;//radius of sensitive wires//ATTENZIONE:in first erano 0.003
const Double_t DCH_RFIELD 	=0.0045;//radius of field wires//ATTENZIONE:in first erano 0.008
const Double_t DCH_STEP		=0.5;//distance between wires (z distance)
const Double_t DCH_PASSO	=0.8;//distance between wires (x or y distance)
const Double_t DCH_LAYDIST   	=0.3;//distance between layer U and V 

//ATTENZIONE: da controllare
const Double_t DCH_DELTAY	=4.8;//wires x shift from the beam monitor internal sides
const Double_t DCH_DELTAX	=4.8;//wires y shift from the beam monitor internal sides
//Alessio's version: const Double_t DCH_DELTA_Z	=2.8;
const Double_t DCH_DELTAZ	=2.85;//wires z shift from the beam monitor internal sides

const Double_t DCH_MYL_THICK	=0.01;//mylar windows thickness

const TString  DCH_SHI_MEDIUM   ="ALUMINUM";
const TString  DCH_MYL_MEDIUM   ="Mylar";
const TString  DCH_GAS_MEDIUM   ="Ar-CO2";
const TString  DCH_SWIRE_MEDIUM ="TUNGSTEN";
const TString  DCH_FWIRE_MEDIUM ="ALUMINUM";

//************************************************************
// Scintillator

const Double_t SCN_X 		=0.0;//middle of scintillator
const Double_t SCN_Y 		=0.0;//middle of scintillator
const Double_t SCN_Z 		=99.7;//middle of scintillator

const Double_t SCN_STRIP_WIDTH  =2.0;//strip dimension in x (I lay) or y (II lay)
const Double_t SCN_STRIP_HEIGHT =44.0;//strip dimension in y (I lay) or x (II lay) 
const Double_t SCN_STRIP_THICK  =0.3;//strip (layer) thickness

const Int_t SCN_NLAY            =2;//no. of layers
const Int_t SCN_NSTRIP          =SCN_STRIP_HEIGHT/SCN_STRIP_WIDTH;//no. of strip per layer

const TString  SCN_MEDIUM       ="EJ-232";

//************************************************************
// Calorimeter

const Double_t CAL_X 		=0.0;//middle of calorimeter
const Double_t CAL_Y 		=0.0;//middle of calorimeter
const Double_t CAL_Z 		=103.5;//middle of calorimeter

const Double_t CAL_WIDTH  	=44.0;//calorimeter dimension along X
const Double_t CAL_HEIGHT  	=CAL_WIDTH;//calorimeter dimension along Y

const Double_t CAL_CRY_WIDTH  	=2.0;//crystal dimension along X
const Double_t CAL_CRY_HEIGHT  	=CAL_CRY_WIDTH;//crystal dimension along Y
const Double_t CAL_CRY_THICK   	=7.0;//crystal thickness

const Int_t CAL_NROW            =CAL_WIDTH/CAL_CRY_WIDTH;//no. of crystal rows
const Int_t CAL_NCOL            =CAL_HEIGHT/CAL_CRY_HEIGHT;//no. of crystal columns

const TString  CAL_MEDIUM       ="BGO";


#endif

///*************************************************************
///*************************** EOF *****************************
///*************************************************************

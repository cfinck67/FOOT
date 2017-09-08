#ifndef FOOT_GEOMETRY
#define FOOT_GEOMETRY


const Int_t kVerbosity		=0;

/* FLUKA version: 0->pro  1->dev */
const Int_t FLK_VERS		=0;

/*  constants   */
const Double_t  PI		=3.141592653589793238462643383279502884197169399375;
const Double_t  ZERO            =0.0;
const Double_t  FWHM2Sig	=2.354820;//conversion FWHM to sigma (Gaussian)

const Double_t  GEV2MEV         =1.e3;
const Double_t  DEG2RAD 	=PI/180;//conversion FWHM to sigma (Gaussian)


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

const Double_t  PRIM_T		=0.2; //GeV/n (kinetic energy of primaries)
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
const Double_t TRANS_THRES_EM       = 1.;
const Double_t PROD_THRES_EM        = 1.;				      
const Double_t DELTA_THRES_EM       = 1.;

/* lowth=true to activate low thresholds in gas */
const bool lowth		    =false;
const Double_t GAS_THRES	    = 0.0001;

//************************************************************
// MAGNETIC FIELD
//************************************************************

/* MagField = true if magnetic field is on
            = false if magnetic field is off*/
const bool MagField            =true;
//const bool MagField            =false;

/* Largest angle in degrees that a charged particle is allowed to travel in a single step */
const Double_t MaxAng		=0.1;

/* Upper limit to the error of the boundary iteration in cm */
const double BoundAcc		=0.00001;

/* Bx,By,Bz -> value of the mag field. To use a mag field map -> set Bx=By=Bz=0 (required TString)*/
const double Bx               =0.;
const double By               =0.;
const double Bz               =0.;

/* VacChamber = true if you want a vacuum chamber in the mag region*/
//const bool VacChamber          =true;
const bool VacChamber          =false;
//VAC_MAG = true if mag field region includes VAC
const bool VAC_MAG		=false;


//************************************************************
// GEOMETRY & MEDIA
//************************************************************

//************************************************************
// Blackbody and Air

const TString  BLK_MEDIUM       ="BLCKHOLE";
const TString  AIR_MEDIUM	="AIR"; 

//************************************************************
// Start Counter

const Double_t STC_X		=0.0;//center x coordinate
const Double_t STC_Y		=0.0;//center y coordinate
const Double_t STC_Z		=-29.0;//center z coordinate
		 
const Double_t STC_RAD    	=2.6;//STC radius
const Double_t STC_THICK  	=0.025;//STC thickness (z dimension)
		 
const TString  STC_MEDIUM	="EJ-232"; 

//************************************************************
// Beam Monitor

const Double_t BMN_X 		=0.0;//center x coordinate
const Double_t BMN_Y 		=0.0;//center y coordinate
const Double_t BMN_Z 		=-14.0;//center z coordinate

const Double_t BMN_SHI_LENGTH  	=23.0;//length (z dimension) of beam monitor shield
const Double_t BMN_SHI_THICK   	=1.5;//thickness of beam monitor shield

const Double_t BMN_WIDTH  	=11.2;//width of beam monitor  (gas region)
const Double_t BMN_HEIGHT 	=11.2;//height of beam monitor (gas region) 
const Double_t BMN_LENGTH    	=21.0;//length of beam monitor (gas region)

const Int_t    BMN_NLAY         =6;//no. of layers
const Int_t    BMN_NSENSELAY    =3;//no. of sense wires (-> number of cells) per layers
const Int_t    BMN_NWIRELAY     =6*BMN_NSENSELAY+3;//tot no. of wires (field or sense) per layer

const Double_t BMN_UVIEW  	=1;//wire along x (side view) 
const Double_t BMN_VVIEW  	=-1;//wire along y (top view) 

//ATTENZIONE:controllare i prossimi parametri
const Double_t BMN_RSENSE 	=0.0015;//radius of sensitive wires (in first erano 0.003)
const Double_t BMN_RFIELD 	=0.0045;//radius of field wires (in first erano 0.008)
const Double_t BMN_STEP		=0.5;//distance between wires (z distance)
const Double_t BMN_PASSO	=0.8;//distance between wires (x or y distance)
const Double_t BMN_LAYDIST   	=0.3;//distance between layers U and V

//ATTENZIONE: da controllare
const Double_t BMN_DELTAY	=2.8;//wires x shift from the beam monitor internal sides
const Double_t BMN_DELTAX	=2.8;//wires y shift from the beam monitor internal sides 
const Double_t BMN_DELTAZ	=2.85;//wires z shift from the beam monitor internal sides (Alessio's version in FIRST: const Double_t BMN_DELTA_Z	=2.8;)

const Double_t BMN_MYL_THICK	=0.0025;//mylar windows thickness

const TString  BMN_SHI_MEDIUM   ="ALUMINUM";
const TString  BMN_MYL_MEDIUM   ="Mylar";
const TString  BMN_GAS_MEDIUM   ="Ar-CO2";
const TString  BMN_SWIRE_MEDIUM ="TUNGSTEN";
const TString  BMN_FWIRE_MEDIUM ="ALUMINUM";

//BMN_MAG = true if mag field region includes BMN
const bool BMN_MAG		=false;

//************************************************************
// Target

const Double_t TG_X 		=0.0;//center x coordinate
const Double_t TG_Y 		=0.0;//center y coordinate
const Double_t TG_Z 		=0.0;//center z coordinate

const Double_t TG_WIDTH   	=1.5;//x dimension
const Double_t TG_HEIGHT  	=1.5;//y dimension
const Double_t TG_THICK   	=0.2;//z dimension

const TString  TG_MEDIUM        ="Polyethy";
//const TString  TG_MEDIUM        ="CARBON";

//TG_MAG = true if mag field region includes TG
const bool TG_MAG		=true;

//************************************************************
// Vertex

const Double_t VTX_X 		=0.0;//center x coordinate
const Double_t VTX_Y 		=0.0;//center y coordinate
const Double_t VTX_Z 		=1.35;//center z coordinate
				     
const Double_t VTX_WIDTH   	=2.0;//x dimension
const Double_t VTX_HEIGHT  	=2.0;//y dimension
const Double_t VTX_THICK   	=0.005;//z dimension

const Int_t    VTX_NLAY         =4;//no. of layers
const Double_t VTX_LAYDIST   	=0.5;//distrance btw layers

//pixel parameters
const Double_t VTX_XMIN		=-VTX_WIDTH/2.;
const Double_t VTX_YMIN		=-VTX_HEIGHT/2.;
const Double_t VTX_DX		=0.002;//x dimension of pixel
const Double_t VTX_DY		=VTX_DX;//y dimension of pixel
const Int_t VTX_XPIX		=int(VTX_WIDTH/VTX_DX);//n. pixel in x
const Int_t VTX_YPIX		=int(VTX_HEIGHT/VTX_DY);//n. pixel in y

const TString  VTX_MEDIUM       ="SILICON";

//VTX_MAG = true if mag field region includes VTX
const bool VTX_MAG		=true;

//************************************************************
// Inner Tracker

const Double_t ITR_X 		=0.0; //center x coordinate
const Double_t ITR_Y 		=0.0; //center y coordinate
const Double_t ITR_Z 		=14.;//center z coordinate
				     
const Double_t ITR_WIDTH   	=6.0;//x dimension
const Double_t ITR_HEIGHT  	=6.0;//y dimension
const Double_t ITR_THICK   	=0.005;//z dimension

const Double_t ITR_EPO_THICK    =0.001;//epoxy glue layers thickness
const Double_t ITR_COV_THICK    =0.0025;//cover layers thickness
const Double_t ITR_AL_THICK     =0.001;//aluminum layers thickness
const Double_t ITR_KAP_THICK    =0.005;//kaptron layers thickness
const Double_t ITR_FOAM_THICK   =0.2;//foam layer thickness

const Int_t    ITR_NLAY         =2;
const Double_t ITR_LAYDIST	=1.;

//pixel parameters
const Double_t ITR_XMIN		=-ITR_WIDTH/2.;
const Double_t ITR_YMIN		=-ITR_HEIGHT/2.;
const Double_t ITR_DX		=0.002;//x dimension of pixel
const Double_t ITR_DY		=ITR_DX;//y dimension of pixel
const Int_t ITR_XPIX		=int(ITR_WIDTH/ITR_DX);//n. pixel in x
const Int_t ITR_YPIX		=int(ITR_HEIGHT/ITR_DY);//n. pixel in y

const TString  ITR_MEDIUM       ="SILICON";//mimosa28
const TString  ITR_COV_MEDIUM   ="KAPTON";
const TString  ITR_EPO_MEDIUM   ="Epoxy";
const TString  ITR_AL_MEDIUM    ="ALUMINUM";
const TString  ITR_KAP_MEDIUM   ="KAPTON";
const TString  ITR_FOAM_MEDIUM  ="SiCFoam";

//ITR_MAG = true if mag field region includes ITR
const bool ITR_MAG		=true;

//************************************************************
// Magnets

const Double_t MAG_X 		=0.0;//center x coordinate of the first magnet
const Double_t MAG_Y 		=0.0;//center y coordinate of the first magnet
const Double_t MAG_Z 		=8.;//center z coordinate of the first magnet

const Double_t MAG_DIST		=12.0;//distance btw magnets
const Int_t    MAG_N		=2;//no. of magnets

const Double_t MAG_ANG		=10.*DEG2RAD;//required semiapertura angolare

const Double_t MAG_PM_THICK 	=3.5;//thickness of permanent magnets
const Double_t MAG_PM_LENGTH 	=10.;//length of permanent magnets
const Double_t MAG_CV_THICK 	=0.2;//thickness of Al cover
const Double_t MAG_CV_LENGTH 	=MAG_PM_LENGTH+2*MAG_CV_THICK;//length of magnets Al cover

const Double_t MAG_CV0_INRAD 	=2.5;//(MAG_Z+MAG_CV_LENGTH/2-TG_Z)*tan(MAG_ANG);//inner radius of magnet 0 Al cover
const Double_t MAG_PM0_INRAD 	=MAG_CV0_INRAD+MAG_CV_THICK;//inner radius of permanent magnet 0
const Double_t MAG_PM0_OUTRAD 	=MAG_PM0_INRAD+MAG_PM_THICK;//outer radius of permanent magnet 0
const Double_t MAG_CV0_OUTRAD	= MAG_PM0_OUTRAD+MAG_CV_THICK;//outer radius of magnet 0 Al cover

const Double_t MAG_CV1_INRAD 	=4.6;//(MAG_Z+MAG_CV_LENGTH/2+MAG_DIST-TG_Z)*tan(MAG_ANG);//inner radius of magnet 1 Al cover
const Double_t MAG_PM1_INRAD 	=MAG_CV1_INRAD+MAG_CV_THICK;//inner radius of permanent magnet 1
const Double_t MAG_PM1_OUTRAD 	=MAG_PM1_INRAD+MAG_PM_THICK;//outer radius of permanent magnet 1
const Double_t MAG_CV1_OUTRAD	= MAG_PM1_OUTRAD+MAG_CV_THICK;//outer radius of magnet 1 Al cover

const Double_t MAG_AIR_X 	=0.0;//_mean of magnetic field region
const Double_t MAG_AIR_Y 	=0.0;//_mean of magnetic field region
const Double_t MAG_AIR_Z 	=MAG_Z+MAG_DIST/2;//_mean of magnetic field region

const Double_t MAG_AIR_WIDTH	=10.0;//width of magnetic field region
const Double_t MAG_AIR_HEIGHT 	=10.0;//height of magnetic field region
const Double_t MAG_AIR_LENGTH 	=60.0;//length of magnets

const TString  MAG_PM_MEDIUM    ="SmCo";
const TString  MAG_CV_MEDIUM    ="ALUMINUM";
const TString  MAG_AIR_MEDIUM   =AIR_MEDIUM;

//MAG_AIR_MAG = true if mag field region includes MAG_AIR
const bool MAG_AIR_MAG		=true;

//************************************************************
// Silicon MicroStrip Detector

const double MSD_X		=0.0;
const double MSD_Y		=0.0;
const double MSD_Z		=29.0;
				     
const Double_t MSD_WIDTH   	=9.0;//x dimension
const Double_t MSD_HEIGHT  	=9.0;//y dimension
const Double_t MSD_THICK   	=0.007;//z dimension
				     
const Double_t MSD_KAP_WIDTH   	=MSD_WIDTH;
const Double_t MSD_KAP_HEIGHT  	=MSD_HEIGHT;
const Double_t MSD_KAP_THICK   	=0.003; 

const Int_t    MSD_NVIEW        =2;//no. of layers
const Int_t    MSD_NLAY         =3;//no. of layers
const Double_t MSD_LAYDIST   	=2.;//distrance btw layers

//strip parameters
const Double_t MSD_XMIN		=-MSD_WIDTH/2.;
const Double_t MSD_YMIN		=-MSD_HEIGHT/2.;
const Double_t MSD_DX		=0.0125;//x dimension of strip
const Double_t MSD_DY		=MSD_DX;//y dimension of strip
const Int_t MSD_XSTRIP		=int(MSD_WIDTH/MSD_DX);//n. strip in x
const Int_t MSD_YSTRIP		=int(MSD_HEIGHT/MSD_DY);//n. strip in y

const TString  MSD_KAP_MEDIUM   ="KAPTON";
const TString  MSD_MEDIUM       ="SILICON";

//MSD_MAG = true if mag field region includes MSD
const bool MSD_MAG		=true;

//************************************************************
// Drift Chamber
/*
const Double_t DCH_X 		=0.0; //center x coordinate
const Double_t DCH_Y 		=0.0; //center y coordinate
//const Double_t DCH_Z 		=37.7;//center z coordinate
const Double_t DCH_Z 		=43.7+5.;//center z coordinate

const Double_t DCH_SHI_LENGTH  	=23.0;//length (along z) of drift chamber shield
const Double_t DCH_SHI_THICK   	=1.5;//thickness of drift chamber shield

const Double_t DCH_WIDTH  	=25.0;//width of drift chamber (gas region)
const Double_t DCH_HEIGHT 	=DCH_WIDTH;//height of drift chamber (gas region) 
const Double_t DCH_LENGTH    	=21.0;//length of drift chamber (gas region)

const Int_t    DCH_NLAY         =6;//no. of layers
const Int_t    DCH_NSENSELAY    =12;//no. of sense wires (->no. of cells) per layers
const Int_t    DCH_NWIRELAY     =6*DCH_NSENSELAY+3;//tot no. of wires (field or sense) per layer

const Double_t DCH_UVIEW  	=1;//wire along x (side view)
const Double_t DCH_VVIEW  	=-1;//wire along y (top view)

//ATTENZIONE:controllare i prossimi parametri
const Double_t DCH_RSENSE 	=0.0015;//radius of sensitive wires (in first erano 0.003)
const Double_t DCH_RFIELD 	=0.0045;//radius of field wires (in first erano 0.008)

const Double_t DCH_STEP		=0.5;//distance between wires (z distance)
const Double_t DCH_PASSO	=0.8;//distance between wires (x or y distance)
const Double_t DCH_LAYDIST   	=0.3;//distance between layer U and V 

const Double_t DCH_DELTAY	=DCH_HEIGHT/2.-DCH_PASSO*DCH_NSENSELAY-DCH_PASSO/2;//wires x shift from the beam monitor internal sides
const Double_t DCH_DELTAX	=DCH_HEIGHT/2.-DCH_PASSO*DCH_NSENSELAY-DCH_PASSO/2;//wires y shift from the beam monitor internal sides
const Double_t DCH_DELTAZ	=2.85;//ATTENZIONE: da controllare: wires z shift from the beam monitor internal sides (Alessio's version: const Double_t DCH_DELTA_Z=2.8)

const Double_t DCH_MYL_THICK	=0.0025;//mylar windows thickness

const TString  DCH_SHI_MEDIUM   ="ALUMINUM";
const TString  DCH_MYL_MEDIUM   ="Mylar";
const TString  DCH_GAS_MEDIUM   ="Ar-CO2";
const TString  DCH_SWIRE_MEDIUM ="TUNGSTEN";
const TString  DCH_FWIRE_MEDIUM ="ALUMINUM";

//DCH_MAG = true if mag field region includes DCH
const bool DCH_MAG		=false;
*/
//************************************************************
// Scintillator

const Double_t SCN_STRIP_WIDTH  =2.0;//strip dimension in x (I lay) or y (II lay)
const Double_t SCN_STRIP_HEIGHT =44.0;//strip dimension in y (I lay) or x (II lay) 
const Double_t SCN_STRIP_THICK  =0.3;//strip (layer) thickness

const Double_t SCN_X 		=0.0; //center x coordinate
const Double_t SCN_Y 		=0.0; //center y coordinate
const Double_t SCN_Z 		=100.-SCN_STRIP_THICK;//center z coordinate

const Int_t SCN_NLAY            =2;//no. of layers
const Int_t SCN_NSTRIP          =SCN_STRIP_HEIGHT/SCN_STRIP_WIDTH;//no. of strip per layer

const TString  SCN_MEDIUM       ="EJ-232";

//************************************************************
// Calorimeter

const Double_t CAL_WIDTH  	=44.0;//calorimeter dimension along X
const Double_t CAL_HEIGHT  	=CAL_WIDTH;//calorimeter dimension along Y

const Double_t CAL_CRY_WIDTH  	=2.0;//crystal dimension along X
const Double_t CAL_CRY_HEIGHT  	=CAL_CRY_WIDTH;//crystal dimension along Y
const Double_t CAL_CRY_THICK   	=14.0;//crystal thickness


const Double_t CAL_X 		=0.0;  //center x coordinate
const Double_t CAL_Y 		=0.0;  //center y coordinate
const Double_t CAL_Z 		=100.+CAL_CRY_THICK/2.;//center z coordinate

const Int_t CAL_NROW            =CAL_WIDTH/CAL_CRY_WIDTH;//no. of crystal rows
const Int_t CAL_NCOL            =CAL_HEIGHT/CAL_CRY_HEIGHT;//no. of crystal columns

const TString  CAL_MEDIUM       ="BGO";


#endif

///*************************************************************
///*************************** EOF *****************************
///*************************************************************

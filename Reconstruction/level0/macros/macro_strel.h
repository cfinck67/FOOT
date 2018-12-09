#define debug 0
#define DEG2RAD	 0.01745329251994329577
#define RAD2DEG	 57.2957795130823208768
#define BMNEV            1000 //number of bm events to analyze
#define MSDNEV           1000 //number of msd events to analyze
#define STBIN          100 //strel binning on rdrift
#define MAXBMHITNUM     30      //number of maximum bm hit 


//~ #include "../../../Simulation/foot_geo.h"

//bm theorical geo par
#define BMISOZ         -22.   //position of the isocenter with respect to bm center in the BM syst 
#define BMISOX         0.     //position of the isocenter with respect to bm center in the BM syst        
#define BMISOY         0.     //position of the isocenter with respect to bm center in the BM syst        
#define BMISOXZANGLE   0.     //rotation of the isocenter with respect to bm center in the BM syst        
#define BMISOYZANGLE   0.     //rotation of the isocenter with respect to bm center in the BM syst        

//bm residual calculated shift & rotation
#define BMSHIFTX       0.221057
#define BMSHIFTY       -0.162858
#define BMSHIFTZ       0.
#define BMYZANGLE      0.715066     //bm rotation around X axis (provv)        
#define BMXZANGLE      -0.0753088    //bm rotation around Y axis (provv)        

#define MSD1Z         10.     //position of the MSD first plane respect to bm (to be evaluated!!!!!!!)        
#define MSD2Z         20.     //position of the MSD second plane respect to bm (to be evaluated!!!!!!!)        
#define MSD3Z         30.     //position of the MSD third plane respect to bm (to be evaluated!!!!!!!)        
//msd calibration par...???

//bm fixed par
#define BMN_WIDTH  	    11.2    //width of beam monitor  (gas region)
#define BMN_HEIGHT 	    11.2    //height of beam monitor (gas region) 
#define BMN_LENGTH    	21.0    //length of beam monitor (gas region)
#define BMN_MYL_THICK	   0.0025 //length of beam monitor (gas region)
#define BMN_PASSO	       0.8    //distance between wires (x or y distance)
#define BMN_NWIRELAY	   21    
#define BMN_NLAY	        6    
#define BMN_STEP	        0.5
#define BMN_DELTAX	      2.8
#define BMN_DELTAY	      2.8
#define BMN_DELTAZ	      2.85
#define BMN_LAYDIST	      0.3
    

//msd par
#define MSDCUT 3.


typedef struct BM_evstruct {
  int evnum;                  //number of event
  int timeacq;
  int hitnum;//not from ttree
  double bm_track_chi2;
  TVector3 bm_track_pvers;//in the bm system of reference
  TVector3 bm_track_r0pos;//in the bm system of reference
  double bm_hit_rdrift[MAXBMHITNUM];
  double bm_hit_residual[MAXBMHITNUM];
  double bm_hit_time[MAXBMHITNUM];
  int bm_hit_cellid[MAXBMHITNUM];

} BM_evstruct;

typedef struct MSD_evstruct {
  int evnum;                  //number of event
  int timeacq;
  double msd_track_chi2;
  TVector3 msd_track_pvers;//what sys of ref.?
  TVector3 msd_track_r0pos;//?
  TVector3 msd_track_r0err;//?
  double thetaerr;
  double phierr;
} MSD_evstruct;

//general functions
void Booking(TFile* f_out);
void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<MSD_evstruct> &allmsdeventin, vector<vector<vector<double>>> &space_residual);
void EvaluateSpaceResidual(vector<vector<vector<double>>> &space_residual, BM_evstruct &bmevent, MSD_evstruct &msdevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);

//bm functions
TVector3 BMlocaltoiso(TVector3 local);//from local to labo syst of ref., including the shift
void clean_bmevstruct(BM_evstruct &bmevstruct, bool forced);
bool bmreadevent(TTreeReader &bmReader, BM_evstruct &bmevent, TTreeReaderValue<int> &evnumreader,   TTreeReaderValue<int> &timeacqreader, TTreeReaderValue<double> &trackchi2reader, TTreeReaderValue<double> &pversxreader,   TTreeReaderValue<double> &pversyreader, TTreeReaderValue<double> &pverszreader,   TTreeReaderValue<double> &r0xreader,   TTreeReaderValue<double> &r0yreader,   TTreeReaderValue<double> &rdriftreader,   TTreeReaderValue<double> &residualreader,   TTreeReaderValue<double> &hittimereader,   TTreeReaderValue<int> &planereader,  TTreeReaderValue<int> &viewreader,   TTreeReaderValue<int> &cellreader);
TVector3 BMProjections(TVector3 pvers, TVector3 r0pos, double proposz, bool global);
void setbmgeo(vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);
bool Getlvc(const Int_t cellid, Int_t& ilay, Int_t& iview, Int_t& icell);
Double_t FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers);
Int_t GetBMNcell(Int_t ilay, Int_t iview, Int_t icell){return icell+((iview==-1) ? 1:0)*3+ilay*6;};
void print_bmevstruct(BM_evstruct &bmevstruct);

//msd functions
void clean_msdevstruct(MSD_evstruct &msdevstruct);
bool msdreadevent(TTreeReader &msdReader, MSD_evstruct &msdevent, TTreeReaderValue<int> &evnumMSDreader,   TTreeReaderValue<int> &timeacqMSDreader, TTreeReaderValue<double> &trackchi2MSDreader, TTreeReaderValue<double> &thetaMSDreader,   TTreeReaderValue<double> &thetaerrMSDreader, TTreeReaderValue<double> &phiMSDreader,   TTreeReaderValue<double> &phierrMSDreader,   TTreeReaderValue<double> &r0xMSDreader,   TTreeReaderValue<double> &r0xerrMSDreader,   TTreeReaderValue<double> &r0yMSDreader,   TTreeReaderValue<double> &r0yerrMSDreader,   TTreeReaderValue<double> &r0zMSDreader,  TTreeReaderValue<double> &r0zerrMSDreader);
void print_msdevstruct(MSD_evstruct &msdevstruct);


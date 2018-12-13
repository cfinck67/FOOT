//fixed par
#define DEG2RAD	 0.01745329251994329577
#define RAD2DEG	 57.2957795130823208768

//general par
#define debug            0
#define BMNEV            0    //number of bm events to analyze
#define MSDNEV           0    //number of msd events to analyze
#define STBIN          100    //strel binning on rdrift
#define MSDCUT          10.   //cut on msd track chi2 
#define BMNCUT          10.   //cut on bm track chi2 
#define MAXBMHITNUM     30    //number of maximum bm hit 
#define NUMEVTSHIFT      0    //shift of the MSD evnum with respect to BM evnum (to be checked carefully!!)     

//~ #include "../../../Simulation/foot_geo.h"

//bm theorical geo par
#define BMISOZ         -22.   //position of the isocenter with respect to bm center in the BM syst 
#define BMISOX         0.     //position of the isocenter with respect to bm center in the BM syst        
#define BMISOY         0.     //position of the isocenter with respect to bm center in the BM syst        
#define BMISOXZANGLE   0.     //rotation of the isocenter with respect to bm center in the BM syst        
#define BMISOYZANGLE   0.     //rotation of the isocenter with respect to bm center in the BM syst        

//bm residual calculated shift & rotation
#define BMSHIFTX       0.
#define BMSHIFTY       0.
#define BMSHIFTZ       0.
#define BMYZANGLE      0.     //bm rotation around X axis in deg (provv)        
#define BMXZANGLE      0.    //bm rotation around Y axis in deg (provv)        

//msd layer positions
#define MSD1Z         -6.9     //position of the MSD first plane respect to iso. (to be evaluated!!!!!!!)        
#define MSD2Z         31.     //position of the MSD second plane respect to iso. (to be evaluated!!!!!!!)        
#define MSD3Z         45.8     //position of the MSD third plane respect to iso. (to be evaluated!!!!!!!)        
#define MSDISOX       0.      //position of the isocenter with respect to MSD track ref.
#define MSDISOY       0.      //position of the isocenter with respect to MSD track ref.
#define MSDISOZ       0.      //position of the isocenter with respect to MSD track ref.

//msd residual calculated shift & rotation (a tentative)
#define MSDSHIFTX       0.
#define MSDSHIFTY       0.
#define MSDSHIFTZ       0.
#define MSDYZANGLE      0.     //msd rotation around X axis in deg (provv)        
#define MSDXZANGLE      0.     //msd rotation around Y axis in deg (provv)  

//bm fixed par
#define BMN_MYLAR1Z	      -10.50125 //mylar1 position with respecto to bm center
#define BMN_MYLAR2Z	      10.50125

//provv: ISO system: where everything is alligned 

typedef struct BM_evstruct {
  int evnum;                  //number of event
  int timeacq;                //microseconds
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
  //~ int timeacq;                //units??
  double msd_track_chi2;      //chi2 or chi2red? 
  TVector3 msd_track_pvers;//what sys of ref.?
  TVector3 msd_track_r0pos;//?
  TVector3 msd_track_r0err;//?
  double thetaerr;
  double phierr;
} MSD_evstruct;

//general functions
void Booking(TFile* f_out);
void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<MSD_evstruct> &allmsdeventin, vector<vector<vector<double>>> &space_residual, vector<vector<int>> &selected_index);
void EvaluateSpaceResidual(vector<vector<vector<double>>> &space_residual, BM_evstruct &bmevent, MSD_evstruct &msdevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);
void Allign_estimate();

//bm functions
TVector3 BMlocaltoiso(TVector3 local);//from local to labo syst of ref., including the shift
void clean_bmevstruct(BM_evstruct &bmevstruct, bool forced);
bool bmreadevent(TTreeReader &bmReader, BM_evstruct &bmevent, TTreeReaderValue<int> &evnumreader,   TTreeReaderValue<int> &timeacqreader, TTreeReaderValue<double> &trackchi2reader, TTreeReaderValue<double> &pversxreader,   TTreeReaderValue<double> &pversyreader, TTreeReaderValue<double> &pverszreader,   TTreeReaderValue<double> &r0xreader,   TTreeReaderValue<double> &r0yreader,   TTreeReaderValue<double> &rdriftreader,   TTreeReaderValue<double> &residualreader,   TTreeReaderValue<double> &hittimereader,   TTreeReaderValue<int> &planereader,  TTreeReaderValue<int> &viewreader,   TTreeReaderValue<int> &cellreader);
TVector3 ExtrapolateZ(TVector3 pvers, TVector3 r0pos, double proposz, bool global,  bool beammonitor);
void setbmgeo(vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);
bool Getlvc(const Int_t cellid, Int_t& ilay, Int_t& iview, Int_t& icell);
Double_t FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers);
Int_t GetBMNcell(Int_t ilay, Int_t iview, Int_t icell){return icell+((iview==-1) ? 1:0)*3+ilay*6;};
void print_bmevstruct(BM_evstruct &bmevstruct);

//msd functions
void clean_msdevstruct(MSD_evstruct &msdevstruct);
bool msdreadevent(TTreeReader &msdReader, MSD_evstruct &msdevent, TTreeReaderValue<int> &evnumMSDreader, TTreeReaderValue<double> &trackchi2MSDreader, TTreeReaderValue<double> &thetaMSDreader,   TTreeReaderValue<double> &thetaerrMSDreader, TTreeReaderValue<double> &phiMSDreader,   TTreeReaderValue<double> &phierrMSDreader,   TTreeReaderValue<double> &r0xMSDreader,   TTreeReaderValue<double> &r0xerrMSDreader,   TTreeReaderValue<double> &r0yMSDreader,   TTreeReaderValue<double> &r0yerrMSDreader,   TTreeReaderValue<double> &r0zMSDreader,  TTreeReaderValue<double> &r0zerrMSDreader);
void print_msdevstruct(MSD_evstruct &msdevstruct);


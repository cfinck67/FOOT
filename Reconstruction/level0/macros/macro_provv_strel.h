//fixed par
#define DEG2RAD	 0.01745329251994329577
#define RAD2DEG	 57.2957795130823208768

//general par
#define debug            0
#define BMNEV            0    //number of bm events to analyze
#define MSDNEV           0    //number of msd events to analyze
#define STBIN          100    //strel binning on rdrift
#define BMNCUT          10.   //cut on bm track chi2 
#define MAXBMHITNUM     30    //number of maximum bm hit 
#define NUMEVTSHIFT      1    //shift of the MSD evnum with respect to BM evnum (to be checked carefully!!)     
#define NUMEVT2SHIFT     2    //shift of the MSD evnum with respect to BM evnum after changeshift 
#define CHANGESHIFT      91432//event number in which numevtshift change to numevt2shift      

//~ #include "../../../Simulation/foot_geo.h"

//bm theorical geo par
#define BMISOZ         11.5   //position of the BM with respect to ISO 
#define BMISOX         0.             
#define BMISOY         0.             
#define BMISOXZANGLE   0.     //rotation of the BM with respect to ISO        
#define BMISOYZANGLE   0.             

//bm residual calculated shift & rotation
#define BMSHIFTX       0.0202608
#define BMSHIFTY       0.0521604
#define BMSHIFTZ       0.
#define BMYZANGLE      -0.0509621     //bm rotation around X axis in deg (provv)        
#define BMXZANGLE      0.472936    //bm rotation around Y axis in deg (provv)        

//msd layer positions
#define MSD1Z         -4.45     //[cm] position of the MSD first plane respect to iso in         
#define MSD1X         800.199 //[channel] position of the MSD first plane respect to iso,      
#define MSD1Y         273.418        //[channel] position of the MSD first plane respect to iso.         
#define MSD2X         322.837        //[channel] position of the MSD second plane respect to iso.         
#define MSD2Z         28.65     //[cm] position of the MSD second plane respect to iso.         

#define MSD1PASSX     0.0104     //pitch size in cm          
#define MSD1PASSY     0.011      //         
#define MSD2PASSX     0.011      //         


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
  int evnum;        //number of event
  double x1val;     //position in cm
  double y1val;
  double x2val;
  int x1raw;        //raw data (strip channel)
  int y1raw;
  int x2raw;
  TVector3 msd_track_pvers;//what sys of ref.?
  TVector3 msd_track_r0pos;//?
  bool trackset;
} MSD_evstruct;

//general functions
void readallMSDfile();
void BookingBMMSD(TFile* f_out, bool onlyMSD);
void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<MSD_evstruct> &allmsdeventin, vector<vector<vector<double>>> &space_residual, vector<vector<int>> &selected_index, bool onlyMSD);
void EvaluateSpaceResidual(vector<vector<vector<double>>> &space_residual, BM_evstruct &bmevent, MSD_evstruct &msdevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);
void Allign_estimate();
void fitPositionResidual();

//bm functions
TVector3 BMlocaltoiso(TVector3 local);//from local to labo syst of ref., including the shift
void clean_bmevstruct(BM_evstruct &bmevstruct, bool forced);
bool bmreadevent(TTreeReader &bmReader, BM_evstruct &bmevent, TTreeReaderValue<int> &evnumreader,   TTreeReaderValue<int> &timeacqreader, TTreeReaderValue<double> &trackchi2reader, TTreeReaderValue<double> &pversxreader,   TTreeReaderValue<double> &pversyreader, TTreeReaderValue<double> &pverszreader,   TTreeReaderValue<double> &r0xreader,   TTreeReaderValue<double> &r0yreader,   TTreeReaderValue<double> &rdriftreader,   TTreeReaderValue<double> &residualreader,   TTreeReaderValue<double> &hittimereader,   TTreeReaderValue<int> &planereader,  TTreeReaderValue<int> &viewreader,   TTreeReaderValue<int> &cellreader);
TVector3 ExtrapolateZ(TVector3 pvers, TVector3 r0pos, double proposz, bool global,  bool beammonitor);
void setbmgeo(vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);
bool Getlvc(const Int_t cellid, Int_t& ilay, Int_t& iview, Int_t& icell);
Double_t FindRdrift(TVector3 pos, TVector3 dir, TVector3 A0, TVector3 Wvers);
Int_t GetBMNcell(Int_t ilay, Int_t iview, Int_t icell){return icell+iview*3+ilay*6;};
void print_bmevstruct(BM_evstruct &bmevstruct);
int CellId2lay(Int_t cellid){return (int)(cellid/6);}; 
int CellId2view(Int_t cellid){return (((int)(cellid/3))%2==0) ? 0:1;}; 
  
//msd functions
void clean_msdevstruct(MSD_evstruct &msdevstruct);
bool msdreadevent(MSD_evstruct &msdevent, TTreeReader &msd1Reader,TTreeReader &msd2Reader, TTreeReaderValue<double> &x1msd, TTreeReaderValue<double> &y1msd, TTreeReaderValue<double> &x2msd);
void print_msdevstruct(MSD_evstruct &msdevstruct);
void fitMSDgraph();


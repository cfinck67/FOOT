//fixed par
#define DEG2RAD	 0.01745329251994329577
#define RAD2DEG	 57.2957795130823208768

//general par
#define debug            0
#define BMNEV            0    //number of bm events to analyze
#define VTXNEV           1000    //number of msd events to analyze
#define STBIN          100    //strel binning on rdrift/tdrift
#define BMNCUT          1.   //cut on bm track chi2 
#define MAXBMHITNUM     30    //number of maximum bm hit 
#define NUMEVTSHIFT      0    //shift of the MSD evnum with respect to BM evnum (to be checked carefully!!)     
#define NUMEVT2SHIFT     0    //shift of the MSD evnum with respect to BM evnum after changeshift 
#define CHANGESHIFT      999999999 //event number in which numevtshift change to numevt2shift      
#define CORRMINIMUM      0.3 //minimum value of the correlation factor between the pvers of the BM and the vtx to evaluate the new strel      

//~ #include "../../../Simulation/foot_geo.h"

//bm theorical geo par
#define BMISOZ         -14.   //position of the BM with respect to ISO 
#define BMISOX         0.             
#define BMISOY         0.             
#define BMISOXZANGLE   0.     //rotation of the BM with respect to ISO        
#define BMISOYZANGLE   0.             

//bm residual calculated shift & rotation
#define BMSHIFTX       0.
#define BMSHIFTY       0.
#define BMSHIFTZ       0.
#define BMYZANGLE      0.     //bm rotation around X axis in deg (provv)        
#define BMXZANGLE      0.    //bm rotation around Y axis in deg (provv)        

//VTX residual calculated shift & rotation (a tentative)
#define VTXLAYERNUM       4
#define VTXISOZ       1.5
#define VTXISOX       0.
#define VTXISOY       0.
#define VTXSHIFTX       0.
#define VTXSHIFTY       0.
#define VTXSHIFTZ       0.
#define VTXYZANGLE      0.     //VTX rotation around X axis in deg (provv)        
#define VTXXZANGLE      0.     //VTX rotation around Y axis in deg (provv)  

//bm FIXED par
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

typedef struct vtx_evstruct {
  int evnum;        //number of event
  int tracknum;        //number of event
  double vtx_track_chi2tot;
  double vtx_track_chi2uview;
  double vtx_track_chi2vview;
  TVector3 vtx_track_pvers;
  TVector3 vtx_track_r0pos;
} vtx_evstruct;

//general functions
void readallMSDfile();
void BookingBMVTX(TFile* f_out, bool onlyVTX, bool merging);
void Printoutput(TFile* f_out, vector<BM_evstruct> &allbmeventin, vector<vtx_evstruct> &allvtxeventin, vector<vector<double>> &space_residual, vector<vector<double>> &time_residual, vector<vector<int>> &selected_index, bool onlyVTX);
void EvaluateSpaceResidual(vector<vector<double>> &space_residual,vector<vector<double>> &time_residual, BM_evstruct &bmevent, vtx_evstruct &vtxevent, vector<TVector3> &wire_pos, vector<TVector3> &wire_dir);
void Allign_estimate();
void fitPositionResidual();
void fitStrel(TFile *f_out, const int index);
void merge_graphics(TFile* infile, TFile* f_out);

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
void clean_vtxevstruct(vtx_evstruct &vtxevstruct);
bool vtxreadevent(vtx_evstruct &vtxevent,TTreeReader &vtxReader, TTreeReaderValue<int> &evnum, TTreeReaderValue<int> &tracknum, TTreeReaderValue<double> &chi2tot, TTreeReaderValue<double> &chi2uview, TTreeReaderValue<double> &chi2vview, TTreeReaderValue<double> &r0x, TTreeReaderValue<double> &r0y, TTreeReaderValue<double> &pversx, TTreeReaderValue<double> &pversy);
void print_vtxevstruct(vtx_evstruct &vtxevstruct);
void fitVTXgraph();
void vtxpversr02global(const TVector3 vtxpvers, const TVector3 vtxr0pos, TVector3 &pversout, TVector3 &r0posout );


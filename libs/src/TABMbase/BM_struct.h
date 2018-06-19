#ifndef _BM_STRUCT_H_
#define _BM_STRUCT_H_

using namespace std;

#define MAXHITTDC 200

//The BM acquisition software structure
typedef struct BM_struct {
  
  Int_t evnum;//number of event
  Int_t words;//total number of words
  Int_t hitnum;//total number of hits
  Int_t hit_id[MAXHITTDC];//measurement BM cell id (not the tdc channel number)
  Int_t hit_meas[MAXHITTDC];//measurement value (10^-8 sec.)
  Int_t status;//error flag: -1000=ok, 0=not set
  Int_t synctime[MAXHITTDC];//time of the sync channel -1000=not set, (10^-8 sec.)

} BM_struct;

#endif

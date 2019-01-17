/*!
  \file
  \version $Id: TABMactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TABMactDatRaw.
*/

#include "TABMparMap.hxx"
#include "TABMparCon.hxx"
#include "TAGmbsEvent.hxx"
#include "TABMdatRaw.hxx"

#include "TABMactDatRaw.hxx"
#include <iomanip>

/*!
  \class TABMactDatRaw TABMactDatRaw.hxx "TABMactDatRaw.hxx"
  \brief Get Beam Monitor raw data from MBS. **
*/

ClassImp(TABMactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TABMactDatRaw::TABMactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_datmbs,
			     TAGparaDsc* p_parmap,
			     TAGparaDsc* p_parcon)
  : TAGaction(name, "TABMactDatRaw - Unpack BM raw data"),
    fpDatRaw(p_datraw),
    fpDatMbs(p_datmbs),
    fpParMap(p_parmap),
    fpParCon(p_parcon)
{
  AddDataOut(p_datraw, "TABMdatRaw");
  AddDataIn(p_datmbs, "TAGmbsEvent");
  AddPara(p_parmap, "TABMparMap");
  AddPara(p_parcon, "TABMparCon");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TABMactDatRaw::~TABMactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TABMactDatRaw::Action() {

  TABMdatRaw*    p_datraw = (TABMdatRaw*)    fpDatRaw->Object();
  //~ TAGmbsEvent*   p_datmbs = (TAGmbsEvent*)  fpDatMbs->Object();
  
  //From there we get the Mapping of the wires into the Chamber to the TDC channels
  TABMparMap*    p_parmap = (TABMparMap*)    fpParMap->Object();
  TABMparCon*    p_parcon = (TABMparCon*)    fpParCon->Object();
  Int_t sID, plane, view;
  
  Int_t i_nse = p_datmbs->NSubEvent();
  
  unsigned long geo_add,measurement,channel,trailing;
  int nwords, overflow, board_id, board_ty;
  bool tdcEv = kFALSE; bool hit_found = kFALSE;
  int ntdcW(0), max_tdcW(0);
  bool skipFiWord = kFALSE;
  double t0_corr;

  //Select only Physics Triggers
  if(p_datmbs->EventTrigger()==1) {
    for (Int_t i_se = 0; i_se < i_nse; i_se++) {
      if (p_datmbs->SubEventType(i_se) == 42) {
        Int_t i_nw  = p_datmbs->SubEventSize(i_se);
        const UInt_t* p_se = p_datmbs->SubEventData(i_se);
            
        Int_t i_ntdc  = 0;
        Int_t i_ndrop = 0;
            
        int evt_num = 0, tra_stat = 0;
      
        /* 
           Events Words
           1) Header
           2) Tdc evt, ch, time
           3) Trailer + error
        */
        int tmp_word = 0; 
        int glb_hea = 0;
            
        board_id = -1;
      
        Info("Action()","Num of words :: %d",i_nw);
        //Number of words. I assume that this is TDC like...
        for (Int_t iT = 0; iT < i_nw; iT++) {
      
          //Single word from MBS
          UInt_t i_data = p_se[iT];
          skipFiWord = kFALSE;
          if(i_data>>28 == 0xF && !tdcEv) {
            //Check if this is a global event header
            nwords = i_data & 0x3FFFF;
            overflow = i_data>>22 & 0x1;
            board_id = i_data>>23 & 0x1F;
            board_ty = i_data>>19 & 0x7;
            //TDC is in slot 12 and TDC board type is 4
            if(board_id == 17 && board_ty == 4) { 
              Info("Action()","-->> TDC Global event HEADE = %2d nW:: %2d %2d bID:: %2d :::: Word: %x <<-- \n",iT,nwords,overflow,board_id,i_data);
              if(!nwords) {
                Warning("Action()","-->> TDC Global event HEADE = %2d nW:: %2d %2d bID:: %2d :::: Word: %x <<-- \n",iT,nwords,overflow,board_id,i_data);
              } else {
                tdcEv = kTRUE;
              }
              max_tdcW = nwords; 
              skipFiWord = kTRUE;
            }
          }
          //Checking the TDC words/data.
      
          hit_found = kFALSE;
      
          if(tdcEv) {
            if(skipFiWord) {
              skipFiWord = kFALSE;
            } else {
              Info("Action()","Decoding TDC words = %2d %x",iT,i_data);
              tmp_word = (i_data>>27) & 0x1F;
              Info("Action()","Tmp TDC words = %2d %x",iT,tmp_word);
              glb_hea = (i_data>>28);
            
              ntdcW++;
            
              if( glb_hea == 0x4 ) {
                /* Global Header!*/
                Info("Action()","Global header:: %X\n",(unsigned int)glb_hea);
                geo_add = i_data>>27;
                evt_num = i_data>>5 & 0x3fffff;
              } else if(tmp_word == 0x10) {
                /* Global Trailer!*/
                Info("Action()","Global trailer:: %X\n",(unsigned int)i_data);
                tra_stat = i_data>>24 & 0x1;
                //Global, trailed, TDC event ends here!
                tdcEv = kFALSE;
              } else if(tmp_word == 0x3) {
                /* TDC Trailer!*/
                Info("Action()","TDC trailer:: %X\n",(unsigned int)i_data);
              } else if(tmp_word == 0x1) {
                /* TDC Header!*/
                Info("Action()","TDC header:: %X\n",(unsigned int)i_data);
              } else if(!tmp_word) {
                /* TDC measurement!*/
                measurement = i_data & 0x3ffff;
                channel = (i_data>>19) & 0x7f;
                trailing = (i_data>>26) & 0x1;
            
                //Converts the TDC channel into a wire knowing the mapping
                hit_found = p_parmap->GetWireIDFromTDC(channel,board_id,sID,plane,view);
                if(hit_found) {
                  //Creates and add the hit ONLY if the channel has a valid sID, plane, view
                  Info("Action()","TDC word:: %d %d %d %d %d %d\n",(int)evt_num,(int)channel,(int)sID,(int)plane,(int)view,(int)measurement);
                  //Sets the Hit info (time is stored in ns)
                  //		t0_corr = p_parcon->GetT0(view,plane,sID);
                  p_datraw->SetHitData(view,plane,sID,measurement*0.1);
                  i_ntdc++;
                }
              }
            }//SkipFirstWord
          }//Decoding of TDC!
        }//End the Loop on the Event words.
            
        //Check the event consistency: read words are containing the global
        //header for each word
        if(ntdcW && (ntdcW != max_tdcW)) {
          Error("Action()"," Missing some TDC words! Read words:: %d Header words: %d ",ntdcW,max_tdcW+1);
        }
        p_datraw->SetCounter(i_ntdc, i_ndrop);

      }
    }
  } else {
    Info("Action()","Cosmics Trigger :: %d . Not going to process the event",p_datmbs->EventTrigger());
  }

  fpDatRaw->SetBit(kValid);
  return kTRUE;
}


/*!
  \file
  \version $Id: TATRactDatRaw.cxx,v 1.5 2003/06/22 10:35:47 mueller Exp $
  \brief   Implementation of TATRactDatRaw.
*/

#include "TATRparMap.hxx"
#include "TAGmbsEvent.hxx"
#include "TATRdatRaw.hxx"
#include "TATRactDatRaw.hxx"

#include <iomanip>


/*!
  \class TATRactDatRaw TATRactDatRaw.hxx "TATRactDatRaw.hxx"
  \brief Get Beam Monitor raw data from MBS. **
*/

ClassImp(TATRactDatRaw);

//------------------------------------------+-----------------------------------
//! Default constructor.

TATRactDatRaw::TATRactDatRaw(const char* name,
			     TAGdataDsc* p_datraw, 
			     TAGdataDsc* p_datmbs,
			     TAGparaDsc* p_parmap)
  : TAGaction(name, "TATRactDatRaw - Unpack BM raw data"),
    fpDatRaw(p_datraw),
    fpDatMbs(p_datmbs),
    fpParMap(p_parmap)
{
  AddDataOut(p_datraw, "TATRdatRaw");
  AddDataIn(p_datmbs, "TAGmbsEvent");
  AddPara(p_parmap, "TATRparMap");
}

//------------------------------------------+-----------------------------------
//! Destructor.

TATRactDatRaw::~TATRactDatRaw()
{}

//------------------------------------------+-----------------------------------
//! Action.

Bool_t TATRactDatRaw::Action() {

  TATRdatRaw*    p_datraw = (TATRdatRaw*)    fpDatRaw->Object();
  TAGmbsEvent*   p_datmbs = (TAGmbsEvent*)  fpDatMbs->Object();
  
  //From there we get the Mapping of the wires into the Chamber to the VUL channels
  TATRparMap*    p_parmap = (TATRparMap*)    fpParMap->Object();
  
  if(!p_datraw->htr) p_datraw->SetupClones();

  Int_t i_nse = p_datmbs->NSubEvent();

  unsigned long geo_add,measurement,channel,trailing;
  int nwords, overflow, board_id, board_ty, evt_cnt, trg_num, trg_pat;
  bool vulEv = kFALSE;
  bool scaEv = kFALSE;
  bool tdcEv = kFALSE;
  bool tdcKeEv = kFALSE;
  int nvulW(0), max_vulW(0), nhits(0);
  int nscaW(0), max_scaW(0), i_nsca(0);
  int ntdcW(0), max_tdcW(0), tr_id(-1);
  //int ntdcKeW(0);
  int max_tdcKeW(0);
  bool skipFiWord = kFALSE, tdc_hit_found = kFALSE;
  bool dumpH = kFALSE;
  for (Int_t i_se = 0; i_se < i_nse; i_se++) {

    if (p_datmbs->SubEventType(i_se) == 42) {
      Int_t i_nw  = p_datmbs->SubEventSize(i_se);
      const UInt_t* p_se = p_datmbs->SubEventData(i_se);
      
      /* 
	 Events Words
	 1) Header
	 2) Vul evt, ch, time
	 3) Trailer + error
      */

      Info("Action()","Num of words :: %d",i_nw);
      board_id = -1;
      if(dumpH) cout<<" Hexadecimal Dump of ONE Event:: START :: nW:: "<<i_nw<<endl;
      //Number of words. I assume that this is VUL like...
      for (Int_t iT = 0; iT < i_nw; iT++) {

	//Single word from MBS
	UInt_t i_data = p_se[iT];
	skipFiWord = kFALSE;

	if(dumpH) cout<<hex<<i_data<<endl;

	if(i_data>>28 == 0xF && !vulEv && !scaEv && !tdcEv) {
	  //Check if this is a global event header
	  nwords = i_data & 0x3FFFF;
	  overflow = i_data>>22 & 0x1;
	  board_id = i_data>>23 & 0x1F;
	  board_ty = i_data>>19 & 0xF;

	  //Vulom
	  if(board_id == 3 && board_ty == 1) { 
	    if(dumpH) cout<<" Hex :: VULOM evt "<<endl;
	    if(!nwords) {
	      Warning("Action()","-->> vulom Global event HEADE = %2d nW:: %2d %2d bID:: %2d :::: Word: %x <<-- \n",iT,nwords,overflow,board_id,i_data);
	    } else {
	      vulEv = kTRUE;
	    }
	    max_vulW = nwords;
	    skipFiWord = kTRUE;
	  }
	  //Scaler
	  if(board_id == 19 && board_ty == 2) {
	    if(dumpH) cout<<" Hex :: SCALER evt "<<endl;
	    if(!nwords) {
	      Warning("Action()","-->> Scaler Global event HEADE = %2d nW:: %2d %2d bID:: %2d :::: Word: %x <<-- \n",iT,nwords,overflow,board_id,i_data);
	    } else {
	      scaEv = kTRUE;
	    }
 	    max_scaW = nwords;
	    skipFiWord = kTRUE;
	  }
	  //LNF TDC
	  if(board_id == 17 && board_ty == 4) { 
	    if(dumpH) cout<<" Hex :: TDC LNF evt "<<endl;
	    if(!nwords) {
	      Warning("Action()","-->> TDC Global event HEADE = %2d nW:: %2d %2d bID:: %2d :::: Word: %x <<-- \n",iT,nwords,overflow,board_id,i_data);
	    } else {
	      tdcEv = kTRUE;
	    }
	    max_tdcW = nwords; 
	    skipFiWord = kTRUE;
	  }

	  //Kentros TDC
	  if(board_id == 16 && board_ty == 4) { 
	    if(dumpH) cout<<" Hex :: TDC Kentros evt "<<endl;
	    if(!nwords) {
	      Warning("Action()","-->> TDC Global event HEADE = %2d nW:: %2d %2d bID:: %2d :::: Word: %x <<-- \n",iT,nwords,overflow,board_id,i_data);
	    } else {
	      tdcKeEv = kTRUE;
	    }
	    max_tdcKeW = nwords; 
	    skipFiWord = kTRUE;
	  }
	}

	//Checking the VUL words/data.
	if(vulEv) {
	  if(skipFiWord) {
	    skipFiWord = kFALSE;
	  } else {
	    if(dumpH) cout<<hex<<i_data<<endl;
	    Info("Action()","Decoding VUL words = %2d %x :: nW %d",iT,i_data,nvulW);
	    if(!nvulW) {
	      //First word:: vulom status

	      //Sets the Hit Vul status info
	      nhits = p_datraw->ntrhit;
	      Info("Action()","Going to start hits %d addition!!!",nhits);
	      if(nhits == 1) {
		//Handle multiple hits
		if(p_datraw->Hit(0)->Status()<0) {
		  p_datraw->Hit(0)->SetStatus(i_data);
		} else {
		  Error("Action()","More than 1 Vulom trigger Status!!!");
		}
	      } else if (nhits == 0) {
		Info("Action()","Going to create hit :: %d status:: %d",nhits,i_data);
		TATRrawHit *mytmp = new((*(p_datraw->htr))[nhits]) TATRrawHit();
		mytmp->SetStatus(i_data);
		Info("Action()","Going to start hits %d addition!!!",nhits);
		p_datraw->ntrhit++;
	      } else { 
		Error("Action()","More than 1 Vulom trigger Status!!!");
	      }
	      if(i_data != 0) {
		Error("Action()","Vulom Status presents ERRORS:: %x",i_data);
	      } 
	    } else if(nvulW == 1) {
	      Info("Action()","Vulom Trigger Pattern:: %x",i_data);

	      evt_cnt = (i_data>>28) & 0xF;
	      trg_num = (i_data>>24) & 0xF;
	      trg_pat = i_data & 0xFFFFFF;

	      Info("Action()","Trigger Pattern:: Evt:: %d, Trg Num %d, Patt %x",evt_cnt, trg_num, trg_pat);
	      //Sets the Hit Vul pettern info
	      nhits = p_datraw->ntrhit;
	      if(nhits == 1) {
		//Handle multiple hits
		if(p_datraw->Hit(0)->Pattern()<0) {
		  p_datraw->Hit(0)->SetPattern(i_data);
		} else {
		  Error("Action()","More than 1 Vulom trigger Pattern!!!");
		}
	      } else if (nhits == 0) {
		TATRrawHit *mytmp = new((*(p_datraw->htr))[nhits]) TATRrawHit();
		mytmp->SetPattern(i_data);
		p_datraw->ntrhit++;
	      } else { 
		Error("Action()","More than 1 Vulom trigger Pattern!!!");
	      }

	    } else if(nvulW == 2) {
	      Info("Action()","Vulom Trigger Counts:: %x",i_data);
	      //Sets the Hit Vul counts info
	      nhits = p_datraw->ntrhit;
	      if(nhits == 1) {
		//Handle multiple hits
		if(p_datraw->Hit(0)->Count()<0) {
		  p_datraw->Hit(0)->SetCount(i_data);
		} else {
		  Error("Action()","More than 1 Vulom trigger Counts!!!");
		}
	      } else if (nhits == 0) {
		TATRrawHit *mytmp = new((*(p_datraw->htr))[nhits]) TATRrawHit();
		mytmp->SetCount(i_data);
		p_datraw->ntrhit++;
	      } else { 
		Error("Action()","More than 1 Vulom trigger Counts!!!");
	      }
	    } else if(nvulW == 3) {
	      Info("Action()","Vulom Trigger Time:: %x",i_data);
	      p_datraw->Hit(0)->SetVulomTime(i_data);
	    } else if (nvulW >3 && nvulW<20) {
	      Info("Action()","Vulom Scalers Before MU");
	      p_datraw->Hit(0)->SetTrigBefLMU(nvulW-4,i_data);
	    } else if (nvulW >19 && nvulW<36) {
	      p_datraw->Hit(0)->SetTrigBefDT(nvulW-20,i_data);
	    } else if (nvulW >35 && nvulW<52) {
	      p_datraw->Hit(0)->SetTrigAftDT(nvulW-36,i_data);
	    } else if (nvulW >51 && nvulW<68) {
	      p_datraw->Hit(0)->SetTrigAftRed(nvulW-52,i_data);
	    } else {
	      Error("Action()","Too many Words from the VULOM :: %d ! Check the vulom encoding!",nvulW);
	    }

	    nvulW++;
	    if(nvulW == max_vulW) {
	      vulEv = kFALSE;
	    }
	  }

 	}//End of Decoding of VUL!

	if(scaEv) {
	  if(skipFiWord) {
	    skipFiWord = kFALSE;
	  } else {
	    if(dumpH) cout<<hex<<i_data<<endl;
	    Info("Action()","Decoding SCA words = %2d %x nwrd :: %d",iT,i_data,nscaW);
	    //Sets the Hit info
	    nhits = p_datraw->ntrhit;
	    if(nhits == 1) {
	      //Handle multiple hits
	      if(p_datraw->Hit(0)->ScaCount(nscaW)>=0) {
		Warning("Action()","Multiple Scaler hit  %d %d",p_datraw->Hit(0)->ScaCount(nscaW),(int)i_data);
	      } else {
		p_datraw->Hit(0)->SetScaCount(nscaW,i_data);
	      }
	      i_nsca++;
	    } else if (nhits == 0) {
	      TATRrawHit *mytmp = new((*(p_datraw->htr))[nhits]) TATRrawHit();
	      mytmp->SetScaCount(nscaW,i_data);
	      p_datraw->ntrhit++;
	      i_nsca++;
	    } else {
	      Error("Action()","More than One trigger Time!!!");
	    }

	    nscaW++;
	    if(nscaW == max_scaW) {
	      scaEv = kFALSE;
	    }
	  }
	}//End the SCA reading loop

	int evt_num = 0, tra_stat = 0;
	int tmp_word = 0; 
	int glb_hea = 0;
	Int_t i_ntdc  = 0;
	tr_id = -1;
	//Checking the TDC words/data.
	tdc_hit_found = kFALSE;
	if(tdcEv) {
	  if(skipFiWord) {
	    skipFiWord = kFALSE;
	  } else {
	    if(dumpH) cout<<hex<<i_data<<endl;
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
	      tdc_hit_found = p_parmap->IsTrigID(channel,board_id,tr_id);
	      if(tdc_hit_found) {
		//		cout<<" "<<tr_id<<" "<<measurement<<" "<<endl;
		//Creates and add the hit ONLY if the channel
		Info("Action()","TDC word:: %d %d %d \n",(int)evt_num,(int)channel,(int)measurement);
		//Sets the Hit info (time is stored in ns)
		nhits = p_datraw->ntrhit;
		if(nhits == 1) {
		  //Handle multiple hits
		  if(p_datraw->Hit(0)->TrigTime(tr_id)<measurement*0.1) {
		    //		    Warning("Action()","Multiple TDC hit in trigger discarded %lf %lf",p_datraw->Hit(0)->TrigTime(),measurement*0.1);
		    p_datraw->Hit(0)->SetTrigMult(tr_id,p_datraw->Hit(0)->TrigMult(tr_id)+1);
		  } else {
		    p_datraw->Hit(0)->SetTrigTime(tr_id,measurement*0.1);
		    p_datraw->Hit(0)->SetTrigMult(tr_id,p_datraw->Hit(0)->TrigMult(tr_id)+1);
		  }
		  i_ntdc++;
		} else if (nhits == 0) {
		  TATRrawHit *mytmp = new((*(p_datraw->htr))[nhits]) TATRrawHit();
		  mytmp->SetTrigTime(tr_id,measurement*0.1);
		  mytmp->SetTrigMult(tr_id,1);
		  p_datraw->ntrhit++;
		  i_ntdc++;
		} else {
		  Error("Action()","More than One trigger Time!!!");
		}
	      }
	    }
	  }//SkipFirstWord
 	}//End of Decoding of TDC!


	//Starting decoding of Kentros TDC
	tdc_hit_found = kFALSE;
	Int_t i_ntdcke  = 0;
	tr_id = -1;
	if(tdcKeEv) {
	  if(skipFiWord) {
	    skipFiWord = kFALSE;
	  } else {
	    if(dumpH) cout<<hex<<i_data<<endl;
	    Info("Action()","Decoding TDC words = %2d %x",iT,i_data);
	    tmp_word = (i_data>>27) & 0x1F;

	    Info("Action()","Tmp TDC words = %2d %x",iT,tmp_word);
	    glb_hea = (i_data>>28);

	    //	    cout<<" TDC:: Ken :: "<<iT<<" "<<i_data<<" "<<tmp_word<<" "<<endl;
	    
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
	      tdcKeEv = kFALSE;
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
	      tdc_hit_found = p_parmap->IsTrigID(channel,board_id,tr_id);
	      if(tdc_hit_found) {
		//Creates and add the hit ONLY if the channel
		Info("Action()","TDC word:: %d %d %d \n",(int)evt_num,(int)channel,(int)measurement);
		//Sets the Hit info (time is stored in ns)
		nhits = p_datraw->ntrhit;
		if(nhits == 1) {
		  //Handle multiple hits
		  if(p_datraw->Hit(0)->TrigTime(tr_id)<measurement*0.1) {
		    //		    Warning("Action()","Multiple TDC hit in trigger discarded %lf %lf",p_datraw->Hit(0)->TrigTime(),measurement*0.1);
		    p_datraw->Hit(0)->SetTrigMult(tr_id,p_datraw->Hit(0)->TrigMult(tr_id)+1);
		  } else {
		    p_datraw->Hit(0)->SetTrigTime(tr_id,measurement*0.1);
		  }
		  i_ntdcke++;
		} else if (nhits == 0) {
		  TATRrawHit *mytmp = new((*(p_datraw->htr))[nhits]) TATRrawHit();
		  mytmp->SetTrigTime(tr_id,measurement*0.1);
		  p_datraw->ntrhit++;
		  i_ntdcke++;
		} else {
		  Error("Action()","More than One trigger Time!!!");
		}
	      }
	    }
	  } // Skip first word
	}//Decoding of TDC!

	
      }//End the Loop on the Event words.

      if(dumpH) cout<<" Hexadecimal Dump of ONE Event:: END "<<endl;

      //Check the event consistency: read words are containing the global
      //header for each word
      if(nvulW && (nvulW != max_vulW)) {
	Error("Action()"," Missing some VUL words! Read words:: %d Header words: %d ",nvulW,max_vulW+1);
      }
      if(nscaW && (nscaW != max_scaW)) {
	Error("Action()"," Missing some SCA words! Read words:: %d Header words: %d ",nscaW,max_scaW+1);
      }
    }
  }

  fpDatRaw->SetBit(kValid);
  return kTRUE;
}


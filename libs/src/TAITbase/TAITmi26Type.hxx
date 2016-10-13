#ifndef _TAITmi26Type_HXX_
#define _TAITmi26Type_HXX_
/*
 *  TAITmi26Type.h
 *  First
 *
 *  Created by christian finck on 21/9/11.
 *  Copyright 2011 IPHC. All rights reserved.
 *
 */
// structure for PXI format

#define MI26__FFRAME_RAW_MAX_W16  1140

// structure for MBS and ascii file
typedef struct {
   
   UInt_t Header;
   UInt_t TriggerCnt;
   UInt_t TimeStamp;
   UInt_t FrameCnt;
   UInt_t DataLength;
   
   UShort_t ADataW16[MI26__FFRAME_RAW_MAX_W16];
   UInt_t Trailer;
   
} MI26_FrameRaw; 

// structure of state and line
typedef union {
   
   UShort_t W16;
   
   struct {
	  
	  UShort_t StateNb  :  4;
	  UShort_t LineAddr : 11;
	  UShort_t Ovf      :  1;
	  
   } F;
   
} MI26__TStatesLine;

typedef union {
   
   UShort_t W16;
   
   struct {
	  
	  UShort_t HitNb   :  2;
	  UShort_t ColAddr : 11;
	  UShort_t NotUsed :  3;
	  
   } F;
   
} MI26__TState;

#endif

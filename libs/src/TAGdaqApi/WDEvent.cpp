#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include "WDEvent.hh"

WDEvent::~WDEvent(){
}

void WDEvent::readData( unsigned int **p1 ){

   unsigned int *p = *p1;
   int chn=0;
   int b=0,i=0,j=0,k=0;
   std::vector<double> tmp_tcalib;
   std::vector<short int> tmp_adc;
   double t1, t2, dt;
   unsigned int scaler;
   int key=0;

   
   //get the file header   
   memcpy(&m_fh, p, sizeof(m_fh));
   if (m_fh.tag[0] != 'D' || m_fh.tag[1] != 'R' || m_fh.tag[2] != 'S') {
      printf("Warning::Found invalid file header in the WD stream!!\n");
   }
   if (m_fh.version != '8') {
      printf("Warning::Found invalid file version \'%c\' in thw WD stream, should be \'8\'\n", m_fh.version);
   }
   p+=sizeof(m_fh);


   
   //get the time header
   memcpy(&m_th, p, sizeof(m_th));
   if (memcmp(m_th.time_header, "TIME", 4) != 0) {
     printf("Invalid time header in thw WD stream\n");
   }
   p+=sizeof(m_th);
   

   //get the data
   //looping on the wavedream boards


   //gtraini:: to be changed with the proper numer of boards
   for (b=0; b<NBOARDS; b++) {
     // read board header
     memcpy(&m_bh, p, sizeof(m_bh));
     p+=sizeof(m_bh);
     if (memcmp(m_bh.bn, "B#", 2) != 0) {
	// probably event header found
       p-=sizeof(m_bh);
       break;
      }
     
      printf("Found data for WD board #%d\n", m_bh.board_serial_number);
      

      // read time bin widths
      for (chn=0; chn<16; chn++) {
	memcpy(&m_ch, p, sizeof(m_ch));
	p+=sizeof(m_ch);
	if (m_ch.c[0] != 'C') {
	  // event header found
	  p-=sizeof(m_ch);
	  break;
	}
	i = (m_ch.cn[1] - '0')*10 + m_ch.cn[2] - '0';
	printf("WD:Found timing calibration for channel #%d\n", i);


	float bwidth=0;

	key = i+16*b;
	for(int isam=0;isam<1024;isam++){
	  memcpy(&bwidth, p, sizeof(float));
	  p+=sizeof(float);
	   p+=sizeof(float);
	  m_time_calib[key].push_back(bwidth);
	}
      }
   }


   // read event header
   memcpy(&m_eh, p, sizeof(m_eh));
   p+=sizeof(m_eh);

   printf("Found event #%d, t=%d.%d\n", m_eh.event_serial_number, m_eh.second, m_eh.millisecond);
   
   // loop over all boards in data file
   for (b=0 ; b<NBOARDS ; b++) {
     
     // read board header
     memcpy(&m_bh, p, sizeof(m_bh));
     p+=sizeof(m_bh);
     if (memcmp(m_bh.bn, "B#", 2) != 0) {
       printf("Warning::Invalid WD board header\n");
     }
     
       
     // reach channel data
     int chn_index=0;
     for (chn=0 ; chn<16 ; chn++) {

       // read channel header
       memcpy(&m_ch, p, sizeof(m_ch));
       p+=sizeof(m_ch);
       if (m_ch.c[0] != 'C') {
	 // event header found
	 p-=sizeof(m_ch);
	 break;
       }
       chn_index = (m_ch.cn[1] - '0')*10 + m_ch.cn[2] - '0';
       memcpy(&scaler, p, sizeof(unsigned int));
       p+=sizeof(unsigned int);

       
       // read trigger cell
       memcpy(&m_tch, p, sizeof(m_tch));
       p+=sizeof(m_tch);
       if (memcmp(m_tch.tc, "T#", 2) != 0) {
	 printf("Warning::Invalid trigger cell header in WD stream\n");
       }


       tmp_adc.clear();
       tmp_adc.assign(1024,0);
    
       memcpy(&tmp_adc[0], p, sizeof(short)*1024);
       p+=sizeof(short)*1024;

       key = chn_index+b*16;
       int oldadc = 0;
       for (i=0 ; i<1024 ; i++) {

	 // convert data to volts (with patch)
	 if(i != 0 && abs(oldadc - (int)tmp_adc.at(i))>30000) {
	   m_voltage[key].push_back((-65535+tmp_adc.at(i))%65535 / 65536. + m_eh.range/1000.0 - 0.5);
	 }else {
	   m_voltage[key].push_back(tmp_adc[i] / 65536. + m_eh.range/1000.0 - 0.5);
	   oldadc = tmp_adc.at(i);
	 }

	 // calculate time for this cell
	 double t=0;
	 for (j=0; j<i ; j++){
	   t+=(double)(m_time_calib.find(key)->second.at((j+m_tch.trigger_cell) % 1024));
	 }

	 m_time[key].push_back(t);
       } 
     }
   }

   
     // // align cell #0 of all channels
     // t1 = time[b][0][(1024-tch.trigger_cell) % 1024];
     // for (chn=1 ; chn<18 ; chn++) {
     //   t2 = time[b][chn][(1024-tch.trigger_cell) % 1024];
     //   dt = t1 - t2;
     //   for (i=0 ; i<1024 ; i++)
     // 	 time[b][chn][i] += dt;
     // }
     
       
      


   

   // unsigned int buffer1;

  // channelID = *p;
  // ++p;
  // tdc_sec = *p;
  // ++p;
  // tdc_usec = *p;
  // ++p;
  // eventNumber = *p;
  // ++p;
  // globalHeader = *p;
  // buffer1 = ( p[1] >>27 ) & 0x1f;
  // while ( buffer1 != 0x10 ){
  //   if ( buffer1 == 0x0 ){
  //     ++p;
  //     ( measurement ).push_back( *p ); //& 0x7FFFF to select real measurement
  //   }
  //   else if ( buffer1 == 0x1 ){
  //     ++p;
  //     ( tdcHeader ).push_back( *p );
  //   }
  //   else if ( buffer1 == 0x3 ){
  //     ++p;
  //     ( tdcTrailer ).push_back( *p );
  //   }
  //   else {
  //     printf("EventReader::getWDData --> unexpected sequence in lookAtWD evaluation \n");
  //     exit(0);
  //   }
  //   buffer1 = ( p[1] >>27 ) & 0x1f;
  // }
  // ++p;
  // globalTrailer = *p;
  // ++p;
  // ++p;
  // *p1 = p;
}


void WDEvent::printData() const {

  // printf ("WD DATA: \n");
  // printf ("Channel ID (hex): %x\n",  channelID);
  // printf ("Time in seconds: %d\n",  tdc_sec);
  // printf ("Time in microseconds: %d\n",  tdc_usec);
  // printf ("Number of Event: %d\n",  eventNumber);
  // printf ("Global Header: %x\n",  globalHeader);
  // for ( unsigned int i = 0; i < tdcHeader.size(); ++i ){
  //   printf ("WD Header %i: %x\n", i, tdcHeader.at( i ) );
  // }
  // for ( unsigned int i = 0; i < measurement.size(); ++i ){
  //   printf ("Measurement number %i: %d\n", i, measurement.at( i ) );
  // }
  // for ( unsigned int i = 0; i < tdcTrailer.size(); ++i ){
  //   printf ("WD Trailer %i: %x\n", i, tdcTrailer.at( i ) );
  // }
  // printf ("Global Trailer: %x\n",  globalTrailer);
  // printf("\n");
}

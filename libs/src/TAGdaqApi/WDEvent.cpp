#include <stdlib.h>
#include <stdio.h>
#include "WDEvent.hh"
#include <iostream>

WDEvent::~WDEvent(){}


void WDEvent::readData(unsigned int **p1){

  RemoteEvent::readData(p1);

  if( evtSize!=0 ){
    detectorHeader = values[0];
    boardHeader = values[1];
    numberWords = values[2];
  }
}


void WDEvent::printData () const{

  printf ("WDEvent DATA: \n");
  printf ("Channel ID (hex): %x\n",  channelID);
  printf ("Time in seconds: %d\n",  time_sec);
  printf ("Time in microseconds: %d\n",  time_usec);
  printf ("Number of Event: %d\n",  eventNumber);
  printf ("Detector Header: %x\n",  detectorHeader);
  printf ("Board Header: %x\n",  boardHeader);
  printf ("Number of words: %d\n",  numberWords);
  printf("\n");
}

#include <stdlib.h>
#include <stdio.h>
#include "DECardEvent.hh"
#include <iostream>

DECardEvent::~DECardEvent(){}


void DECardEvent::readData(unsigned int **p1){

  RemoteEvent::readData(p1);

  if( evtSize!=0){
    detectorHeader = values[0];
    boardHeader = values[1];
    triggerCounter = values[2];
    BCOofTrigger = values[3];
    clockCounter = values[4];
  }
}


void DECardEvent::printData () const{

  printf ("DECardEvent DATA: \n");
  printf ("Channel ID (hex): %x\n",  channelID);
  printf ("Time in seconds: %d\n",  time_sec);
  printf ("Time in microseconds: %d\n",  time_usec);
  printf ("Lumi Block: %d\n",  lumiBlock);
  printf ("Number of Event: %d\n",  eventNumber);
  printf ("Detector Header: %x\n",  detectorHeader);
  printf ("Board Header: %x\n",  boardHeader);
  printf ("Trigger Counter (hw): %d\n",  triggerCounter);
  printf ("BCO counter: %d\n",  BCOofTrigger);
  printf ("BX counter: %d\n",  clockCounter);
  printf("\n");
}


bool DECardEvent::check() const { 
  if( eventNumber==triggerCounter)
    return true;
  else 
    return false;
}
	

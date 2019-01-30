#include <stdlib.h>
#include <stdio.h>
#include "EmptyEvent.hh"

EmptyEvent::~EmptyEvent(){}

void EmptyEvent::readData(unsigned int ** p1){

  unsigned int * p = *p1;
  channelID= *p;
  ++p;
  time_sec= *p;
  ++p;
  time_usec= *p;
  ++p;
  lumiBlock= *p;
  ++p;
  eventNumber= *p;
  ++p;
  ++p;
  ++p;
  *p1 = p;
}


void EmptyEvent::printData () const {

    printf ("EMPTY DATA ROB: \n");
    printf ("Channel ID (hex): %x\n",  channelID);
    printf ("Time in Seconds: %d\n",  time_sec);
    printf ("Time in MicroSeconds: %d\n",  time_usec);
    printf ("LumiBlock Number: %d\n",  lumiBlock);
    printf ("Event Number: %d\n",  eventNumber);
    printf ("\n");

}

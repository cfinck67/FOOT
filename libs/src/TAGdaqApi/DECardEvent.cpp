#include <stdlib.h>
#include <stdio.h>
#include "DECardEvent.hh"
#include <iostream>

const u_int DECardEvent::m_vtxHeader = 0xfafafafa;
const u_int DECardEvent::m_vtxTail   = 0xabcdabcd;


DECardEvent::~DECardEvent()
{
}

void DECardEvent::readData(unsigned int **p1)
{
   evtSize   = 0;
   u_int * p = *p1;
   
   // check header
   if (*p != m_vtxHeader)
      printf("Error in the event reader %x instead of %x\n", *p, m_vtxHeader);
   
   detectorHeader = *p;
   evtSize++;
   values.push_back(*p);
   
   // 1st board header
   boardHeader = *(++p);
   evtSize++;
   values.push_back(*p);
   
   // event number
   eventNumber = *(++p);
   evtSize++;
   values.push_back(*p);
   
   // skip 2 words and frame header
   p += 3;
   
   // trigger number
   triggerCounter = *(++p);
   evtSize++;
   values.push_back(*p);
   
   // timestamp
   clockCounter = *(++p);
   evtSize++;
   values.push_back(*p);
   
   // frame counter
   BCOofTrigger = *(++p);
   evtSize++;
   values.push_back(*p);

   do {
      p++;
      evtSize++;
      values.push_back(*p);
    
   } while (*p != m_vtxTail);
}

void DECardEvent::printData () const
{
   printf ("DECardEvent DATA: \n");
   printf ("Channel ID (hex): %x\n",  channelID);
   printf ("Number of Event: %d\n",  eventNumber);
   printf ("Data size: %d\n",  evtSize);
   printf ("Detector Header: %x\n",  detectorHeader);
   printf ("Board Header: %x\n",  boardHeader);
   printf ("Trigger Counter (hw): %d\n",  triggerCounter);
   printf ("BCO counter: %d\n",  BCOofTrigger);
   printf ("BX counter: %d\n",  clockCounter);
   printf("\n");
}


bool DECardEvent::check() const
{
  if( eventNumber==triggerCounter)
    return true;
  else 
    return false;
}
	

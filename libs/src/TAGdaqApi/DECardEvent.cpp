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
   values.push_back(*p);
   evtSize++;
   
   // event number
   eventNumber = *(++p);
   evtSize++;
   values.push_back(*p);

   while (*p != m_vtxTail) {
      values.push_back(*p);
      p++;
      evtSize++;
   }
   
   // push back trailer
   values.push_back(*p);
}

void DECardEvent::printData () const
{

  printf ("DECardEvent DATA: \n");
  printf ("Number of Event: %d\n",  eventNumber);
  printf("\n");
}


bool DECardEvent::check() const
{
  if( eventNumber==triggerCounter)
    return true;
  else 
    return false;
}
	

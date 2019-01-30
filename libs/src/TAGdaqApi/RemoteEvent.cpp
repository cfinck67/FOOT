#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "RemoteEvent.hh"

RemoteEvent::~RemoteEvent(){
}

void RemoteEvent::readData(unsigned int **p1){

  unsigned int * p = *p1;
  // std::cout<<" Valore di p:"<<p<<std::endl;
  // for(int i=0; i<10; i++)
  //   std::cout<<i<<" - "<<std::hex<<*(p+i)<<std::endl;

  channelID= *p;
  ++p;
  time_sec= *p;
  ++p;
  time_usec= *p;
  ++p;
  //  lumiBlock= *p;
  //  ++p;
  eventNumber= *p;
  ++p;
  //  remoteInfo= *p;
  ++p;
  evtSize= *p;
  ++p;
  if( evtSize!=0){
    for (unsigned int i=0; i<evtSize-1; i++){
      values.push_back(*p);
      ++p;
    }
     int WORD= *p;
        std::cout<<"\n The previous word is:"<<*(p-1)<<std::endl;
     std::cout<<"\n The current word is:"<<WORD<<std::endl;
  } else {
    std::cout<<"SIZE IS ZERO!"<<std::endl;
    ++p;
  }
  *p1 = p;
}


void RemoteEvent::printData () const {
  printf ("Remote DATA: \n");
  printf ("Channel ID (hex): %x\n",  channelID);
  printf ("Time in seconds: %d\n",  time_sec);
  printf ("Time in microseconds: %d\n",  time_usec);
  printf ("Lumi Block: %d\n",  lumiBlock);
  printf ("Number of Event: %d\n",  eventNumber);
  printf ("Data size: %d\n",  evtSize);
  printf("\n");
}

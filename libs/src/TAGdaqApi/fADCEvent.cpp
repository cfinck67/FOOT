#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "fADCEvent.hh"

fADCEvent::~fADCEvent(){}

/****************************************/
void  fADCEvent::readData(unsigned int **p1){

  unsigned int * p = *p1;

  channelID= *p;
  ++p;
  word= *p;
  ++p;
  data= *p;
  ++p;
  eventNumber= *p;
  ++p;
  ++p;
  nchans = getnchans(data);
  if( nchans>0 ){
    //reading the number of word per channel
    int wordsPerChannel = getWordsChannel(word, nchans); 
    //riempire array
    for (int i=0; i<8; i++){
      if(active[i]==true){
	for(int j=0; j < wordsPerChannel; j++) {
	  int WORD= *p;
	  short lowersample, highersample;
	  lowersample = WORD & 0xffff;
	  highersample = (WORD>>16) & 0xffff;
	  channel[i].push_back(lowersample);
	  channel[i].push_back(highersample);
	  ++p;
	}
      }
    }
  } else {
    std::cout<< "FADC error: 0 channels" << std::endl;
    std::cout<< "TIME OUT"<<std::endl;
    std::cout<< "sizeof(unsigned int)" << sizeof(unsigned int)<<std::endl;

    p = *p1;
    p++;
    p++;
  }

  *p1 = p;
}


void fADCEvent::printV (){

  for ( u_int sample = 0; sample < channel[0].size(); sample++ ){
    std::cout << sample <<" - ";
    for ( u_int i = 0; i < nchans; i++ ){
      std::cout << channel[i][sample] << " ";
    }
    std::cout << std::endl;
  }
}


int fADCEvent::getWordsChannel( int aWord, int nCHans ) {
  int nword=aWord & 0x00000fff;
  int WordsChannel=(nword-4)/nCHans;
  return WordsChannel;
}


int fADCEvent::getnchans ( int aData ){

  int mask=1;
  int numChans=0;
  bool status;
  for ( int i = 0; i < 8; i++ ){
    status = false;
    if( ( aData & mask )== mask ){
      status = true;
      numChans++;
    }
    active.push_back(status);
    mask *=2;
  }
  return numChans;
}


void fADCEvent::printData () const {

  printf ("FLASH ADC DATA: \n");
  printf ("Channel ID (hex): %x\n",  channelID);
  printf ("Number of Event: %d\n",  eventNumber);
  printf ("Number of channels: %d\n",  nchans);
  printf("\n");
}

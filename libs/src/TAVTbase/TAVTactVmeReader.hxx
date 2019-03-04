#ifndef _TAVTactVmeReader_HXX
#define _TAVTactVmeReader_HXX
/*!
  \file
  \brief   Declaration of TAVTactVmeReader.
*/
/*------------------------------------------+---------------------------------*/
#include <fstream>

#include "TString.h"

#include "TAVTactBaseRaw.hxx"

class TH2F;
class TH1F;
//##############################################################################
class TAVTactVmeReader : public TAVTactBaseRaw  {
public:
   explicit        TAVTactVmeReader(const char* name=0, TAGdataDsc* pDatRaw = 0, TAGparaDsc* p_geomap=0, TAGparaDsc* p_config=0);
   virtual         ~TAVTactVmeReader();
      
   //! Open file with a given prefix and suffix for the files
   virtual Int_t   Open(const TString& prefix, Option_t* opt = "");
   //! close files
   virtual void    Close();
   //! Process
   virtual Bool_t  Process();
   
 public:
   enum {kSize = 10};
   static void     SetDefaultFolderName(const Char_t* name) { fgDefaultFolderName = name; }
   static TString  GetDefaultFolderName()                   { return fgDefaultFolderName; }
   
private:
   ifstream          fRawFileAscii[kSize]; // file streamm
   Int_t             fEventNumber;         // number of the event
   Int_t             fRunNumber;           // run number

   TString           fPrefixName;          // prefix folder name
   TString           fBaseName;         // base file name
   Int_t             fDebugLevel;          // debug level
   
private:
   static       TString fgDefaultFolderName;
   static       TString fgDefaultExtName;

private:
   //! Build event
   Bool_t FetchEvent();
	
   //! Get the starting point of each event
   Bool_t GetEventHeader(Int_t iSensor);

   //! Get the starting point of each frame
   Bool_t GetStart(Int_t iSensor);
   
   //! Get frame and returns frameRaw
   void   GetFrame(Int_t iSensor, MI26_FrameRaw* data);
   
   //! Get next frames with same trigger
   void   GetNextFrames(Int_t iSensor, UInt_t trigger);
   
   //! Set run number
   void   SetRunNumber(const TString& name);
   
   ClassDef(TAVTactVmeReader,0)
};

#endif

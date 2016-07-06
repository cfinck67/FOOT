#ifndef _TAVTactAscReader_HXX
#define _TAVTactAscReader_HXX
/*!
  \file
  \version $Id: TAVTactAscReader.hxx,v 1.8 2003/07/08 18:54:19 mueller Exp $
  \brief   Declaration of TAVTactAscReader.
*/
/*------------------------------------------+---------------------------------*/
#include "TAVTmi26Type.hxx"

#include <fstream>
#include "TString.h"

#include "TAGactionFile.hxx"
#include "TAVTrawEvent.hxx"

//##############################################################################
class TAVTrawEvent;
class TAVTactAscReader : public TAGactionFile  {
public:
   explicit        TAVTactAscReader(const char* name=0, TAGdataDsc* pAscEvent = 0,TAGparaDsc* p_config=0);
   virtual         ~TAVTactAscReader();
      
   //! Open file with a given prefix and suffix for the files
   virtual Int_t   Open(const TString& prefix, Option_t* suffix = ".dat");
   //! close files
   virtual void    Close();
   //! check if open (not used)
   virtual Bool_t  IsOpen() const;
   //! Process
   virtual Bool_t  Process();
   
public:
   
   static void     SetDefaultBaseName(TString& name)   { fgDefaultBaseName = name; }
   static TString  GetDefaultBaseName()                { return fgDefaultBaseName; }
   
   static void     SetDefaultExtName(TString& name)    { fgDefaultExtName = name; }
   static TString  GetDefaultExtName()                 { return fgDefaultExtName; }
   
   static void     SetDefaultFolderName(TString& name) { fgDefaultFolderName = name; }
   static TString  GetDefaultFolderName()              { return fgDefaultFolderName; }
  
private:
   TAGdataDsc*       fpAscEvent;        // ascii event
   TAGparaDsc*       fpConfig;		    // config para dsc
   UInt_t*           fData;             // data array to fill
   ifstream          fRawFileAscii[10]; // file streamm
   Int_t             fIndex;            // index of data array
   Int_t             fEventSize;        // size of the event
   Int_t             fRunNumber;        // run number
   TString           fPrefixName;       // suffix file name
   Int_t             fDebugLevel;       // debug level

private:
   
   static TString fgDefaultBaseName;
   static TString fgDefaultExtName;
   static TString fgDefaultFolderName;

private:
	 //! Build event
	 Bool_t FetchEvent();
	
   //! Get the starting point of each frame
   Bool_t GetStart(Int_t iSensor);
   
   //! Get frame and returns frameRaw
   void   GetFrame(Int_t iSensor, MI26_FrameRaw* data);
   
   //! Get next frames with same trigger
   void   GetNextFrames(Int_t iSensor, UInt_t trigger);
   
   //! Set run number
   void SetRunNumber(const TString& name);
      
   ClassDef(TAVTactAscReader,0)
};

#endif


#ifndef TCITgeometryConstructor_h
#define TCITgeometryConstructor_h 1

#include "TString.h"
#include "TCVTgeometryConstructor.hxx"

/** Building IT detector geometry
 
 \author Ch, Finck
 */

class TAVTparGeo;

class TCITgeometryConstructor : public TCVTgeometryConstructor
{
public:
   TCITgeometryConstructor(TAVTparGeo* pParGeo);
   virtual ~TCITgeometryConstructor();
  
public:
   static const Char_t* GetSDname()  { return fgkItEpiSDname.Data();}

private:
   static TString fgkItEpiSDname;

private:
   void DefineSensitive();

};

#endif

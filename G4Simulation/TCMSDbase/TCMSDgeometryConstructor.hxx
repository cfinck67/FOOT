
#ifndef TCMSDgeometryConstructor_h
#define TCMSDgeometryConstructor_h 1

#include "TString.h"
#include "TCVTgeometryConstructor.hxx"

/** Building IT detector geometry
 
 \author Ch, Finck
 */

class TAVTparGeo;

class TCMSDgeometryConstructor : public TCVTgeometryConstructor
{
public:
   TCMSDgeometryConstructor(TAVTparGeo* pParGeo);
   virtual ~TCMSDgeometryConstructor();
  
public:
   static const Char_t* GetSDname()  { return fgkMsdEpiSDname.Data();}

private:
   static TString fgkMsdEpiSDname;

private:
   void DefineSensitive();
   void DefineMaxMinDimension();

};

#endif

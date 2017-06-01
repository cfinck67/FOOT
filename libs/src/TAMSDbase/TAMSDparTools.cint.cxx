// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDparToolsdOcint

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "TAMSDparTools.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDparTools(void *p = 0);
   static void *newArray_TAMSDparTools(Long_t size, void *p);
   static void delete_TAMSDparTools(void *p);
   static void deleteArray_TAMSDparTools(void *p);
   static void destruct_TAMSDparTools(void *p);
   static void streamer_TAMSDparTools(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDparTools*)
   {
      ::TAMSDparTools *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDparTools >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDparTools", ::TAMSDparTools::Class_Version(), "TAMSDparTools.hxx", 22,
                  typeid(::TAMSDparTools), DefineBehavior(ptr, ptr),
                  &::TAMSDparTools::Dictionary, isa_proxy, 16,
                  sizeof(::TAMSDparTools) );
      instance.SetNew(&new_TAMSDparTools);
      instance.SetNewArray(&newArray_TAMSDparTools);
      instance.SetDelete(&delete_TAMSDparTools);
      instance.SetDeleteArray(&deleteArray_TAMSDparTools);
      instance.SetDestructor(&destruct_TAMSDparTools);
      instance.SetStreamerFunc(&streamer_TAMSDparTools);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDparTools*)
   {
      return GenerateInitInstanceLocal((::TAMSDparTools*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDparTools*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDparTools::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDparTools::Class_Name()
{
   return "TAMSDparTools";
}

//______________________________________________________________________________
const char *TAMSDparTools::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparTools*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDparTools::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparTools*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDparTools::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparTools*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDparTools::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparTools*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDparTools::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDparTools.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TAGpara::Streamer(R__b);
      R__b.StreamObject(&(fFileStream),typeid(fFileStream));
      fFileName.Streamer(R__b);
      R__b >> fDebugLevel;
      R__b.CheckByteCount(R__s, R__c, TAMSDparTools::IsA());
   } else {
      R__c = R__b.WriteVersion(TAMSDparTools::IsA(), kTRUE);
      TAGpara::Streamer(R__b);
      R__b.StreamObject(&(fFileStream),typeid(fFileStream));
      fFileName.Streamer(R__b);
      R__b << fDebugLevel;
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDparTools(void *p) {
      return  p ? new(p) ::TAMSDparTools : new ::TAMSDparTools;
   }
   static void *newArray_TAMSDparTools(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDparTools[nElements] : new ::TAMSDparTools[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDparTools(void *p) {
      delete ((::TAMSDparTools*)p);
   }
   static void deleteArray_TAMSDparTools(void *p) {
      delete [] ((::TAMSDparTools*)p);
   }
   static void destruct_TAMSDparTools(void *p) {
      typedef ::TAMSDparTools current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TAMSDparTools(TBuffer &buf, void *obj) {
      ((::TAMSDparTools*)obj)->::TAMSDparTools::Streamer(buf);
   }
} // end of namespace ROOT for class ::TAMSDparTools

namespace {
  void TriggerDictionaryInitialization_TAMSDparToolsdOcint_Impl() {
    static const char* headers[] = {
"TAMSDparTools.hxx",
0
    };
    static const char* includePaths[] = {
"/opt/exp_software/foot/root/current/include",
"../TAGbase",
"../TAVTbase",
"../../../Simulation/TXT2ROOT",
"../../../Simulation",
"../../../libs/GenFit/fitters/include",
"../../../libs/GenFit/core/include",
"/opt/exp_software/foot/root/root-6.04.18/include",
"/home/FOOT-T3/sartifoott3/software/libs/src/TAMSDbase/",
0
    };
    static const char* fwdDeclCode = 
R"DICTFWDDCLS(
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate("$clingAutoload$TAMSDparTools.hxx")))  TAMSDparTools;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDparTools.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDparTools", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDparTools.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDparToolsdOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDparToolsdOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDparToolsdOcint() {
  TriggerDictionaryInitialization_TAMSDparToolsdOcint_Impl();
}

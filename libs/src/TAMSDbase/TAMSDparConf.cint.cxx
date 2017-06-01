// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDparConfdOcint

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
#include "TAMSDparConf.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDparConf(void *p = 0);
   static void *newArray_TAMSDparConf(Long_t size, void *p);
   static void delete_TAMSDparConf(void *p);
   static void deleteArray_TAMSDparConf(void *p);
   static void destruct_TAMSDparConf(void *p);
   static void streamer_TAMSDparConf(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDparConf*)
   {
      ::TAMSDparConf *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDparConf >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDparConf", ::TAMSDparConf::Class_Version(), "TAMSDparConf.hxx", 20,
                  typeid(::TAMSDparConf), DefineBehavior(ptr, ptr),
                  &::TAMSDparConf::Dictionary, isa_proxy, 16,
                  sizeof(::TAMSDparConf) );
      instance.SetNew(&new_TAMSDparConf);
      instance.SetNewArray(&newArray_TAMSDparConf);
      instance.SetDelete(&delete_TAMSDparConf);
      instance.SetDeleteArray(&deleteArray_TAMSDparConf);
      instance.SetDestructor(&destruct_TAMSDparConf);
      instance.SetStreamerFunc(&streamer_TAMSDparConf);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDparConf*)
   {
      return GenerateInitInstanceLocal((::TAMSDparConf*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDparConf*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDparConf::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDparConf::Class_Name()
{
   return "TAMSDparConf";
}

//______________________________________________________________________________
const char *TAMSDparConf::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparConf*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDparConf::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparConf*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDparConf::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparConf*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDparConf::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDparConf*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDparConf::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDparConf.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      TAMSDparTools::Streamer(R__b);
      R__b >> fSensorsN;
      int R__i;
      for (R__i = 0; R__i < 10; R__i++)
         fSensorParameter[R__i].Streamer(R__b);
      R__b.StreamObject(&(fAnalysisParameter),typeid(fAnalysisParameter));
      R__b.CheckByteCount(R__s, R__c, TAMSDparConf::IsA());
   } else {
      R__c = R__b.WriteVersion(TAMSDparConf::IsA(), kTRUE);
      TAMSDparTools::Streamer(R__b);
      R__b << fSensorsN;
      int R__i;
      for (R__i = 0; R__i < 10; R__i++)
         fSensorParameter[R__i].Streamer(R__b);
      R__b.StreamObject(&(fAnalysisParameter),typeid(fAnalysisParameter));
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDparConf(void *p) {
      return  p ? new(p) ::TAMSDparConf : new ::TAMSDparConf;
   }
   static void *newArray_TAMSDparConf(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDparConf[nElements] : new ::TAMSDparConf[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDparConf(void *p) {
      delete ((::TAMSDparConf*)p);
   }
   static void deleteArray_TAMSDparConf(void *p) {
      delete [] ((::TAMSDparConf*)p);
   }
   static void destruct_TAMSDparConf(void *p) {
      typedef ::TAMSDparConf current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TAMSDparConf(TBuffer &buf, void *obj) {
      ((::TAMSDparConf*)obj)->::TAMSDparConf::Streamer(buf);
   }
} // end of namespace ROOT for class ::TAMSDparConf

namespace {
  void TriggerDictionaryInitialization_TAMSDparConfdOcint_Impl() {
    static const char* headers[] = {
"TAMSDparConf.hxx",
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
class __attribute__((annotate("$clingAutoload$TAMSDparConf.hxx")))  TAMSDparConf;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDparConf.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDparConf", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDparConf.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDparConfdOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDparConfdOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDparConfdOcint() {
  TriggerDictionaryInitialization_TAMSDparConfdOcint_Impl();
}

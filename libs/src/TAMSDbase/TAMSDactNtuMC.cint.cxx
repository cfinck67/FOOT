// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME TAMSDactNtuMCdOcint

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
#include "TAMSDactNtuMC.hxx"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_TAMSDactNtuMC(void *p = 0);
   static void *newArray_TAMSDactNtuMC(Long_t size, void *p);
   static void delete_TAMSDactNtuMC(void *p);
   static void deleteArray_TAMSDactNtuMC(void *p);
   static void destruct_TAMSDactNtuMC(void *p);
   static void streamer_TAMSDactNtuMC(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::TAMSDactNtuMC*)
   {
      ::TAMSDactNtuMC *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::TAMSDactNtuMC >(0);
      static ::ROOT::TGenericClassInfo 
         instance("TAMSDactNtuMC", ::TAMSDactNtuMC::Class_Version(), "TAMSDactNtuMC.hxx", 17,
                  typeid(::TAMSDactNtuMC), DefineBehavior(ptr, ptr),
                  &::TAMSDactNtuMC::Dictionary, isa_proxy, 16,
                  sizeof(::TAMSDactNtuMC) );
      instance.SetNew(&new_TAMSDactNtuMC);
      instance.SetNewArray(&newArray_TAMSDactNtuMC);
      instance.SetDelete(&delete_TAMSDactNtuMC);
      instance.SetDeleteArray(&deleteArray_TAMSDactNtuMC);
      instance.SetDestructor(&destruct_TAMSDactNtuMC);
      instance.SetStreamerFunc(&streamer_TAMSDactNtuMC);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::TAMSDactNtuMC*)
   {
      return GenerateInitInstanceLocal((::TAMSDactNtuMC*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::TAMSDactNtuMC*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr TAMSDactNtuMC::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *TAMSDactNtuMC::Class_Name()
{
   return "TAMSDactNtuMC";
}

//______________________________________________________________________________
const char *TAMSDactNtuMC::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDactNtuMC*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int TAMSDactNtuMC::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::TAMSDactNtuMC*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *TAMSDactNtuMC::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDactNtuMC*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *TAMSDactNtuMC::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::TAMSDactNtuMC*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void TAMSDactNtuMC::Streamer(TBuffer &R__b)
{
   // Stream an object of class TAMSDactNtuMC.

   TAGaction::Streamer(R__b);
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_TAMSDactNtuMC(void *p) {
      return  p ? new(p) ::TAMSDactNtuMC : new ::TAMSDactNtuMC;
   }
   static void *newArray_TAMSDactNtuMC(Long_t nElements, void *p) {
      return p ? new(p) ::TAMSDactNtuMC[nElements] : new ::TAMSDactNtuMC[nElements];
   }
   // Wrapper around operator delete
   static void delete_TAMSDactNtuMC(void *p) {
      delete ((::TAMSDactNtuMC*)p);
   }
   static void deleteArray_TAMSDactNtuMC(void *p) {
      delete [] ((::TAMSDactNtuMC*)p);
   }
   static void destruct_TAMSDactNtuMC(void *p) {
      typedef ::TAMSDactNtuMC current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_TAMSDactNtuMC(TBuffer &buf, void *obj) {
      ((::TAMSDactNtuMC*)obj)->::TAMSDactNtuMC::Streamer(buf);
   }
} // end of namespace ROOT for class ::TAMSDactNtuMC

namespace {
  void TriggerDictionaryInitialization_TAMSDactNtuMCdOcint_Impl() {
    static const char* headers[] = {
"TAMSDactNtuMC.hxx",
0
    };
    static const char* includePaths[] = {
"/opt/exp_software/foot/root/current/include",
"../TAGbase",
"../TAGfoot",
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
class __attribute__((annotate("$clingAutoload$TAMSDactNtuMC.hxx")))  TAMSDactNtuMC;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "TAMSDactNtuMC.hxx"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"TAMSDactNtuMC", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("TAMSDactNtuMC.cint",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_TAMSDactNtuMCdOcint_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_TAMSDactNtuMCdOcint_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_TAMSDactNtuMCdOcint() {
  TriggerDictionaryInitialization_TAMSDactNtuMCdOcint_Impl();
}

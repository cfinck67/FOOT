#ifndef _TAGgeoTrafo_HXX
#define _TAGgeoTrafo_HXX

#include <TVector3.h>
#include <TString.h>
#include <TGeoMatrix.h>

#include "TAGaction.hxx"

class TObjArray;

class TAGgeoTrafo : public TAGaction {

  //Everything is in cm... Conversion to other units must be performed
  //by the user

  public:
                    TAGgeoTrafo();
    virtual         ~TAGgeoTrafo();

    bool InitGeo(TString ifile);
    bool LoadIRGeo(TString ifile);
    bool LoadBMGeo(TString ifile);
    bool LoadDCGeo(TString ifile);
    bool LoadVtxGeo(TString ifile);
    bool LoadInTrGeo(TString ifile);
    bool LoadTofGeo(TString ifile);
    bool LoadCalGeo(TString ifile);

    TVector3 FromBMLocalToGlobal(TVector3 apoi);
    TVector3 FromDCLocalToGlobal(TVector3 apoi);
    TVector3 FromVTLocalToGlobal(TVector3 apoi);
    TVector3 FromTWLocalToGlobal(TVector3 apoi);
    TVector3 FromITLocalToGlobal(TVector3 apoi);
    TVector3 FromCALocalToGlobal(TVector3 apoi);

    TVector3 VecFromDCLocalToGlobal(TVector3 avec);
    TVector3 VecFromBMLocalToGlobal(TVector3 avec);
    TVector3 VecFromVTLocalToGlobal(TVector3 avec);
    TVector3 VecFromTWLocalToGlobal(TVector3 avec);
    TVector3 VecFromITLocalToGlobal(TVector3 avec);
    TVector3 VecFromCALocalToGlobal(TVector3 avec);

    TVector3 FromGlobalToBMLocal(TVector3 apoi);
    TVector3 FromGlobalToDCLocal(TVector3 apoi);
    TVector3 FromGlobalToVTLocal(TVector3 apoi);
    TVector3 FromGlobalToTWLocal(TVector3 apoi);
    TVector3 FromGlobalToITLocal(TVector3 apoi);
    TVector3 FromGlobalToCALocal(TVector3 apoi);

    TVector3 VecFromGlobalToDCLocal(TVector3 avec);
    TVector3 VecFromGlobalToBMLocal(TVector3 avec);
    TVector3 VecFromGlobalToVTLocal(TVector3 avec);
    TVector3 VecFromGlobalToTWLocal(TVector3 avec);
    TVector3 VecFromGlobalToITLocal(TVector3 avec);
    TVector3 VecFromGlobalToCALocal(TVector3 avec);

    //Interaction point
    TVector3 GetIP();
    void     SetIP(TVector3 ip);

    // Start Counter
    TVector3 GetSCCenter();
    TVector3 GetSCAngles();
    void     SetSCCenter(double x_c, double y_c, double z_c);
    void     SetSCCenter(TVector3 cen);
    void     SetSCAngles(double x_d, double y_d, double z_d);
    void     SetSCAngles(TVector3 ang);

    // Beam Monitor
    TVector3 GetBMCenter();
    TVector3 GetBMAngles();
    void     SetBMCenter(double x_c, double y_c, double z_c);
    void     SetBMCenter(TVector3 cen);
    void     SetBMAngles(double x_d, double y_d, double z_d);
    void     SetBMAngles(TVector3 ang);

    TVector3 GetDCCenter();
    TVector3 GetDCAngles();
    void     SetDCCenter(double x_c, double y_c, double z_c);
    void     SetDCCenter(TVector3 cen);
    void     SetDCAngles(double x_d, double y_d, double z_d);
    void     SetDCAngles(TVector3 ang);

    // Vertex
    TVector3 GetVTCenter();
    TVector3 GetVTAngles();
    void     SetVTCenter(double x_c, double y_c, double z_c);
    void     SetVTCenter(TVector3 cen);
    void     SetVTAngles(double x_d, double y_d, double z_d);
    void     SetVTAngles(TVector3 ang);

    // tof Wall
    TVector3 GetTWCenter();
    TVector3 GetTWAngles();
    void     SetTWCenter(double x_c, double y_c, double z_c);
    void     SetTWCenter(TVector3 cen);
    void     SetTWAngles(double x_d, double y_d, double z_d);
    void     SetTWAngles(TVector3 ang);

    // tof Wall
    TVector3 GetITCenter();
    TVector3 GetITAngles();
    void     SetITCenter(double x_c, double y_c, double z_c);
    void     SetITCenter(TVector3 cen);
    void     SetITAngles(double x_d, double y_d, double z_d);
    void     SetITAngles(TVector3 ang);

    // tof Wall
    TVector3 GetCACenter();
    TVector3 GetCAAngles();
    void     SetCACenter(double x_c, double y_c, double z_c);
    void     SetCACenter(TVector3 cen);
    void     SetCAAngles(double x_d, double y_d, double z_d);
    void     SetCAAngles(TVector3 ang);

    // Veto
    TVector3 GetVECenter();
    TVector3 GetVEAngles();
    void     SetVECenter(double x_c, double y_c, double z_c);
    void     SetVECenter(TVector3 cen);
    void     SetVEAngles(double x_d, double y_d, double z_d);
    void     SetVEAngles(TVector3 ang);

    const TGeoHMatrix*    GetTrafo(const char* name) const;
    void                  AddTrafo(TGeoHMatrix* mat);
   
    TVector3              Global2Local(const char* name, TVector3& glob) const;
    TVector3              Global2LocalVect(const char* name, TVector3& glob) const;
    TVector3              Local2Global(const char* name, TVector3& loc)  const;
    TVector3              Local2GlobalVect(const char* name, TVector3& loc)  const;
   
  public:
   
   static void     SetDefaultActName(TString& name) { fgDefaultActName = name; }
   static TString  GetDefaultActName()              { return fgDefaultActName; }
   
   static           Float_t CmToMu()                { return fgkCmToMu; }
   static           Float_t MuToCm()                { return fgkMuToCm; }
   
   static           Float_t MmToMu()                { return fgkMmToMu; }
   static           Float_t MuToMm()                { return fgkMuToMm; }
   
   static           Float_t MmToCm()                { return fgkMmToCm; }
   static           Float_t CmToMm()                { return fgkCmToMm; }
   
   static           Float_t MToCm()                 { return fgkMToCm; }
   static           Float_t CmToM()                 { return fgkCmToM; }
	
   static           Float_t SecToNs()                { return fgkSecToNs; }
   static           Float_t NsToSec()                { return fgkNsToSec; }	


   static     const Char_t* GetDefaultGeomName()    { return fgkGeomName; }
   static     const Char_t* GetDefaultGeomTitle()   { return fgkGeomTitle; }

   static           Char_t  GetDefaultTransp()      { return fgDefaultTransp; }
   static           void    SetDefaultTransparency(Char_t transparency) { fgDefaultTransp = transparency; }
   
    ClassDef(TAGgeoTrafo,1)

  private:

    TObjArray* fMatrixList;
    TGeoHMatrix* fIdentity; // dummy matrix
   
    TVector3 m_ip;

    //Those are the positions and the angles in the FIRST ref. frame
    TVector3 m_SC_center, m_SC_angles;
    TVector3 m_BM_center, m_BM_angles;
    TVector3 m_VT_center, m_VT_angles;
    TVector3 m_DC_center, m_DC_angles;
    TVector3 m_TW_center, m_TW_angles;
    TVector3 m_IT_center, m_IT_angles;
    TVector3 m_CA_center, m_CA_angles;
    TVector3 m_VE_center, m_VE_angles;
   
  private:
   
   static TString fgDefaultActName;
   static const Float_t fgkCmToMu;        //! conversion factor bw cm and micron
   static const Float_t fgkMuToCm;        //! conversion factor bw micron and cm
   static const Float_t fgkMmToMu;        //! conversion factor bw mm and micron
   static const Float_t fgkMuToMm;        //! conversion factor bw micron and mm
   static const Float_t fgkMmToCm;        //! conversion factor bw mm and cm
   static const Float_t fgkCmToMm;        //! conversion factor bw cm and mm
   static const Float_t fgkSecToNs;        //! conversion factor bw second and nanosecond
   static const Float_t fgkNsToSec;        //! conversion factor bw nanosecond and second
   static const Float_t fgkCmToM;         //! conversion factor bw cm and m
   static const Float_t fgkMToCm;         //! conversion factor bw m and cm
   
   static const Char_t* fgkGeomName;      //! name of GeoManager
   static const Char_t* fgkGeomTitle;     //! title of GeoManager
   static       Char_t  fgDefaultTransp;  // default transparency value
};

#include "TAGgeoTrafo.icc"

#endif

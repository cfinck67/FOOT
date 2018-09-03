#ifndef MAGICSKILLS_h
#define MAGICSKILLS_h


#include <TROOT.h>
#include "TFile.h"
#include <TSystem.h> 

// #include <TTree.h>
// #include "TH1.h"
// #include "TH1D.h"
#include "TH1F.h"
// #include <TF1.h>
// #include "TH2.h"
#include "TH2F.h"
// #include "TF2.h"
// #include <TGraph.h>
// #include <TGraphErrors.h>
#include "TGraphAsymmErrors.h"
// #include <TGraphPainter.h>
// #include <TAxis.h>
// #include "TProfile.h"
// #include "THStack.h"
// #include "TBox.h"

#include <TStyle.h>
#include <TLegend.h>
// #include <TCanvas.h>
// #include <TPaveStats.h>
// #include "TPaveText.h"
// #include "TArrayI.h"

// #include "TObject.h"
// #include "TKey.h"
// #include <TString.h>
// #include <TClass.h>
// #include "TVector3.h"
// #include "TRotation.h" 
// #include "TString.h"
// #include "TFormula.h"
// #include "TLorentzVector.h"
// #include "TRandom3.h"
// #include "TRandom.h"
// #include "TChain.h" 
// #include "TGenPhaseSpace.h"

#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <memory>

using namespace std;

#define LegX1 0.6
#define LegY1 0.6
#define LegX2 0.75
#define LegY2 0.75


vector<string> Tokenize(string inString, string divisor );

int ThinkPositive(int x) ;

// int ForinosRule(double d) {
// 
//   if (d == 0)
//     return 0;
//   
//   int x = trunc(d);
// 
//   double y = d;
//   int n = 0;
//   
//   if ( x == 0 ) {
//     
//     while( x == 0 && n < 15 ) {
//       n++;
//       y *= 10;
//       x = trunc(y);
//     }
//     if (n==15) return 0;
//     if (x != 1) return n;
//   }
//   
//   if ( x == 1 ) {
//     y *= 10;
//     x = trunc(y);
//     n++;
//     return n;
//   }
//   
//   if ( x > 1 && x<20 ) {
//     return n; 
//   }
//   
// 
//   if (x>20) {
//     do {
//       
//       y = y/10;
//       x = trunc(y);
//       n++;
//     }  while ( x>20 );
//     return -n;
//   }
//   
//   return 0;
// }

// return num of signif  
// BG vers
int ForinosRule(double d) ;

void RemoveSpace( string* s );
double Round( double in, int precision );

//traspose a matrix
void Traspose(TH2F* h2);

template <class T>
T* VtoA (vector<T> x) {
  static T* a = new T[x.size()];
  for (int i=0; i<(int)x.size(); i++) {
    a[i]=x[i];
    //cout<<"a :"<<a[i]<<endl;
  }
  return a;
}

template <class T>
vector<T> AtoV (T* x, int arrDim, int dim) {
  vector<T> av(x, x + arrDim / dim );
  return av;
}
  
//Replace part of "original" if it founds "erase" with "add". Otherwise return input string.
string StrReplace(string original, string erase, string add);



template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8, class T9>
  string AllToS (T1 x, T2 y, T3 z, T4 t, T5 k, T6 h, T7 l, T8 e, T9 f) {
  
  stringstream ss;
  ss<<x<<y<<z<<t<<k<<h<<l<<e<<f;
  return ss.str();

}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8>
  string AllToS (T1 x, T2 y, T3 z, T4 t, T5 k, T6 h, T7 l, T8 e) {
  
  stringstream ss;
  ss<<x<<y<<z<<t<<k<<h<<l<<e;
  return ss.str();

}

template <class T1, class T2, class T3, class T4, class T5, class T6, class T7>
  string AllToS (T1 x, T2 y, T3 z, T4 t, T5 k, T6 h, T7 l) {
  
  stringstream ss;
  ss<<x<<y<<z<<t<<k<<h<<l;
  return ss.str();

}

template <class T1, class T2, class T3, class T4, class T5, class T6>
  string AllToS (T1 x, T2 y, T3 z, T4 t, T5 k, T6 h) {
  
  stringstream ss;
  ss<<x<<y<<z<<t<<k<<h;
  return ss.str();

}

template <class T1, class T2, class T3, class T4, class T5>
  string AllToS (T1 x, T2 y, T3 z, T4 t, T5 k) {
  
  stringstream ss;
  ss<<x<<y<<z<<t<<k;
  return ss.str();

}

template <class T1, class T2, class T3, class T4>
string AllToS (T1 x, T2 y, T3 z, T4 t) {
  
  stringstream ss;
  ss<<x<<y<<z<<t;
  return ss.str();

}

template <class T1, class T2, class T3>
string AllToS (T1 x, T2 y, T3 z) {
  
  stringstream ss;
  ss<<x<<y<<z;
  return ss.str();

}

template <class T1, class T2>
string AllToS (T1 x, T2 y) {
  
  stringstream ss;
  ss<<x<<y;
  return ss.str();

}

template <typename T>
string AllToS (vector<T> x) {
  
  stringstream ss;
  for (int i=0; i<(int)x.size(); i++) 
    ss<<x[i];
  return ss.str();

}

template <typename T>
string ss(T x) {
  stringstream ss;
  string s1 = "fai";
  string s2 = x;
  //string s2 = "cacca";

  ss<<s1<<s2;

  string s3;
  string s4;

  //ss>>s3;
  //ss>>s4;
 

  //cout<<"Parere: "<<s3<<"+"<<s4<<endl;

  //cout<<"Parere: "<<ss.str()<<endl;

  return ss.str();

}

/*template <typename T>
string ss(T x) ;*/

bool isInt(double x) ;

void StdLegend( TLegend* fLegend );

void ColorList () ;
/********************************************************/
double mcm (vector<double> x) ;
  
void SmartCopy( const TH1F * h_in, TH1F *&h_out );
  
void SmartCopy( const TH2F * h_in, TH2F *&h_out );
 

void DivideByBinWidth( TH1F * h );
  
  
 string MakeLatexArray( float up, float down );
void GetPlusAndMinus( float & u, float & d );
void MirrorRebin( TH2F * h_in );
void MirrorRebin( TH1F * h_in );


#endif


































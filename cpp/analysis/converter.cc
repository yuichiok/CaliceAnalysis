/*------------------------------------------------------------------------------
   main.cpp
------------------------------------------------------------------------------*/
#define _GLIBCXX_USE_CXX11_ABI 0

#include "TROOT.h"
#include "TFile.h"
#include "src/Sim2Build.cc"
#include "src/TreeWriter.cc"

int main(){
  
  Int_t set_energy = 10;

  TString particle = "e-";
  TString energy   = TString::Format("%d",set_energy);
  TString settings = particle + " " + energy;
  
  Sim2Build Sim2Build(settings);
  Sim2Build.Loop();

  return 0;
}

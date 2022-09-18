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

  TString file_path = "../../data/sim/";
  TString prefix    = "ECAL_QGSP_BERT_conf6";
  TString setup     = "_" + particle + "_" + energy + ".0GeV_";
  TString suffix    = "build.root";

  TString full_name = file_path + prefix + setup + suffix;

  Sim2Build Sim2Build(full_name);
  Sim2Build.Loop();

  return 0;
}

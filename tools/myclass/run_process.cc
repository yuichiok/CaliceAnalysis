#include "TROOT.h"
#include "TFile.h"
#include "Sim2Build.C"
#include "src/TreeWriter.cc"

int run_process(Int_t set_energy = 10){
  
  TString particle = "e-";
  TString energy   = TString::Format("%d",set_energy);

  TString file_path = "../../data/sim/";
  TString prefix    = "ECAL_QGSP_BERT_conf6";
  TString setup     = "_" + particle + "_" + energy + "_";
  TString suffix    = "build.root";

  TString full_name = file_path + prefix + setup + suffix;

  Sim2Build Sim2Build(full_name);
  Sim2Build.Loop();

  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

  gSystem->Exit(0);

  return 0;
}

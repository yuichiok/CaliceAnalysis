#include "TROOT.h"
#include "TFile.h"
#include "Sim2Build.C"
#include "src/TreeWriter.cc"

int run_process(){
  
  TTree *tree = 0;
  Sim2Build Sim2Build(tree);
  Sim2Build.Loop();

  ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

  gSystem->Exit(0);

  return 0;
}

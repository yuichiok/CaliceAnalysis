#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  // TString filename = "../data/3.0GeV_W_run_050282/full_run.root";
  TString filename = "../../../download/3GeV_MIPscan_eudaq/3GeV_MIPscan_eudaq_run_050480_build.root";
  TFile *file = new TFile(filename);

  TString treename = "ecal";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("TBEvent");

  std::cout << "processed..." << std::endl;

  return 0;


}


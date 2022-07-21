#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  TString filename = "/home/calice/TB2022-06/SiWECAL-TB-monitoring/data/eudaq_run_090121/full_run.root";
  TFile *file = new TFile(filename);

  TString treename = "ecal";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("EventControl");

  std::cout << "processed..." << std::endl;

  return 0;

}

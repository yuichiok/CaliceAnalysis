#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  TString filename = "~/Work/TBData/rootfiles/3GeVMIPscan_run_050043_merged.root";
  TFile *file = new TFile(filename);

  TString treename = "siwecaldecoded";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("EventControl");

  std::cout << "processed..." << std::endl;

  return 0;


}
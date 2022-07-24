#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  TString filename = "../../../data/sim/e10GeV/ECAL_QGSP_BERT_conf5_e-_10.0GeV.root";
  TFile *file = new TFile(filename);

  TString treename = "ecal";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("EventControl");

  std::cout << "processed..." << std::endl;

  return 0;

}

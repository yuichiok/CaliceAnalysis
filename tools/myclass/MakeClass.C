#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  TString filename = "../../data/conv_sim/ECAL_QGSP_BERT_conf6_e-_10_converted.root";
  TFile *file = new TFile(filename);

  TString treename = "ecal";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("ConvSimSample");

  std::cout << "processed..." << std::endl;

  return 0;


}
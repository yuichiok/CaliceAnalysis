#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  TString filename = "../../data/reco/raw_siwecal_90320_e-_10.0GeV_build.root";
  TFile *file = new TFile(filename);

  TString treename = "ecal";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("RecoSample");

  std::cout << "processed..." << std::endl;

  return 0;


}
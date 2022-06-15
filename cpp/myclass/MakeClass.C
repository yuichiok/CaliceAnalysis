#include "TROOT.h"
#include "TFile.h"

int MakeClass(){

  TString filename = "../../data/build/raw_siwecal_90132/full_run.root";
  TFile *file = new TFile(filename);

  TString treename = "ecal";
  TTree* tree = (TTree*)file->Get( treename ) ;

  file->ls();

  tree->MakeClass("EventControl");

  std::cout << "processed..." << std::endl;

  return 0;


}
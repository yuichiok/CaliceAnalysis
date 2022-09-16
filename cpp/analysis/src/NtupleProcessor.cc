#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/NtupleProcessor.hh"

using std::cout;   using std::endl;

NtupleProcessor::NtupleProcessor(TString o, int me)
: eAnalyzer(o), tIter(eAnalyzer), options(o), maxEvents(me)
{
  // PARAM output
  TString dummy_label = "raw_siwecal_90378_e-_20.0GeV_build";
  TString filename = "../../data/reco/" + dummy_label + ".root";

  ntupleFile = TFile::Open(filename);
  if(!ntupleFile) cout << "NtupleProcessor: ERROR: Unable to open file " << filename << endl;
  
  TTree *ntuple_reco   = (TTree*) ntupleFile->Get("ecal");
  if(!ntuple_reco) cout << "NtupleProcessor: ERROR: Unable to open ttree in " << filename << endl;
  
  ntuple_reco->Process(&tIter, options);

}
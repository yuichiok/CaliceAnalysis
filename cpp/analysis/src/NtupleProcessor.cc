#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/NtupleProcessor.hh"
#include "../include/FileSelector.hh"

using std::cout;   using std::endl;

NtupleProcessor::NtupleProcessor(TString o, int me)
: eAnalyzer(o), tIter(eAnalyzer), options(o), maxEvents(me)
{
  FileSelector fs(options);

  // PARAM output
  TString filename = fs.GetRunName_with_Path();

  ntupleFile = TFile::Open(filename);
  if(!ntupleFile) cout << "NtupleProcessor: ERROR: Unable to open file " << filename << endl;
  
  TTree *ntuple_reco   = (TTree*) ntupleFile->Get("ecal");
  if(!ntuple_reco) cout << "NtupleProcessor: ERROR: Unable to open ttree in " << filename << endl;
  
  ntuple_reco->Process(&tIter, options);

}
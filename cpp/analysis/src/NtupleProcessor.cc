#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/NtupleProcessor.hh"
#include "../include/FileSelector.hh"

using std::cout;   using std::endl;
using std::cerr;

NtupleProcessor::NtupleProcessor(TString o, int me)
: eAnalyzer(o), tIter(eAnalyzer), options(o), maxEvents(me)
{
  // Reconstructed
    FileSelector fs(options);
    TString filename = fs.GetRunName_with_path();

    ntupleFile = TFile::Open(filename);
    if(!ntupleFile)  cerr << " [NtupleProcessor] ERROR: Unable to open file " << filename << endl;
    
    TTree *ntuple = (TTree*) ntupleFile->Get("ecal");
    if(!ntuple)      cerr << " [NtupleProcessor] ERROR: Unable to open ttree in " << filename << endl;
    
    ntuple->Process(&tIter, options);

}
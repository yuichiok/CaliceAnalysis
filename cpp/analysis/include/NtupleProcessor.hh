#ifndef GUARD_NtupleProcessor_h
#define GUARD_NtupleProcessor_h

#include <TString.h>
#include "ECALAnalyzer.hh"
#include "TreeIterator.hh"

class NtupleProcessor
{
  public:
    NtupleProcessor(TString o="", int me = -1);
    ~NtupleProcessor(){}

    ECALAnalyzer   eAnalyzer;
    TreeIterator   tIter;
    TString        options;        // Extra options for this processing.
    int            maxEvents;      // Max number of events to run over in this ntuple

    TFile          *ntupleFile;

  private: 

};

#endif
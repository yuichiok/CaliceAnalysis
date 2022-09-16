#ifndef GUARD_ECALAnalyzer_h
#define GUARD_ECALAnalyzer_h

/*------------------------------------------------------------------------------
   NtupleProcessor
 Created : 2022-09-02  okugawa
 Main class of NtupleProcessor program. Created to handle input and running of
 ntuple processing.
 Takes input variables (datasets, configurations, etc) and sets up the
 appropriate classes to handle each portion of the analysis process.
------------------------------------------------------------------------------*/
#include <TString.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <fstream>
#include "../library/TreeStructures.hh"
#include "TreeReader.hh"

class ECALAnalyzer
{
  public:
    ECALAnalyzer(TString o="");
    virtual ~ECALAnalyzer(){};

  // methods
    bool             MapTree( TTree* ); // Maps class variables to an input TTree.
    void             Analyze( Long64_t entry );
    bool             Select();  // Evaluates the class' list of event selection criteria
    virtual Bool_t   Notify();


  // Running Variables
    TString  options;  // Options input with TreeIterator.
    TTree   *fChain;   //!pointer to the analyzed TTree or TChain
    Int_t    fCurrent; //!current Tree number in a TChain

  // Extra data
    long patEventsAnalyzed;     // Number of events that were processed to make the Ntuple.
    long entriesInNtuple  ;     // Number of events that were processed to make the Ntuple.

  // Fixed size dimensions of array or collections stored in the TTree if any.

  private:

    ECAL_reco   _reco;
    Branch_reco _branch;

};

#endif
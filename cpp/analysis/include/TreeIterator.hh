#ifndef GUARD_TreeIterator_h
#define GUARD_TreeIterator_h

/*------------------------------------------------------------------------------
   NtupleProcessor
 Created : 2022-09-02  okugawa
 Main class of NtupleProcessor program. Created to handle input and running of
 ntuple processing.
 Takes input variables (datasets, configurations, etc) and sets up the
 appropriate classes to handle each portion of the analysis process.
------------------------------------------------------------------------------*/
#include <map>
#include <sstream>
#include <vector>
#include <TBranch.h>
#include <TChain.h>
#include <TSelector.h>
#include <TTree.h>
#include "ECALAnalyzer.hh"

typedef unsigned long counter;

class TreeIterator : public TSelector
{
  public:
    TreeIterator(ECALAnalyzer &eh) : eAnalyzer(eh), fChain(0) {}
    virtual ~TreeIterator(){}

  // Overloaded TSelector Functions
    virtual Int_t   Version() const { return 2; }
    virtual void    Begin     (TTree *tree);
    virtual void    SlaveBegin(TTree *tree);
    virtual void    Init      (TTree *tree);
    virtual Bool_t  Notify();
    virtual Bool_t  Process   (Long64_t entry);

    virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
    virtual void    SetOption(const char *option) { fOption = option; }
    virtual void    SetObject(TObject *obj)       { fObject = obj;    }
    virtual void    SetInputList(TList *input)    { fInput  = input;  }
    virtual TList  *GetOutputList() const         { return fOutput;   }

    virtual void    SlaveTerminate();
    virtual void    Terminate();

  // Entry handler & list of Histogram Extractors
    ECALAnalyzer &eAnalyzer;

  // TTree
    TTree *fChain;

  // Counters
    Long64_t nEntries;             // Total number of entries in tree/chain
    Long64_t finalEntry;           // Index of last entry
    Long64_t nEntriesProcessed;    // Total number of events processed from chain


  private: 

};

#endif
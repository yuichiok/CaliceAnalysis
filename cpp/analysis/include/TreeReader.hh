#ifndef GUARD_TreeReader_h
#define GUARD_TreeReader_h

/*------------------------------------------------------------------------------
   TreeReader
 Created : 2022-09-02  okugawa
 Main class of TreeReader program.
------------------------------------------------------------------------------*/

#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include "../library/TreeStructures.hh"

class TreeReader
{
  public:
    TreeReader();
    virtual ~TreeReader() {};

    void InitializeRecoReadTree (TTree *tree, ECAL_reco & _data,  Branch_reco & _branch);

  private: 

};

#endif
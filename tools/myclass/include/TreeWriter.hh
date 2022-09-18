#include <iostream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include "../library/TreeStructures.hh"
#ifndef _TreeWriter_hh
#define _TreeWriter_hh

class TreeWriter 
{
  public:
    //
    //	Constants
    //
    
    //
    //	Constructors
    //
    TreeWriter ();
    virtual ~TreeWriter () {};
    //
    //	Methods
    //
    
    void InitializeECALTree(TTree * tree, ECAL_data & data);
  private:
    //
    //	Data
    //
    
    //
    //	Private methods
    //
};

#endif

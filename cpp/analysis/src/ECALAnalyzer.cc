#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/ECALAnalyzer.hh"
#include "../include/TreeReader.hh"
#include "../include/FileSelector.hh"
#include "../include/HistManager.hh"

using std::cout;   using std::endl;

ECALAnalyzer::ECALAnalyzer(TString o)
: options(o)
{
  // TEST output
    cout << "[Begin ECALAnalyzer]" << endl;

    patEventsAnalyzed = 0;
    entriesInNtuple   = 0;

}

bool ECALAnalyzer::MapTree(TTree* tree)
{
  // Maps TTree to class' variables.
  // TO DO: Implement check for correct mapping, return result?
  //    - Set up exception handling for negative result.

    entriesInNtuple = tree->GetEntries();

  // Set branch addresses and branch pointers
    if (!tree) return false;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    TreeReader reader;
    _data = _reco;
    reader.InitializeRecoReadTree(fChain, _data, _branch);

    Notify();

    return true;

}

void ECALAnalyzer::Analyze(Long64_t entry)
{

   HistManager hm;
   Float_t sum_slab_energy[NSLABS] = {0};
   
   if(_data.nhit_slab < 13) return;

   hm.h_sum_energy->Fill(_data.sum_energy);

   for (int ihit = 0; ihit < _data.nhit_len; ihit++)
   {
      hm.h_hit_slab->Fill(_data.hit_slab[ihit]);
      hm.h_hit_energy->Fill(_data.hit_energy[ihit]);

      int ihit_slab = _data.hit_slab[ihit];
      sum_slab_energy[ihit_slab] += _data.hit_energy[ihit];
      hm.h_hit_slab_energy[ihit_slab]->Fill(_data.hit_energy[ihit]);
   }

   for (int islab = 0; islab < NSLABS; islab++)
   {
      hm.h_sum_slab_energy[islab]->Fill(sum_slab_energy[islab]);
   }

   FileSelector fs(options);

   TFile *outfile = new TFile( "rootfiles/" + fs.GetRecoSim() + "/" + fs.GetRunName() + "_quality.root","RECREATE");
   cout << "Output: " << outfile << endl;

   hm.WriteLists(outfile);

}

Bool_t ECALAnalyzer::Notify()
{
   return kTRUE;
}

bool ECALAnalyzer::Select()
{ // Evaluates the class' list of event selection criteria

    return true;

}
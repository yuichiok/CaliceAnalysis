#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/ECALAnalyzer.hh"
#include "../include/TreeReader.hh"
#include "../include/FileSelector.hh"

using std::cout;   using std::endl;

ECALAnalyzer::ECALAnalyzer(TString o)
: options(o)
{
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
    reader.InitializeRecoReadTree(fChain, _data, _branch);

    Notify();

    return true;

}

void ECALAnalyzer::Analyze(Long64_t entry, HistManager hm)
{

   Float_t sum_slab_energy_stack = 0;
   Float_t sum_slab_energy[NSLABS] = {0};

   Float_t sum_energy_corrected = 0;
   Float_t sum_slab_energy_stack_corrected = 0;
   Float_t sum_slab_energy_corrected[NSLABS] = {0};

   if(_data.nhit_slab < 13) return;

   hm.h_sum_energy->Fill(_data.sum_energy);
   hm.h_nhit_len->Fill(_data.nhit_len);

   for (int ihit = 0; ihit < _data.nhit_len; ihit++)
   {
      hm.h_hit_slab->Fill(_data.hit_slab[ihit]);
      hm.h_hit_energy->Fill(_data.hit_energy[ihit]);

      int ihit_slab = _data.hit_slab[ihit];
      sum_slab_energy[ihit_slab] += _data.hit_energy[ihit];
      hm.h_hit_slab_energy[ihit_slab]->Fill(_data.hit_energy[ihit]);

      if(_data.hit_energy[ihit] > 0)
      {
         hm.h_hit_slab_corrected->Fill(_data.hit_slab[ihit]);
         hm.h_hit_energy_corrected->Fill(_data.hit_energy[ihit]);

         sum_energy_corrected += _data.hit_energy[ihit];
         sum_slab_energy_corrected[ihit_slab] += _data.hit_energy[ihit];
         hm.h_hit_slab_energy_corrected[ihit_slab]->Fill(_data.hit_energy[ihit]);
      }

   }

   hm.h_sum_energy_corrected->Fill(sum_energy_corrected);

   for (int islab = 0; islab < NSLABS; islab++)
   {
      sum_slab_energy_stack += sum_slab_energy[islab];
      hm.h_sum_slab_energy[islab]->Fill(sum_slab_energy[islab]);
      hm.h_sum_slab_energy_stack[islab]->Fill(sum_slab_energy_stack);

      sum_slab_energy_stack_corrected += sum_slab_energy_corrected[islab];
      hm.h_sum_slab_energy_corrected[islab]->Fill(sum_slab_energy_corrected[islab]);
      hm.h_sum_slab_energy_stack_corrected[islab]->Fill(sum_slab_energy_stack_corrected);

   }


}

Bool_t ECALAnalyzer::Notify()
{
   return kTRUE;
}

bool ECALAnalyzer::Select()
{ // Evaluates the class' list of event selection criteria

   FileSelector fs(options);
   TString   recosim = fs.GetRecoSim();
   Int_t      energy = fs.GetEnergy();
   Int_t nhit_len_th = 0;

   if( recosim == "conv_sim" ) return true;

   std::pair<Int_t,Int_t> opt_nhit_len_threshold[7] = {
      std::make_pair(  10, 150 ),
      std::make_pair(  20, 250 ),
      std::make_pair(  40, 410 ),
      std::make_pair(  60, 535 ),
      std::make_pair(  80, 641 ),
      std::make_pair( 100, 736 ),
      std::make_pair( 150, 935 )
   };

   if( _data.nhit_slab < 13 ) return false;

   for (auto pair : opt_nhit_len_threshold){
      if( energy == pair.first ) {
         nhit_len_th = pair.second;
         break;
      }
   }

   if( _data.nhit_len < nhit_len_th ) return false;

   return true;

}
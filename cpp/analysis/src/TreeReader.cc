
/*------------------------------------------------------------------------------
TreeReader.cpp
 Created : 2022-09-08  okugawa
------------------------------------------------------------------------------*/

#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/TreeReader.hh"

using std::cout;   using std::endl;

TreeReader::TreeReader() {}

void TreeReader::InitializeRecoReadTree(TTree *_hTree, ECAL_data & _data, ECAL_branch & _branch)
{
   _hTree->SetBranchAddress("event", &_data.event, &_branch.b_event);
   _hTree->SetBranchAddress("spill", &_data.spill, &_branch.b_spill);
   _hTree->SetBranchAddress("cycle", &_data.cycle, &_branch.b_cycle);
   _hTree->SetBranchAddress("bcid", &_data.bcid, &_branch.b_bcid);
   _hTree->SetBranchAddress("bcid_first_sca_full", &_data.bcid_first_sca_full, &_branch.b_bcid_first_sca_full);
   _hTree->SetBranchAddress("bcid_merge_end", &_data.bcid_merge_end, &_branch.b_bcid_merge_end);
   _hTree->SetBranchAddress("id_run", &_data.id_run, &_branch.b_id_run);
   _hTree->SetBranchAddress("id_dat", &_data.id_dat, &_branch.b_id_dat);
   _hTree->SetBranchAddress("nhit_slab", &_data.nhit_slab, &_branch.b_nhit_slab);
   _hTree->SetBranchAddress("nhit_chip", &_data.nhit_chip, &_branch.b_nhit_chip);
   _hTree->SetBranchAddress("nhit_chan", &_data.nhit_chan, &_branch.b_nhit_chan);
   _hTree->SetBranchAddress("nhit_len", &_data.nhit_len, &_branch.b_nhit_len);
   _hTree->SetBranchAddress("sum_energy", &_data.sum_energy, &_branch.b_sum_energy);
   _hTree->SetBranchAddress("sum_energy_lg", &_data.sum_energy_lg, &_branch.b_sum_energy_lg);
   _hTree->SetBranchAddress("hit_slab", _data.hit_slab, &_branch.b_hit_slab);
   _hTree->SetBranchAddress("hit_chip", _data.hit_chip, &_branch.b_hit_chip);
   _hTree->SetBranchAddress("hit_chan", _data.hit_chan, &_branch.b_hit_chan);
   _hTree->SetBranchAddress("hit_sca", _data.hit_sca, &_branch.b_hit_sca);
   _hTree->SetBranchAddress("hit_x", _data.hit_x, &_branch.b_hit_x);
   _hTree->SetBranchAddress("hit_y", _data.hit_y, &_branch.b_hit_y);
   _hTree->SetBranchAddress("hit_z", _data.hit_z, &_branch.b_hit_z);
   _hTree->SetBranchAddress("hit_adc_high", _data.hit_adc_high, &_branch.b_hit_adc_high);
   _hTree->SetBranchAddress("hit_adc_low", _data.hit_adc_low, &_branch.b_hit_adc_low);
   _hTree->SetBranchAddress("hit_energy", _data.hit_energy, &_branch.b_hit_energy);
   _hTree->SetBranchAddress("hit_energy_lg", _data.hit_energy_lg, &_branch.b_hit_energy_lg);
   _hTree->SetBranchAddress("hit_n_scas_filled", _data.hit_n_scas_filled, &_branch.b_hit_n_scas_filled);
   _hTree->SetBranchAddress("hit_isHit", _data.hit_isHit, &_branch.b_hit_isHit);
   _hTree->SetBranchAddress("hit_isMasked", _data.hit_isMasked, &_branch.b_hit_isMasked);
   _hTree->SetBranchAddress("hit_isCommissioned", _data.hit_isCommissioned, &_branch.b_hit_isCommissioned);
}
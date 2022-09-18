#define Sim2Build_cxx
#include "../include/Sim2Build.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <vector>

using std::vector;

void Sim2Build::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      _data.event               = event               ;
      _data.spill               = spill               ;
      _data.cycle               = cycle               ;
      _data.bcid                = bcid                ;
      _data.bcid_first_sca_full = bcid_first_sca_full ;
      _data.bcid_merge_end      = bcid_merge_end      ;
      _data.id_run              = id_run              ;
      _data.id_dat              = id_dat              ;
      _data.nhit_slab           = nhit_slab           ;
      _data.nhit_chip           = nhit_chip           ;
      _data.nhit_chan           = nhit_chan           ;
      _data.nhit_len            = nhit_len            ;
      _data.sum_energy          = sum_energy          ;
      _data.sum_energy_lg       = sum_energy_lg       ;

      std::copy(hit_slab->begin(),           hit_slab->end(),           _data.hit_slab);
      std::copy(hit_chip->begin(),           hit_chip->end(),           _data.hit_chip);
      std::copy(hit_chan->begin(),           hit_chan->end(),           _data.hit_chan);
      std::copy(hit_sca->begin(),            hit_sca->end(),            _data.hit_sca);
      std::copy(hit_x->begin(),              hit_x->end(),              _data.hit_x);
      std::copy(hit_y->begin(),              hit_y->end(),              _data.hit_y);
      std::copy(hit_z->begin(),              hit_z->end(),              _data.hit_z);
      std::copy(hit_adc_high->begin(),       hit_adc_high->end(),       _data.hit_adc_high);
      std::copy(hit_adc_low->begin(),        hit_adc_low->end(),        _data.hit_adc_low);
      std::copy(hit_energy->begin(),         hit_energy->end(),         _data.hit_energy);
      std::copy(hit_energy_lg->begin(),      hit_energy_lg->end(),      _data.hit_energy_lg);
      std::copy(hit_n_scas_filled->begin(),  hit_n_scas_filled->end(),  _data.hit_n_scas_filled);
      std::copy(hit_isHit->begin(),          hit_isHit->end(),          _data.hit_isHit);
      std::copy(hit_isMasked->begin(),       hit_isMasked->end(),       _data.hit_isMasked);
      std::copy(hit_isCommissioned->begin(), hit_isCommissioned->end(), _data.hit_isCommissioned);

      _hTree->Fill();

   }

   _hfile->Write();
   _hfile->Close();

   if(0) std::cout << "nbytes = " << nbytes << std::endl;

}

#include "../include/TreeWriter.hh"
using std::string;
using std::vector;

TreeWriter:: TreeWriter() {}
void TreeWriter::InitializeECALTree(TTree * _hTree, ECAL_data & data)
{
  _hTree->Branch("event",               &data.event,               "event/I");
  _hTree->Branch("spill",               &data.spill,               "spill/I");
  _hTree->Branch("cycle",               &data.cycle,               "cycle/I");
  _hTree->Branch("bcid",                &data.bcid,                "bcid/I");
  _hTree->Branch("bcid_first_sca_full", &data.bcid_first_sca_full, "bcid_first_sca_full/I");
  _hTree->Branch("bcid_merge_end",      &data.bcid_merge_end,      "bcid_merge_end/I");
  _hTree->Branch("id_run",              &data.id_run,              "id_run/I");
  _hTree->Branch("id_dat",              &data.id_dat,              "id_dat/I");
  _hTree->Branch("nhit_slab",           &data.nhit_slab,           "nhit_slab/I");
  _hTree->Branch("nhit_chip",           &data.nhit_chip,           "nhit_chip/I");
  _hTree->Branch("nhit_chan",           &data.nhit_chan,           "nhit_chan/I");
  _hTree->Branch("nhit_len",            &data.nhit_len,            "nhit_len/I");
  _hTree->Branch("sum_energy",          &data.sum_energy,          "sum_energy/I");
  _hTree->Branch("sum_energy_lg",       &data.sum_energy_lg,       "sum_energy_lg/I");

  _hTree->Branch("hit_slab", data.hit_slab, "hit_slab[nhit_len]/F");
  _hTree->Branch("hit_chip", data.hit_chip, "hit_chip[nhit_len]/F");
  _hTree->Branch("hit_chan", data.hit_chan, "hit_chan[nhit_len]/F");
  _hTree->Branch("hit_sca", data.hit_sca, "hit_sca[nhit_len]/F");
  _hTree->Branch("hit_x", data.hit_x, "hit_x[nhit_len]/F");
  _hTree->Branch("hit_y", data.hit_y, "hit_y[nhit_len]/F");
  _hTree->Branch("hit_z", data.hit_z, "hit_z[nhit_len]/F");
  _hTree->Branch("hit_adc_high", data.hit_adc_high, "hit_adc_high[nhit_len]/F");
  _hTree->Branch("hit_adc_low", data.hit_adc_low, "hit_adc_low[nhit_len]/F");
  _hTree->Branch("hit_energy", data.hit_energy, "hit_energy[nhit_len]/F");
  _hTree->Branch("hit_energy_lg", data.hit_energy_lg, "hit_energy_lg[nhit_len]/F");
  _hTree->Branch("hit_n_scas_filled", data.hit_n_scas_filled, "hit_n_scas_filled[nhit_len]/F");
  _hTree->Branch("hit_isHit", data.hit_isHit, "hit_isHit[nhit_len]/F");
  _hTree->Branch("hit_isMasked", data.hit_isMasked, "hit_isMasked[nhit_len]/F");
  _hTree->Branch("hit_isCommissioned", data.hit_isCommissioned, "hit_isCommissioned[nhit_len]/F");

}

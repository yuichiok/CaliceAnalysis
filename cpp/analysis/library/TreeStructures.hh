#ifndef GUARD_TreeStructures_h
#define GUARD_TreeStructures_h

const static int MAX_NHITS = 9999;

struct ECAL_reco  {

  public:
  // Declaration of leaf types
    Int_t           event = 0;
    Int_t           spill = 0;
    Int_t           cycle = 0;
    Int_t           bcid = 0;
    Int_t           bcid_first_sca_full = 0;
    Int_t           bcid_merge_end = 0;
    Int_t           id_run = 0;
    Int_t           id_dat = 0;
    Int_t           nhit_slab = 0;
    Int_t           nhit_chip = 0;
    Int_t           nhit_chan = 0;
    Int_t           nhit_len = 0;
    Float_t         sum_energy = 0;
    Float_t         sum_energy_lg = 0;
    Int_t           hit_slab[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_chip[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_chan[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_sca[MAX_NHITS] = {0};   //[nhit_len]
    Float_t         hit_x[MAX_NHITS] = {0};   //[nhit_len]
    Float_t         hit_y[MAX_NHITS] = {0};   //[nhit_len]
    Float_t         hit_z[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_adc_high[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_adc_low[MAX_NHITS] = {0};   //[nhit_len]
    Float_t         hit_energy[MAX_NHITS] = {0};   //[nhit_len]
    Float_t         hit_energy_lg[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_n_scas_filled[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_isHit[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_isMasked[MAX_NHITS] = {0};   //[nhit_len]
    Int_t           hit_isCommissioned[MAX_NHITS] = {0};   //[nhit_len]
                                                                                      
};

struct Branch_reco  {

  public:
  // List of branches
    TBranch        *b_event;   //!
    TBranch        *b_spill;   //!
    TBranch        *b_cycle;   //!
    TBranch        *b_bcid;   //!
    TBranch        *b_bcid_first_sca_full;   //!
    TBranch        *b_bcid_merge_end;   //!
    TBranch        *b_id_run;   //!
    TBranch        *b_id_dat;   //!
    TBranch        *b_nhit_slab;   //!
    TBranch        *b_nhit_chip;   //!
    TBranch        *b_nhit_chan;   //!
    TBranch        *b_nhit_len;   //!
    TBranch        *b_sum_energy;   //!
    TBranch        *b_sum_energy_lg;   //!
    TBranch        *b_hit_slab;   //!
    TBranch        *b_hit_chip;   //!
    TBranch        *b_hit_chan;   //!
    TBranch        *b_hit_sca;   //!
    TBranch        *b_hit_x;   //!
    TBranch        *b_hit_y;   //!
    TBranch        *b_hit_z;   //!
    TBranch        *b_hit_adc_high;   //!
    TBranch        *b_hit_adc_low;   //!
    TBranch        *b_hit_energy;   //!
    TBranch        *b_hit_energy_lg;   //!
    TBranch        *b_hit_n_scas_filled;   //!
    TBranch        *b_hit_isHit;   //!
    TBranch        *b_hit_isMasked;   //!
    TBranch        *b_hit_isCommissioned;   //!

};

#endif
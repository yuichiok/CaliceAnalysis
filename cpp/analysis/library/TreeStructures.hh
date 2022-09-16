#ifndef GUARD_TreeStructures_h
#define GUARD_TreeStructures_h

const static int MAX_NHITS = 9999;

struct ECAL_reco  {

  public:
  // Declaration of leaf types
    Int_t           event;
    Int_t           spill;
    Int_t           cycle;
    Int_t           bcid;
    Int_t           bcid_first_sca_full;
    Int_t           bcid_merge_end;
    Int_t           id_run;
    Int_t           id_dat;
    Int_t           nhit_slab;
    Int_t           nhit_chip;
    Int_t           nhit_chan;
    Int_t           nhit_len;
    Float_t         sum_energy;
    Float_t         sum_energy_lg;
    Int_t           hit_slab[MAX_NHITS];   //[nhit_len]
    Int_t           hit_chip[MAX_NHITS];   //[nhit_len]
    Int_t           hit_chan[MAX_NHITS];   //[nhit_len]
    Int_t           hit_sca[MAX_NHITS];   //[nhit_len]
    Float_t         hit_x[MAX_NHITS];   //[nhit_len]
    Float_t         hit_y[MAX_NHITS];   //[nhit_len]
    Float_t         hit_z[MAX_NHITS];   //[nhit_len]
    Int_t           hit_adc_high[MAX_NHITS];   //[nhit_len]
    Int_t           hit_adc_low[MAX_NHITS];   //[nhit_len]
    Float_t         hit_energy[MAX_NHITS];   //[nhit_len]
    Float_t         hit_energy_lg[MAX_NHITS];   //[nhit_len]
    Int_t           hit_n_scas_filled[MAX_NHITS];   //[nhit_len]
    Int_t           hit_isHit[MAX_NHITS];   //[nhit_len]
    Int_t           hit_isMasked[MAX_NHITS];   //[nhit_len]
    Int_t           hit_isCommissioned[MAX_NHITS];   //[nhit_len]
                                                                                      
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
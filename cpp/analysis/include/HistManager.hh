#ifndef GUARD_HistManager_h
#define GUARD_HistManager_h

#include <iostream>
#include <map>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TList.h>
#include <TFile.h> 

const int NSLABS = 15;

class HistManager
{
  public:
    HistManager();
    ~HistManager(){}

  // Methods
    virtual void InitializeHists();
    virtual void Hist2List();
    virtual void WriteLists( TFile * output );

  // Declear histograms
  // h1 hist
    enum h1_index{
      h_sum_energy,
      h_hit_slab,
      h_nhit_len,
      h_hit_energy,

      h_sum_energy_corrected,
      h_hit_slab_corrected,
      h_hit_energy_corrected,

      h_sum_energy_corrected_MeanSD,

      h_energy_profile,

      h_nhit_len_radius,

      dummy1,
      LastH1 = dummy1
    };
    TH1F * h1[LastH1];

    enum h1_layer_index{
      h_hit_slab_energy,
      h_sum_slab_energy,
      h_sum_slab_energy_stack,

      h_hit_slab_energy_corrected,
      h_sum_slab_energy_corrected,
      h_sum_slab_energy_stack_corrected,

      h_nhit_len_slab,
      h_nhit_len_radius_slab,

      dummy1_layer,
      LastH1_layer = dummy1_layer
    };
    TH1F * h1_layer[LastH1_layer][NSLABS];

    enum h2_layer_index{
      h_hit_slab_xy,

      dummy2_layer,
      LastH2_layer = dummy2_layer
    };
    TH2F * h2_layer[LastH2_layer][NSLABS];

  private:
    TList* hList = new TList();
    TList* hList_slab_energy = new TList();
    TList* hList_nhit_len = new TList();
    TList* hList_sum_slab_energy = new TList();
    TList* hList_sum_slab_energy_stack = new TList();
    TList* hList_slab_xy = new TList();



};

#endif
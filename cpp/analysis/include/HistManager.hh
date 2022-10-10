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
    TH1F * h_sum_energy;
    TH1F * h_hit_slab;
    TH1F * h_nhit_len;
    TH1F * h_hit_energy;

    TH1F * h_sum_energy_corrected;
    TH1F * h_hit_slab_corrected;
    TH1F * h_hit_energy_corrected;

    TH1F * h_energy_profile;

    TH1F * h_hit_slab_energy[NSLABS];
    TH1F * h_sum_slab_energy[NSLABS];
    TH1F * h_sum_slab_energy_stack[NSLABS];

    TH1F * h_hit_slab_energy_corrected[NSLABS];
    TH1F * h_sum_slab_energy_corrected[NSLABS];
    TH1F * h_sum_slab_energy_stack_corrected[NSLABS];

  private:
    TList* hList = new TList();
    TList* hList_slab_energy = new TList();
    TList* hList_sum_slab_energy = new TList();
    TList* hList_sum_slab_energy_stack = new TList();



};

#endif
#include <iostream>
#include <TString.h>
#include <TH1.h>
#include <TH2.h>
#include <TList.h>
#include <TFile.h> 

#include "../include/HistManager.hh"

using std::cout;   using std::endl;

HistManager::HistManager()
{
  // Initialization
    // InitializeHists();
    // Hist2List();
}

void HistManager::InitializeHists()
{
    h1[h_sum_energy] = new TH1F("h_sum_energy","; sum_energy; Entries",500,0,1.5E4);
    h1[h_hit_slab]   = new TH1F("h_hit_slab","; hit_slab; Entries",NSLABS,-0.5,14.5);
    h1[h_nhit_len]   = new TH1F("h_nhit_len","; nhit_len; Entries",500,0,1.5E3);
    h1[h_hit_energy] = new TH1F("h_hit_energy","; hit_energy; Entries",120,-20,100);

    h1[h_sum_energy_corrected] = new TH1F("h_sum_energy_corrected","; sum_energy; Entries",500,0,1.5E4);
    h1[h_hit_slab_corrected]   = new TH1F("h_hit_slab_corrected","; hit_slab; Entries",NSLABS,-0.5,14.5);
    h1[h_hit_energy_corrected] = new TH1F("h_hit_energy_corrected","; hit_energy; Entries",120,-20,100);

    h1[h_sum_energy_corrected_MeanSD] = new TH1F("h_sum_energy_corrected_MeanSD","; sum_energy; Entries",500,0,1.5E4);

    h1[h_energy_profile] = new TH1F("h_energy_profile",";X0;dE/dx",100,0,22);

    h1[h_nhit_len_radius] = new TH1F("h_nhit_len_radius","; nhit_len_radius; Entries",700,0,700);

    for (int islab = 0; islab < NSLABS; islab++)
    {
      TString hname_hit_slab_energy       = "h_hit_slab_energy" + TString::Format("%d",islab);
      TString hname_sum_slab_energy       = "h_sum_slab_energy" + TString::Format("%d",islab);
      TString hname_sum_slab_energy_stack = "h_sum_slab_energy_stack" + TString::Format("%d",islab);

      TString hname_hit_slab_energy_corrected       = "h_hit_slab_energy_corrected" + TString::Format("%d",islab);
      TString hname_sum_slab_energy_corrected       = "h_sum_slab_energy_corrected" + TString::Format("%d",islab);
      TString hname_sum_slab_energy_stack_corrected = "h_sum_slab_energy_stack_corrected" + TString::Format("%d",islab);

      TString hname_hit_slab_xy = "h_hit_slab_xy" + TString::Format("%d",islab);

      TString hname_nhit_len_slab = "h_nhit_len_slab" + TString::Format("%d",islab);
      TString hname_nhit_len_radius_slab = "h_nhit_len_radius_slab" + TString::Format("%d",islab);

      h1_layer[h_hit_slab_energy][islab]            = new TH1F(hname_hit_slab_energy,hname_hit_slab_energy,120,-20,100);
      h1_layer[h_sum_slab_energy][islab]            = new TH1F(hname_sum_slab_energy,hname_sum_slab_energy,500,0,4.0E3);
      h1_layer[h_sum_slab_energy_stack][islab]      = new TH1F(hname_sum_slab_energy_stack,hname_sum_slab_energy_stack,500,0,1.5E4);

      h1_layer[h_hit_slab_energy_corrected][islab]            = new TH1F(hname_hit_slab_energy_corrected,hname_hit_slab_energy_corrected,120,-20,100);
      h1_layer[h_sum_slab_energy_corrected][islab]            = new TH1F(hname_sum_slab_energy_corrected,hname_sum_slab_energy_corrected,500,0,4.0E3);
      h1_layer[h_sum_slab_energy_stack_corrected][islab]      = new TH1F(hname_sum_slab_energy_stack_corrected,hname_sum_slab_energy_stack_corrected,500,0,1.5E4);

      h2_layer[h_hit_slab_xy][islab] = new TH2F(hname_hit_slab_xy,hname_hit_slab_xy,32,-90,90,32,-90,90);

      h1_layer[h_nhit_len_slab][islab] = new TH1F(hname_nhit_len_slab,hname_nhit_len_slab,100,0,100);
      h1_layer[h_nhit_len_radius_slab][islab] = new TH1F(hname_nhit_len_radius_slab,hname_nhit_len_radius_slab,100,0,100);

    }

    Hist2List();

}

void HistManager::Hist2List()
{
  for (auto ih : h1) {
    hList->Add(ih);
  }

  for(int ih=0;ih<NSLABS;ih++){
    hList_slab_energy->Add(h1_layer[h_hit_slab_energy][ih]);
    hList_sum_slab_energy->Add(h1_layer[h_sum_slab_energy][ih]);
    hList_sum_slab_energy_stack->Add(h1_layer[h_sum_slab_energy_stack][ih]);

    hList_slab_energy->Add(h1_layer[h_hit_slab_energy_corrected][ih]);
    hList_sum_slab_energy->Add(h1_layer[h_sum_slab_energy_corrected][ih]);
    hList_sum_slab_energy_stack->Add(h1_layer[h_sum_slab_energy_stack_corrected][ih]);

    hList_slab_xy->Add(h2_layer[h_hit_slab_xy][ih]);

    hList_nhit_len->Add(h1_layer[h_nhit_len_slab][ih]);
    hList_nhit_len->Add(h1_layer[h_nhit_len_radius_slab][ih]);
  }

}

void HistManager::WriteLists( TFile * output)
{
  // Focus to this file
  output->cd();

  // Write histogram lists
  hList->Write();

  TDirectory * d_hit_slab_energy = output->mkdir("hit_slab_energy");
    d_hit_slab_energy->cd();
    hList_slab_energy->Write();
    output->cd();

  TDirectory * d_sum_slab_energy = output->mkdir("sum_slab_energy");
    d_sum_slab_energy->cd();
    hList_sum_slab_energy->Write();
    output->cd();

  TDirectory * d_sum_slab_energy_stack = output->mkdir("sum_slab_energy_stack");
    d_sum_slab_energy_stack->cd();
    hList_sum_slab_energy_stack->Write();
    output->cd();

  TDirectory * d_hit_slab_xy = output->mkdir("hit_slab_xy");
    d_hit_slab_xy->cd();
    hList_slab_xy->Write();
    output->cd();

  TDirectory * d_nhit_len_slab = output->mkdir("nhit_len_slab");
    d_nhit_len_slab->cd();
    hList_nhit_len->Write();
    output->cd();
}
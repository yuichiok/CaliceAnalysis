#define EventControl_cxx
#include "../include/EventControl.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void EventControl::Loop(Bool_t isMaskReq)
{
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    Int_t count_nhit_len = 0;
    Int_t nhit_len_slab[NSLAB] = {0};

    for( int ihit=0; ihit < nhit_len; ihit++ ){
      if( isMaskReq && hit_isMasked[ihit] == 1 ) continue;
      count_nhit_len++;

      nhit_len_slab[ hit_slab[ihit] ]++;
      h_hit_slab->Fill( hit_slab[ihit] );
      h_hit_xy_slab.at( hit_slab[ihit] )->Fill( hit_x[ihit], hit_y[ihit] );

    }
    h_nhit_len->Fill( count_nhit_len );

    for( int islab=0; islab<NSLAB; islab++ ){
      h_nhit_len_slab.at(islab)->Fill( nhit_len_slab[islab] );
    }

  }
}

void EventControl::SaveFile(TString output_name)
{
  TFile *f = new TFile(output_name,"recreate");
  h_hit_slab->Write();
  h_nhit_len->Write();

  TDirectory *dir_nhit_len = f->mkdir("nhit_len");
  dir_nhit_len->cd();
  for (int i=0; i<NSLAB; i++) h_nhit_len_slab.at(i)->Write();
  f->cd();

  TDirectory *dir_hit_xy = f->mkdir("hit_xy");
  dir_hit_xy->cd();
  for (int i=0; i<NSLAB; i++) h_hit_xy_slab.at(i)->Write();
  f->cd();

  f->Close();
}
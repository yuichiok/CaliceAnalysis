#include <iostream>
#include <string>
#include <vector>

#include <TStyle.h>
#include <TCanvas.h>
#include "include/Styles.h"
#include "include/tools.h"

const static int NSLABS = 15;

template <class TH>
TH * GetHist(TFile *file, TString hname)
{
  return (TH*)file->Get(hname);
}

void nhit_len_radius()
{
  gStyle->SetOptStat(0);
  vector<TString> recosim = {"reco", "conv_sim"};
  vector<int> energy = {10, 20, 40, 60, 80, 100, 150};

  TCanvas *c = new TCanvas("c", "c", 800, 800);
  c->Divide(3,3);

  for( int ie = 0; ie < energy.size(); ie++ ){

    int idraw = 0;
    vector<TH1F*> hlegs;

    for( auto irecosim : recosim ){

      TFile *file = readfile(irecosim + " e- " + TString::Format("%d",energy.at(ie)));
      Int_t nevents = GetHist<TH1F>(file, "h_sum_energy")->GetEntries();
      auto h = GetHist<TH1F>(file, "h_nhit_len_radius");
      h->SetTitle(TString::Format("Number of hits in the M.R. at %d GeV;nhits; Entries / Events",energy.at(ie)));
      h->Scale(1./nevents);
      h->GetYaxis()->SetRangeUser(0, 0.3);
      h->GetXaxis()->SetRangeUser(0, 20);

      c->cd(ie+1);
      StylePad(gPad,0,0.12,0,0.15);
      if( !idraw ){
        StyleHist(h, kBlue);
        h->Draw("h");
      }else{
        StyleHist(h, kBlack);
        h->Draw("hsame");
      }
      idraw++;

      if( !ie ) hlegs.push_back(h);

    }

    if( !ie ){
      TLegend *leg = new TLegend(0.6,0.7,0.85,0.85);
      leg->SetLineColor(0);
      leg->AddEntry(hlegs.at(0), "Reco", "l");
      leg->AddEntry(hlegs.at(1), "Sim", "l");
      leg->Draw();
    }

  }

}
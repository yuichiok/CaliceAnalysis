#include <iostream>
#include <string>
#include <vector>

#include <TStyle.h>
#include <TCanvas.h>
#include "include/Styles.h"
#include "include/tools.h"

const static int NSLABS = 15;
TFile *file = readfile("reco e- 10");

void hit_2d()
{
  gStyle->SetOptStat(0);
  TCanvas *c = new TCanvas("c", "c", 800, 800);
  c->Divide(4,4);

  TH2F *h_xy[NSLABS];
  for( int ih = 0; ih < NSLABS; ih++ ){
    h_xy[ih] = (TH2F*)file->Get(TString::Format("hit_slab_xy/h_hit_slab_xy%d",ih));
    h_xy[ih]->SetTitle(TString::Format("Hit XY e^{-} 10 GeV slab %d;X (mm);Y (mm)",ih));
    c->cd(ih+1);
    StylePad(gPad,0,0.12,0,0.15);
    h_xy[ih]->Draw("colz");
  }

}
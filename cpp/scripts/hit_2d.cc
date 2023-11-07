#include <iostream>
#include <string>
#include <vector>

#include <TStyle.h>
#include <TCanvas.h>
#include "include/Styles.h"
#include "include/tools.h"

const static int NSLABS = 15;
TString energy = "10";
TFile *file = readfile("conv_sim e- " + energy);

void draw_profile(TCanvas *c, TH2F *h, Int_t ih, TString option)
{
  TH1D *h_proj;
  if(option == "x")
    h_proj = (TH1D*)h->ProjectionX("h_proj_" + option + TString::Format("_%d",ih),-90,90);
  else if(option == "y")
    h_proj = (TH1D*)h->ProjectionY("h_proj_" + option + TString::Format("_%d",ih),-90,90);
  else
    std::cout << "Invalid option" << std::endl;

  h_proj->SetTitle(TString::Format("Hit Distribution e^{-} " + energy + " GeV slab %d;" + option + " (mm);Entry",ih));
  c->cd(ih+1);
  StylePad(gPad,0,0.12,0,0.15);
  gPad->SetGrid();
  StyleHist(h_proj,kBlue);
  h_proj->Draw("h");
}

void draw_xy(TCanvas *c, TH2F *h, Int_t ih)
{
  h->SetTitle(TString::Format("Hit XY e^{-} " + energy + " GeV slab %d;X (mm);Y (mm)",ih));
  c->cd(ih+1);
  StylePad(gPad,0,0.12,0.12,0.15);
  h->GetZaxis()->SetTitle("Hits");
  h->Draw("colz");
}

void hit_2d()
{
  gStyle->SetOptStat(0);
  TCanvas *c_xy = new TCanvas("c_xy", "c_xy", 1200, 1200);
  c_xy->Divide(4,4);
  TCanvas *c_projx = new TCanvas("c_projx", "c_projx", 1200, 1200);
  c_projx->Divide(4,4);
  TCanvas *c_projy = new TCanvas("c_projy", "c_projy", 1200, 1200);
  c_projy->Divide(4,4);

  TH2F *h_xy[NSLABS];
  for( int ih = 0; ih < NSLABS; ih++ ){
    h_xy[ih] = (TH2F*)file->Get(TString::Format("hit_slab_xy/h_hit_slab_xy%d",ih));
    draw_xy(c_xy, h_xy[ih], ih);
    draw_profile(c_projx, h_xy[ih], ih, "x");
    draw_profile(c_projy, h_xy[ih], ih, "y");
  }

}
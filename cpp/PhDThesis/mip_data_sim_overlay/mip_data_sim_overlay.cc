#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "../../analysis/src/FileSelector.cc"
#include "../../scripts/include/Styles.h"

using std::cout;
using std::endl;

#define MAXV 8
#define NSLABS 15

void SetStyle()
{
	gStyle->SetOptFit(0);
	gStyle->SetOptStat(0);  
	gStyle->SetOptTitle(1);
	gStyle->SetTitleBorderSize(0);
	gStyle->SetTitleStyle(0);
	gStyle->SetMarkerSize(0);
	gStyle->SetTitleX(0.2); 
	gStyle->SetTitleY(0.9); 

	gStyle->SetTitleAlign(33);
	gStyle->SetTitleX(.85);
	gStyle->SetTitleY(.97);

}
void Normalize(TH1F* h)
{
	h->Scale(1.0/h->GetEntries());
	// h->GetYaxis()->SetRangeUser(0.0001,1.0);
	// h->GetYaxis()->SetRangeUser(0.,0.3);
}

template <class P1>
void MakePretty(P1 *h, TString option)
{
	h->SetLineWidth(3);
	if(option == "reco"){
		h->SetLineColor(kBlue+1);
	}else{
		h->SetLineColor(kBlack);
	}

}

void Legend(TH1F *rh,TH1F *sh)
{
	TLegend *leg0 = new TLegend(0.16,0.76,0.40,0.86,"","brNDC");
	leg0->SetFillStyle(0);
	leg0->SetBorderSize(0);
	leg0->SetTextSize(0.04);
	leg0->SetMargin(0.7);
	leg0->AddEntry(rh,"Reco");
	leg0->AddEntry(sh,"Sim");
	leg0->Draw("same");	
}

void Draw2E(TH1F *h, Int_t recosim)
{
	if(recosim==0){
		h->Draw("E1");
	}else{
		h->Draw("E1 same");
	}
}

void Draw2H(TH1F *h, Int_t recosim)
{
	if(recosim==0){
		h->Draw("h");
	}else{
		h->Draw("hsame");
	}
}

void mip_data_sim_overlay()
{
	// TFile* fdata = new TFile("mip_files/MIPSummary_pedestalsubmode1_raw_siwecal_90259to90285_highgain.root","read");
	// TFile* fdata = new TFile("mip_files/MIPSummary_pedestalsubmode1_raw_siwecal_90484_highgain.root","read");
	TFile* fdata = new TFile("mip_files/MIPSummary_pedestalsubmode1_raw_siwecal_90021to90070_highgain.root","read");
	TFile* fsim  = new TFile("mip_files/TB2022-06_CONF6_mu-_10GeV_conv_test.root","read");

	Int_t layer = 1;
	Int_t chip  = 12;
	TH1F* h_mip_reco = (TH1F*) fdata->Get(TString::Format("layer_%d/mip_high_layer%d_chip%d",layer,layer,chip));
	TH1F* h_mip_sim  = (TH1F*) fsim->Get(TString::Format("Energy_GeV__layer_%d",layer));

	TCanvas *c_reco = new TCanvas("c_reco","c_reco",800,800);
	h_mip_reco->GetXaxis()->SetRangeUser(0,100);
	h_mip_reco->Draw();


	Int_t NbinsSim = h_mip_sim->GetNbinsX();
	Float_t binw  = h_mip_sim->GetBinWidth(1);
	Float_t xlow  = h_mip_sim->GetBinLowEdge(1);
	Float_t xhigh = h_mip_sim->GetBinLowEdge(NbinsSim)+binw;
	TH1F* h_mip_sim_GeV2MIP = new TH1F("h_mip_sim_GeV2MIP","h_mip_sim_GeV2MIP",NbinsSim,xlow*1E5,xhigh*1E5);
	for(int ibin=1;ibin<=NbinsSim;ibin++){
		Float_t ibinc = h_mip_sim->GetBinContent(ibin);
		Float_t ibine = h_mip_sim->GetBinError(ibin);
		Float_t ibinX = h_mip_sim->GetBinCenter(ibin);
		Float_t ibinX_GeV2MIP = ibinX * 1E5;
		h_mip_sim_GeV2MIP->SetBinContent(ibinX_GeV2MIP,ibinc);
	}

	TCanvas *c_sim  = new TCanvas("c_sim","c_sim",800,800);
	h_mip_sim->Draw();

	TCanvas *c_sim2  = new TCanvas("c_sim2","c_sim2",800,800);
	h_mip_sim_GeV2MIP->Draw("h");



	TCanvas *c_reco_sim  = new TCanvas("c_reco_sim","c_reco_sim",800,800);
	h_mip_reco->Scale(1.0/h_mip_reco->Integral());
	h_mip_sim_GeV2MIP->Scale(1.0/h_mip_sim_GeV2MIP->Integral());
	h_mip_reco->GetYaxis()->SetRangeUser(0,0.5);
	h_mip_reco->Draw();
	h_mip_sim_GeV2MIP->Draw("hsame");


}
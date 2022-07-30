#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using std::cout;
using std::endl;

#define MAXV 8

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
}

void legend(TH1F *rh,TH1F *sh)
{
	TLegend *leg0 = new TLegend(0.6,0.85,0.75,0.65,"","brNDC");
	leg0->SetFillStyle(0);
	leg0->SetBorderSize(0);
	leg0->SetTextSize(0.04);
	leg0->AddEntry(rh,"Reco");
	leg0->AddEntry(sh,"Sim");
	leg0->Draw("same");	
}

void readfiles(string name, TFile *f_rs[])
{
	std::map<std::string, std::string> run_list {
		{"e.10", "320"},
		{"e.20", "378"},
		{"e.40", "375"},
		{"e.60", "372"},
		{"e.80", "367"},
		{"e.100", "365"},
		{"e.150", "355"},
	};

	TString reco_file = "../reco/rootfiles/quality/run_90" + run_list[name] + "." + name + "GeV.quality.root";
	TString sim_file  = "../sim/rootfiles/ECAL.sim." + name + "GeV.quality.root";

	cout << "reco file: " << reco_file << endl;
	cout << " sim file: " << sim_file << endl;

	f_rs[0] = TFile::Open(reco_file);
	f_rs[1] = TFile::Open(sim_file);
}

void slab_energy(int set_ene, TFile *f_rs[])
{
	TH1F * rh_sum_energy;
	TH1F * sh_sum_energy;

	rh_sum_energy = (TH1F*)f_rs[0]->Get("h_sum_energy");
	sh_sum_energy = (TH1F*)f_rs[1]->Get("h_sum_energy");

	rh_sum_energy->Scale(1/rh_sum_energy->GetEntries());
	sh_sum_energy->Scale(1/sh_sum_energy->GetEntries());

	rh_sum_energy->GetXaxis()->SetRangeUser(0,1.5E4);
	sh_sum_energy->GetXaxis()->SetRangeUser(0,1.5E4);

	rh_sum_energy->GetYaxis()->SetRangeUser(0,0.15);
	sh_sum_energy->GetYaxis()->SetRangeUser(0,0.15);

	rh_sum_energy->SetLineColor(kBlue+1);
	sh_sum_energy->SetLineColor(kBlack);

	TString title = "Sum energy " + to_string(set_ene) + "GeV (reco & sim)";
	sh_sum_energy->SetTitle(title);
	sh_sum_energy->Draw("h");
	rh_sum_energy->Draw("hsame");

	if(set_ene==10) legend(rh_sum_energy,sh_sum_energy);

}

void nhit_slab(int set_ene, TFile *f_rs[])
{
	TH1F * rh_nhit_slab;
	TH1F * sh_nhit_slab;

	rh_nhit_slab = (TH1F*)f_rs[0]->Get("h_hit_slab");
	sh_nhit_slab = (TH1F*)f_rs[1]->Get("h_hit_slab");

	rh_nhit_slab->Scale(1/rh_nhit_slab->GetEntries());
	sh_nhit_slab->Scale(1/sh_nhit_slab->GetEntries());

	rh_nhit_slab->GetXaxis()->SetRangeUser(0,1.5E4);
	sh_nhit_slab->GetXaxis()->SetRangeUser(0,1.5E4);

	rh_nhit_slab->GetYaxis()->SetRangeUser(0,0.23);
	sh_nhit_slab->GetYaxis()->SetRangeUser(0,0.23);

	rh_nhit_slab->SetLineColor(kBlue+1);
	sh_nhit_slab->SetLineColor(kBlack);

	TString title = "nhit_slab " + to_string(set_ene) + "GeV (reco & sim)";
	rh_nhit_slab->SetTitle(title);
	rh_nhit_slab->Draw("h");
	sh_nhit_slab->Draw("hsame");

	if(set_ene==10) legend(rh_nhit_slab,sh_nhit_slab);

}

void reco_sim_analysis(string particle = "e")
{
	TFile *MyFile = new TFile("rootfiles/reco_sim_analysis.root","RECREATE");
	TCanvas *c_sum_energy = new TCanvas("c_sum_energy","c_sum_energy",700,700);
	c_sum_energy->Divide(3,3);
	TCanvas *c_nhit_slab = new TCanvas("c_nhit_slab","c_nhit_slab",700,700);
	c_nhit_slab->Divide(3,3);

	const int nene = 7;
	int l_energy[nene] = {10, 20, 40, 60, 80, 100, 150};
	for (int ie=0; ie<nene; ie++){
		TFile *f_rs[2];
		string name = particle + "." + to_string(l_energy[ie]);
		readfiles(name,f_rs);
		c_sum_energy->cd(ie+1);
		slab_energy(l_energy[ie],f_rs);
		c_nhit_slab->cd(ie+1);
		nhit_slab(l_energy[ie],f_rs);
	}


	MyFile->cd();
	c_sum_energy->Write();
	c_nhit_slab->Write();

}
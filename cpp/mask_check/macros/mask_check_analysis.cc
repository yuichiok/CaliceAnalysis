#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using std::cout;
using std::endl;
using std::map;
using std::vector;

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
	leg0->AddEntry(rh,"No Mask");
	leg0->AddEntry(sh,"With Mask");
	leg0->Draw("same");	
}

void readfiles(int energy, TFile *f_rs[])
{
	TString path	= "../rootfiles/ECAL_QGSP_BERT_conf6_e-_" + TString(to_string(energy)) + ".0GeV_checks" ;
	TString extension = ".root"	;

	TString nomask   = path + extension;
	TString withmask = path + "_masked" + extension;

	cout << "  no mask file: " << nomask << endl;
	cout << "with mask file: " << withmask  << endl;

	f_rs[0] = TFile::Open(nomask);
	f_rs[1] = TFile::Open(withmask);
}

void h_single(int set_ene, TFile *f_rs[], TString hist_name)
{
	TH1F * h_nomask;
	TH1F * h_mask;

	Int_t nevents = ((TH1F*)f_rs[0]->Get("h_nhit_len"))->GetEntries();
	h_nomask = (TH1F*)f_rs[0]->Get(hist_name);
	h_mask   = (TH1F*)f_rs[1]->Get(hist_name);

	h_nomask->Scale((Float_t)1/(Float_t)nevents);
	h_mask->Scale((Float_t)1/(Float_t)nevents);

	h_nomask->SetLineColor(kBlue+1);
	h_mask->SetLineColor(kBlack);

	TString title = hist_name + " " + TString(to_string(set_ene)) + "GeV";
	h_nomask->SetTitle(title);
	h_nomask->Draw("h");
	h_mask->Draw("hsame");

	if(set_ene==10) legend(h_nomask,h_mask);

}

void h_mult(int set_ene, TFile *f_rs[], TString hist_name)
{

	TH1F * h_nomask;
	TH1F * h_mask;

	Int_t nevents = ((TH1F*)f_rs[0]->Get("h_nhit_len"))->GetEntries();
	h_nomask = (TH1F*)f_rs[0]->Get(hist_name);
	h_mask   = (TH1F*)f_rs[1]->Get(hist_name);

	h_nomask->Scale((Float_t)1/(Float_t)nevents);
	h_mask->Scale((Float_t)1/(Float_t)nevents);

	h_nomask->SetLineColor(kBlue+1);
	h_mask->SetLineColor(kBlack);

	TString title = hist_name + " " + TString(to_string(set_ene)) + "GeV";
	h_nomask->SetTitle(title);
	h_nomask->Draw("h");
	h_mask->Draw("hsame");

	if(set_ene==10) legend(h_nomask,h_mask);

}

void mask_check_analysis()
{
	gStyle->SetOptStat(0);

	TString outpath	= "plots/" ;
	TFile *MyFile = new TFile(outpath+"reco_sim_analysis.root","RECREATE");
	
	vector<TString> hnames = {"hit_slab","nhit_len"};
	map<TString,TCanvas*> cmap;

	for( auto &hname : hnames){
		cmap[hname] = new TCanvas("c_"+hname,"c_"+hname,800,800);
		cmap[hname]->Divide(3,3);
	}

	// TCanvas *c_hit_slab = new TCanvas("c_hit_slab","c_hit_slab",800,800);
	// c_hit_slab->Divide(3,3);
	// TCanvas *c_nhit_len = new TCanvas("c_nhit_len","c_nhit_len",800,800);
	// c_nhit_len->Divide(3,3);

	const int nene = 7;
	int l_energy[nene] = {10, 20, 40, 60, 80, 100, 150};
	for (int ie=0; ie<nene; ie++){
		TFile *f_rs[2];
		readfiles(l_energy[ie],f_rs);

		// hit slab and nhit len
		for( auto &hname : hnames){
			cmap[hname]->cd(ie+1);
			h_single(l_energy[ie],f_rs,"h_"+hname);
		}
	
	}


	MyFile->cd();
	for (auto const& [key, val] : cmap)
	{
		val->Write();
		val->Print(outpath+val->GetName()+".png");
	}
	// c_hit_slab->Write();

}
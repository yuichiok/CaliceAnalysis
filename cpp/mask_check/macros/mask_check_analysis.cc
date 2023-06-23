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
#define NSLAB 15

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

void plot_h_single(int set_ene, TFile *f_rs[], TString hist_name)
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

void plot_h_2d(int set_ene, TFile *f_rs, TString hist_name)
{
	TH2F * h2d = (TH2F*)f_rs->Get(hist_name);

	TString title = hist_name + " " + TString(to_string(set_ene)) + "GeV";
	h2d->SetTitle(title);
	h2d->Draw("colz");

}

TCanvas * draw_h_nhit_len_slab(int set_ene, TFile *f_rs[])
{
	TString cname = "c_nhit_len_"+TString(to_string(set_ene))+"GeV";
	TCanvas *c = new TCanvas(cname,cname,800,800);
	c->Divide(4,4);

	for( int islab = 0; islab<NSLAB; islab++){
		c->cd(islab+1);
		plot_h_single(set_ene,f_rs,"nhit_len/h_nhit_len_slab_"+TString(to_string(islab)));
	}

	return c;

}

vector<TCanvas*> draw_h_hit_xy(int set_ene, TFile *f_rs[])
{
  TString base_name[2];
  base_name[0] = "hit_xy_"+TString(to_string(set_ene))+"GeV";
  base_name[1] = base_name[0] + "_masked";

	vector<TCanvas*> c;
  for (int i=0; i<2; i++) {
    c.push_back( new TCanvas("c_"+base_name[i],"c_"+base_name[i],800,800) );
    c.at(i)->Divide(4,4);
  }

	for( int islab = 0; islab<NSLAB; islab++){
    for (int i=0; i<2; i++) {
      c.at(i)->cd(islab+1);
      plot_h_2d(set_ene,f_rs[i],"hit_xy/h_hit_xy_slab_"+TString(to_string(islab)));
    }
		
	}

	return c;

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

	const int nene = 7;
	vector<TCanvas*> c_nhit_len_slab;
	int l_energy[nene] = {10, 20, 40, 60, 80, 100, 150};
	for (int ie=0; ie<nene; ie++){
		TFile *f_rs[2];
		readfiles(l_energy[ie],f_rs);

		// hit slab and nhit len
		for( auto &hname : hnames){
			cmap[hname]->cd(ie+1);
			plot_h_single(l_energy[ie],f_rs,"h_"+hname);
		}

		c_nhit_len_slab.push_back(draw_h_nhit_len_slab(l_energy[ie],f_rs));
	
	}


	MyFile->cd();
	for (auto const& [key, val] : cmap)
	{
		val->Write();
		val->Print(outpath+val->GetName()+".png");
	}
	TDirectory * d_nhit_len_slab = MyFile->mkdir("nhit_len_slab");
		d_nhit_len_slab->cd();
		for( auto ic : c_nhit_len_slab ){
			ic->Write();
			ic->Print(outpath+"/nhit_len_slab/"+ic->GetName()+".png");
		}
		MyFile->cd();


}
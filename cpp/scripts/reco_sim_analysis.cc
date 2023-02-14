#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "../analysis/src/FileSelector.cc"

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

	gStyle->SetTitleAlign(33);
	gStyle->SetTitleX(.85);
	gStyle->SetTitleY(.97);

}
void Normalize(TH1F* h)
{
	h->Scale(1.0/h->GetEntries());
	// h->GetYaxis()->SetRangeUser(0.0001,1.0);
	h->GetYaxis()->SetRangeUser(0.,0.3);
}

void MakePretty(TH1F *h, TString option)
{
	h->SetLineWidth(1);
	if(option == "reco"){
		h->SetLineColor(kBlue+1);
	}else{
		h->SetLineColor(kBlack);
	}

}

void Legend(TH1F *rh,TH1F *sh)
{
	TLegend *leg0 = new TLegend(0.6,0.85,0.75,0.65,"","brNDC");
	leg0->SetFillStyle(0);
	leg0->SetBorderSize(0);
	leg0->SetTextSize(0.04);
	leg0->AddEntry(rh,"Reco");
	leg0->AddEntry(sh,"Sim");
	leg0->Draw("same");	
}

void Draw2H(TH1F *h, Int_t recosim)
{
	if(recosim==0){
		h->Draw("h");
	}else{
		h->Draw("hsame");
	}
}

TFile * readfile( TString option )
{

	TString suffix      = "_quality.root";

	FileSelector fs(option);
	TString name = fs.GetRunName();
	TString data_path = "../analysis/rootfiles/" + fs.GetRecoSim() + "/";
	cout << data_path + name + suffix << endl;

	return TFile::Open(data_path + name + suffix);

}

void analysis ( TString particle = "e-", Int_t ienergy = 150 )
{
	TFile   *MyFile			  = new TFile("rootfiles/reco_sim_analysis/reco_sim_analysis_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_sum_energy = new TCanvas("c_sum_energy","c_sum_energy",700,700);
	TCanvas *c_nhit_slab  = new TCanvas("c_nhit_slab" ,"c_nhit_slab" ,700,700);

	TString recosims[2] = {"conv_sim","reco"};
	TString energy  = TString::Format("%d",ienergy);
	TFile * files[2];
	for (int irecosim=0; irecosim < 2; irecosim++){

		TString setting = recosims[irecosim] + " " + particle + " " + energy;
		files[irecosim] = readfile(setting);
	
	}

	TH1F * hs_sum_energy[2];
	TH1F * hs_hit_slab[2];
	for (int irecosim=0; irecosim < 2; irecosim++)
	{
		// hs_sum_energy[irecosim] = (TH1F*) files[irecosim]->Get("h_sum_energy_corrected");
		hs_sum_energy[irecosim] = (TH1F*) files[irecosim]->Get("h_sum_energy_corrected_MeanSD");
		hs_hit_slab[irecosim]   = (TH1F*) files[irecosim]->Get("h_hit_slab_corrected");

		Normalize(hs_sum_energy[irecosim]);
		MakePretty(hs_sum_energy[irecosim],recosims[irecosim]);

		Normalize(hs_hit_slab[irecosim]);
		MakePretty(hs_hit_slab[irecosim],recosims[irecosim]);

		hs_sum_energy[irecosim]->SetTitle(TString::Format("sum energy at %d GeV;Stack energy (MIPs); Entries",ienergy));
		hs_hit_slab[irecosim]->SetTitle(TString::Format("hit slab at %d GeV;nhits; Entries",ienergy));

		c_sum_energy->cd();
		if( particle == "mu-" ){
			// hs_sum_energy[irecosim]->GetYaxis()->SetRangeUser(0.0001,0.20);
			hs_sum_energy[irecosim]->GetXaxis()->SetRangeUser(1,2E3);
		}
		Draw2H(hs_sum_energy[irecosim],irecosim);
		c_nhit_slab->cd();
		Draw2H(hs_hit_slab[irecosim]  ,irecosim);
	}

	c_sum_energy->cd();
	Legend(hs_sum_energy[1],hs_sum_energy[0]);
	c_nhit_slab->cd();
	Legend(hs_hit_slab[1],hs_hit_slab[0]);


	MyFile->cd();
	c_sum_energy->Write();
	c_nhit_slab->Write();
	c_sum_energy->Print("rootfiles/reco_sim_analysis/sum_energy_" + particle + "_" + energy + ".0GeV.png");
	c_nhit_slab->Print("rootfiles/reco_sim_analysis/nhit_slab_" + particle + "_" + energy + ".0GeV.png");


}

void analysis_allE( TString particle = "e-" )
{
	TFile *MyFile = new TFile("rootfiles/reco_sim_analysis/reco_sim_analysis_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_sum_energy = new TCanvas("c_sum_energy","c_sum_energy",700,700);
	c_sum_energy->Divide(3,3);
	TCanvas *c_nhit_slab = new TCanvas("c_nhit_slab","c_nhit_slab",700,700);
	c_nhit_slab->Divide(3,3);

	const static int nEconfigs = 7;
	TString recosims[2]       = {"conv_sim","reco"};
	Int_t energies[nEconfigs] = {10, 20, 40, 60, 80, 100, 150};
	// Int_t energies[nEconfigs] = {150};
	TFile * files[2][nEconfigs];


	for (int irecosim=0; irecosim < 2; irecosim++)
	{
		for (int ie=0; ie < nEconfigs; ie++)
		{
			TString energy  = TString::Format("%d",energies[ie]);
			TString setting = recosims[irecosim] + " " + particle + " " + energy;
			files[irecosim][ie] = readfile(setting);
		}
	}

	for (int ie=0; ie < nEconfigs; ie++)
	{

		TH1F * hs_sum_energy[2];
		TH1F * hs_sum_energy2[2];
		TH1F * hs_hit_slab[2];
		for (int irecosim=0; irecosim < 2; irecosim++)
		{
			// hs_sum_energy[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy");
			hs_sum_energy[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy_corrected_MeanSD");
			hs_hit_slab[irecosim]   = (TH1F*) files[irecosim][ie]->Get("h_hit_slab");

			hs_sum_energy2[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy");
			Int_t NEvents = hs_sum_energy2[irecosim]->GetEntries();
			Int_t NHits   = hs_hit_slab[irecosim]->GetEntries();
			cout << "NEvents: " << NEvents << " NHits: " << NHits << " Hits/Events = " << (Float_t) NHits / (Float_t) NEvents << endl;

			Normalize(hs_sum_energy[irecosim]);
			MakePretty(hs_sum_energy[irecosim],recosims[irecosim]);

			// Normalize(hs_hit_slab[irecosim]);
			hs_hit_slab[irecosim]->Scale((Float_t)1/(Float_t)NEvents);
			MakePretty(hs_hit_slab[irecosim],recosims[irecosim]);

			hs_sum_energy[irecosim]->SetTitle(TString::Format("sum energy at %d GeV;Stack energy (MIPs); Entries",energies[ie]));
			hs_hit_slab[irecosim]->SetTitle(TString::Format("hit slab at %d GeV;nhits; Entries",energies[ie]));

			c_sum_energy->cd(ie+1);
			Draw2H(hs_sum_energy[irecosim],irecosim);
			c_nhit_slab->cd(ie+1);
			Draw2H(hs_hit_slab[irecosim],irecosim);
		}

		c_sum_energy->cd(ie+1);
		Legend(hs_sum_energy[1],hs_sum_energy[0]);

		c_nhit_slab->cd(ie+1);
		Legend(hs_hit_slab[1],hs_hit_slab[0]);

	}

	MyFile->cd();
	c_sum_energy->Write();
	c_nhit_slab->Write();
	c_sum_energy->Print("rootfiles/reco_sim_analysis/sum_energy_" + particle + "_allE.png");
	c_nhit_slab->Print("rootfiles/reco_sim_analysis/nhit_slab_" + particle + "_allE.png");

}


void reco_sim_analysis(TString particle = "e-", Int_t ienergy = 150)
{

	TGaxis::SetMaxDigits(3);

	SetStyle();

	if( particle == "e-" ){
		analysis_allE( particle );
	}else if ( particle == "mu-" ){
		analysis( particle, ienergy );
	}

}
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "../analysis/src/FileSelector.cc"
#include "include/Styles.h"

using std::cout;
using std::endl;

#define MAXV 8

double crystalball_function(double x, double alpha, double n, double sigma, double mean) {
  // evaluate the crystal ball function
  if (sigma < 0.)     return 0.;
  double z = (x - mean)/sigma; 
  if (alpha < 0) z = -z; 
  double abs_alpha = std::abs(alpha);
  if (z  > - abs_alpha)
    return std::exp(- 0.5 * z * z);
  else {
    double nDivAlpha = n/abs_alpha;
    double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
    double B = nDivAlpha -abs_alpha;
    double arg = nDivAlpha/(B-z);
    return AA * std::pow(arg,n);
  }
}

double crystalball_function(const double *x, const double *p) {
  return (p[0] * crystalball_function(x[0], p[3], p[4], p[2], p[1]));
}

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
	if(fs.GetRecoSim() == "conv_sim"){
		suffix = "_quality_masked.root";
	}

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

		// Normalize(hs_sum_energy[irecosim]);
		MakePretty(hs_sum_energy[irecosim],recosims[irecosim]);

		// Normalize(hs_hit_slab[irecosim]);
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
	TCanvas *c_nhit_slab_1slab = new TCanvas("c_nhit_slab_1slab","c_nhit_slab_1slab",700,700);
	TCanvas *c_sum_energy_1slab = new TCanvas("c_sum_energy_1slab","c_sum_energy_1slab",700,700);

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
			hs_sum_energy[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy");
			// hs_sum_energy[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy_corrected_MeanSD");
			hs_hit_slab[irecosim]   = (TH1F*) files[irecosim][ie]->Get("h_hit_slab");

			hs_sum_energy2[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy");
			Int_t NEvents = hs_sum_energy2[irecosim]->GetEntries();
			Int_t NHits   = hs_hit_slab[irecosim]->GetEntries();
			cout << "NEvents: " << NEvents << " NHits: " << NHits << " Hits/Events = " << (Float_t) NHits / (Float_t) NEvents << endl;

			// Normalize(hs_sum_energy[irecosim]);
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

			if(ie==0 && irecosim==1){
				c_nhit_slab_1slab->cd();
				StylePad(gPad,0,0.12,0,0.15);
				StyleHist(hs_hit_slab[irecosim],kBlue);
				hs_hit_slab[irecosim]->SetTitle("Hits per layer at 10 GeV;Layer; Entries / Event");
				hs_hit_slab[irecosim]->GetXaxis()->SetRangeUser(0,300);
				hs_hit_slab[irecosim]->Draw("hist");

				c_sum_energy_1slab->cd();
				StylePad(gPad,0,0.12,0,0.15);
				StyleHist(hs_sum_energy[irecosim],kBlue);
				hs_sum_energy[irecosim]->Scale(1./NEvents);

				// TF1* crystalball = new TF1("crystalball", crystalball_function, 200, 1200, 5) ;
				// crystalball->SetParNames("Constant", "Mean", "Sigma", "Alpha", "N");
				// crystalball->SetTitle("crystalball");
				// crystalball->SetLineColor(kBlue);
				// crystalball->SetLineWidth(6);
				// float p0 = 0.14;
				// float p1 = hs_sum_energy[irecosim]->GetMean();
				// float p2 = 100;
				// float p3 = -2.05;
				// float p4 = 3.0;
				// crystalball->SetParameters(p0,p1,p2,p3,p4);
				// hs_sum_energy[irecosim]->Fit(crystalball,"MN");

				TF1* fgaus = new TF1("fgaus","gaus",200,1200);
				fgaus->SetLineColor(kBlue);
				fgaus->SetLineWidth(6);
				hs_sum_energy[irecosim]->Fit(fgaus,"MN");

				hs_sum_energy[irecosim]->SetTitle("Total Hit Energy at 10 GeV;Energy (MIP); Entries / Event");
				hs_sum_energy[irecosim]->GetXaxis()->SetRangeUser(0,2000);
				hs_sum_energy[irecosim]->Draw("hist");
				fgaus->Draw("same");

				// TLegend *leg0 = new TLegend(0.5,0.65,0.75,0.75,"","brNDC");
				// // leg0->SetFillStyle(0);
				// leg0->SetBorderSize(0);
				// leg0->SetTextSize(0.03);
				// leg0->AddEntry(fgaus,"  #sigma / #mu = 15.7%","l");
				// leg0->Draw("same");

			}

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
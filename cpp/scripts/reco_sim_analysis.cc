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
#define NSLABS 15

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
	leg0->AddEntry(rh,"Data");
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

TFile * readfile( TString option )
{

	TString suffix      = "_quality.root";

	FileSelector fs(option);
	TString name = fs.GetRunName();
	TString data_path = "../analysis/rootfiles/" + fs.GetRecoSim() + "/";
	if(fs.GetRecoSim() == "conv_sim"){
		suffix = "_quality_masked.root";
		// suffix = "_quality.root";
	}

	cout << data_path + name + suffix << endl;

	return TFile::Open(data_path + name + suffix);

}

void analysis ( TString particle = "e-", Int_t ienergy = 10 )
{
	TFile   *MyFile			  = new TFile("rootfiles/reco_sim_analysis/reco_sim_analysis_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_sum_energy = new TCanvas("c_sum_energy","c_sum_energy",900,900);
	gPad->SetGrid(1,1);
	TCanvas *c_nhit_slab  = new TCanvas("c_nhit_slab" ,"c_nhit_slab" ,900,900);
	gPad->SetGrid(1,1);

	TString recosims[2] = {"conv_sim","reco"};
	TString energy  = TString::Format("%d",ienergy);
	TFile * files[2];
	for (int irecosim=0; irecosim < 2; irecosim++){

		TString setting = recosims[irecosim] + " " + particle + " " + energy;
		files[irecosim] = readfile(setting);
	
	}

	Float_t ymax = 0.2;
	TH1F * hs_sum_energy[2];
	TH1F * hs_hit_slab[2];
	for (int irecosim=0; irecosim < 2; irecosim++)
	{
		hs_sum_energy[irecosim] = (TH1F*) files[irecosim]->Get("h_sum_energy_corrected");
		// hs_sum_energy[irecosim] = (TH1F*) files[irecosim]->Get("h_sum_energy_corrected_MeanSD");
		hs_hit_slab[irecosim]   = (TH1F*) files[irecosim]->Get("h_hit_slab_corrected");

		Normalize(hs_sum_energy[irecosim]);
		MakePretty(hs_sum_energy[irecosim],recosims[irecosim]);
		hs_sum_energy[irecosim]->GetXaxis()->SetRangeUser(1,1.2E3);
		hs_sum_energy[irecosim]->GetXaxis()->SetTitleOffset(1.2);
		hs_sum_energy[irecosim]->GetYaxis()->SetRangeUser(0,ymax);

		Normalize(hs_hit_slab[irecosim]);
		MakePretty(hs_hit_slab[irecosim],recosims[irecosim]);

		hs_sum_energy[irecosim]->SetTitle(";Total energy (MIPs); Entries (norm.)");
		hs_hit_slab[irecosim]->SetTitle(";Layer; Entries (norm.)");

		c_sum_energy->cd();
		if( particle == "mu-" ){
			hs_sum_energy[irecosim]->GetXaxis()->SetRangeUser(1,2E3);
		}
		StylePad(gPad,0,0.12,0,0.15);

		Float_t xmin = 0.4E3;
		Float_t xmax = 0.75E3;
		TF1 *crystalball = new TF1("crystalball", crystalball_function, xmin, xmax, 5);
		crystalball->SetParNames("Constant", "Mean", "Sigma", "Alpha", "N");
		crystalball->SetTitle("crystalball");

		float p0 = 1.3E-1;
		float p1 = 5.8E2;
		float p2 = 8.7E1;
		float p3 = 1.0;
		float p4 = 1.0;
		crystalball->SetParameters(p0,p1,p2,p3,p4);
		hs_sum_energy[irecosim]->Fit("crystalball","NR");
		cout << "chi2/ndf = " << crystalball->GetChisquare() << " / " << crystalball->GetNDF() << " = " << crystalball->GetChisquare() / crystalball->GetNDF() << endl;

		MakePretty(crystalball,recosims[irecosim]);

		Draw2E(hs_sum_energy[irecosim],irecosim);
		crystalball->Draw("same");

		TLatex *latex = new TLatex();
		latex->SetTextSize(0.025);
		latex->SetTextAlign(13);  //align at top

		TString recosim = (irecosim==0) ? "Simulation" : "Data"; 
		TString text = TString::Format("#splitline{#splitline{#splitline{%s}{#mu_{E} = %.1f #pm %.1f}}{#sigma_{E} = %.1f #pm %.1f}}{#chi^{2}/ndf = %.1f/%d = %.1f}",
		recosim.Data(),
		crystalball->GetParameter(1), crystalball->GetParError(1),
		crystalball->GetParameter(2), crystalball->GetParError(2),
		crystalball->GetChisquare(), crystalball->GetNDF(), crystalball->GetChisquare() / crystalball->GetNDF()
		);

		if(irecosim==0){
			latex->DrawLatex(807,ymax - 0.015,text);
		}else{
			latex->DrawLatex(807,ymax - 0.050,text);
		}


		c_nhit_slab->cd();
		StylePad(gPad,0,0.12,0,0.15);
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

void analysis_slab( TString particle = "e-", Int_t ienergy = 10 )
{
	TString recosims[2] = {"conv_sim","reco"};
	TString energy  = TString::Format("%d",ienergy);
	TFile * files[2];
	for (int irecosim=0; irecosim < 2; irecosim++){

		TString setting = recosims[irecosim] + " " + particle + " " + energy;
		files[irecosim] = readfile(setting);
	
	}

	TCanvas * c_hit_slab_energy = new TCanvas("c_hit_slab_energy","c_hit_slab_energy",1200,1200);
	TCanvas * c_sum_slab_energy = new TCanvas("c_sum_slab_energy","c_sum_slab_energy",1200,1200);
	c_hit_slab_energy->Divide(4,4);
	c_sum_slab_energy->Divide(4,4);

	TH1F * hs_hit_slab_energy[2][NSLABS];
	TH1F * hs_sum_slab_energy[2][NSLABS];
	for (int irecosim=0; irecosim < 2; irecosim++)
	{

		for( int islab = 0; islab < NSLABS; islab++ ){

			TString hs_hit_slab_energy_name = TString::Format("hit_slab_energy/h_hit_slab_energy_corrected%d",islab);
			TString hs_sum_slab_energy_name = TString::Format("sum_slab_energy/h_sum_slab_energy_corrected%d",islab);
			hs_hit_slab_energy[irecosim][islab] = (TH1F*) files[irecosim]->Get(hs_hit_slab_energy_name);
			hs_sum_slab_energy[irecosim][islab] = (TH1F*) files[irecosim]->Get(hs_sum_slab_energy_name);

			MakePretty(hs_hit_slab_energy[irecosim][islab],recosims[irecosim]);
			MakePretty(hs_sum_slab_energy[irecosim][islab],recosims[irecosim]);
		
			hs_hit_slab_energy[irecosim][islab]->SetTitle(TString::Format("Layer %d;Hit energy (MIPs);Entries (norm.)",islab));
			hs_sum_slab_energy[irecosim][islab]->SetTitle(TString::Format("Layer %d;Total energy / layer (MIPs);Entries (norm.)",islab));

			Normalize(hs_hit_slab_energy[irecosim][islab]);
			Normalize(hs_sum_slab_energy[irecosim][islab]);

			c_hit_slab_energy->cd(islab+1);
			StylePad(gPad,0,0.12,0,0.15);
			gPad->SetGrid();
			Draw2H(hs_hit_slab_energy[irecosim][islab],irecosim);

			c_sum_slab_energy->cd(islab+1);
			StylePad(gPad,0,0.12,0,0.15);
			gPad->SetGrid();
			Draw2H(hs_sum_slab_energy[irecosim][islab],irecosim);

			hs_hit_slab_energy[irecosim][islab]->GetXaxis()->SetRangeUser(0,20);
			hs_sum_slab_energy[irecosim][islab]->GetXaxis()->SetRangeUser(0,200);

			hs_hit_slab_energy[irecosim][islab]->GetYaxis()->SetRangeUser(0,0.5);
			hs_sum_slab_energy[irecosim][islab]->GetYaxis()->SetRangeUser(0,1);

		}

	}

	c_hit_slab_energy->cd(1);
	Legend(hs_hit_slab_energy[1][0],hs_hit_slab_energy[0][0]);
	c_sum_slab_energy->cd(1);
	Legend(hs_sum_slab_energy[1][0],hs_sum_slab_energy[0][0]);

	TCanvas *c_ihit_slab_enery = new TCanvas("c_ihit_slab_enery","c_ihit_slab_enery",900,900);
	StylePad(gPad,0,0.12,0,0.15);
	gPad->SetGrid();
	hs_hit_slab_energy[0][0]->Draw("h");
	hs_hit_slab_energy[1][0]->Draw("hsame");
	Legend(hs_hit_slab_energy[1][0],hs_hit_slab_energy[0][0]);

	TCanvas *c_isum_slab_enery = new TCanvas("c_isum_slab_enery","c_isum_slab_enery",900,900);
	StylePad(gPad,0,0.12,0,0.15);
	gPad->SetGrid();
	// hs_sum_slab_energy[0][0]->GetYaxis()->SetRangeUser(0,0.15);
	hs_sum_slab_energy[0][0]->Draw("h");
	hs_sum_slab_energy[1][0]->Draw("hsame");
	Legend(hs_sum_slab_energy[1][0],hs_sum_slab_energy[0][0]);

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
			hs_sum_energy[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy_corrected");
			// hs_sum_energy[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy_corrected_MeanSD");
			hs_hit_slab[irecosim]   = (TH1F*) files[irecosim][ie]->Get("h_hit_slab");

			hs_sum_energy2[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_sum_energy");
			Int_t NEvents = hs_sum_energy2[irecosim]->GetEntries();
			Int_t NHits   = hs_hit_slab[irecosim]->GetEntries();
			cout << "NEvents: " << NEvents << " NHits: " << NHits << " Hits/Events = " << (Float_t) NHits / (Float_t) NEvents << endl;

			Normalize(hs_sum_energy[irecosim]);
			MakePretty(hs_sum_energy[irecosim],recosims[irecosim]);
			hs_sum_energy[irecosim]->GetXaxis()->SetRangeUser(1,5E3);

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

void simNoMask( TString particle = "e-", Int_t ienergy = 10 )
{
	TFile *MyFile = new TFile("rootfiles/reco_sim_analysis/reco_sim_analysis_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_sum_energy = new TCanvas("c_sum_energy","c_sum_energy",800,800);
	gPad->SetGrid(1,1);

	TString input = "conv_sim " + particle + " " + TString::Format("%d",ienergy);
	TString recosim = "conv_sim";

	TFile* file = readfile(input);

	Float_t ymax = 0.25;
	TH1F * hs_sum_energy = (TH1F*) file->Get("h_sum_energy_corrected");
	Normalize(hs_sum_energy);
	MakePretty(hs_sum_energy,recosim);
	hs_sum_energy->GetXaxis()->SetRangeUser(1,1.2E3);
	hs_sum_energy->GetXaxis()->SetTitleOffset(1.2);
	hs_sum_energy->GetYaxis()->SetRangeUser(0,ymax);
	hs_sum_energy->SetTitle(";Total energy (MIPs); Entries (norm.)");
	hs_sum_energy->Draw("E1");


	StylePad(gPad,0,0.12,0,0.15);

	Float_t xmin = 0.4E3;
	Float_t xmax = 1.0E3;
	TF1 *crystalball = new TF1("crystalball", crystalball_function, xmin, xmax, 5);
	crystalball->SetParNames("Constant", "Mean", "Sigma", "Alpha", "N");
	crystalball->SetTitle("crystalball");

	float p0 = 1.3E-1;
	float p1 = 5.8E2;
	float p2 = 8.7E1;
	float p3 = 1.0;
	float p4 = 1.0;
	crystalball->SetParameters(p0,p1,p2,p3,p4);
	hs_sum_energy->Fit("crystalball","NR");
	cout << "chi2/ndf = " << crystalball->GetChisquare() << " / " << crystalball->GetNDF() << " = " << crystalball->GetChisquare() / crystalball->GetNDF() << endl;

	MakePretty(crystalball,recosim);

	crystalball->Draw("same");

	TLatex *latex = new TLatex();
	latex->SetTextSize(0.025);
	latex->SetTextAlign(13);  //align at top

	TString latexsim = "Simulation (no mask)"; 
	TString text = TString::Format("#splitline{#splitline{#splitline{%s}{#mu_{E} = %.1f #pm %.1f}}{#sigma_{E} = %.1f #pm %.1f}}{#chi^{2}/ndf = %.1f/%d = %.1f}",
	latexsim.Data(),
	crystalball->GetParameter(1), crystalball->GetParError(1),
	crystalball->GetParameter(2), crystalball->GetParError(2),
	crystalball->GetChisquare(), crystalball->GetNDF(), crystalball->GetChisquare() / crystalball->GetNDF()
	);

	latex->DrawLatex(807,ymax - 0.015,text);
	

}


void reco_sim_analysis(TString particle = "e-", Int_t ienergy = 10)
{

	TGaxis::SetMaxDigits(3);

	SetStyle();

	if( particle == "e-" ){
		// analysis_allE( particle );

		analysis( particle, ienergy );
		// simNoMask( particle, ienergy );
		
		// analysis_slab( particle, ienergy );
	}else if ( particle == "mu-" ){
		analysis( particle, ienergy );
	}

}
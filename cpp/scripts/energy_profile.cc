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
const static int nEconfigs = 7;
Color_t colors[nEconfigs] = {kBlue+2,kMagenta+2,kRed+2,kOrange+8,kYellow+2,kGreen+2,kCyan+2};
Int_t energies[nEconfigs] = {10, 20, 40, 60, 80, 100, 150};

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
	h->Scale(1.0/h->Integral(1,100));
}

void MakePrettyH(TH1F *h, Int_t ie)
{
	h->SetLineWidth(2);
	h->SetLineColor(colors[ie]);
}

void MakePrettyGR(TGraph *gr, Int_t ie)
{
	gr->SetMarkerStyle(8);
	gr->SetMarkerSize(1);
	gr->SetMarkerColor(colors[ie]);

}

void MakePrettyF(TF1 *func, Int_t ie)
{
	func->SetLineWidth(2);
	func->SetLineColor(colors[ie]);

}

void Legend(TF1 *funcs[])
{
	TLegend *leg0 = new TLegend(0.6,0.85,0.8,0.65,"","brNDC");
	// leg0->SetFillStyle(0);
	leg0->SetBorderSize(0);
	leg0->SetTextSize(0.025);
	for ( int ifunc=0; ifunc<nEconfigs; ifunc++ ){
		leg0->AddEntry(funcs[ifunc], TString::Format("%d GeV",energies[ifunc]), "l" );
	}
	leg0->Draw("same");	
}

void DrawH(TH1F *h, Int_t ie)
{
	if(ie==0){
		h->Draw("");
	}else{
		h->Draw("same");
	}
}

void DrawGR(TGraph *gr, Int_t ie)
{
	if(ie==0){
		gr->Draw("AP");
	}else{
		gr->Draw("P");
	}
}

void DrawF(TF1 *func, Int_t ie)
{
	if(ie==0){
		func->Draw("");
	}else{
		func->Draw("same");
	}
}

TGraph *H2GR(TH1F *h)
{
	Int_t nbins = h->GetNbinsX();
	std::vector<Double_t> xvec;
	std::vector<Double_t> yvec;

	for (int ibin=0; ibin<nbins; ibin++){
		Double_t binc = h->GetBinContent(ibin);
		if(binc){
			xvec.push_back(h->GetXaxis()->GetBinCenter(ibin));
			yvec.push_back(binc);
		}
	}

	TGraph * gr = new TGraph(xvec.size(), &xvec[0], &yvec[0]);
	return gr;

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

Double_t fitf(Double_t *x, Double_t *par)
{
	Double_t arg = x[0] - par[1];
	Double_t fitval = par[0] * TMath::Power( arg, par[2] ) * TMath::Exp( -1.0 * par[3] * arg );

	return fitval;
}

void profile( TString particle = "e-" )
{
	TFile *MyFile = new TFile("rootfiles/shower_profile/shower_profile_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_shower_profile[2];
	c_shower_profile[0] = new TCanvas("c_sim_shower_profile","c_sim_shower_profile",800,800);
	gPad->SetGrid(1,1);
	c_shower_profile[1] = new TCanvas("c_reco_shower_profile","c_reco_shower_profile",800,800);
	gPad->SetGrid(1,1);

	TString recosims[2]       = {"conv_sim","reco"};
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

	TH1F * h_shower_profile[nEconfigs];
	TGraph * gr_shower_profile[nEconfigs];

	for (int irecosim=0; irecosim < 2; irecosim++)
	{

		c_shower_profile[irecosim]->cd();
		TF1 *func[nEconfigs];

		for (int ie=0; ie < nEconfigs; ie++)
		{
			h_shower_profile[ie] = (TH1F*) files[irecosim][ie]->Get("h_energy_profile");

			Normalize(h_shower_profile[ie]);
			MakePrettyH(h_shower_profile[ie],ie);

			gr_shower_profile[ie] = H2GR(h_shower_profile[ie]);
			MakePrettyGR(gr_shower_profile[ie],ie);

			func[ie] = new TF1("fitf",fitf,0,21,4);
			func[ie]->SetParLimits(1,-2,0);
			gr_shower_profile[ie]->Fit("fitf","R");
			// gr_shower_profile[ie]->Fit("fitf","QN");
			MakePrettyF(func[ie],ie);

			gr_shower_profile[ie]->SetTitle(";X0;Layer Energy / Layer Hits");
			DrawGR(gr_shower_profile[ie],ie);
			func[ie]->Draw("same");
		}

		Legend(func);

	}

	MyFile->cd();
	c_shower_profile[0]->Write();
	c_shower_profile[1]->Write();
	// c_nhit_len->Print("rootfiles/nhit_len_analysis/nhit_len_" + particle + "_allE.png");



}


void energy_profile(TString particle = "e-", Int_t ienergy = 150)
{

	TGaxis::SetMaxDigits(3);

	SetStyle();

	if( particle == "e-" ){
		profile( particle );
	}

}
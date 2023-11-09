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
}

void MakePretty(TH1F *h, TString option)
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
	// if(recosim==1){
	// 	StyleHist(h, kBlue);
	// 	h->Draw("h");
	// }

	if(recosim==0){
		h->GetXaxis()->SetRangeUser(0,600);
		h->GetYaxis()->SetRangeUser(0,0.15);
		h->Draw("h");
	}else{
		h->Draw("hsame");
	}
	if(recosim==0){
		h->GetXaxis()->SetRangeUser(0,600);
		h->GetYaxis()->SetRangeUser(0,0.15);
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
	TFile   *MyFile			  = new TFile("rootfiles/nhit_len_analysis/nhit_len_analysis_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_nhit_len  = new TCanvas("c_nhit_len" ,"c_nhit_len" ,700,700);
	gPad->SetGrid(1,1);

	TString recosims[2] = {"conv_sim","reco"};
	TString energy  = TString::Format("%d",ienergy);
	TFile * files[2];
	for (int irecosim=0; irecosim < 2; irecosim++){

		TString setting = recosims[irecosim] + " " + particle + " " + energy;
		files[irecosim] = readfile(setting);
	
	}

	TH1F * h_nhit_len[2];
	for (int irecosim=0; irecosim < 2; irecosim++)
	{
		h_nhit_len[irecosim]   = (TH1F*) files[irecosim]->Get("h_nhit_len");

		Normalize(h_nhit_len[irecosim]);
		MakePretty(h_nhit_len[irecosim],recosims[irecosim]);

		h_nhit_len[irecosim]->SetTitle(TString::Format("Number of total hits at %d GeV;nhits; Entries",ienergy));

		c_nhit_len->cd();
		Draw2H(h_nhit_len[irecosim],irecosim);
	}

	c_nhit_len->cd();
	Legend(h_nhit_len[1],h_nhit_len[0]);


	MyFile->cd();
	c_nhit_len->Write();
	c_nhit_len->Print("rootfiles/nhit_len_analysis/nhit_len_" + particle + "_" + energy + ".0GeV.png");


}

void analysis_allE( TString particle = "e-" )
{
	TFile *MyFile = new TFile("rootfiles/nhit_len_analysis/nhit_len_analysis_" + particle + "_" + ".root","RECREATE");
	TCanvas *c_nhit_len = new TCanvas("c_nhit_len","c_nhit_len",900,900);
	c_nhit_len->Divide(3,3);

	TDirectory *dir_nhit_len_slab = MyFile->mkdir("nhit_len_slab");

	const static int nEconfigs = 7;
	TString recosims[2]       = {"conv_sim","reco"};
	// TString recosims[2]       = {"reco","conv_sim"};
	Int_t energies[nEconfigs] = {10, 20, 40, 60, 80, 100, 150};
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
		TString c_nhit_len_slab_name = TString::Format("c_nhit_len_slab_%d",ie);
		TCanvas *c_nhit_len_slab = new TCanvas(c_nhit_len_slab_name,c_nhit_len_slab_name,900,900);
		c_nhit_len_slab->Divide(4,4);	

		TH1F * h_nhit_len[2];
		TH1F * h_nhit_len_slab[2][NSLABS];
	
		for (int irecosim=0; irecosim < 2; irecosim++)
		{
			h_nhit_len[irecosim] = (TH1F*) files[irecosim][ie]->Get("h_nhit_len");

			// Normalize(h_nhit_len[irecosim]);
			MakePretty(h_nhit_len[irecosim],recosims[irecosim]);

			h_nhit_len[irecosim]->SetTitle(TString::Format("Number of total hits at %d GeV;nhits; Entries",energies[ie]));

			if( recosims[irecosim] == "conv_sim" ){
				
				TF1 *f1 = new TF1("f1","gaus");
				h_nhit_len[irecosim]->Fit(f1,"QN");
				Float_t mean  = f1->GetParameter(1);
				Float_t sigma = f1->GetParameter(2);
				cout << "Fit at "  << energies[ie] << ".0GeV ====\n"
						 << "  mean : " << mean  << "\n"
						 << "  sigma: " << sigma << "\n"
						 << "  mean - sigma = " << mean - sigma << endl;

			}

			h_nhit_len[irecosim]->Scale(1/h_nhit_len[irecosim]->GetEntries());
			c_nhit_len->cd(ie+1);
			StylePad(gPad,0,0.12,0,0.15);
			// if(recosims[irecosim]== "conv_sim") Draw2H(h_nhit_len[irecosim],irecosim);
			Draw2H(h_nhit_len[irecosim],irecosim);

			for( int islab = 0; islab < NSLABS; islab++ ){
				
				TString h_nhit_len_slab_name = TString::Format("nhit_len_slab/h_nhit_len_slab%d",islab);
				h_nhit_len_slab[irecosim][islab] = (TH1F*) files[irecosim][ie]->Get(h_nhit_len_slab_name);

				MakePretty(h_nhit_len_slab[irecosim][islab],recosims[irecosim]);

				h_nhit_len_slab[irecosim][islab]->SetTitle(TString::Format("Number of total hits at %d GeV in slab %d;nhits; Entries",energies[ie],islab));

				h_nhit_len_slab[irecosim][islab]->Scale(1/h_nhit_len[irecosim]->GetEntries());
				h_nhit_len_slab[irecosim][islab]->GetYaxis()->SetRangeUser(0,0.35);
				c_nhit_len_slab->cd(islab+1);
				StylePad(gPad,0,0.12,0,0.15);
				h_nhit_len_slab[irecosim][islab]->GetXaxis()->SetRangeUser(0,100);
				h_nhit_len_slab[irecosim][islab]->GetXaxis()->SetRangeUser(0,100);
				Draw2H(h_nhit_len_slab[irecosim][islab],irecosim);
			}

			c_nhit_len_slab->cd(1);
			// Legend(h_nhit_len_slab[irecosim][1],h_nhit_len_slab[irecosim][0]);

			dir_nhit_len_slab->cd();
			c_nhit_len_slab->Write();
			MyFile->cd();

		}

		c_nhit_len->cd(ie+1);
		// Legend(h_nhit_len[1],h_nhit_len[0]);

	}

	MyFile->cd();
	c_nhit_len->Write();
	MyFile->Write();
	c_nhit_len->Print("rootfiles/nhit_len_analysis/nhit_len_" + particle + "_allE.png");

}


void nhit_len_analysis(TString particle = "e-", Int_t ienergy = 10)
{

	TGaxis::SetMaxDigits(3);

	SetStyle();

	if( particle == "e-" ){
		// analysis_allE( particle );
		analysis( particle, ienergy );
	}else if ( particle == "mu-" ){
		analysis( particle, ienergy );
	}

}
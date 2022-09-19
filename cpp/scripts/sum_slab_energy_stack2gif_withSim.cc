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
const static int NSLABS = 15;

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
}

void MakePretty(TH1F *h, TString option)
{
	h->SetLineWidth(1.5);
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

void sum_slab_energy_stack2gif_withSim(TString particle = "e-", Bool_t correct = true)
{

	auto c1 = new TCanvas("c1","The HSUM example",800,800);
	c1->Divide(3,3);

	SetStyle();

	TString suffix      = "_quality.root";
	TString stack       = "sum_slab_energy_stack";
	TString output_path = "rootfiles/sum_slab_energy_stack2gif/";
	TString output			= output_path + "reco_conv_sim_" + particle + "_allEStack.gif";

	const static int nEconfigs = 7;
	// TString recosims[2]       = {"reco","conv_sim"};
	TString recosims[2]       = {"conv_sim","reco"};
	Int_t energies[nEconfigs] = {10, 20, 40, 60, 80, 100, 150};
	TFile * files[2][nEconfigs];

	for (int irecosim=0; irecosim < 2; irecosim++)
	{
		for (int ie=0; ie < nEconfigs; ie++)
		{
			TString energy  = TString::Format("%d",energies[ie]);
			TString setting = recosims[irecosim] + " " + particle + " " + energy;

			FileSelector fs(setting);
			TString name = fs.GetRunName();
			TString data_path = "../analysis/rootfiles/" + fs.GetRecoSim() + "/";
			cout << data_path + name + suffix << endl;
			files[irecosim][ie] = TFile::Open(data_path + name + suffix);
		}
	}

	for (int islab=0; islab < NSLABS; islab++)
	{
		for (int ie=0; ie < nEconfigs; ie++)
		{
			TH1F * hs[2];
			for (int irecosim=0; irecosim < 2; irecosim++)
			{
				if(correct)
				{
					hs[irecosim] = (TH1F*) files[irecosim][ie]->Get(stack + "/h_" + stack + "_corrected" + TString::Format("%d",islab));
				}else{
					hs[irecosim] = (TH1F*) files[irecosim][ie]->Get(stack + "/h_" + stack + TString::Format("%d",islab));
				}
				Normalize(hs[irecosim]);
				MakePretty(hs[irecosim],recosims[irecosim]);
				hs[irecosim]->SetTitle(TString::Format("Layer energy stacked (0 - %d) at %d GeV;Stack energy (MIPs); Entries",islab,energies[ie]));

				c1->cd(ie+1);
				// hs->Draw("hsame");
				if(irecosim==0){
					hs[irecosim]->Draw("h");
				}else{
					hs[irecosim]->Draw("hsame");
				}
			}

			Legend(hs[1],hs[0]);

		}
		c1->Update();
		c1->Print(output + "+");
	}

	c1->Modified();
	c1->Print(output + "++");

}

void sum_slab_energy_stack2gif(TString recosim  = "reco", TString particle = "e-")
{

	auto c1 = new TCanvas("c1","The HSUM example",800,800);
	c1->Divide(3,3);

	TString suffix      = "_quality.root";
	TString output_path = "rootfiles/sum_slab_energy_stack2gif/";
	TString stack       = "sum_slab_energy_stack";
	TString output;

	const static int nEconfigs = 7;
	Int_t energies[nEconfigs] = {10, 20, 40, 60, 80, 100, 150};
	TFile * files[nEconfigs];
	for (int ie=0; ie < nEconfigs; ie++)
	{
		TString energy  = TString::Format("%d",energies[ie]);
		TString setting = recosim + " " + particle + " " + energy;

		FileSelector fs(setting);
		TString name = fs.GetRunName();
		TString data_path = "../analysis/rootfiles/" + fs.GetRecoSim() + "/";

		output = output_path + fs.GetRecoSim() + "_" + fs.GetParticleName() + "_allEStack.gif";

		files[ie] = TFile::Open(data_path + name + suffix);
	}

	TH1F * hs[NSLABS];
	for (int islab=0; islab < NSLABS; islab++)
	{
		for (int ie=0; ie < nEconfigs; ie++)
		{
			hs[islab] = (TH1F*) files[ie]->Get(stack + "/h_" + stack + TString::Format("%d",islab));
			hs[islab]->SetTitle(TString::Format("Layer energy stacked (0 - %d) at %d GeV;Stack energy (MIPs); Entries",islab,energies[ie]));

			c1->cd(ie+1);
			hs[islab]->Draw("h");
		}
		c1->Update();
		c1->Print(output + "+");
	}

	c1->Modified();
	c1->Print(output + "++");

}
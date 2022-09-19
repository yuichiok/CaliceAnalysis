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
}

void sum_slab_energy_stack2gif(TString recosim  = "reco", TString particle = "e-")
{
	// TString energy   = TString::Format("%d",ienergy);
	// TString setting  = recosim + " " + particle + " " + energy;

	auto c1 = new TCanvas("c1","The HSUM example",800,800);
	c1->Divide(3,3);
	// c1->SetGrid();

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

			c1->cd(ie+1);
			hs[islab]->Draw("h");
		}
		c1->Update();
		c1->Print(output + "+");
	}

	c1->Modified();
	c1->Print(output + "++");


/*
	for (int ie=0; ie < nEconfigs; ie++)
	{


		output = output_path + fs.GetRecoSim() + "_" + fs.GetParticleName() + "_allEStack.gif";

		TGaxis::SetMaxDigits(3);

		TH1F * hs[NSLABS];
		for (int ih=0; ih < NSLABS; ih++)
		{
			hs[ih] = (TH1F*) file->Get(stack + "/h_" + stack + TString::Format("%d",ih));

			c1->cd(ih);
			hs[ih]->Draw("h");
		}
		c1->Update();
		c1->Print(output + "+");

	}

	c1->Modified();
	c1->Print(output + "++");

*/











	// TList* tdf_obs  = file100->GetListOfKeys();	
	// TList* tdf_obs2 = file100->GetDirectory(tdf_obs->At(1)->GetName())->GetListOfKeys();

	// TObject* tobj = tdf_obs2->FindObject("mip_high_layer1_chip0");

	// TH1F* h = (TH1F*)tdf_obs2->GetObject("mip_high_layer1_chip0");

	// TH1F* h = (TH1F*)tobj->Get();

	// TH1F* h = (TH1F*)tdf_obs2->Read("mip_high_layer1_chip0");

	// h->DrawNormalized();

	// layer_0->cd();






	// TTree * siwecaldecoded = (TTree*) file->Get( "siwecaldecoded" ) ;

	// TCanvas * c1 = new TCanvas("c1", "converted",500,500);

	// TH1F * h_nhits = new TH1F("h_nhits","nhits",30,0,30);

	// float nhits = siwecaldecoded->Draw("nhits >> h_nhits","gain_hit_high[0][12][0][63]==0");

}
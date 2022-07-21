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

void draw_tree()
{

	// TString filename = "../../data/3GeV_MIPscan_eudaq_run_050480/full_run.root";
	TString filename = "../../data/3GeV_MIPscan_eudaq_run_050480/converted/converted_3GeV_MIPscan_eudaq_run_050480_raw.bin_0000.root";

	TFile * file = TFile::Open(filename);
	TGaxis::SetMaxDigits(3);
	TTree * siwecaldecoded = (TTree*) file->Get( "siwecaldecoded" ) ;

	TCanvas * c1 = new TCanvas("c1", "converted",500,500);

	TH1F * h_nhits = new TH1F("h_nhits","nhits",30,0,30);

	float nhits = siwecaldecoded->Draw("nhits >> h_nhits","gain_hit_high[0][12][0][63]==0");


}
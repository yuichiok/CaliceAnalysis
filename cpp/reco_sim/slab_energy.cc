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

void slab_energy(float energy = 10)
{

	TString filename = "../data/full_run_90268.root";

	TFile * file = TFile::Open(filename);
	TGaxis::SetMaxDigits(3);
	TTree * ecal = (TTree*) file->Get( "ecal" ) ;

	TCanvas * c1 = new TCanvas("c1", "converted",500,500);

	TH1F * h_adc_bcid = new TH1F("h_nhits","nhits",30,0,30);

	float nhits = siwecaldecoded->Draw("nhits >> h_nhits","gain_hit_high[0][12][0][63]==0");


}
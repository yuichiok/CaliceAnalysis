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

void hold()
{

	TString data_path = "~/mnt/pc-calice2/mip_calib/";
	TString str_hold100 = "MIPSummary_pedestalsubmode1_raw_siwecal_90015_highgain.root";
	TString str_hold130 = "MIPSummary_pedestalsubmode1_raw_siwecal_90011to90013_highgain.root";
	TString str_hold150 = "MIPSummary_pedestalsubmode1_raw_siwecal_90016_highgain.root";
	TString str_hold200 = "MIPSummary_pedestalsubmode1_raw_siwecal_90017_highgain.root";

	TFile * file100 = TFile::Open(data_path + str_hold100);
	TFile * file130 = TFile::Open(data_path + str_hold130);
	TFile * file150 = TFile::Open(data_path + str_hold150);
	TFile * file200 = TFile::Open(data_path + str_hold200);
	TGaxis::SetMaxDigits(3);

	file100->cd();

	TList* tdf_obs = file100->GetListOfKeys();

	cout << tdf_obs->At(0)->GetName() << endl;

	// layer_0->cd();






	// TTree * siwecaldecoded = (TTree*) file->Get( "siwecaldecoded" ) ;

	// TCanvas * c1 = new TCanvas("c1", "converted",500,500);

	// TH1F * h_nhits = new TH1F("h_nhits","nhits",30,0,30);

	// float nhits = siwecaldecoded->Draw("nhits >> h_nhits","gain_hit_high[0][12][0][63]==0");


}
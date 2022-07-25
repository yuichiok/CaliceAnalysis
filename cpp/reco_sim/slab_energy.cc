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

void slab_energy(int set_ene = 10, string particle = "e")
{
	string name = particle + "." + to_string(set_ene);

	std::map<std::string, std::string> run_list {
		{"e.10", "320"},
		{"e.20", "378"},
		{"e.40", "375"},
		{"e.60", "372"},
		{"e.80", "367"},
		{"e.100", "365"},
		{"e.150", "355"},
	};

	TString reco_file = "../reco/rootfiles/run_90" + run_list[name] + "." + name + "GeV.quality.root";
	TString sim_file  = "../sim/rootfiles/ECAL.sim." + name + "GeV.quality.root";

	cout << "reco file: " << reco_file << endl;
	cout << " sim file: " << sim_file << endl;

	TFile * f_reco = TFile::Open(reco_file);
	TFile * f_sim = TFile::Open(sim_file);
	TGaxis::SetMaxDigits(3);




}
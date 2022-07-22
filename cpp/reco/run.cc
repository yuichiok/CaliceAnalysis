#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run(int set_ene = 10, string particle = "e"){

	// TString name = particle + TString(to_string(set_ene));
	string name = particle + "." + to_string(set_ene);
	cout << name << endl;

	std::map<std::string, std::string> run_list {
		{"e.10", "320"},
		{"e.80", "367"},
	};

	TString filein = "default";
	string fileinpath = "../../data/raw_siwecal_90";
	filein = fileinpath + run_list[name] + "/full_run.root";

	TString fileout = "default";
	string fileoutpath = "rootfiles/";
	fileout = fileoutpath + "run_90" + run_list[name] + "." + name + "GeV.quality.root";

	cout << "Input: " << filein << endl; 

	TBEvent TBEvent(filein,fileout);
	// TBEvent.ana_SumE();
	// TBEvent.ana_Eff();
	// TBEvent.ana_Energy();
	// TBEvent.ana_adc_bcid();
	TBEvent.ana_quality();

	cout << "Output: " << fileout << endl; 
	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

}

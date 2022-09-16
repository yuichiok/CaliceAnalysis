#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run(int energy = 10, string particle = "e-"){

	std::map<int, std::string> run_list {
		{10, "90320"},
		{20, "90378"},
		{40, "90375"},
		{60, "90372"},
		{80, "90367"},
		{100,"90365"},
		{150,"90355"},
	};

	TString name 			 = "raw_siwecal_" + run_list[energy] + "_" + particle + "_" + to_string(energy) + ".0GeV";
	TString input_path = "../../data/reco/";

	cout << "Input: " << name << endl; 

	TBEvent TBEvent(input_path,name);
	// TBEvent.ana_SumE();
	// TBEvent.ana_Eff();
	// TBEvent.ana_Energy();
	// TBEvent.ana_adc_bcid();
	TBEvent.ana_quality();
	// TBEvent.ana_radius();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

}

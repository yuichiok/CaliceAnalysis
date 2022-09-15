#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run_sim(int energy = 10, string particle = "e-"){

	TString input_path = "../../data/sim/";
	TString sim_name   = "ECAL_QGSP_BERT_conf6_" + particle + "_" + to_string(energy) + ".0GeV";

	cout << "Input: " << input_path << endl; 

	TBEvent TBEvent(input_path,sim_name);
	TBEvent.ana_quality();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

}

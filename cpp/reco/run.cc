#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run(TString particle = "e", int set_ene = 10){

	// TString name = particle + TString(set_ene);

	// std::map<TString, int> m { {TString("e10"), 320}, {TString("e80"), 367}, };

	TString filein = "default";
	// // filein = "../data/full_run_90268.root";
	// // filein = "../../data/raw_siwecal_90320/full_run.root";
	// filein = TString("../../data/raw_siwecal_90") + TString(m[name]) + TString("/full_run.root");

	filein = "../../data/raw_siwecal_90367/full_run.root";

	TBEvent TBEvent(filein);
	// TBEvent.ana_SumE();
	// TBEvent.ana_Eff();
	// TBEvent.ana_Energy();
	// TBEvent.ana_adc_bcid();
	TBEvent.ana_quality();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

	// return 0;

}

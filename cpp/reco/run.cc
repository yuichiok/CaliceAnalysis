#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run(TString process="default",TString filein="default",TString fileout="default"){

	// filein = "../data/full_run_90268.root";
	filein = "../../data/raw_siwecal_90320/full_run.root";

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

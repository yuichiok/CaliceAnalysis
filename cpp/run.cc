#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

int run(TString process="default",TString filein="default",TString fileout="default"){

	// filein = "../data/3.0GeV_W_run_050282/full_run.root";
	// filein = "../data/3GeV_MIPscan_eudaq_run_050490/full_run.root";
	// filein = "../data/3GeV_MIPscan_eudaq_run_050480/full_run.root"; // *
	// filein = "../data/3GeV_MIPscan_eudaq_run_050470/full_run.root";

	filein = "../../download/3GeV_MIPscan_eudaq/3GeV_MIPscan_eudaq_run_050480_build.root";

	TBEvent TBEvent(filein);
	TBEvent.AnalysisLoop();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

	return 0;

}
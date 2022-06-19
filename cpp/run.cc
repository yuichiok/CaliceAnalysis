#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run(TString process="default",TString filein="default",TString fileout="default"){

	// filein = "../data/3.0GeV_W_run_050282/full_run.root";
	// filein = "../data/3GeV_MIPscan_eudaq_run_050490/full_run.root";
	// filein = "../data/3GeV_MIPscan_eudaq_run_050480/full_run.root"; // *
	// filein = "../data/3GeV_MIPscan_eudaq_run_050470/full_run.root";
	// filein = "../../download/3GeV_MIPscan_eudaq/3GeV_MIPscan_eudaq_run_050480_build.root";
	// filein = "~/eos/project/s/siw-ecal/TB2022-03/beamData/buildfiles/20220504_3GeV_MIPscan_eudaq/3GeV_MIPscan_eudaq/3GeV_MIPscan_eudaq_run_050480_build.root";
	
	// filein = "../data/build/raw_siwecal_90131/full_run.root";
	filein = "../data/full_run.root";

	TBEvent TBEvent(filein);
	// TBEvent.Ana_SumE();
	// TBEvent.Ana_Eff();
	TBEvent.Ana_Energy();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	cout << "tes" <<endl;

	// gSystem->Exit(0);

	// return 0;

}

#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run_sim(int set_ene = 10, string particle = "sim.e"){

	// TString name = particle + TString(to_string(set_ene));
	string name = particle + "." + to_string(set_ene);
	cout << name << endl;

	TString filein = "default";
	string fileinpath = "../../data/sim/e" + to_string(set_ene) + "GeV/";
	filein = fileinpath + "/ECAL_QGSP_BERT_conf5_e-_" + to_string(set_ene) + ".0GeV.root";

	TString fileout = "default";
	string fileoutpath = "rootfiles/";
	fileout = fileoutpath + "ECAL." + name + "GeV.quality.root";

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

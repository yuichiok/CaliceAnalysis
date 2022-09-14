#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBEvent.cc"

void run_sim(int set_ene = 10, string particle = "e-"){

	// TString name = particle + TString(to_string(set_ene));
	string name = particle + "." + to_string(set_ene);
	cout << name << endl;

	TString filein = "default";
	string fileinpath = "../../data/sim/";
	filein = fileinpath + "/ECAL_QGSP_BERT_conf6_" + particle + "_" + to_string(set_ene) + ".0GeV_build.root";

	TString fileout = "default";
	string fileoutpath = "rootfiles/";
	fileout = fileoutpath + "ECAL.sim." + name + "GeV.quality.root";

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

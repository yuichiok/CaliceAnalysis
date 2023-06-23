#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/EventControl.cc"

void run_sim(int energy = 10, TString particle = "e-"){

	TString input_path = "../../data/conv_sim/";
	TString sim_prefix = "ECAL_QGSP_BERT_conf6";
	TString str_energy = TString(to_string(energy));
	TString sim_name   = sim_prefix + "_" + particle + "_" + str_energy + ".0GeV_converted_masked";
	TString extension = ".root";

	TString full_in_name = input_path + sim_name + extension;
	cout << "Input:  " << full_in_name << endl; 

	EventControl ECMask(full_in_name);
	ECMask.Loop(0);

	TString output_path = "rootfiles/";
	TString output_name = sim_prefix + "_" + particle + "_" + str_energy + ".0GeV_checks";

	TString full_out_name = output_path + output_name + extension;
	cout << "Output: " << full_out_name << endl; 
	ECMask.SaveFile(full_out_name);

	ECMask.ResetHists();
	ECMask.Loop(1);

	full_out_name = output_path + output_name + "_masked" + extension;
	cout << "Output: " << full_out_name << endl;
	ECMask.SaveFile(full_out_name);

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

}

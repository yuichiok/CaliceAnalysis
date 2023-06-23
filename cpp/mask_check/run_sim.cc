#include <iostream>

#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/EventControl.cc"

void run_sim(int energy = 10, string particle = "e-"){

	TString input_path = "../../data/conv_sim/";
	TString sim_name   		= "ECAL_QGSP_BERT_conf6_" + particle + "_" + to_string(energy) + ".0GeV_converted";
	TString sim_name_mask	= "ECAL_QGSP_BERT_conf6_" + particle + "_" + to_string(energy) + ".0GeV_converted_masked";
	TString extension = ".root";

	TString full_name 		 = input_path + sim_name 			+ extension;
	TString full_name_mask = input_path + sim_name_mask + extension;

	cout << "Input:      " << full_name << endl; 
	cout << "Input mask: " << full_name_mask << endl; 

	EventControl EC(full_name);
	EventControl ECMask(full_name_mask);

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

}

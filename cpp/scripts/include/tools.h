#include "../../analysis/src/FileSelector.cc"

TFile * readfile( TString option )
{

	TString suffix      = "_quality.root";

	FileSelector fs(option);
	TString name = fs.GetRunName();
	TString data_path = "../analysis/rootfiles/" + fs.GetRecoSim() + "/";

	if(fs.GetRecoSim() == "conv_sim"){
		suffix = "_quality_masked.root";
	}

	cout << data_path + name + suffix << endl;

	return TFile::Open(data_path + name + suffix);

}
#include <iostream>
#include <filesystem>

#include <TString.h>
#include <TFile.h> 

#include "../include/FileSelector.hh"

using std::cout;   using std::endl;

FileSelector::FileSelector(TString o)
:options(o)
{
  // Make object array
    tobj_arr = options.Tokenize(" ");

  // Extract information (run(reco/sim) particle energy)
    _input_size = tobj_arr->GetEntriesFast();
    for( int i = 0; i < tobj_arr->GetEntriesFast(); i++ ){
      TString input = ((TObjString *)(tobj_arr->At(i)))->String();
      _inputs[_input_names[i]] = input;
    }

    std::pair<TString, Int_t> setup = std::make_pair(_inputs["particle"], _inputs["energy"].Atoi());

    if(_inputs["recosim"].Contains("reco")) _runID = RunMap[setup];

    // using std::filesystem::directory_iterator;
    // std::string directory_path = "../../data/conv_sim/";
    // for (const auto & file: directory_iterator(directory_path)) {
    //     cout << file.path() << endl;
    // }


    MakeRunName();

}

Int_t FileSelector::GetRunID()
{
  return _runID;
}

Int_t FileSelector::GetEnergy()
{
  return _inputs["energy"].Atoi();
}

TString FileSelector::GetRecoSim()
{
  return _inputs["recosim"];
}

TString FileSelector::GetParticleName()
{
  return _inputs["particle"];
}

Bool_t FileSelector::GetMaskOut()
{
  return _inputs["masked"].Length();
}

TString FileSelector::GetRunName()
{
  return _runname;
}

TString FileSelector::GetRunName_with_path()
{
  TString title    = data_path + "/" + _inputs["recosim"] + "/" + _runname;
  TString fullpath = title + build;
  if( _inputs["recosim"] == "conv_sim" ) {
    fullpath = title + conv;
    if( _inputs["masked"].Length() ){
      fullpath += masked;
    }
  }

  fullpath += extension;

  return  fullpath;
}

TString FileSelector::JoinInputs()
{
  return _inputs["particle"] + "_" + TString::Format("%d.0GeV",_inputs["energy"].Atoi());
}

void FileSelector::MakeRunName()
{
  if( _inputs["recosim"] == "reco" ){
    _runname = prefix_reco + TString::Format("%d",_runID) + "_" + JoinInputs();
  }else if( _inputs["recosim"] == "conv_sim" || _inputs["recosim"] == "sim" ){
    _runname = prefix_sim + "_" + JoinInputs();
  }

}
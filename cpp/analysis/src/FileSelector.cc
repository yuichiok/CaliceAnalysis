#include <iostream>
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
    _recosim  = ((TObjString *)(tobj_arr->At(0)))->String();
    _particle = ((TObjString *)(tobj_arr->At(1)))->String();
    _energy   = ((TObjString *)(tobj_arr->At(2)))->String().Atoi();
    std::pair<TString, Int_t> setup = std::make_pair(_particle, _energy);

    if(_recosim.Contains("reco")) _runID = RunMap[setup];

    MakeRunName();

}

Int_t FileSelector::GetRunID()
{
  return _runID;
}

Int_t FileSelector::GetEnergy()
{
  return _energy;
}

TString FileSelector::GetRecoSim()
{
  return _recosim;
}

TString FileSelector::GetParticleName()
{
  return _particle;
}

TString FileSelector::GetRunName()
{
  return _runname;
}

TString FileSelector::GetRunName_with_Path()
{
  TString fullpath = data_path + "/" + _recosim + "/" + _runname + suffix;
  return  fullpath;
}

void FileSelector::MakeRunName()
{
  if(_recosim.Contains("reco")){
    _runname = prefix_reco + TString::Format("%d",_runID) + "_" + _particle + "_" + _energy;
  }else if(_recosim.Contains("sim")){
    _runname = prefix_sim + "_" + _particle + "_" + _energy;
  }
}
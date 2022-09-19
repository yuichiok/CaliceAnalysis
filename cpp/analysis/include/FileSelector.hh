#ifndef GUARD_FileSelector_h
#define GUARD_FileSelector_h

#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <map>

class FileSelector
{
  public:
    FileSelector(TString o="");
    ~FileSelector(){}

    virtual Int_t   GetRunID();
    virtual Int_t   GetEnergy();
    virtual TString GetRecoSim();
    virtual TString GetParticleName();

    virtual void    MakeRunName();
    virtual TString GetRunName();
    virtual TString GetRunName_with_path();

    TString     options;
    TObjArray *tobj_arr;

    TString data_path         = "../../data/";

    TString prefix_reco       = "raw_siwecal_";
    TString prefix_sim        = "ECAL_QGSP_BERT_conf6";
    TString gev               = ".0GeV";
    TString suffix_build      = "_build.root";
    TString suffix_conv       = "_converted.root";

    std::map<std::pair<TString, Int_t>, Int_t > RunMap {
      {std::make_pair("e-", 10),  90320},
      {std::make_pair("e-", 20),  90378},
      {std::make_pair("e-", 40),  90375},
      {std::make_pair("e-", 60),  90372},
      {std::make_pair("e-", 80),  90367},
      {std::make_pair("e-", 100), 90365},
      {std::make_pair("e-", 150), 90355}, {std::make_pair("mu-", 150), 90266}
    };

  private:
    Int_t     _runID    ;
    Int_t     _energy   ;
    TString   _recosim  ;
    TString   _particle ;

    TString   _runname  ;



};

#endif
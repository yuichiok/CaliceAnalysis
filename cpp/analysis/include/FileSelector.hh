#ifndef GUARD_FileSelector_h
#define GUARD_FileSelector_h

#include <TString.h>
#include <TObjArray.h>
#include <TObjString.h>
#include <map>

using std::map; using std::vector;

class FileSelector
{
  public:
    FileSelector(TString o="");
    ~FileSelector(){}

    virtual Int_t   GetRunID();
    virtual Int_t   GetEnergy();
    virtual TString GetRecoSim();
    virtual TString GetParticleName();
    virtual Bool_t  GetMaskOut();

    virtual void    MakeRunName();
    virtual TString JoinInputs();
    virtual TString GetRunName();
    virtual TString GetRunName_with_path();

    TString     options;
    TObjArray *tobj_arr;

    TString data_path         = "../../data/";

    TString prefix_reco       = "raw_siwecal_";
    TString prefix_sim        = "ECAL_QGSP_BERT_conf6";
    TString gev               = ".0GeV";
    TString build             = "_build";
    TString conv              = "_converted";
    TString masked            = "_masked";
    TString extension         = ".root";

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
    TString   _masked   ;
    TString   _particle ;

    vector<TString>       _input_names = {"recosim", "particle", "energy", "masked"};
    Int_t                 _input_size;
    map<TString, TString> _inputs;

    TString   _runname  ;



};

#endif
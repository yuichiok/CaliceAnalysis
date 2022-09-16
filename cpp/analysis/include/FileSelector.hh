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
    virtual TString GetParticleName();

    TString     options;
    TObjArray *tobj_arr;

    std::map<std::pair<TString, Int_t>, Int_t > RunMap {
      {std::make_pair("e-", 10),  90320},
      {std::make_pair("e-", 20),  90378},
      {std::make_pair("e-", 40),  90375},
      {std::make_pair("e-", 60),  90372},
      {std::make_pair("e-", 80),  90367},
      {std::make_pair("e-", 100), 90365},
      {std::make_pair("e-", 150), 90355}
    };

  private:
    Int_t        _runID;
    Int_t       _energy;
    TString    _recosim;
    TString   _particle;


};

#endif
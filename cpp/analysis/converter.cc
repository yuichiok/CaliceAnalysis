/*------------------------------------------------------------------------------
   main.cpp
------------------------------------------------------------------------------*/
#define _GLIBCXX_USE_CXX11_ABI 0

#include "TROOT.h"
#include "TFile.h"
#include "src/Sim2Build.cc"
#include "src/TreeWriter.cc"

// int main(){
int main(int argc, char* argv[]){

  std::string arg = argv[1];

  try {
    std::size_t pos;
    int x = std::stoi(arg, &pos);
    if (pos < arg.size()) {
      std::cerr << "Trailing characters after number: " << arg << '\n';
    }
  } catch (std::invalid_argument const &ex) {
    std::cerr << "Invalid number: " << arg << '\n';
  } catch (std::out_of_range const &ex) {
    std::cerr << "Number out of range: " << arg << '\n';
  }

  Int_t set_energy = std::stoi(arg);

  TString particle = "e-";
  TString energy   = TString::Format("%d",set_energy);
  TString settings = particle + " " + energy;
  
  Sim2Build Sim2Build(settings);
  Sim2Build.Loop();

  return 0;
}

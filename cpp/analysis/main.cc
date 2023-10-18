/*------------------------------------------------------------------------------
   main.cpp
------------------------------------------------------------------------------*/
#define _GLIBCXX_USE_CXX11_ABI 0

// HEADERS
#include <iostream>                 // stdlib
#include <string>
#include <TFile.h>                  // ROOT class headers
#include <TString.h>
#include <TTree.h>
#include "../library/TreeStructures.hh"
#include "include/NtupleProcessor.hh"

using namespace std;

int main(int argc, char* argv[])
// int main()
{

  if(argc < 3) cerr << "Error: Arguments missing. (e.g. ./main.exe reco e- 10 for 10 GeV electron)" << endl;
  // ./main.exe conv_sim e- 10 masked

  TString option;
  for( int i = 1; i <= argc; i++ ){
    option += argv[i];
    if( i < argc ) option += " ";
  }

  NtupleProcessor ntplproc(option,-1);

  return 0;
}
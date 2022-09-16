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
#include "library/TreeStructures.hh"
#include "include/NtupleProcessor.hh"

using namespace std;

// int main(int argc, char* argv[])
int main()
{

  NtupleProcessor ntplproc("reco e- 20",-1);

  return 0;
}
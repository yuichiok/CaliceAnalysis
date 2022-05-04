#define EventControl_cxx
#include "../include/EventControl.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void EventControl::ProcessEvents(bool debug=false){

   if (fChain == 0) return;
   Long64_t nentries = fChain->GetEntriesFast();

   cout << "nentries = " << nentries << "\n";

   // for (Long64_t jentry=0; jentry<nentries;jentry++) {

   // }
}
#define TBEvent_cxx
#include "../include/TBEvent.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using std::cout;
using std::endl;

void TBEvent::Loop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // if (Cut(ientry) < 0) continue;

      if(!(jentry % 10000)){
         cout << "(#events, cycle, spill, bcid) = (" << event << ", " << cycle << ", " << spill << ", " << bcid << ")" << "\n";
         cout << "sum energy = " << sum_energy << "\n";
      }

      // cout << event << "\n";



   }

   cout << "loop over\n";

}

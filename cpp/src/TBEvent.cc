#define TBEvent_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "../include/TBEvent.hh"
#include "../include/Hists.hh"

using std::cout;
using std::endl;

void TBEvent::AnalysisLoop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Hists H;
   H.init();

   TFile *MyFile = new TFile("output.root","RECREATE");

   MyFile->cd();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if (Cut(ientry) < 0) continue;

      H.h_sum_energy->Fill(sum_energy);


      for (int ihit = 0; ihit < nhit_len; ++ihit)
      {
         if(hit_slab[ihit]==0) {

         }
         
      }


      if(!(jentry % 10000)){

         for (int ihit = 0; ihit < nhit_len; ++ihit)
         {

            cout << hit_slab[ihit] << "=" << hit_z[ihit] << " " ;

         }

         cout << endl;

         cout << "(#events, cycle, nhit_len) = (" << event << ", " << cycle << ", " << nhit_len << ")" << "\n";
         cout << "sum energy = " << sum_energy << "\n";

      }


   } // end of loop


   H.writes();

   cout << "loop over\n";

}

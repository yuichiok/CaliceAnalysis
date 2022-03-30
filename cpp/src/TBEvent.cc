#define TBEvent_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include "../include/TBEvent.hh"
#include "../include/Hists.hh"

using std::cout;
using std::endl;

const bool debug = false;

void TBEvent::AnalysisLoop()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Hists H;
   H.init();

   TFile *MyFile = new TFile("output.root","RECREATE");

   // MyFile->cd();

   // TDirectory *cdhisto[15];
   // for(int ilayer=0; ilayer<nlayers; ilayer++) {
   //  cdhisto[ilayer] = MyFile->mkdir(TString::Format("layer_%i",ilayer));
   // }

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      if (Cut(ientry) < 0) continue;

      H.h_sum_energy->Fill(sum_energy);


      for (int ihit = 0; ihit < nhit_len; ++ihit)
      {

         H.h_channel_energy[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

         // if(hit_slab[ihit]==0) {

         // }

      }


      if(debug && !(jentry % 10000)){

         for (int ihit = 0; ihit < nhit_len; ++ihit)
         {

            cout << hit_slab[ihit] << "=" << hit_z[ihit] << " " ;

         }

         cout << endl;

         cout << "(#events, cycle, nhit_len) = (" << event << ", " << cycle << ", " << nhit_len << ")" << "\n";
         cout << "sum energy = " << sum_energy << "\n";

      }


   } // end of loop


   H.writes(MyFile);

   cout << "loop over\n";

}

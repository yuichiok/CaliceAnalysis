#define TBEvent_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end

#include "../include/TBEvent.hh"
#include "../include/Hists.hh"

using std::cout;
using std::endl;

const bool debug = false;

void TBEvent::Ana_SumE()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Hists H;
   H.init();

   TFile *MyFile = new TFile("rootfiles/output.root","RECREATE");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      // if (Cut(ientry) < 0) continue;

      float timeSec = CycleToSec(cycle);
      float sum_energy_beam      = 0;
      float sum_energy_layer[15] = {0};

      H.h_sum_energy->Fill(sum_energy);

      for (int ihit = 0; ihit < nhit_len; ++ihit)
      {

         float beam_spotX = (hit_x[ihit] + 15.0)*(hit_x[ihit] + 15.0);
         float beam_spotY = (hit_y[ihit] + 15.0)*(hit_y[ihit] + 15.0);
         // if(hit_chan[ihit]==63){
         // if( -50.0 < hit_x[ihit] && hit_x[ihit] < -23.0 && -50.0 < hit_y[ihit] && hit_y[ihit] < -23.0 ){
         if( (beam_spotX + beam_spotY) < 28.0*28.0 ){
            H.hL_hg_beam[hit_slab[ihit]]->Fill(hit_adc_high[ihit]);
            H.hL_energy_beam[hit_slab[ihit]]->Fill(hit_energy[ihit]);

            H.hL_xy_energy_beam[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

            sum_energy_layer[hit_slab[ihit]] += hit_energy[ihit];
            sum_energy_beam += hit_energy[ihit];
         }

         H.hL_hitrate[hit_slab[ihit]]->Fill(timeSec);
         H.hL_xy_energy[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

      }

      H.h_sum_energy_beam->Fill(sum_energy_beam);

      for (int ilayer = 0; ilayer < 15; ++ilayer)
      {
         H.h_sum_energy_layer->Fill(sum_energy_layer[ilayer],ilayer);
      }







      // Playground
      Debug(debug,jentry);

   } // end of loop


   H.writes(MyFile);

   cout << "loop over\n";

}

void TBEvent::Ana_Eff()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   Hists H;
   H.init();

   TFile *MyFile = new TFile("rootfiles/output2.root","RECREATE");

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;

      bool slab7_check = std::find(std::begin(hit_slab), std::end(hit_slab), 7) != std::end(hit_slab);
      if( (nhit_slab < 6) || (!slab7_check) ) continue;

      for (int ihit=0; ihit<nhit_len; ihit++){
         // cout << "(event, bcid) = ("<< event << "," << bcid << "), slab hit = " << hit_slab[ihit] << endl;

         

      }


      // Playground
      Debug(debug,jentry);

   } // end of loop

   H.writes(MyFile);

   cout << "loop over\n";

}

float TBEvent::CycleToSec(int cyc=-1)
{
   float aq_sec      = 0.001;
   float aqdelay_sec = 0.01;
   float spc = aq_sec + aqdelay_sec;
   float time_passed = spc * cyc;

   return time_passed;

}

void TBEvent::Debug(bool debug=false, Long64_t entry=0)
{

   if(!debug) return;
   else if( !(entry % 10000) ) return;

   for (int ihit = 0; ihit < nhit_len; ++ihit)
   {

      cout << hit_sca[ihit] << " " ;

   }

   cout << endl;

   // cout << "(id_dat, cycle, sec) = (" << id_dat << ", " << cycle << ", " << timeSec << ")" << "\n";
   // cout << "sum energy = " << sum_energy << "\n";

   
}

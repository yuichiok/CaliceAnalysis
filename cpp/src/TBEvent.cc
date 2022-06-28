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
const int nslabs = 15;
const float beamX = 20.0, beamY = 15.0;

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
      float sum_energy_slab[15] = {0};

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

            sum_energy_slab[hit_slab[ihit]] += hit_energy[ihit];
            sum_energy_beam += hit_energy[ihit];
         }

         H.hL_hitrate[hit_slab[ihit]]->Fill(timeSec);
         H.hL_xy_energy[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

      }

      H.h_sum_energy_beam->Fill(sum_energy_beam);

      for (int islab = 0; islab < 15; ++islab)
      {
         H.h_sum_energy_slab->Fill(sum_energy_slab[islab],islab);
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

      // must have more than 6 slab hits and slab 7 inclusive
      bool slab7_check = std::find(std::begin(hit_slab), std::end(hit_slab), 7) != std::end(hit_slab);
      if( (nhit_slab < 6) || (!slab7_check) ) continue;

      bool not_conseq = false;
      int  cslab = -1;

      // conseq check
      for (int ihit=0; ihit<nhit_len; ihit++){
         if(ihit==0){
            cslab = hit_slab[ihit];
         }else if( !(hit_slab[ihit]==cslab) && !(hit_slab[ihit]==(cslab+1)) ){
            not_conseq = true;
            break;
         }else{
            cslab = hit_slab[ihit];
         }
      }

      if (not_conseq) continue;

      bool beamcheck = false;
      bool nobeamcheck = false;

      float sum_energy_beam             = 0;
      float sum_energy_nobeam           = 0;

      float sum_energy_slab[nslabs]        = {0};
      float sum_energy_slab_beam[nslabs]   = {0};
      float sum_energy_slab_nobeam[nslabs] = {0};

      for (int ihit=0; ihit<nhit_len; ihit++){

         float beam_spotX = (hit_x[ihit] + beamX)*(hit_x[ihit] + beamX);
         float beam_spotY = (hit_y[ihit] + beamY)*(hit_y[ihit] + beamY);

         if( (beam_spotX + beam_spotY) < 28.0*28.0 ){ // IN beam spot

            beamcheck = true;

            H.hL_xy_energy_beam[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);
            H.hL_xy_hit_beam[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit]);
            sum_energy_slab_beam[hit_slab[ihit]] += hit_energy[ihit];
            sum_energy_beam += hit_energy[ihit];

         }else{ // OUT beam spot

            nobeamcheck = true;

            H.hL_xy_energy_nobeam[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);
            H.hL_xy_hit_nobeam[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit]);
            sum_energy_slab_nobeam[hit_slab[ihit]] += hit_energy[ihit];
            sum_energy_nobeam += hit_energy[ihit];

         }

         H.hL_xy_energy[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);
         H.hL_xy_hit[hit_slab[ihit]]->Fill(hit_x[ihit],hit_y[ihit]);
         
         sum_energy_slab[hit_slab[ihit]] += hit_energy[ihit];

      } // hit loop

      if(!beamcheck){
         sum_energy_beam = -1;
      }
      if(!nobeamcheck){
         sum_energy_nobeam = -1;
      }

      H.h_sum_energy->Fill(sum_energy);
      H.h_sum_energy_beam->Fill(sum_energy_beam);
      H.h_sum_energy_nobeam->Fill(sum_energy_nobeam);

      for (int islab = 0; islab < nslabs; ++islab)
      {
         H.h_sum_energy_slab->Fill(sum_energy_slab[islab],islab);
         H.h_sum_energy_slab_beam->Fill(sum_energy_slab_beam[islab],islab);
         H.h_sum_energy_slab_nobeam->Fill(sum_energy_slab_nobeam[islab],islab);
      }


      // playground
      Debug(debug,jentry);

   } // end of event loop

   H.writes(MyFile);

   cout << "Done.\n";

}

void TBEvent::Ana_Energy()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TFile *MyFile = new TFile("rootfiles/energy.root","RECREATE");

   TH1F * h_hit_wall = new TH1F("h_hit_wall","h_hit_wall;Layer;Entry",15,-0.5,14.5);
   TH1F * h_hit[4];
   for(int ih=0;ih<4;ih++)
   {
      TString str0 = TString::Format("h_hit_w%i",ih);
      TString str1 = str0 + TString::Format(";Layer;Entry");
      h_hit[ih] = new TH1F(str0,str1,15,-0.5,14.5);
   }

   TH1F * pass_stat = new TH1F("pass_stat","pass_stat",4,-0.5,3.5);

   Long64_t nbytes = 0, nb = 0;
   for (int jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      int hs[10];
      for (int i = 0; i < 10; i++) hs[i] = -1;
      int pass[4] = {0};

      // peneturate slab 5-14?
      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         if(hit_energy[ihit]<0.5) continue;
         for (int itr = 0; itr < 10; itr++)
         {
            if (hit_slab[ihit]==itr){
               if( (0 <= hit_chip[ihit]) && (hit_chip[ihit] <= 3) ){
                  hs[itr] = 0;
               }else if ( (4 <= hit_chip[ihit]) && (hit_chip[ihit] <= 7) ){
                  hs[itr] = 1;
               }else if ( (8 <= hit_chip[ihit]) && (hit_chip[ihit] <= 11) ){
                  hs[itr] = 2;
               }else if ( (12 <= hit_chip[ihit]) && (hit_chip[ihit] <= 15) ){
                  hs[itr] = 3;
               }
            }
         }
      }
      for(int itr = 0; itr < 10; itr++)
      {
         for (int i = 0; i < 4; i++)
         {
            if(hs[itr]==i) pass[i]++;
         }
      }
      
      // if penetrates

      for(int i=0; i<4; i++){

         if(pass[i]==10){

            pass_stat->Fill(i);

            for (int ihit = 0; ihit < nhit_len; ihit++)
            {
               h_hit_wall->Fill(hit_slab[ihit]);
               h_hit[i]->Fill(hit_slab[ihit]);
            }

         }else{
            continue;
         }

      }

   }

   MyFile->cd();

   h_hit_wall->Write();
   for(int ih=0; ih<4; ih++) h_hit[ih]->Write();

   TCanvas *c0 = new TCanvas("c0","c0",700,700);
   c0->Divide(2,2);
   c0->cd(1);
   h_hit[1]->Draw("h");
   c0->cd(2);
   h_hit[0]->Draw("h");
   c0->cd(3);
   h_hit[3]->Draw("h");
   c0->cd(4);
   h_hit[2]->Draw("h");

   c0->Write();

   TCanvas *c1 = new TCanvas("c1","c1",500,500);
   c1->cd();
   c1->SetLogy();   
   pass_stat->Draw("h");
   c1->Write();

   cout << "Done.\n";

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

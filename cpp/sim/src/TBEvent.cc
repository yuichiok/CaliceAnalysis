#define TBEvent_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <string>
#include <map>

#include "../include/TBEvent.hh"
#include "../include/Hists.hh"

using std::cout;
using std::endl;

const bool debug = false;
const int nslabs = 15;
const int nscas = 15;
const float beamX = 20.0, beamY = 15.0;

void TBEvent::ana_quality()
{
   gStyle->SetOptStat(0);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TString outpath = "rootfiles/quality/";
   OutFile = new TFile(outpath + OutFileName + "_quality.root","RECREATE");

   TList* hList = new TList();
   TList* hList_energy = new TList();
   TH1F * h_sum_energy = new TH1F("h_sum_energy","; sum_energy; Entries",500,0,1.5E4);
   TH1F * h_hit_energy = new TH1F("h_hit_energy","; hit_energy; Entries",500,0,1000);
   TH1F * h_hit_slab_energy[nslabs];
   for (int islab = 0; islab < nslabs; islab++)
   {
      TString hname = "h_hit_slab_energy" + to_string(islab);
      h_hit_slab_energy[islab] = new TH1F(hname,hname,120,-20,100);
   }

   TH1F * h_hit_slab   = new TH1F("h_hit_slab","; hit_slab; Entries",nslabs,-0.5,14.5);

   hList->Add(h_sum_energy);
   hList->Add(h_hit_energy);
   for(int ih=0;ih<nslabs;ih++) hList_energy->Add(h_hit_slab_energy[ih]);
   hList->Add(h_hit_slab);

   Long64_t nbytes = 0, nb = 0;
   for (int jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);

      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      if(nhit_slab < 13) continue;

      h_sum_energy->Fill(sum_energy);


      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         // if(hit_adc_high->at(ihit)<400) continue;
         // if(hit_sca->at(ihit)>0) continue;

         h_hit_slab->Fill(hit_slab->at(ihit));
         h_hit_energy->Fill(hit_energy->at(ihit));

         for (int islab = 0; islab < nslabs; islab++)
         {
            if (hit_slab->at(ihit)==islab) h_hit_slab_energy[islab]->Fill(hit_energy->at(ihit));
         }
         
      }
      // break;

   }

   TCanvas * c_hit_slab_energy = new TCanvas("c_hit_slab_energy","c_hit_slab_energy",700,700);
   c_hit_slab_energy->Divide(4,4);
   for (int islab = 0; islab < nslabs; islab++)
   {
      c_hit_slab_energy->cd(islab+1);
      h_hit_slab_energy[islab]->Draw("h");
   }

   OutFile->cd();
   hList->Write();
   
   TDirectory * d_ene = OutFile->mkdir("hit_slab_energy");
   d_ene->cd();
   c_hit_slab_energy->Write();
   hList_energy->Write();


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

      cout << hit_sca->at(ihit) << " " ;

   }

   cout << endl;

   // cout << "(id_dat, cycle, sec) = (" << id_dat << ", " << cycle << ", " << timeSec << ")" << "\n";
   // cout << "sum energy = " << sum_energy << "\n";

   
}

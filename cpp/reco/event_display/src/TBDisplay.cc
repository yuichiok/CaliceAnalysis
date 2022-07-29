#define TBDisplay_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <string>
#include <map>

#include "../include/TBDisplay.hh"

using std::cout;
using std::endl;

const bool debug = false;
const int nslabs = 15;
const int nscas = 15;
const float beamX = 20.0, beamY = 15.0;

void TBDisplay::Display()
{
   gStyle->SetOptStat(0);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   OutFile = new TFile("test.root","RECREATE");

   Long64_t nbytes = 0, nb = 0;
   int ievent = 0;
   int cnt_event = 0;


   TCanvas * c = new TCanvas("c","c",800,800);

   while( cin >> ievent ){

      TH3F * h_display    = new TH3F("h_display",";z;x;y",285,0.0,285.0,32,-90.0,90.0,32,-90.0,90.0);

      for (int jentry=0; jentry<nentries;jentry++) {

         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;
         nb = fChain->GetEntry(jentry);
         nbytes += nb;

         if(nhit_slab < 13) continue;
         cnt_event++;

         if(cnt_event==ievent){
            for (int ihit = 0; ihit < nhit_len; ihit++)
            {
               h_display->Fill(hit_z[ihit],hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

            } // hit loop
            break;
         }

      }

      h_display->Draw("lego");
      c->Draw();
      c->Modified();
      c->Update();
      gSystem->ProcessEvents();
      delete h_display;


   };

   cout << "Done.\n";

}

float TBDisplay::CycleToSec(int cyc=-1)
{
   float aq_sec      = 0.001;
   float aqdelay_sec = 0.01;
   float spc = aq_sec + aqdelay_sec;
   float time_passed = spc * cyc;

   return time_passed;

}

void TBDisplay::Debug(bool debug=false, Long64_t entry=0)
{

   if(!debug) return;
   else if( !(entry % 10000) ) return;

   for (int ihit = 0; ihit < nhit_len; ++ihit)
   {

      cout << hit_sca[ihit] << " " ;

   }

   cout << endl;
   
}

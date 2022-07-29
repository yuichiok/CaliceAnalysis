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

   OutFile = new TFile(OutFileName + "GeV.MR.root","RECREATE");

   TList* hList = new TList();
   TH3F * h_3d_charge_map = new TH3F("h_3d_charge_map",";z;x;y",15,0,15,16,-90,0,16,-90,0);
   TH3F * h_3d_beam_cm    = new TH3F("h_3d_beam_cm",";z;x;y",15,0,15,16,-90,0,16,-90,0);

   hList->Add(h_3d_charge_map);
   hList->Add(h_3d_beam_cm);

   int offset = 0;
   int last_bcid = -1;
   int true_bcid = 0;

   Long64_t nbytes = 0, nb = 0;
   for (int jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);
      nbytes += nb;

      if(bcid < last_bcid){
         offset=offset+4096;
      }
      true_bcid=bcid+offset;
      last_bcid=bcid;

      if(nhit_slab < 13) continue;

      float Xcm[nslabs] = {0};
      float Ycm[nslabs] = {0};
      float Wsum[nslabs] = {0};

      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         h_3d_charge_map->Fill(hit_slab[ihit],hit_x[ihit],hit_y[ihit],hit_energy[ihit]);
         
         for (int islab = 0; islab < nslabs; islab++)
         {
            if (hit_slab[ihit]==islab)
            {
               Xcm[islab]  += hit_x[ihit] * hit_energy[ihit];
               Ycm[islab]  += hit_y[ihit] * hit_energy[ihit];
               Wsum[islab] += hit_energy[ihit];
            }

         } // match slab

      } // hit loop

      for (int islab = 0; islab < nslabs; islab++)
      {
         Xcm[islab] = Xcm[islab] / Wsum[islab];
         Ycm[islab] = Ycm[islab] / Wsum[islab];
      
         h_3d_beam_cm->Fill(islab,Xcm[islab],Ycm[islab]);

      } // match slab
      


   } // event loop

   OutFile->cd();
   hList->Write();

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

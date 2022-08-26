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

void TBEvent::ana_SumE()
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

void TBEvent::ana_Eff()
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

void TBEvent::ana_Energy()
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

void TBEvent::ana_adc_bcid()
{
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TFile *MyFile = new TFile("rootfiles/adc_bcid_run_90352.root","RECREATE");

   TH2F * h_adc_bcid_filter = new TH2F("h_adc_bcid_filter","Layer*20+Chip vs. bcid vs. adc_high (filtered);true_bcid;Layer*20+Chip",2530,0,10.4E7,300,-0.5,299.5);
   TH2F * h_adc_bcid_noise = new TH2F("h_adc_bcid_noise","Layer*20+Chip vs. bcid vs. adc_high (filtered);true_bcid;Layer*20+Chip",2530,0,10.4E7,300,-0.5,299.5);
   TH2F * h_adc_bcid_nofilter = new TH2F("h_adc_bcid_nofilter","Layer*20+Chip vs. bcid vs. adc_high (no filter);true_bcid;Layer*20+Chip",2530,0,10.4E7,300,-0.5,299.5);

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

      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         if(hit_energy[ihit]>1.0) h_adc_bcid_filter->Fill(true_bcid,hit_slab[ihit]*20+hit_chip[ihit],hit_adc_high[ihit]);
         if(hit_energy[ihit]<1.0) h_adc_bcid_noise->Fill(true_bcid,hit_slab[ihit]*20+hit_chip[ihit],hit_adc_high[ihit]);
         h_adc_bcid_nofilter->Fill(true_bcid,hit_slab[ihit]*20+hit_chip[ihit],hit_adc_high[ihit]);

      }

   }

   MyFile->cd();
   h_adc_bcid_filter->Write();
   h_adc_bcid_noise->Write();
   h_adc_bcid_nofilter->Write();

   cout << "Done.\n";

}

void TBEvent::ana_quality()
{
   gStyle->SetOptStat(0);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   // TFile *MyFile = new TFile("rootfiles/run_90320.e.10GeV.quality.root","RECREATE");
   OutFile = new TFile(OutFileName + "GeV.quality.root","RECREATE");

   TList* hList = new TList();
   TList* hList_energy = new TList();
   TList* hList_energy_sca = new TList();
   TH1F * h_sum_energy = new TH1F("h_sum_energy","; sum_energy; Entries",500,0,1.5E4);
   TH1F * h_hit_energy = new TH1F("h_hit_energy","; hit_energy; Entries",120,-20,100);
   TH1F * h_hit_slab_energy[nslabs];
   for (int islab = 0; islab < nslabs; islab++)
   {
      TString hname = "h_hit_slab_energy" + to_string(islab);
      h_hit_slab_energy[islab] = new TH1F(hname,hname,120,-20,100);
   }
   
   TH1F * h_hit_slab7_energy_sca[nscas];
   for (int isca = 0; isca < nscas; isca++)
   {
      TString hname = "h_hit_slab7_energy_sca" + to_string(isca);
      h_hit_slab7_energy_sca[isca] = new TH1F(hname,hname,120,-20,100);
   }

   TH1F * h_hit_slab   = new TH1F("h_hit_slab","; hit_slab; Entries",nslabs,-0.5,14.5);

   hList->Add(h_sum_energy);
   hList->Add(h_hit_energy);
   for(int ih=0;ih<nslabs;ih++) hList_energy->Add(h_hit_slab_energy[ih]);
   for(int isca=0;isca<nscas;isca++) hList_energy_sca->Add(h_hit_slab7_energy_sca[isca]);
   hList->Add(h_hit_slab);

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

      h_sum_energy->Fill(sum_energy);

      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         // if(hit_adc_high[ihit]<400) continue;
         // if(hit_sca[ihit]>0) continue;

         h_hit_slab->Fill(hit_slab[ihit]);
         h_hit_energy->Fill(hit_energy[ihit]);

         for (int islab = 0; islab < nslabs; islab++)
         {
            if (hit_slab[ihit]==islab) h_hit_slab_energy[islab]->Fill(hit_energy[ihit]);
         }

         if(hit_slab[ihit]==7){
            for (int isca = 0; isca < nscas; isca++)
            {
               if (hit_sca[ihit]==isca) h_hit_slab7_energy_sca[isca]->Fill(hit_energy[ihit]);
            }
         }
         
      }

   }

   TCanvas * c_hit_slab_energy = new TCanvas("c_hit_slab_energy","c_hit_slab_energy",700,700);
   c_hit_slab_energy->Divide(4,4);
   for (int islab = 0; islab < nslabs; islab++)
   {
      c_hit_slab_energy->cd(islab+1);
      h_hit_slab_energy[islab]->Draw("h");
   }

   TCanvas * c_hit_slab7_energy_sca = new TCanvas("c_hit_slab7_energy_sca","c_hit_slab7_energy_sca",700,700);
   c_hit_slab7_energy_sca->Divide(4,4);
   for (int isca = 0; isca < nscas; isca++)
   {
      c_hit_slab7_energy_sca->cd(isca+1);
      h_hit_slab7_energy_sca[isca]->Draw("h");
   }


   OutFile->cd();
   hList->Write();
   
   TDirectory * d_ene = OutFile->mkdir("hit_slab_energy");
   d_ene->cd();
   c_hit_slab_energy->Write();
   hList_energy->Write();
   OutFile->cd();

   TDirectory * d_ene_sca = OutFile->mkdir("hit_slab_energy_sca");
   d_ene_sca->cd();
   c_hit_slab7_energy_sca->Write();
   hList_energy_sca->Write();
   OutFile->cd();

   cout << "Done.\n";

}

void TBEvent::ana_radius()
{
   gStyle->SetOptStat(0);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   OutFile = new TFile(OutFileName + "GeV.MR.root","RECREATE");

   TList* hList = new TList();
   TH3F * h_3d_charge_map      = new TH3F("h_3d_charge_map",";z;x;y",15,0,15,16,-90,0,16,-90,0);
   TH3F * h_3d_charge_map_dist = new TH3F("h_3d_charge_map_dist",";z;x;y",210,0,210,16,-90,0,16,-90,0);
   TH3F * h_3d_beam_cm         = new TH3F("h_3d_beam_cm",";z;x;y",15,0,15,16,-90,0,16,-90,0);
   TH1F * h_sum_energy         = new TH1F("h_sum_energy","; sum_energy; Entries",500,0,1.5E4);
   TH1F * h_sum_energy_isC_isH = new TH1F("h_sum_energy_isC_isH","; h_sum_energy_isC_isH; Entries",500,0,1.5E4);
   TH1F * h_hit_energy         = new TH1F("h_hit_energy","; hit_energy; Entries",500,0,1000);

   TH1F * h_hit_LowEnergy      = new TH1F("h_hit_LowEnergy","; hit_energy (sumE < 1000); Entries",500,0,1000);
   TH1F * h_hit_MidEnergy      = new TH1F("h_hit_MidEnergy","; hit_energy (1000 < sumE < 2000); Entries",500,0,1000);
   TH1F * h_hit_HighEnergy     = new TH1F("h_hit_HighEnergy","; hit_energy (2000 < sumE); Entries",500,0,1000);

   TH1F * h_LowHitLen      = new TH1F("h_LowHitLen","; nhit_len (sumE < 1000); Entries",300,0,300);
   TH1F * h_MidHitLen      = new TH1F("h_MidHitLen","; nhit_len (1000 < sumE < 2000); Entries",300,0,300);
   TH1F * h_HighHitLen     = new TH1F("h_HighHitLen","; nhit_len (2000 < sumE); Entries",300,0,300);

   hList->Add(h_3d_charge_map);
   hList->Add(h_3d_charge_map_dist);
   hList->Add(h_3d_beam_cm);
   hList->Add(h_sum_energy);
   hList->Add(h_sum_energy_isC_isH);
   hList->Add(h_hit_energy);
   hList->Add(h_hit_LowEnergy);
   hList->Add(h_hit_MidEnergy);
   hList->Add(h_hit_HighEnergy);
   hList->Add(h_LowHitLen);
   hList->Add(h_MidHitLen);
   hList->Add(h_HighHitLen);


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
      // if(sum_energy > 1000) continue;

      TH2F * h_xy_energy[nslabs];
      for ( int islab=0; islab < nslabs; islab++ ){
         h_xy_energy[islab]  = new TH2F(TString::Format("h_xy_energy_%i",islab),"; x;y",32,-90,90,32,-90,90);
      }

      float Xcm[nslabs] = {0};
      float Ycm[nslabs] = {0};
      float Wsum[nslabs] = {0};

      float Etot100 = 0;
      float EisCisH = 0;
      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         h_3d_charge_map->Fill(hit_slab[ihit],hit_x[ihit],hit_y[ihit],hit_energy[ihit]);
         h_3d_charge_map_dist->Fill(hit_z[ihit],hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

         h_hit_energy->Fill(hit_energy[ihit]);
         if(0 < hit_energy[ihit] && hit_energy[ihit] < 100){
            Etot100 += hit_energy[ihit];
         }else if(100 < hit_energy[ihit]){
            Etot100 += 100;
         }
         
         for (int islab = 0; islab < nslabs; islab++)
         {
            if (hit_slab[ihit]==islab)
            {
               h_xy_energy[islab]->Fill(hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

               Xcm[islab]  += hit_x[ihit] * hit_energy[ihit];
               Ycm[islab]  += hit_y[ihit] * hit_energy[ihit];
               Wsum[islab] += hit_energy[ihit];
            }

         } // match slab

         if(hit_isHit[ihit] && hit_isCommissioned[ihit]){
            EisCisH += hit_energy[ihit];
         }

      } // hit loop

      h_sum_energy_isC_isH->Fill(EisCisH);

      float Etot = 0;
      for (int islab = 0; islab < nslabs; islab++)
      {
         Xcm[islab] = Xcm[islab] / Wsum[islab];
         Ycm[islab] = Ycm[islab] / Wsum[islab];
      
         h_3d_beam_cm->Fill(islab,Xcm[islab],Ycm[islab]);

         Etot += h_xy_energy[islab]->Integral();

      } // match slab
      

      // Shower axis set to entrance point of the beam
      float orgX = Xcm[0];
      float orgY = Ycm[0];      
      
      float En   = 0;
      float Eeff = 0;
      float MR     = 40.0;
      float MRstep = 2.0;
      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         float beam_spotX = (hit_x[ihit] - orgX)*(hit_x[ihit] - orgX);
         float beam_spotY = (hit_y[ihit] - orgY)*(hit_y[ihit] - orgY);

         if( (beam_spotX + beam_spotY) < MR*MR ){
            En += hit_energy[ihit];
         }

      } // hit loop

      Eeff = En / Etot;

      h_sum_energy->Fill(sum_energy); 

      if(Etot < 1000){
         h_LowHitLen->Fill(nhit_len);
      }else if (1000 < Etot && Etot < 2000){
         h_MidHitLen->Fill(nhit_len);
      }else{
         // cout << jentry << endl;
         h_HighHitLen->Fill(nhit_len);
      }

      bool isHighHit = false;

      for (int ihit = 0; ihit < nhit_len; ihit++)
      {
         // if(hit_energy[ihit]>700) isHighHit = true;
         if(hit_adc_high[ihit] < hit_energy[ihit]) isHighHit = true;

         if(Etot < 1000){
            h_hit_LowEnergy->Fill(hit_energy[ihit]);
         }else if (1000 < Etot && Etot < 2000){
            h_hit_MidEnergy->Fill(hit_energy[ihit]);
         }else{
            h_hit_HighEnergy->Fill(hit_energy[ihit]);
         }
      }

      // if(isHighHit) cout << jentry << endl;

      for (int islab=0; islab < nslabs; islab++) delete h_xy_energy[islab];
      // break;

   } // event loop

   OutFile->cd();
   hList->Write();

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

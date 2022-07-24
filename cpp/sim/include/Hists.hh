#ifndef Hists_h
#define Hists_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class Hists {

public :

   Hists () {};
   virtual ~Hists () {};
   virtual void init();
   virtual void writes(TFile* file);

   // Hist Event
   // TH1F
   TH1F* h_sum_energy;
   TH1F* h_sum_energy_beam;
   TH1F* h_sum_energy_nobeam;

   TH2F* h_sum_energy_slab;
   TH2F* h_sum_energy_slab_beam;
   TH2F* h_sum_energy_slab_nobeam;
   

   // Hist slab
   const static int nslabs = 15;

   // TH1F
   std::array<TH1F*, nslabs> hL_hitrate;

   std::array<TH1F*, nslabs> hL_hg_beam;
   std::array<TH1F*, nslabs> hL_energy_beam;

   // TH2F
   std::array<TH2F*, nslabs> hL_xy_energy;
   std::array<TH2F*, nslabs> hL_xy_energy_beam;
   std::array<TH2F*, nslabs> hL_xy_energy_nobeam;

   std::array<TH2F*, nslabs> hL_xy_hit;
   std::array<TH2F*, nslabs> hL_xy_hit_beam;
   std::array<TH2F*, nslabs> hL_xy_hit_nobeam;

private :
   
   std::vector<TH1F*> _TH1Fvec;
   std::vector<TH2F*> _TH2Fvec;

   std::vector<std::array<TH1F*, nslabs>> _TH1FvecL;
   std::vector<std::array<TH2F*, nslabs>> _TH2FvecL;

};

#endif

void Hists::init()
{

   // Initialize TH1F
   h_sum_energy    = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,250);
   h_sum_energy_beam = new TH1F("h_sum_energy_beam","; sum_energy_beam; Entries",100,0,250);
   h_sum_energy_nobeam = new TH1F("h_sum_energy_nobeam","; sum_energy_nobeam; Entries",100,0,250);

   h_sum_energy_slab = new TH2F("h_sum_energy_slab","; sum_energy / slab; slab",50,0,3,15,0,15);
   h_sum_energy_slab_beam = new TH2F("h_sum_energy_slab_beam","; sum_energy / slab; slab",50,0,3,15,0,15);
   h_sum_energy_slab_nobeam = new TH2F("h_sum_energy_slab_nobeam","; sum_energy / slab; slab",50,0,3,15,0,15);

   _TH1Fvec.push_back(h_sum_energy);
   _TH1Fvec.push_back(h_sum_energy_beam);
   _TH1Fvec.push_back(h_sum_energy_nobeam);

   _TH2Fvec.push_back(h_sum_energy_slab);
   _TH2Fvec.push_back(h_sum_energy_slab_beam);
   _TH2Fvec.push_back(h_sum_energy_slab_nobeam);

   // Initialize TH2F
   for (int islab = 0; islab < nslabs; ++islab)
   {
      TString slab = std::to_string(islab);

      hL_hitrate[islab]      = new TH1F(TString::Format("hL_hitrate_slab%s",slab.Data()),"; Time (s); Hit",3600,0,3600);
      hL_hg_beam[islab]      = new TH1F(TString::Format("hL_hg_beam_slab%s",slab.Data()),"; High Gain; Hit",300,0,3E3);
      hL_energy_beam[islab]  = new TH1F(TString::Format("hL_energy_beam_slab%s",slab.Data()),"; Energy ((HG-Ped)/MIP); Hit",400,0,40);

      hL_xy_energy[islab]        = new TH2F(TString::Format("hL_xy_energy_slab%s",slab.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_energy_beam[islab]   = new TH2F(TString::Format("hL_xy_energy_beam_slab%s",slab.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_energy_nobeam[islab] = new TH2F(TString::Format("hL_xy_energy_nobeam_slab%s",slab.Data()),"; x;y",32,-90,90,32,-90,90);

      hL_xy_energy[islab]->SetMinimum(0);
      hL_xy_energy_beam[islab]->SetMinimum(0);
      hL_xy_energy_nobeam[islab]->SetMinimum(0);

      hL_xy_hit[islab] = new TH2F(TString::Format("hL_xy_hit_slab%s",slab.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_hit_beam[islab] = new TH2F(TString::Format("hL_xy_hit_beam_slab%s",slab.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_hit_nobeam[islab] = new TH2F(TString::Format("hL_xy_hit_nobeam_slab%s",slab.Data()),"; x;y",32,-90,90,32,-90,90);
      
      hL_xy_hit[islab]->SetMinimum(0);
      hL_xy_hit_beam[islab]->SetMinimum(0);
      hL_xy_hit_nobeam[islab]->SetMinimum(0);

   }

   _TH1FvecL.push_back(hL_hitrate);
   
   _TH1FvecL.push_back(hL_hg_beam);
   _TH1FvecL.push_back(hL_energy_beam);

   _TH2FvecL.push_back(hL_xy_energy);
   _TH2FvecL.push_back(hL_xy_energy_beam);
   _TH2FvecL.push_back(hL_xy_energy_nobeam);

   _TH2FvecL.push_back(hL_xy_hit);
   _TH2FvecL.push_back(hL_xy_hit_beam);
   _TH2FvecL.push_back(hL_xy_hit_nobeam);

}

void Hists::writes(TFile* file)
{

   file->cd();

   TDirectory *cdhisto[15];
   for(int islab=0; islab<nslabs; islab++) {
    cdhisto[islab] = file->mkdir(TString::Format("slab_%i",islab));
   }

   for(int h=0; h < _TH1Fvec.size(); h++) _TH1Fvec.at(h)->Write();
   for(int h=0; h < _TH2Fvec.size(); h++) _TH2Fvec.at(h)->Write();

   for (int islab = 0; islab < nslabs; ++islab)
   {
      cdhisto[islab]->cd();

      for(int h=0; h < _TH1FvecL.size(); h++) _TH1FvecL.at(h)[islab]->Write();
      for(int h=0; h < _TH2FvecL.size(); h++) _TH2FvecL.at(h)[islab]->Write();

   }

   file->mkdir("efficiency");
   file->cd("efficiency");
   gDirectory->mkdir("energy/beam");
   gDirectory->mkdir("energy/nobeam");

   gDirectory->cd("/efficiency/energy/beam");

   TH2F * eff_E7_n[15];
   TH2F * eff_E_ref = (TH2F*) hL_xy_energy_beam[7]->Clone();
   eff_E_ref->SetName("eff_E_ref");
   
   for (int islab = 0; islab < 15; islab++){
      eff_E7_n[islab] = (TH2F*) hL_xy_energy_beam[islab]->Clone();
      eff_E7_n[islab]->SetName(TString::Format("eff_E7_%i",islab));
      eff_E7_n[islab]->Divide(eff_E_ref);
      eff_E7_n[islab]->Write();
   }
   
   gDirectory->cd("/efficiency/energy/nobeam");

   TH2F * eff_E_noise7_n[15];
   TH2F * eff_E_noise_ref = (TH2F*) hL_xy_energy_nobeam[7]->Clone();
   eff_E_ref->SetName("eff_E_noise_ref");
   
   for (int islab = 0; islab < 15; islab++){
      eff_E_noise7_n[islab] = (TH2F*) hL_xy_energy_nobeam[islab]->Clone();
      eff_E_noise7_n[islab]->SetName(TString::Format("eff_E_noise7_%i",islab));
      eff_E_noise7_n[islab]->Divide(eff_E_noise_ref);
      eff_E_noise7_n[islab]->Write();
   }

   gDirectory->cd("/efficiency");
   gDirectory->mkdir("hit/beam");
   gDirectory->mkdir("hit/nobeam");

   gDirectory->cd("/efficiency/hit/beam");

   TH2F * eff_hit7_n[15];
   TH2F * eff_hit_ref = (TH2F*) hL_xy_hit_beam[7]->Clone();
   eff_hit_ref->SetName("eff_hit_ref");
   
   for (int islab = 0; islab < 15; islab++){
      eff_hit7_n[islab] = (TH2F*) hL_xy_hit_beam[islab]->Clone();
      eff_hit7_n[islab]->SetName(TString::Format("eff_hit7_%i",islab));
      eff_hit7_n[islab]->Divide(eff_hit_ref);
      eff_hit7_n[islab]->Write();
   }

   gDirectory->cd("/efficiency/hit/nobeam");

   TH2F * eff_hit_noise7_n[15];
   TH2F * eff_hit_noise_ref = (TH2F*) hL_xy_hit_nobeam[7]->Clone();
   eff_hit_noise_ref->SetName("eff_hit_noise_ref");
   
   for (int islab = 0; islab < 15; islab++){
      eff_hit_noise7_n[islab] = (TH2F*) hL_xy_hit_nobeam[islab]->Clone();
      eff_hit_noise7_n[islab]->SetName(TString::Format("eff_hit_noise7_%i",islab));
      eff_hit_noise7_n[islab]->Divide(eff_hit_noise_ref);
      eff_hit_noise7_n[islab]->Write();
   }

}

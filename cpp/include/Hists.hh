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

   TH2F* h_sum_energy_layer;
   TH2F* h_sum_energy_layer_beam;
   TH2F* h_sum_energy_layer_nobeam;
   

   // Hist Layer
   const static int nlayers = 15;

   // TH1F
   std::array<TH1F*, nlayers> hL_hitrate;

   std::array<TH1F*, nlayers> hL_hg_beam;
   std::array<TH1F*, nlayers> hL_energy_beam;

   // TH2F
   std::array<TH2F*, nlayers> hL_xy_energy;
   std::array<TH2F*, nlayers> hL_xy_energy_beam;
   std::array<TH2F*, nlayers> hL_xy_energy_nobeam;

private :
   
   std::vector<TH1F*> _TH1Fvec;
   std::vector<TH2F*> _TH2Fvec;

   std::vector<std::array<TH1F*, nlayers>> _TH1FvecL;
   std::vector<std::array<TH2F*, nlayers>> _TH2FvecL;

};

#endif

void Hists::init()
{

   // Initialize TH1F
   h_sum_energy    = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,250);
   h_sum_energy_beam = new TH1F("h_sum_energy_beam","; sum_energy_beam; Entries",100,0,250);
   h_sum_energy_nobeam = new TH1F("h_sum_energy_nobeam","; sum_energy_nobeam; Entries",100,0,250);

   h_sum_energy_layer = new TH2F("h_sum_energy_layer","; sum_energy / layer; layer",50,0,3,15,0,15);
   h_sum_energy_layer_beam = new TH2F("h_sum_energy_layer_beam","; sum_energy / layer; layer",50,0,3,15,0,15);
   h_sum_energy_layer_nobeam = new TH2F("h_sum_energy_layer_nobeam","; sum_energy / layer; layer",50,0,3,15,0,15);

   _TH1Fvec.push_back(h_sum_energy);
   _TH1Fvec.push_back(h_sum_energy_beam);
   _TH1Fvec.push_back(h_sum_energy_nobeam);

   _TH2Fvec.push_back(h_sum_energy_layer);
   _TH2Fvec.push_back(h_sum_energy_layer_beam);
   _TH2Fvec.push_back(h_sum_energy_layer_nobeam);

   // Initialize TH2F
   for (int ilayer = 0; ilayer < nlayers; ++ilayer)
   {
      TString layer = std::to_string(ilayer);

      hL_hitrate[ilayer]      = new TH1F(TString::Format("hL_hitrate_layer%s",layer.Data()),"; Time (s); Hit",3600,0,3600);
      hL_hg_beam[ilayer]      = new TH1F(TString::Format("hL_hg_beam_layer%s",layer.Data()),"; High Gain; Hit",300,0,3E3);
      hL_energy_beam[ilayer]  = new TH1F(TString::Format("hL_energy_beam_layer%s",layer.Data()),"; Energy ((HG-Ped)/MIP); Hit",400,0,40);

      hL_xy_energy[ilayer] = new TH2F(TString::Format("hL_xy_energy_layer%s",layer.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_energy_beam[ilayer] = new TH2F(TString::Format("hL_xy_energy_beam_layer%s",layer.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_energy_nobeam[ilayer] = new TH2F(TString::Format("hL_xy_energy_nobeam_layer%s",layer.Data()),"; x;y",32,-90,90,32,-90,90);

      hL_xy_energy[ilayer]->SetMinimum(0); 
      hL_xy_energy_beam[ilayer]->SetMinimum(0);
      hL_xy_energy_nobeam[ilayer]->SetMinimum(0);
      
   }

   _TH1FvecL.push_back(hL_hitrate);
   
   _TH1FvecL.push_back(hL_hg_beam);
   _TH1FvecL.push_back(hL_energy_beam);

   _TH2FvecL.push_back(hL_xy_energy);
   _TH2FvecL.push_back(hL_xy_energy_beam);
   _TH2FvecL.push_back(hL_xy_energy_nobeam);

}

void Hists::writes(TFile* file)
{

   file->cd();

   TDirectory *cdhisto[15];
   for(int ilayer=0; ilayer<nlayers; ilayer++) {
    cdhisto[ilayer] = file->mkdir(TString::Format("layer_%i",ilayer));
   }

   for(int h=0; h < _TH1Fvec.size(); h++) _TH1Fvec.at(h)->Write();
   for(int h=0; h < _TH2Fvec.size(); h++) _TH2Fvec.at(h)->Write();

   for (int ilayer = 0; ilayer < nlayers; ++ilayer)
   {
      cdhisto[ilayer]->cd();

      for(int h=0; h < _TH1FvecL.size(); h++) _TH1FvecL.at(h)[ilayer]->Write();
      for(int h=0; h < _TH2FvecL.size(); h++) _TH2FvecL.at(h)[ilayer]->Write();

   }

   TDirectory *DirEff = file->mkdir("eff_beam");
   DirEff->cd();

   TH2F * eff7_n[15];
   TH2F * eff_ref = (TH2F*) hL_xy_energy_beam[7]->Clone();
   eff_ref->SetName("eff_ref");
   
   for (int islab = 0; islab < 15; islab++){
      eff7_n[islab] = (TH2F*) hL_xy_energy_beam[islab]->Clone();
      eff7_n[islab]->SetName(TString::Format("eff7_%i",islab));
      eff7_n[islab]->Divide(eff_ref);
      eff7_n[islab]->Write();
   }

}

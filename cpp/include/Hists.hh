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

   TH2F* h_sum_energy_layer;
   

   // Hist Layer
   const static int nlayers = 15;

   // TH1F
   std::array<TH1F*, nlayers> hL_hitrate;

   std::array<TH1F*, nlayers> hL_hg_beam;
   std::array<TH1F*, nlayers> hL_energy_beam;

   // TH2F
   std::array<TH2F*, nlayers> hL_xy_energy;

   std::array<TH2F*, nlayers> hL_xy_energy_beam;

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
   h_sum_energy    = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,1000);
   h_sum_energy_beam = new TH1F("h_sum_energy_beam","; sum_energy_beam; Entries",100,0,1000);

   h_sum_energy_layer = new TH2F("h_sum_energy_layer","; sum_energy / layer; layer",50,0,3,15,0,15);

   _TH1Fvec.push_back(h_sum_energy);
   _TH1Fvec.push_back(h_sum_energy_beam);

   _TH2Fvec.push_back(h_sum_energy_layer);

   // Initialize TH2F
   for (int ilayer = 0; ilayer < nlayers; ++ilayer)
   {
      TString layer = std::to_string(ilayer);

      hL_hitrate[ilayer]     = new TH1F(TString::Format("hL_hitrate_layer%s",layer.Data()),"; Time (s); Hit",3600,0,3600);
      hL_hg_beam[ilayer]       = new TH1F(TString::Format("hL_hg_beam_layer%s",layer.Data()),"; High Gain; Hit",300,0,3E3);
      hL_energy_beam[ilayer]   = new TH1F(TString::Format("hL_energy_beam_layer%s",layer.Data()),"; Energy ((HG-Ped)/MIP); Hit",400,0,40);

      hL_xy_energy[ilayer] = new TH2F(TString::Format("hL_xy_energy_layer%s",layer.Data()),"; x;y",32,-90,90,32,-90,90);
      hL_xy_energy_beam[ilayer] = new TH2F(TString::Format("hL_xy_energy_beam_layer%s",layer.Data()),"; x;y",32,-90,90,32,-90,90);
   }

   _TH1FvecL.push_back(hL_hitrate);
   
   _TH1FvecL.push_back(hL_hg_beam);
   _TH1FvecL.push_back(hL_energy_beam);

   _TH2FvecL.push_back(hL_xy_energy);
   _TH2FvecL.push_back(hL_xy_energy_beam);

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

}

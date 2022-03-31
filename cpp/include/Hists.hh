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
   

   // Hist Layer
   const static int nlayers = 15;

   // TH1F
   std::array<TH1F*, nlayers> hL_hitrate;

   // TH2F
   std::array<TH2F*, nlayers> hL_channel_energy;

private :
   
   std::vector<TH1F*> _TH1Fvec;

   std::vector<std::array<TH1F*, nlayers>> _TH1FvecL;
   std::vector<std::array<TH2F*, nlayers>> _TH2FvecL;

};

#endif

void Hists::init()
{

   // Initialize TH1F
   h_sum_energy = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,1000);

   _TH1Fvec.push_back(h_sum_energy);

   // Initialize TH2F
   for (int ilayer = 0; ilayer < nlayers; ++ilayer)
   {
      TString layer = std::to_string(ilayer);

      hL_hitrate[ilayer]        = new TH1F(TString::Format("hL_hitrate_layer%s",layer.Data()),"; Time (s); Hit",3600,0,3600);
      hL_channel_energy[ilayer] = new TH2F(TString::Format("hL_channel_energy_layer%s",layer.Data()),"; x;y",32,-90,90,32,-90,90);
   }

   _TH1FvecL.push_back(hL_hitrate);
   _TH2FvecL.push_back(hL_channel_energy);

}

void Hists::writes(TFile* file)
{

   file->cd();

   TDirectory *cdhisto[15];
   for(int ilayer=0; ilayer<nlayers; ilayer++) {
    cdhisto[ilayer] = file->mkdir(TString::Format("layer_%i",ilayer));
   }

   for(int h=0; h < _TH1Fvec.size(); h++) _TH1Fvec.at(h)->Write();
   // for(int h=0; h < _TH2FvecL.size(); h++) _TH2FvecL.at(h)->Write();


   for (int ilayer = 0; ilayer < nlayers; ++ilayer)
   {
      cdhisto[ilayer]->cd();

      for(int h=0; h < _TH1FvecL.size(); h++) _TH1FvecL.at(h)[ilayer]->Write();
      for(int h=0; h < _TH2FvecL.size(); h++) _TH2FvecL.at(h)[ilayer]->Write();

   }

}

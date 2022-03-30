//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar 29 16:00:30 2022 by ROOT version 6.24/06
// from TTree ecal/Build ecal events
// found on file: ../data/3.0GeV_W_run_050282/full_run.root
//////////////////////////////////////////////////////////

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

   // TH1F
   TH1F* h_sum_energy;
   
   // TH2F
   const static int nlayers = 15;
   std::array<TH2F*, nlayers> h_channel_energy;

private :
   
   std::vector<TH1F*> _TH1Fvec;
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
      TString histname = TString::Format("h_channel_energy_layer%s",layer.Data());
      h_channel_energy[ilayer] = new TH2F(histname,"; x;y",32,-90,90,32,-90,90);
   }

   _TH2FvecL.push_back(h_channel_energy);

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

      for(int h=0; h < _TH2FvecL.size(); h++) _TH2FvecL.at(h)[ilayer]->Write();

   }

}

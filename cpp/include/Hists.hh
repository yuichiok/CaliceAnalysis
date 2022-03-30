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
   virtual void writes();

   // TH1F
   TH1F* h_sum_energy;
   
   // TH2F
   const static int nlayers = 15;
   TH2F* h_channel_energy[nlayers];

private :
   
   std::vector<TH1F*> _TH1Fvec;
   std::vector<TH2F*> _TH2Fvec;

};

#endif

void Hists::init()
{

   // Initialize TH1F
   h_sum_energy = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,1000);

   _TH1Fvec.push_back(h_sum_energy);

   // Initialize TH2F
   for (int islab = 0; islab < nlayers; ++islab)
   {
      TString layer = std::to_string(islab);
      TString histname = TString::Format("h_channel_energy_layer%s",layer.Data());
      h_channel_energy[islab] = new TH2F(histname,"; x;y",32,-90,90,32,-90,90);
      _TH2Fvec.push_back(h_channel_energy[islab]);
   }

   // h_channel_energy = new TH2F("h_channel_energy","; ;")

}

void Hists::writes()
{

   for(int h=0; h < _TH1Fvec.size(); h++) _TH1Fvec.at(h)->Write();
   for(int h=0; h < _TH2Fvec.size(); h++) _TH2Fvec.at(h)->Write();

}

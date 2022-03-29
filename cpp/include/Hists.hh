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

   TH1F* h_sum_energy;

private :
   
   std::vector<TH1F*> _TH1Fvec;

};

#endif

void Hists::init()
{

   // TH1F* h_sum_energy = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,1000);
   h_sum_energy = new TH1F("h_sum_energy","; sum_energy; Entries",100,0,1000);

}

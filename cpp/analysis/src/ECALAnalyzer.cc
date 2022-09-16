#include <iostream>
#include <TString.h>
#include <TFile.h> 
#include "../include/ECALAnalyzer.hh"
#include "../include/TreeReader.hh"
#include "../include/FileSelector.hh"
#include "../include/HistManager.hh"

using std::cout;   using std::endl;

ECALAnalyzer::ECALAnalyzer(TString o)
: options(o)
{
  // TEST output
    cout << "[Begin ECALAnalyzer]" << endl;

    patEventsAnalyzed = 0;
    entriesInNtuple   = 0;

}

bool ECALAnalyzer::MapTree(TTree* tree)
{
  // Maps TTree to class' variables.
  // TO DO: Implement check for correct mapping, return result?
  //    - Set up exception handling for negative result.

    entriesInNtuple = tree->GetEntries();

  // Set branch addresses and branch pointers
    if (!tree) return false;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    TreeReader reader;
    _data = _reco;
    reader.InitializeRecoReadTree(fChain, _data, _branch);

    Notify();

    return true;

}

void ECALAnalyzer::Analyze(Long64_t entry)
{

   HistManager hm;
   Float_t sum_slab_energy[NSLABS] = {0};
   
   if(_data.nhit_slab < 13) return;

   hm.h_sum_energy->Fill(_data.sum_energy);

   for (int ihit = 0; ihit < _data.nhit_len; ihit++)
   {
      hm.h_hit_slab->Fill(_data.hit_slab[ihit]);
      hm.h_hit_energy->Fill(_data.hit_energy[ihit]);

      int ihit_slab = _data.hit_slab[ihit];
      sum_slab_energy[ihit_slab] += _data.hit_energy[ihit];
      hm.h_hit_slab_energy[ihit_slab]->Fill(_data.hit_energy[ihit]);
   }

   for (int islab = 0; islab < NSLABS; islab++)
   {
      hm.h_sum_slab_energy[islab]->Fill(sum_slab_energy[islab]);
   }

   FileSelector fs(options);

   TFile *outfile = new TFile( "rootfiles/" + fs.GetRecoSim() + "/" + fs.GetRunName() + "_quality.root","RECREATE");
   cout << "Output: " << outfile << endl;

   hm.WriteLists(outfile);

/*
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();

   TString outpath = "rootfiles/quality/";
   OutFile = new TFile(outpath + OutFileName + "_quality.root","RECREATE");

   TList* hList = new TList();
   TList* hList_energy = new TList();
   TList* hList_energy_sca = new TList();
   TH1F * hm.h_sum_energy = new TH1F("hm.h_sum_energy","; sum_energy; Entries",500,0,1.5E4);
   TH1F * hm.h_hit_energy = new TH1F("hm.h_hit_energy","; hit_energy; Entries",120,-20,100);
   TH1F * hm.h_hit_slab_energy[nslabs];
   for (int islab = 0; islab < nslabs; islab++)
   {
      TString hname = "hm.h_hit_slab_energy" + to_string(islab);
      hm.h_hit_slab_energy[islab] = new TH1F(hname,hname,120,-20,100);
   }
   
   TH1F * hm.h_hit_slab7_energy_sca[nscas];
   for (int isca = 0; isca < nscas; isca++)
   {
      TString hname = "hm.h_hit_slab7_energy_sca" + to_string(isca);
      hm.h_hit_slab7_energy_sca[isca] = new TH1F(hname,hname,120,-20,100);
   }

   TH1F * hm.h_hit_slab   = new TH1F("hm.h_hit_slab","; hit_slab; Entries",nslabs,-0.5,14.5);

   hList->Add(hm.h_sum_energy);
   hList->Add(hm.h_hit_energy);
   for(int ih=0;ih<nslabs;ih++) hList_energy->Add(hm.h_hit_slab_energy[ih]);
   for(int isca=0;isca<nscas;isca++) hList_energy_sca->Add(hm.h_hit_slab7_energy_sca[isca]);
   hList->Add(hm.h_hit_slab);

   if(nhit_slab < 13) continue;

   hm.h_sum_energy->Fill(sum_energy);

   for (int ihit = 0; ihit < nhit_len; ihit++)
   {
      hm.h_hit_slab->Fill(hit_slab[ihit]);
      hm.h_hit_energy->Fill(hit_energy[ihit]);

      hm.h_hit_slab_energy[hit_slab[ihit]]->Fill(hit_energy[ihit]);
      
   }

   TCanvas * c_hit_slab_energy = new TCanvas("c_hit_slab_energy","c_hit_slab_energy",700,700);
   c_hit_slab_energy->Divide(4,4);
   for (int islab = 0; islab < nslabs; islab++)
   {
      c_hit_slab_energy->cd(islab+1);
      hm.h_hit_slab_energy[islab]->Draw("h");
   }

   TCanvas * c_hit_slab7_energy_sca = new TCanvas("c_hit_slab7_energy_sca","c_hit_slab7_energy_sca",700,700);
   c_hit_slab7_energy_sca->Divide(4,4);
   for (int isca = 0; isca < nscas; isca++)
   {
      c_hit_slab7_energy_sca->cd(isca+1);
      hm.h_hit_slab7_energy_sca[isca]->Draw("h");
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
*/

}

Bool_t ECALAnalyzer::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

bool ECALAnalyzer::Select()
{ // Evaluates the class' list of event selection criteria

    return true;

}
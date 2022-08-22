//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Mar 29 16:00:30 2022 by ROOT version 6.24/06
// from TTree ecal/Build ecal events
// found on file: ../data/3.0GeV_W_run_050282/full_run.root
//////////////////////////////////////////////////////////

#ifndef TBDisplay_h
#define TBDisplay_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class TBDisplay {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   Int_t           spill;
   Int_t           cycle;
   Int_t           bcid;
   Int_t           bcid_first_sca_full;
   Int_t           bcid_merge_end;
   Int_t           id_run;
   Int_t           id_dat;
   Int_t           nhit_slab;
   Int_t           nhit_chip;
   Int_t           nhit_chan;
   Int_t           nhit_len;
   Float_t         sum_energy;
   Float_t         sum_energy_lg;
   Int_t           hit_slab[9999];   //[nhit_len]
   Int_t           hit_chip[9999];   //[nhit_len]
   Int_t           hit_chan[9999];   //[nhit_len]
   Int_t           hit_sca[9999];   //[nhit_len]
   Float_t         hit_x[9999];   //[nhit_len]
   Float_t         hit_y[9999];   //[nhit_len]
   Float_t         hit_z[9999];   //[nhit_len]
   Int_t           hit_adc_high[9999];   //[nhit_len]
   Int_t           hit_adc_low[9999];   //[nhit_len]
   Float_t         hit_energy[9999];   //[nhit_len]
   Float_t         hit_energy_lg[9999];   //[nhit_len]
   Int_t           hit_n_scas_filled[9999];   //[nhit_len]
   Int_t           hit_isHit[9999];   //[nhit_len]
   Int_t           hit_isMasked[9999];   //[nhit_len]
   Int_t           hit_isCommissioned[9999];   //[nhit_len]

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_spill;   //!
   TBranch        *b_cycle;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_bcid_first_sca_full;   //!
   TBranch        *b_bcid_merge_end;   //!
   TBranch        *b_id_run;   //!
   TBranch        *b_id_dat;   //!
   TBranch        *b_nhit_slab;   //!
   TBranch        *b_nhit_chip;   //!
   TBranch        *b_nhit_chan;   //!
   TBranch        *b_nhit_len;   //!
   TBranch        *b_sum_energy;   //!
   TBranch        *b_sum_energy_lg;   //!
   TBranch        *b_hit_slab;   //!
   TBranch        *b_hit_chip;   //!
   TBranch        *b_hit_chan;   //!
   TBranch        *b_hit_sca;   //!
   TBranch        *b_hit_x;   //!
   TBranch        *b_hit_y;   //!
   TBranch        *b_hit_z;   //!
   TBranch        *b_hit_adc_high;   //!
   TBranch        *b_hit_adc_low;   //!
   TBranch        *b_hit_energy;   //!
   TBranch        *b_hit_energy_lg;   //!
   TBranch        *b_hit_n_scas_filled;   //!
   TBranch        *b_hit_isHit;   //!
   TBranch        *b_hit_isMasked;   //!
   TBranch        *b_hit_isCommissioned;   //!

   TBDisplay(TString filein_s);
   TBDisplay(TList *f=0);
   virtual ~TBDisplay();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Display();
   virtual float    CycleToSec(int cyc);
   virtual void     Debug(bool debug, Long64_t entry);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   virtual void     Next();
   virtual void     Prev();
   virtual void     GoTo();
   virtual void     DropEvent();
   virtual Bool_t   GotoEvent(Int_t ev);
   virtual TEveCaloLego* MakeCaloLego(TEveCaloData* data, TEveWindowSlot* slot);
   virtual void     MakeViewerScene(TEveWindowSlot* slot, TEveViewer*& v, TEveScene*& s);
   virtual void     LoadHits(TEvePointSet*& ps,int i);

   TEvePointSet  *fHits;
   TEventList *evlist;
   Int_t fMaxEv, fCurEv;
   TCut coin = "nhit_slab >= 13";

   TFile *OutFile;
   TString InFileName;
   TString OutFileName;

   ClassDef(TBDisplay, 0);
};

#endif

#ifdef TBDisplay_cxx

TBDisplay::TBDisplay(TString filein_s) : fChain(0), fMaxEv(-1), fCurEv(-1)
{
   InFileName = filein_s;
   TFile *f = new TFile(InFileName);
   TTree *tree = (TTree*)f->Get("ecal");
   fChain = tree;

   tree->Draw( ">>evlist", coin);
   evlist = (TEventList*)gDirectory->Get("evlist");
   tree->SetEventList(evlist);
   
   fMaxEv = fChain->GetEntries(coin);

   Init(tree);
}

TBDisplay::TBDisplay(TList *f) : fChain(0) 
{
   TIter next(f);
   TSystemFile *file;
   TString fname;
   while((file = (TSystemFile*)next())){
      fname = file->GetName();
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
      TTree *tree=0;
      f->GetObject("ecal",tree);
      Init(tree);
   }
}

TBDisplay::~TBDisplay()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

// TBDisplay::Select(TTree* tree)
// {
//    TEventList *evlist = new TEventList( "elist", "Coincidence Selection" ) ;
//    tree->Draw( ">>evlist", "nhit_slab >= 13");

//    TTree * newtree;
//    newtree->SetEventList(evlist);
//    return newtree;
// }

Int_t TBDisplay::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t TBDisplay::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void TBDisplay::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   // fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("spill", &spill, &b_spill);
   fChain->SetBranchAddress("cycle", &cycle, &b_cycle);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("bcid_first_sca_full", &bcid_first_sca_full, &b_bcid_first_sca_full);
   fChain->SetBranchAddress("bcid_merge_end", &bcid_merge_end, &b_bcid_merge_end);
   fChain->SetBranchAddress("id_run", &id_run, &b_id_run);
   fChain->SetBranchAddress("id_dat", &id_dat, &b_id_dat);
   fChain->SetBranchAddress("nhit_slab", &nhit_slab, &b_nhit_slab);
   fChain->SetBranchAddress("nhit_chip", &nhit_chip, &b_nhit_chip);
   fChain->SetBranchAddress("nhit_chan", &nhit_chan, &b_nhit_chan);
   fChain->SetBranchAddress("nhit_len", &nhit_len, &b_nhit_len);
   fChain->SetBranchAddress("sum_energy", &sum_energy, &b_sum_energy);
   fChain->SetBranchAddress("sum_energy_lg", &sum_energy_lg, &b_sum_energy_lg);
   fChain->SetBranchAddress("hit_slab", hit_slab, &b_hit_slab);
   fChain->SetBranchAddress("hit_chip", hit_chip, &b_hit_chip);
   fChain->SetBranchAddress("hit_chan", hit_chan, &b_hit_chan);
   fChain->SetBranchAddress("hit_sca", hit_sca, &b_hit_sca);
   fChain->SetBranchAddress("hit_x", hit_x, &b_hit_x);
   fChain->SetBranchAddress("hit_y", hit_y, &b_hit_y);
   fChain->SetBranchAddress("hit_z", hit_z, &b_hit_z);
   fChain->SetBranchAddress("hit_adc_high", hit_adc_high, &b_hit_adc_high);
   fChain->SetBranchAddress("hit_adc_low", hit_adc_low, &b_hit_adc_low);
   fChain->SetBranchAddress("hit_energy", hit_energy, &b_hit_energy);
   fChain->SetBranchAddress("hit_energy_lg", hit_energy_lg, &b_hit_energy_lg);
   fChain->SetBranchAddress("hit_n_scas_filled", hit_n_scas_filled, &b_hit_n_scas_filled);
   fChain->SetBranchAddress("hit_isHit", hit_isHit, &b_hit_isHit);
   fChain->SetBranchAddress("hit_isMasked", hit_isMasked, &b_hit_isMasked);
   fChain->SetBranchAddress("hit_isCommissioned", hit_isCommissioned, &b_hit_isCommissioned);
   Notify();
}

Bool_t TBDisplay::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void TBDisplay::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t TBDisplay::Cut(Long64_t entry)
{

   float CutTime     = 300.0;
   float aq_sec      = 0.001;
   float aqdelay_sec = 0.01;
   float spc = aq_sec + aqdelay_sec;

   int ncycles = (int)(CutTime / spc);

   if(cycle < ncycles) {
      return -1;
   }else{
      return 1;
   }
   
}
#endif // #ifdef TBDisplay_cxx

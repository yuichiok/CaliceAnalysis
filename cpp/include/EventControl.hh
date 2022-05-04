//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Feb 22 15:54:16 2022 by ROOT version 6.18/00
// from TTree siwecaldecoded/siwecaldecoded
// found on file: /home/calice/Work/TBData/rootfiles/3GeVMIPscan_run_050043_merged.root
//////////////////////////////////////////////////////////

#ifndef EventControl_h
#define EventControl_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include "conf_struct.h"

using std::cout;
using std::endl;

// Header file for the classes stored in the TTree if any.

class EventControl {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           event;
   Int_t           acqNumber;
   Int_t           n_slboards;
   Int_t           slot[15];
   Int_t           slboard_id[15];
   Int_t           chipid[15][16];
   Int_t           nColumns[15][16];
   Float_t         startACQ[15];
   Int_t           rawTSD[15];
   Float_t         TSD[15];
   Int_t           rawAVDD0[15];
   Int_t           rawAVDD1[15];
   Float_t         AVDD0[15];
   Float_t         AVDD1[15];
   Int_t           bcid[15][16][15];
   Int_t           corrected_bcid[15][16][15];
   Int_t           badbcid[15][16][15];
   Int_t           nhits[15][16][15];
   Int_t           charge_lowGain[15][16][15][64];
   Int_t           charge_hiGain[15][16][15][64];
   Int_t           gain_hit_low[15][16][15][64];
   Int_t           gain_hit_high[15][16][15][64];

   // List of branches
   TBranch        *b_event;   //!
   TBranch        *b_acqNumber;   //!
   TBranch        *b_n_slboards;   //!
   TBranch        *b_slot;   //!
   TBranch        *b_slboard_id;   //!
   TBranch        *b_chipid;   //!
   TBranch        *b_nColumns;   //!
   TBranch        *b_startACQ;   //!
   TBranch        *b_rawTSD;   //!
   TBranch        *b_TSD;   //!
   TBranch        *b_rawAVDD0;   //!
   TBranch        *b_rawAVDD1;   //!
   TBranch        *b_AVDD0;   //!
   TBranch        *b_AVDD1;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_corrected_bcid;   //!
   TBranch        *b_badbcid;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_lowGain;   //!
   TBranch        *b_highGain;   //!
   TBranch        *b_gain_hit_low;   //!
   TBranch        *b_gain_hit_high;   //!

   EventControl(TString tree_s);
   EventControl(TList *f=0);

   virtual ~EventControl();
   virtual void     ProcessEvents(bool);
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EventControl_cxx
EventControl::EventControl(TString tree_s) : fChain(0) 
{
   TFile *f = new TFile(tree_s);
   TTree *tree = (TTree*)f->Get("siwecaldecoded");
   //  tree->Print();
   Init(tree);
}

EventControl::EventControl(TList *f) : fChain(0) 
{
   // if parameter tree is not specified (or zero), use a list of of files provided as input

   TIter next(f);
   TSystemFile *file;
   TString fname;
   while((file = (TSystemFile*)next())){
      fname = file->GetName();
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(fname);
      TTree *tree=0;
      f->GetObject("siwecaldecoded",tree);
      Init(tree);
   }
}

EventControl::~EventControl()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EventControl::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EventControl::LoadTree(Long64_t entry)
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

void EventControl::Init(TTree *tree)
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
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("acqNumber", &acqNumber, &b_acqNumber);
   fChain->SetBranchAddress("n_slboards", &n_slboards, &b_n_slboards);
   fChain->SetBranchAddress("slot", slot, &b_slot);
   fChain->SetBranchAddress("slboard_id", slboard_id, &b_slboard_id);
   fChain->SetBranchAddress("chipid", chipid, &b_chipid);
   fChain->SetBranchAddress("nColumns", nColumns, &b_nColumns);
   fChain->SetBranchAddress("startACQ", startACQ, &b_startACQ);
   fChain->SetBranchAddress("rawTSD", rawTSD, &b_rawTSD);
   fChain->SetBranchAddress("TSD", TSD, &b_TSD);
   fChain->SetBranchAddress("rawAVDD0", rawAVDD0, &b_rawAVDD0);
   fChain->SetBranchAddress("rawAVDD1", rawAVDD1, &b_rawAVDD1);
   fChain->SetBranchAddress("AVDD0", AVDD0, &b_AVDD0);
   fChain->SetBranchAddress("AVDD1", AVDD1, &b_AVDD1);
   fChain->SetBranchAddress("bcid", bcid, &b_bcid);
   fChain->SetBranchAddress("corrected_bcid", corrected_bcid, &b_corrected_bcid);
   fChain->SetBranchAddress("badbcid", badbcid, &b_badbcid);
   fChain->SetBranchAddress("nhits", nhits, &b_nhits);
   fChain->SetBranchAddress("charge_lowGain", charge_lowGain, &b_lowGain);
   fChain->SetBranchAddress("charge_hiGain", charge_hiGain, &b_highGain);
   fChain->SetBranchAddress("gain_hit_low", gain_hit_low, &b_gain_hit_low);
   fChain->SetBranchAddress("gain_hit_high", gain_hit_high, &b_gain_hit_high);
   Notify();
}

Bool_t EventControl::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EventControl::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EventControl::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EventControl_cxx

#define TBDisplay_cxx
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

#include <TEveManager.h>
#include <TEveEventManager.h>
#include <TEveVSD.h>
#include <TEveVSDStructs.h>

#include <TEveTrack.h>
#include <TEveTrackPropagator.h>
#include <TEveGeoShape.h>

#include <TGTab.h>
#include <TGButton.h>

#include <TFile.h>
#include <TKey.h>
#include <TSystem.h>
#include <TPRegexp.h>

#include <algorithm> // for std::find
#include <iterator> // for std::begin, std::end
#include <string>
#include <map>

#include "../include/TBDisplay.hh"

#include "../MultiView.C"
MultiView* gMultiView = 0;

using std::cout;
using std::endl;

const bool debug = false;
const int nslabs = 15;
const int nscas = 15;
const float beamX = 20.0, beamY = 15.0;



class TVSDReader
{
public:
   // ----------------------------------------------------------
   // File / Event Data
   // ----------------------------------------------------------

   TFile      *fFile;
   TDirectory *fDirectory;

   TObjArray  *fEvDirKeys;

   TEveVSD    *fVSD;

   Int_t       fMaxEv, fCurEv;

   // ----------------------------------------------------------
   // Event visualization structures
   // ----------------------------------------------------------

   TEveTrackList *fTrackList;
   TEvePointSet  *fITSClusters;
   TEvePointSet  *fTPCClusters;
   TEvePointSet  *fTRDClusters;
   TEvePointSet  *fTOFClusters;

public:
   TVSDReader(const char* file_name) :
      fFile(0), fDirectory(0), fEvDirKeys(0),
      fVSD(0),

      fMaxEv(-1), fCurEv(-1),

      fTrackList(0),
      fITSClusters(0), fTPCClusters(0), fTRDClusters(0), fTOFClusters(0)
   {
      fFile = TFile::Open(file_name);
      if (!fFile)
      {
         Error("VSD_Reader", "Can not open file '%s' ... terminating.",
               file_name);
         gSystem->Exit(1);
      }

      fEvDirKeys = new TObjArray;
      TPMERegexp name_re("ecal");
      TObjLink* lnk = fFile->GetListOfKeys()->FirstLink();
      while (lnk) {
         if (name_re.Match(lnk->GetObject()->GetName()))
         {
            fEvDirKeys->Add(lnk->GetObject());
         }
         lnk = lnk->Next();
      }

      fMaxEv = fEvDirKeys->GetEntriesFast();
      if (fMaxEv == 0) {
         Error("VSD_Reader", "No events to show ... terminating.");
         gSystem->Exit(1);
      }

      fVSD = new TEveVSD;
   }

   virtual ~TVSDReader()
   {
      // Destructor.

      DropEvent();

      delete fVSD;
      delete fEvDirKeys;

      fFile->Close();
      delete fFile;
   }

   void AttachEvent()
   {
      // Attach event data from current directory.

      fVSD->LoadTrees();
      fVSD->SetBranchAddresses();
   }

   void DropEvent()
   {
      // Drup currently held event data, release current directory.

      // Drop old visualization structures.

      gEve->GetViewers()->DeleteAnnotations();
      gEve->GetCurrentEvent()->DestroyElements();

      // Drop old event-data.

      fVSD->DeleteTrees();
      delete fDirectory;
      fDirectory = 0;
   }

   //---------------------------------------------------------------------------
   // Event navigation
   //---------------------------------------------------------------------------

   void NextEvent()
   {
      GotoEvent(fCurEv + 1);
   }

   void PrevEvent()
   {
      GotoEvent(fCurEv - 1);
   }

   Bool_t GotoEvent(Int_t ev)
   {
      if (ev < 0 || ev >= fMaxEv)
      {
         Warning("GotoEvent", "Invalid event id %d.", ev);
         return kFALSE;
      }

      DropEvent();

      // Connect to new event-data.

      fCurEv = ev;
      fDirectory = (TDirectory*) ((TKey*) fEvDirKeys->At(fCurEv))->ReadObj();
      fVSD->SetDirectory(fDirectory);

      AttachEvent();

      // Load event data into visualization structures.

      LoadClusters(fITSClusters, "ITS", 0);
      LoadClusters(fTPCClusters, "TPC", 1);
      LoadClusters(fTRDClusters, "TRD", 2);
      LoadClusters(fTOFClusters, "TOF", 3);

      LoadEsdTracks();

      // Fill projected views.

      auto top = gEve->GetCurrentEvent();

      gMultiView->DestroyEventRPhi();
      gMultiView->ImportEventRPhi(top);

      gMultiView->DestroyEventRhoZ();
      gMultiView->ImportEventRhoZ(top);

      gEve->Redraw3D(kFALSE, kTRUE);

      return kTRUE;
   }


   //---------------------------------------------------------------------------
   // Cluster loading
   //---------------------------------------------------------------------------

   void LoadClusters(TEvePointSet*& ps, const TString& det_name, Int_t det_id)
   {
      if (ps == 0) {
         ps = new TEvePointSet(det_name);
         ps->SetMainColor((Color_t)(det_id + 2));
         ps->SetMarkerSize(0.5);
         ps->SetMarkerStyle(2);
         ps->IncDenyDestroy();
      } else {
         ps->Reset();
      }

      TEvePointSelector ss(fVSD->fTreeC, ps, "fV.fX:fV.fY:fV.fZ",
                           TString::Format("fDetId==%d", det_id));
      ss.Select();
      ps->SetTitle(TString::Format("N=%d", ps->Size()));

      gEve->AddElement(ps);
   }


   //---------------------------------------------------------------------------
   // Track loading
   //---------------------------------------------------------------------------

   enum ESDTrackFlags
   {
      kITSin=0x0001,kITSout=0x0002,kITSrefit=0x0004,kITSpid=0x0008,
      kTPCin=0x0010,kTPCout=0x0020,kTPCrefit=0x0040,kTPCpid=0x0080,
      kTRDin=0x0100,kTRDout=0x0200,kTRDrefit=0x0400,kTRDpid=0x0800,
      kTOFin=0x1000,kTOFout=0x2000,kTOFrefit=0x4000,kTOFpid=0x8000,
      kHMPIDpid=0x20000,
      kEMCALmatch=0x40000,
      kTRDbackup=0x80000,
      kTRDStop=0x20000000,
      kESDpid=0x40000000,
      kTIME=0x80000000
   };

   Bool_t trackIsOn(TEveTrack* t, Int_t mask)
   {
      // Check is track-flag specified by mask are set.

      return (t->GetStatus() & mask) > 0;
   }

   void LoadEsdTracks()
   {
      // Read reconstructed tracks from current event.

      if (fTrackList == 0) {
         fTrackList = new TEveTrackList("ESD Tracks");
         fTrackList->SetMainColor(6);
         fTrackList->SetMarkerColor(kYellow);
         fTrackList->SetMarkerStyle(4);
         fTrackList->SetMarkerSize(0.5);

         fTrackList->IncDenyDestroy();
      } else {
         fTrackList->DestroyElements();
      }

      auto trkProp = fTrackList->GetPropagator();
      // !!!! Need to store field on file !!!!
      // Can store TEveMagField ?
      trkProp->SetMagField(0.5);
      trkProp->SetStepper(TEveTrackPropagator::kRungeKutta);

      Int_t nTracks = fVSD->fTreeR->GetEntries();
      for (Int_t n = 0; n < nTracks; ++n) {
         fVSD->fTreeR->GetEntry(n);

         TEveTrack* track = new TEveTrack(&fVSD->fR, trkProp);
         track->SetName(Form("ESD Track %d", fVSD->fR.fIndex));
         track->SetStdTitle();
         track->SetAttLineAttMarker(fTrackList);
         fTrackList->AddElement(track);
      }

      fTrackList->MakeTracks();

      gEve->AddElement(fTrackList);
   }

   ClassDef(TVSDReader, 0);
};

TVSDReader* gVSDReader = 0;


//______________________________________________________________________________
void make_gui()
{
   // Create minimal GUI for event navigation.

   auto browser = gEve->GetBrowser();
   browser->StartEmbedding(TRootBrowser::kLeft);

   auto frmMain = new TGMainFrame(gClient->GetRoot(), 1000, 600);
   frmMain->SetWindowName("XX GUI");
   frmMain->SetCleanup(kDeepCleanup);

   auto hf = new TGHorizontalFrame(frmMain);
   {
      TString icondir(TString::Format("%s/icons/", gSystem->Getenv("ROOTSYS")));
      TGPictureButton* b = 0;

      // b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoBack.gif"));
      // hf->AddFrame(b);
      // b->Connect("Clicked()", "TVSDReader", gVSDReader, "PrevEvent()");

      // b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoForward.gif"));
      // hf->AddFrame(b);
      // b->Connect("Clicked()", "TVSDReader", gVSDReader, "NextEvent()");
   }
   frmMain->AddFrame(hf);

   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

   browser->StopEmbedding();
   browser->SetTabTitle("Event Control", 0);
}

void TBDisplay::Display()
{

   TFile::SetCacheFileDir(".");

   TEveVSD::DisableTObjectStreamersForVSDStruct();

   gVSDReader = new TVSDReader(InFileName);


   gStyle->SetOptStat(0);

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntriesFast();
   OutFile = new TFile("Display.root","RECREATE");

   Long64_t nbytes = 0, nb = 0;
   int ievent = 0;
   int cnt_event = 0;


   // TCanvas * c = new TCanvas("c","c",800,800);

   // TVSDReader* gVSDReader = 0;

   // Final stuff
   //=============

   gEve->GetViewers()->SwitchColorSet();
   gEve->GetDefaultGLViewer()->SetStyle(TGLRnrCtx::kOutline);

   gEve->GetBrowser()->GetTabRight()->SetTab(1);

   make_gui();

   gEve->AddEvent(new TEveEventManager("Event", "ALICE VSD Event"));

   // gVSDReader->GotoEvent(0);

   gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.



/*
   while( cin >> ievent ){

      TH3F * h_display    = new TH3F("h_display",";z;x;y",285,0.0,285.0,32,-90.0,90.0,32,-90.0,90.0);

      for (int jentry=0; jentry<nentries;jentry++) {

         Long64_t ientry = LoadTree(jentry);
         if (ientry < 0) break;
         nb = fChain->GetEntry(jentry);
         nbytes += nb;

         if(nhit_slab < 13) continue;
         cnt_event++;

         if(cnt_event==ievent){
            for (int ihit = 0; ihit < nhit_len; ihit++)
            {
               h_display->Fill(hit_z[ihit],hit_x[ihit],hit_y[ihit],hit_energy[ihit]);

            } // hit loop
            break;
         }

      }

      h_display->Draw("lego");
      c->Draw();
      c->Modified();
      c->Update();
      gSystem->ProcessEvents();
      delete h_display;


   };
*/

   cout << "Done.\n";

}

float TBDisplay::CycleToSec(int cyc=-1)
{
   float aq_sec      = 0.001;
   float aqdelay_sec = 0.01;
   float spc = aq_sec + aqdelay_sec;
   float time_passed = spc * cyc;

   return time_passed;

}

void TBDisplay::Debug(bool debug=false, Long64_t entry=0)
{

   if(!debug) return;
   else if( !(entry % 10000) ) return;

   for (int ihit = 0; ihit < nhit_len; ++ihit)
   {

      cout << hit_sca[ihit] << " " ;

   }

   cout << endl;
   
}

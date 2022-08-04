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
#include <TEventList.h>

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

Bool_t TBDisplay::GotoEvent(Int_t ev)
{
   Long64_t nb = 0;
   if (fChain == 0) return kFALSE;

   // Long64_t nentries = fChain->GetEntriesFast();
   Long64_t nentries = fChain->GetEntries(coin);

   if (ev < 0 || ev >= nentries)
   {
      Warning("GotoEvent", "Invalid event id %d.", ev);
      return kFALSE;
   }

   gEve->GetViewers()->DeleteAnnotations();
   gEve->GetCurrentEvent()->DestroyElements();


   Long64_t ientry = LoadTree( evlist->GetEntry(ev) );
   if (ientry == 0) return kFALSE;

   nb = fChain->GetEntry(evlist->GetEntry(ev));




   // Load event data into visualization structures.

   LoadHits(fHits);
   // LoadHitArray(fHitsArray);

   // Fill projected views.

   // auto top = gEve->GetCurrentEvent();

   // gMultiView->DestroyEventRPhi();
   // gMultiView->ImportEventRPhi(top);

   // gMultiView->DestroyEventRhoZ();
   // gMultiView->ImportEventRhoZ(top);

   gEve->Redraw3D(kFALSE, kTRUE);

   return kTRUE;
}

void TBDisplay::LoadHits(TEvePointSet*& ps)
{
   // if (ps == 0) {
   //    ps = new TEvePointSet("Hit");
   //    ps->SetMainColor(kRed);
   //    ps->SetMarkerSize(0.5);
   //    ps->SetMarkerStyle(2);
   //    ps->IncDenyDestroy();
   // } else {
   //    ps->Reset();
   // }

   ps = new TEvePointSet("Hit");
   ps->SetOwnIds(kTRUE);
   ps->SetMainColor(kRed);
   ps->SetMarkerSize(0.5);
   ps->SetMarkerStyle(2);
   ps->IncDenyDestroy();

   ps->SetNextPoint(10,10,10);
   ps->SetPointId(new TNamed(Form("Point %d", 0), ""));
   ps->SetNextPoint(15,15,15);
   ps->SetPointId(new TNamed(Form("Point %d", 1), ""));
   // if (!gRandom) gRandom = new TRandom(0);
   // TRandom& r= *gRandom;
   // Float_t s = 100;
   // for(Int_t i = 0; i<10; i++)
   // {
   //    ps->SetNextPoint(r.Uniform(-s,s), r.Uniform(-s,s), r.Uniform(-s,s));
   //    ps->SetPointId(new TNamed(Form("Point %d", i), ""));
   // }

   // TEvePointSelector ss(fChain, ps, "hit_x:hit_y:hit_z");
   // ss.Select("nhit_slab >= 13");
   // ps->SetTitle("Adc count?");

   gEve->AddElement(ps);
}

void TBDisplay::Display()
{
   Long64_t nbytes = 0, nb = 0;
   int ievent = 0;
   int cnt_event = 0;

   // TCanvas * c = new TCanvas("c","c",800,800);


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

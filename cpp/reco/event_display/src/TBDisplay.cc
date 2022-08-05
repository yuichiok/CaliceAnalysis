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


   TH2F *ecalHist2 = new TH2F("ecalHist2","ECAL",32,-90,90,32,-90,90);
   TH3F *ecalHist3 = new TH3F("ecalHist3","ECAL",32,-90,90,32,-90,90,210,0,210);

   Long64_t ientry = LoadTree( evlist->GetEntry(ev) );
   if (ientry == 0) return kFALSE;

   nb = fChain->GetEntry(evlist->GetEntry(ev));

   // Load event data into visualization structures.

   for (int ihit=0; ihit<nhit_len; ihit++){
      // LoadHits(fHits,ihit);
      // if(hit_slab[ihit]==3) ecalHist2->Fill(hit_x[ihit],hit_y[ihit],hit_adc_high[ihit]);
      ecalHist3->Fill(hit_x[ihit],hit_y[ihit],hit_z[ihit],hit_adc_high[ihit]);
   }

   auto data = new TEvePlot3D("EvePlot - TH3F");
   // ecalHist3->SetFillColor(2);
   data->SetPlot(ecalHist3, "glcolz");
   gEve->AddElement(data);

   // auto data = new TEveCaloDataHist();
   // data->AddHistogram(ecalHist2);
   // data->RefSliceInfo(0).Setup("ECAL", 0.3, kBlue);
   // data->IncDenyDestroy();
   // gEve->AddToListTree(data, kFALSE);

   // auto lego = MakeCaloLego(data, 0);

   gEve->Redraw3D(kFALSE, kTRUE);

   return kTRUE;
}

TEveCaloLego* TBDisplay::MakeCaloLego(TEveCaloData* data, TEveWindowSlot* slot)
{
   // Eta-phi lego view.

   TEveViewer* v;
   TEveScene* s;
   if (slot) {
      MakeViewerScene(slot, v, s);
   } else {
      v = gEve->GetDefaultViewer();
      s = gEve->GetEventScene();
   }
   v->SetElementName("Viewer - Lego");
   s->SetElementName("Scene - Lego");

   auto lego = new TEveCaloLego(data);
   s->AddElement(lego);

   // By the default lego extends is (1x1x1). Resize it to put in 'natural'
   // coordinates, so that y extend in 2*Pi and set height of lego two times
   //  smaller than y extend to have better view in 3D perspective.
   // lego->InitMainTrans();
   // lego->RefMainTrans().SetScale(TMath::TwoPi(), TMath::TwoPi(), TMath::Pi());

   // draws scales and axis on borders of window
   // auto glv = v->GetGLViewer();
   // TEveCaloLegoOverlay* overlay = new TEveCaloLegoOverlay();
   // glv->AddOverlayElement(overlay);
   // overlay->SetCaloLego(lego);

   // set event handler to move from perspective to orthographic view.
/*
   glv->SetCurrentCamera(TGLViewer::kCameraOrthoXOY);
   glv->SetEventHandler
      (new TEveLegoEventHandler(glv->GetGLWidget(), glv, lego));
   gEve->AddToListTree(lego, kTRUE);
*/

   return lego;
}

//______________________________________________________________________________
void TBDisplay::MakeViewerScene(TEveWindowSlot* slot, TEveViewer*& v, TEveScene*& s)
{
   // Create a scene and a viewer in the given slot.

   v = new TEveViewer("Viewer");
   v->SpawnGLViewer(gEve->GetEditor());
   slot->ReplaceWindow(v);
   gEve->GetViewers()->AddElement(v);
   s = gEve->SpawnNewScene("Scene");
   v->AddScene(s);
}

void TBDisplay::LoadHits(TEvePointSet*& ps, int i)
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
   ps->SetMarkerSize(2.5);
   ps->SetMarkerStyle(54);
   ps->IncDenyDestroy();

   ps->SetNextPoint(hit_x[i],hit_y[i],hit_z[i]);
   ps->SetPointId(new TNamed(Form("Point %d", i), ""));

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

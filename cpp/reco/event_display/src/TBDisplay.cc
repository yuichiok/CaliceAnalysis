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


void TBDisplay::Next()
{
   GotoEvent(fCurEv + 1);
}

void TBDisplay::Prev()
{
   GotoEvent(fCurEv - 1);
}

Bool_t TBDisplay::GotoEvent(Int_t ev)
{
   Long64_t nb = 0;
   if (fChain == 0) return kFALSE;

   Int_t nentries = fMaxEv;

   if (ev < 0 || ev >= nentries)
   {
      Warning("GotoEvent", "Invalid event id %d.", ev);
      return kFALSE;
   }

   gEve->GetViewers()->DeleteAnnotations();
   gEve->GetCurrentEvent()->DestroyElements();

   fCurEv = ev;

   Long64_t ientry = LoadTree( evlist->GetEntry(ev) );
   if (ientry == 0) return kFALSE;

   nb = fChain->GetEntry(evlist->GetEntry(ev));

   // Load event data into visualization structures.

   for (int ihit=0; ihit<nhit_len; ihit++){
      LoadHits(fHits,ihit);
      // if(hit_slab[ihit]==3) ecalHist2->Fill(hit_x[ihit],hit_y[ihit],hit_adc_high[ihit]);
      // ecalHist3->Fill(hit_x[ihit],hit_y[ihit],hit_z[ihit],hit_adc_high[ihit]);
   }

   for (int i = 0; i < 10; i++)
   {
      TEvePointSet* ps = new TEvePointSet("Hit");
      ps->SetOwnIds(kTRUE);
      ps->SetMarkerSize(5);
      ps->SetMarkerStyle(54);
      ps->IncDenyDestroy();

      ps->SetNextPoint(-95,-95,i * 10);
      ps->SetMainColor(TColor::GetColorPalette
                     (i*50.0));
      ps->SetPointId(new TNamed(Form("Point %d", nhit_len + i), ""));
      ps->SetTitle(TString::Format("Palette=%f", (float)i * 50.0));

      gEve->AddElement(ps);

   }
   

   // auto data = new TEvePlot3D("EvePlot - TH3F");
   // ecalHist3->SetFillColor(2);
   // data->SetPlot(ecalHist3, "box2");
   // gEve->AddElement(data);

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
   ps = new TEvePointSet("Hit");
   ps->SetOwnIds(kTRUE);
   ps->SetMarkerSize(5);
   ps->SetMarkerStyle(54);
   ps->IncDenyDestroy();

   ps->SetNextPoint(hit_x[i],hit_y[i],hit_z[i]);
   // ps->SetMarketColor(TColor::GetColorPalette
   //                  (hit_adc_high[i]));
   ps->SetMainColor(TColor::GetColorPalette
                    (hit_adc_high[i]));
   ps->SetPointId(new TNamed(Form("Point %d", i), ""));
   ps->SetTitle(TString::Format("hit_adc_high=%i", hit_adc_high[i]));

   gEve->AddElement(ps);
}

void TBDisplay::Display()
{
   Long64_t nbytes = 0, nb = 0;
   int ievent = 0;
   int cnt_event = 0;

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

#define TBDisplay_cxx
#define Fit3D_cxx

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
#include "../include/Fit3D.hh"

#include "../MultiView.C"
MultiView* gMultiView = 0;

using std::cout;
using std::endl;

const bool debug = false;
const int nslabs = 15;
const int nscas = 15;
const float beamX = 20.0, beamY = 15.0;

// define the parametric line equation
void line(double t, const double *p, double &x, double &y, double &z) {
   // a parametric line is define from 6 parameters but 4 are independent
   // x0,y0,z0,z1,y1,z1 which are the coordinates of two points on the line
   // can choose z0 = 0 if line not parallel to x-y plane and z1 = 1;
   x = p[0] + p[1]*t;
   y = p[2] + p[3]*t;
   z = t;
}

void TBDisplay::Next()
{
   GotoEvent(fCurEv + 1);
}

void TBDisplay::Prev()
{
   GotoEvent(fCurEv - 1);
}

void TBDisplay::GoTo() {

	int goto_eventNum;
	cout << "Go To: ";
	cin >> goto_eventNum;
	cout << endl;

   GotoEvent(goto_eventNum);

}

void TBDisplay::DropEvent()
{
   gEve->GetViewers()->DeleteAnnotations();
   gEve->GetCurrentEvent()->DestroyElements();
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

   DropEvent();

   cout << endl;
   cout << "Going to " << ev << "..." << endl;
   cout << endl;

   TGraph2D *gr = new TGraph2D();
   fCurEv = ev;

   Long64_t ientry = LoadTree( evlist->GetEntry(ev) );
   if (ientry == 0) return kFALSE;

   nb = fChain->GetEntry(evlist->GetEntry(ev));

   float Xcm[nslabs] = {0};
   float Ycm[nslabs] = {0};
   float Zcm[nslabs] = {0};
   float Wsum[nslabs] = {0};

   // Load event data into visualization structures.
   for (int ihit=0; ihit<nhit_len; ihit++){
      
      LoadHits(fHits,ihit);

      for (int islab = 0; islab < nslabs; islab++)
      {
         if (hit_slab[ihit]==islab)
         {
            // Xcm[islab]  += hit_x[ihit] * hit_energy[ihit];
            // Ycm[islab]  += hit_y[ihit] * hit_energy[ihit];
            Xcm[islab]  += hit_x[ihit] * hit_adc_high[ihit];
            Ycm[islab]  += hit_y[ihit] * hit_adc_high[ihit];
            Zcm[islab]  =  hit_z[ihit];
            Wsum[islab] += hit_adc_high[ihit];
         }

      } // match slab

   } // hit loop

   int N = 0;
   for (int islab = 0; islab < nslabs; islab++)
   {
      if(Xcm[islab]==0 || Ycm[islab]==0) continue;

      Xcm[islab] = Xcm[islab] / Wsum[islab];
      Ycm[islab] = Ycm[islab] / Wsum[islab];

      gr->SetPoint(N,Xcm[islab],Ycm[islab],Zcm[islab]);
      N++;

   } // match slab

   /*
   ROOT::Fit::Fitter  fitter;

   // make the functor objet
   Fit3D sdist(gr);
   ROOT::Math::Functor fcn(sdist,4);
   // set the function and the initial parameter values
   double pStart[4] = {1,1,1,1};
   fitter.SetFCN(fcn,pStart);
   // set step sizes different than default ones (0.3 times parameter values)
   for (int i = 0; i < 4; ++i) fitter.Config().ParSettings(i).SetStepSize(0.01);
 
   bool ok = fitter.FitFCN();
   if (!ok) {
      Error("line3Dfit","Line3D Fit failed");
      return false;
   }

   const ROOT::Fit::FitResult & result = fitter.Result();
 
   std::cout << "Total final distance square " << result.MinFcnValue() << std::endl;
   result.Print(std::cout);
 
   gr->Draw("p0");
   */

   // color scale
   /*
   for (int i = 0; i < 10; i++)
   {
      TEvePointSet* ps = new TEvePointSet("Hit");
      ps->SetOwnIds(kTRUE);
      ps->SetMarkerSize(3.5);
      ps->SetMarkerStyle(54);
      ps->IncDenyDestroy();

      ps->SetNextPoint(-95,-95,i * 10);
      ps->SetMainColor(TColor::GetColorPalette
                     (i*50.0));
      ps->SetPointId(new TNamed(Form("Point %d", nhit_len + i), ""));
      ps->SetTitle(TString::Format("Palette=%f", (float)i * 50.0));

      gEve->AddElement(ps);

   }
   */
   

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
   ps->SetMarkerSize(3.5);
   ps->SetMarkerStyle(54);
   ps->IncDenyDestroy();

   ps->SetNextPoint(hit_x[i],hit_y[i],hit_z[i]);
   ps->SetMainColor(TColor::GetColorPalette
                    (hit_adc_high[i]));
   if(hit_adc_high[i] < hit_energy[i]) ps->SetMainColor(kRed);
   // if(hit_energy[i] > 700) ps->SetMainColor(kRed);
   // ps->SetMainColor(TColor::GetColorPalette
   //                  (hit_energy[i]));
   ps->SetPointId(new TNamed(Form("Point %d", i), ""));
   ps->SetTitle(TString::Format("hit_adc_high=%i\n hit_energy=%f\n hit_isCommissioned=%i\n hit_isHit=%i\n (%i,%i,%i,%i)",
                                 hit_adc_high[i],
                                 hit_energy[i],
                                 hit_isCommissioned[i],
                                 hit_isHit[i],
                                 hit_slab[i], hit_chip[i], hit_chan[i], hit_sca[i]));
   // ps->SetTitle(TString::Format("hit_energy=%f", hit_energy[i]));

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

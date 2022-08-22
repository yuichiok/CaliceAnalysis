#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "src/TBDisplay.cc"
// #include "MultiView.C"

TBDisplay *gDisplay = 0;

void make_gui();

void run(int set_ene = 10, string particle = "e"){

	// TString name = particle + TString(to_string(set_ene));
	string name = particle + "." + to_string(set_ene);
	cout << name << endl;

	std::map<std::string, std::string> run_list {
		{"e.10", "320"},
		{"e.20", "378"},
		{"e.40", "375"},
		{"e.60", "372"},
		{"e.80", "367"},
		{"e.100", "365"},
		{"e.150", "355"},
	};

	TString filein = "default";
	string fileinpath = "../../../data/reco/raw_siwecal_90";
	filein = fileinpath + run_list[name] + "/full_run.root";

	cout << "Input: " << filein << endl; 

	// TBDisplay *gDisplay = new TBDisplay(filein);
	gDisplay = new TBDisplay(filein);

	TFile::SetCacheFileDir(".");

	TEveManager::Create();

	// TEveGeoShape *SiWECAL = new TEveGeoShape;
	// SiWECAL->SetShape(new TGeoBBox(1.760000000e+02, 1.760000000e+02, 2.850000000e+02)); // dx, dy, dz
	// SiWECAL->SetNSegments(100); // number of vertices
	// SiWECAL->SetMainColor(kGreen);
	// SiWECAL->SetMainAlpha(0.2);
	// SiWECAL->RefMainTrans().SetPos(0, 0, 0); // set position
	// gEve->AddGlobalElement(SiWECAL);

	gStyle->SetOptStat(0);

	// gEve->GetViewers()->SwitchColorSet();

	TGLViewer *tglv = gEve->GetDefaultGLViewer();
	tglv->SetGuideState(TGLUtil::kAxesEdge, kTRUE, kFALSE, 0);
	tglv->SetStyle(TGLRnrCtx::kOutline);

	gEve->GetBrowser()->GetTabRight()->SetTab(1);

	make_gui();

	gEve->AddEvent(new TEveEventManager("Event", "SiWECAL VSD Event"));

	gDisplay->GotoEvent(0);

	gEve->Redraw3D(kTRUE); // Reset camera after the first event has been shown.

	// gDisplay->Display();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	// gSystem->Exit(0);

}

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
      TString icondir("./icons/");
      TGPictureButton* b = 0;

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoBack.gif"));
      hf->AddFrame(b);
      b->Connect("Clicked()", "TBDisplay", gDisplay, "Prev()");

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"info.gif"));
      hf->AddFrame(b);
      b->Connect("Clicked()", "TBDisplay", gDisplay, "GoTo()");

      b = new TGPictureButton(hf, gClient->GetPicture(icondir+"GoForward.gif"));
      hf->AddFrame(b);
      b->Connect("Clicked()", "TBDisplay", gDisplay, "Next()");
   }
   frmMain->AddFrame(hf);

   frmMain->MapSubwindows();
   frmMain->Resize();
   frmMain->MapWindow();

   browser->StopEmbedding();
   browser->SetTabTitle("Event Control", 0);
}

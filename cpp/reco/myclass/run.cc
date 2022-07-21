#include "TROOT.h"
#include "TFile.h"
#include "TApplication.h"

#include "EventControl.C"

void run(){

	EventControl EventControl;
	EventControl.Loop();

	ROOT::Math::MinimizerOptions::SetDefaultMaxFunctionCalls( 200 );

	gSystem->Exit(0);

	// return 0;

}


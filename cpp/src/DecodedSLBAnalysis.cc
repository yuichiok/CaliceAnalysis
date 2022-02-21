//# Copyright 2020  Adrián Irles (IJCLab, CNRS/IN2P3)
#define DecodedSLBAnalysis_cxx
#include "../include/DecodedSLBAnalysis.h"
#include <TPaveStats.h>
#include <TH2.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TString.h>
#include <iostream>
#include <TFitResult.h>
#include <TSpectrum.h>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

std::vector<std::array<int,9>>  DecodedSLBAnalysis::NoiseLevels(int acqwindow=150, bool calculate_expected=false)
{

  
  int retrigger_start[15][16][64];
  int retrigger_train[15][16][64];
  int trigger[15][16][64];
  int under_or_over_flow_trig[15][16][64];
  int under_or_over_flow_all[15][16][64];

  int n_SLB=0;


  for(int ilayer=0; ilayer<15; ilayer++) {
    for(int ichip=0; ichip<16; ichip++) {     
      for(int ichn=0; ichn<64; ichn++) {
	retrigger_start[ilayer][ichip][ichn]=0;
	retrigger_train[ilayer][ichip][ichn]=0;
	trigger[ilayer][ichip][ichn]=0;
	under_or_over_flow_trig[ilayer][ichip][ichn]=0;
	under_or_over_flow_all[ilayer][ichip][ichn]=0;
     }
    }
  }
  
  Long64_t nentries = fChain->GetEntriesFast();
  //  nentries=100000;
  float expected=0;
  // -----------------------------------------------------------------------------------------------------   
  // SCA analysis
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

      if(jentry==0) n_SLB=n_slboards;

    if ( jentry > 1000 && jentry % 1000 ==0 ) std::cout << "Progress: " << 100.*jentry/nentries <<" %"<<endl;

    expected=0.25*acqNumber*float(acqwindow)/60.;
  
    for(int ilayer=0; ilayer<n_slboards; ilayer++) {
      
      for(int ichip=0; ichip<16; ichip++) {

	// we search for the isca in which the retrigger train has been started
	// we assumed only one retrigger train per cycle
	bool first_retrig[15];
	bool retrig_started=false;
	for(int isca=0; isca<15; isca++) {
	  first_retrig[isca]=false;
	  if(badbcid[ilayer][ichip][isca]>0 && retrig_started==false) {
	    first_retrig[isca]=true;
	    retrig_started=true;
	  }
	}

	for(int isca=0; isca<15; isca++) {

	  if(bcid[ilayer][ichip][isca]<0) continue;
	  for(int ichn=0;ichn<64;ichn++) {

	    //save number of retrigger initiators and total
	    if(badbcid[ilayer][ichip][isca]>0 ) {
	      if(first_retrig[isca]==true) {
		if(gain_hit_high[ilayer][ichip][isca][ichn]==1) {
		  retrigger_start[ilayer][ichip][ichn]++;
		}
	      }
	      if(gain_hit_high[ilayer][ichip][isca][ichn]==1) retrigger_train[ilayer][ichip][ichn]++;
	    }
	  
	    //save number of under or over flow events .... triggered or not!!
	    if( (charge_hiGain[ilayer][ichip][isca][ichn]<100 || charge_hiGain[ilayer][ichip][isca][ichn]>1000)) {
	      under_or_over_flow_all[ilayer][ichip][ichn]++;
	      if(gain_hit_high[ilayer][ichip][isca][ichn]==1 ) under_or_over_flow_trig[ilayer][ichip][ichn]++;
	    }
	    
	    if(badbcid[ilayer][ichip][isca]==0) {
	      if(gain_hit_high[ilayer][ichip][isca][ichn]==1 && charge_hiGain[ilayer][ichip][isca][ichn]>100 && charge_hiGain[ilayer][ichip][isca][ichn]<1000) trigger[ilayer][ichip][ichn]++;
	    }
	  }
	    

	}//isca
      }//ichip
    }//ilayer
	  
      
  }

  std::vector<std::array<int,9>>  result;

  
  if(calculate_expected==true) {
    float n_enabled_channels=0;
    float ntriggers_total=0;

    for(int ilayer=0; ilayer<n_SLB; ilayer++) {
      for(int ichip=0; ichip<16; ichip++) {
	for(int ichn=0; ichn<64; ichn++) {
	  if(trigger[ilayer][ichip][ichn]>0) {
	    n_enabled_channels++;
	    ntriggers_total+=trigger[ilayer][ichip][ichn];
	  }
	}
      }
      //}
      expected=int(ntriggers_total/n_enabled_channels);
      
      n_enabled_channels=0;
      ntriggers_total=0;
      //   for(int ilayer=0; ilayer<n_SLB; ilayer++) {
      for(int ichip=0; ichip<16; ichip++) {
	for(int ichn=0; ichn<64; ichn++) {
	  if(trigger[ilayer][ichip][ichn]>0 && trigger[ilayer][ichip][ichn]<(float(expected)+3.*sqrt(float(expected)))) {
	    n_enabled_channels++;
	    ntriggers_total+=trigger[ilayer][ichip][ichn];
	  }
	}
      }
      expected=int(ntriggers_total/n_enabled_channels);
      for(int ichip=0; ichip<16; ichip++) {     
	for(int ichn=0; ichn<64; ichn++) {
	  std::array<int,9> temp;
	  temp[0]=ilayer;
	  temp[1]=ichip;
	  temp[2]=ichn;
	  temp[3]=int(expected);
	  temp[4]=trigger[ilayer][ichip][ichn];
	  temp[5]=retrigger_start[ilayer][ichip][ichn];
	  temp[6]=retrigger_train[ilayer][ichip][ichn];
	  temp[7]=under_or_over_flow_trig[ilayer][ichip][ichn];
	  temp[8]=under_or_over_flow_all[ilayer][ichip][ichn];
	  result.push_back(temp);
	}
      }
    }
	
    } else  {
      
      
      for(int ilayer=0; ilayer<n_SLB; ilayer++) {
	for(int ichip=0; ichip<16; ichip++) {     
	  for(int ichn=0; ichn<64; ichn++) {
	    std::array<int,9> temp;
	    temp[0]=ilayer;
	    temp[1]=ichip;
	    temp[2]=ichn;
	    temp[3]=int(expected);
	    temp[4]=trigger[ilayer][ichip][ichn];
	    temp[5]=retrigger_start[ilayer][ichip][ichn];
	    temp[6]=retrigger_train[ilayer][ichip][ichn];
	    temp[7]=under_or_over_flow_trig[ilayer][ichip][ichn];
	    temp[8]=under_or_over_flow_all[ilayer][ichip][ichn];
	    result.push_back(temp);
	  }
	}
      }
    }
	

  return result;
}





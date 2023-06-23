#define EventControl_cxx
#include "../include/EventControl.hh"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void EventControl::Loop()
{
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    Int_t nhit_len_slab[NSLAB] = {0};
    for( int ihit=0; ihit < nhit_len; ihit++ ){
      if( isMaskReq && hit_isMasked[ihit] == 1 ) continue;

      nhit_len_slab[ hit_slab[ihit] ]++;
      h_hit_slab->Fill( hit_slab[ihit] );
      h_hit_xy_slab.at( hit_slab[ihit] )->Fill( hit_x[ihit], hit_y[ihit] );

    }

    for( int islab=0; islab<NSLAB; islab++ ){
      h_nhit_len_slab.at(islab)->Fill( nhit_len_slab[islab] );
    }

  }
}

#include <iostream>
#include <string>
#include <vector>

#include <TStyle.h>
#include <TCanvas.h>
#include "include/Styles.h"
#include "include/tools.h"

const static int NSLABS = 15;
TFile *file = readfile("conv_sim e- 10");

void hit_2d()
{
  TH2F *h_xy[NSLABS];
  for( int ih = 0; ih < NSLABS; ih++ ){
    h_xy[ih] = (TH2F*)file->Get(TString::Format("hit_slab_xy/h_hit_slab_xy%d",ih));
  }

}
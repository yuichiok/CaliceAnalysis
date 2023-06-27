#include <iostream>
#include <string>
#include <vector>

void rdf_tree(){

  auto filename  = "../../data/reco/raw_siwecal_90378_e-_20.0GeV_build.root";
  auto tree_name = "ecal";

  ROOT::RDataFrame df(tree_name,filename);

  // auto df2 = df.Filter("nhit_slab  > 12   &&\
  //                       hit_sca    < 3    &&\
  //                       hit_energy > 1    &&\
  //                       hit_isMasked == 0 &&\
  //                       hit_slab == 4");
  auto df2 = df.Filter("nhit_slab < 12"); //&& hit_energy > 1 && hit_isMasked == 0 && hit_slab == 4");
  auto df3 = df2.Define("any_high_sca", "Sum( hit_sca > 2 )");
  auto df4 = df3.Filter("any_high_sca != 0");

  TCanvas *c_nhit_len = new TCanvas("c_nhit_len","c_nhit_len",700,700);
  auto h_nhit_len = df4.Histo1D({"hit_energy", "nhit_slab;nhit_slab;Entries", 40, 0, 40}, "hit_energy");
  h_nhit_len->DrawClone("");

}
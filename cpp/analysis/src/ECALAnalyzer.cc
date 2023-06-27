#include <iostream>
#include <TString.h>
#include <TFile.h>
#include "../include/ECALAnalyzer.hh"
#include "../include/TreeReader.hh"
#include "../include/FileSelector.hh"

using std::cout;
using std::endl;

ECALAnalyzer::ECALAnalyzer(TString o)
    : options(o)
{
  patEventsAnalyzed = 0;
  entriesInNtuple = 0;
}

bool ECALAnalyzer::MapTree(TTree *tree)
{
  // Maps TTree to class' variables.
  // TO DO: Implement check for correct mapping, return result?
  //    - Set up exception handling for negative result.

  entriesInNtuple = tree->GetEntries();

  // Set branch addresses and branch pointers
  if (!tree)
    return false;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);

  TreeReader reader;
  reader.InitializeRecoReadTree(fChain, _data, _branch);

  Notify();

  return true;
}

void ECALAnalyzer::Analyze(Long64_t entry, HistManager hm)
{
  // if( Is_SCA_Maxed() ) return;

  Float_t X0s[NSLABS] = {1.198630137, 2.397260274, 3.595890411, 4.794520548, 5.993150685, 7.191780822, 8.390410959, 9.589041096, 10.78767123, 12.38584475, 13.98401826, 15.58219178, 17.1803653, 18.77853881, 20.37671233};
  std::vector<Float_t> layer_hit_x[NSLABS];
  std::vector<Float_t> layer_hit_y[NSLABS];

  Float_t sum_slab_energy_stack = 0;
  Float_t sum_slab_energy[NSLABS] = {0};

  Float_t sum_energy_corrected = 0;
  Float_t sum_slab_energy_stack_corrected = 0;
  Float_t sum_slab_energy_corrected[NSLABS] = {0};

  hm.h1[hm.h_sum_energy]->Fill(_data.sum_energy);

  Int_t hit_counter = 0;
  Int_t hit_counter_slab[NSLABS] = {0};

  for (int ihit = 0; ihit < _data.nhit_len; ihit++)
  {
    // mask check
    if (_data.hit_isMasked[ihit] == 1 || _data.hit_energy[ihit] < 1)
      continue;

    hit_counter++;
    hit_counter_slab[_data.hit_slab[ihit]]++;

    // Float_t radius = sqrt(pow(_data.hit_x[ihit] - origin[0], 2) + pow(_data.hit_y[ihit] - origin[1], 2));
    // if (radius < set_radius)
    // {
    //   hit_counter_radius++;
    //   hit_coutner_radius_slab[_data.hit_slab[ihit]]++;
    // }

    hm.h2_layer[hm.h_hit_slab_xy][_data.hit_slab[ihit]]->Fill(_data.hit_x[ihit], _data.hit_y[ihit]);

    layer_hit_x[_data.hit_slab[ihit]].push_back(_data.hit_x[ihit]);
    layer_hit_y[_data.hit_slab[ihit]].push_back(_data.hit_y[ihit]);

    hm.h1[hm.h_hit_slab]->Fill(_data.hit_slab[ihit]);
    hm.h1[hm.h_hit_energy]->Fill(_data.hit_energy[ihit]);

    int ihit_slab = _data.hit_slab[ihit];
    sum_slab_energy[ihit_slab] += _data.hit_energy[ihit];
    hm.h1_layer[hm.h_hit_slab_energy][ihit_slab]->Fill(_data.hit_energy[ihit]);
    hm.h1[hm.h_energy_profile]->Fill(X0s[ihit_slab], _data.hit_energy[ihit]);

    if (_data.hit_energy[ihit] > 1)
    {
      hm.h1[hm.h_hit_slab_corrected]->Fill(_data.hit_slab[ihit]);
      hm.h1[hm.h_hit_energy_corrected]->Fill(_data.hit_energy[ihit]);

      sum_energy_corrected += _data.hit_energy[ihit];
      sum_slab_energy_corrected[ihit_slab] += _data.hit_energy[ihit];
      hm.h1_layer[hm.h_hit_slab_energy_corrected][ihit_slab]->Fill(_data.hit_energy[ihit]);
    }
  }

  hm.h1[hm.h_nhit_len]->Fill(hit_counter);
  // hm.h1[hm.h_nhit_len_radius]->Fill(hit_counter_radius);

  hm.h1[hm.h_sum_energy_corrected]->Fill(sum_energy_corrected);

  std::vector<std::vector<Float_t>> Mean_SD_x;
  std::vector<std::vector<Float_t>> Mean_SD_y;
  for (int islab = 0; islab < NSLABS; islab++)
  {
    sum_slab_energy_stack += sum_slab_energy[islab];
    hm.h1_layer[hm.h_sum_slab_energy][islab]->Fill(sum_slab_energy[islab]);
    hm.h1_layer[hm.h_sum_slab_energy_stack][islab]->Fill(sum_slab_energy_stack);

    sum_slab_energy_stack_corrected += sum_slab_energy_corrected[islab];
    hm.h1_layer[hm.h_sum_slab_energy_corrected][islab]->Fill(sum_slab_energy_corrected[islab]);
    hm.h1_layer[hm.h_sum_slab_energy_stack_corrected][islab]->Fill(sum_slab_energy_stack_corrected);

    hm.h1_layer[hm.h_nhit_len_slab][islab]->Fill(hit_counter_slab[islab]);
    // hm.h1_layer[hm.h_nhit_len_radius_slab][islab]->Fill(hit_coutner_radius_slab[islab]);

    std::vector<Float_t> iMean_SD_x = Mean_SD(islab, layer_hit_x[islab]);
    std::vector<Float_t> iMean_SD_y = Mean_SD(islab, layer_hit_y[islab]);
    Mean_SD_x.push_back(iMean_SD_x);
    Mean_SD_y.push_back(iMean_SD_y);
  }

  Int_t n_valid_slab = Mean_SD_x.size();
  std::vector<Int_t> valid_slabs;
  if(n_valid_slab){
    for (int islab=0; islab<n_valid_slab; islab++){

      Bool_t is_nhit   =  10 < Mean_SD_x.at(islab).at(1);
      Bool_t is_sigw_x = ( 15 < Mean_SD_x.at(islab).at(3) ) && ( Mean_SD_x.at(islab).at(3) < 25 );
      Bool_t is_sigw_y = ( 15 < Mean_SD_y.at(islab).at(3) ) && ( Mean_SD_y.at(islab).at(3) < 25 );

      if( is_nhit && is_sigw_x && is_sigw_y ){
        valid_slabs.push_back(Mean_SD_x.at(islab).at(0));
      }

    }
  }

  // just for radius analysis
  Int_t hit_counter_radius = 0;
  Float_t sum_energy_radius = 0;
  Int_t hit_coutner_radius_slab[NSLABS] = {0};
  Float_t set_radius = 0;
  if(_recosim=="reco"){
    set_radius = 3.;
  }
  if(_recosim=="conv_sim"){
    set_radius = 2.1;
  }

  for (int ihit = 0; ihit < _data.nhit_len; ihit++)
  {
    Float_t radius = sqrt(pow(_data.hit_x[ihit] - Mean_SD_x.at(_data.hit_slab[ihit]).at(2), 2) + pow(_data.hit_y[ihit] - Mean_SD_y.at(_data.hit_slab[ihit]).at(2), 2));
    if (radius < set_radius)
    {
      hit_counter_radius++;
      hit_coutner_radius_slab[_data.hit_slab[ihit]]++;
      sum_energy_radius += _data.hit_energy[ihit];
    }

  }
  hm.h1[hm.h_nhit_len_radius]->Fill(hit_counter_radius);
  hm.h1[hm.h_sum_energy_radius]->Fill(sum_energy_radius);
  for(int islab=0; islab<NSLABS; islab++){
    hm.h1_layer[hm.h_nhit_len_radius_slab][islab]->Fill(hit_coutner_radius_slab[islab]);
  }

  if( 5 < valid_slabs.size() ){
    hm.h1[hm.h_sum_energy_corrected_MeanSD]->Fill(_data.sum_energy);
  }

}

Bool_t ECALAnalyzer::Notify()
{
  return kTRUE;
}

bool ECALAnalyzer::Select()
{ // Evaluates the class' list of event selection criteria

  FileSelector fs(options);
  _recosim = fs.GetRecoSim();
  Int_t energy = fs.GetEnergy();
  Int_t nhit_len_th = 0;

  if (_recosim == "conv_sim")
    return true;

  std::pair<Int_t, Int_t> opt_nhit_len_threshold[7] = {
      std::make_pair(10, 150),
      std::make_pair(20, 250),
      std::make_pair(40, 410),
      std::make_pair(60, 535),
      std::make_pair(80, 641),
      std::make_pair(100, 736),
      std::make_pair(150, 935)};

  if (_data.nhit_slab < 13)
    return false;

  for (int ihit=0; ihit<_data.nhit_len; ihit++){
    if(2 < _data.hit_sca[ihit]) return false;
  }

  for (auto pair : opt_nhit_len_threshold)
  {
    if (energy == pair.first)
    {
      nhit_len_th = pair.second;
      break;
    }
  }

  // if (_data.nhit_len < nhit_len_th)
  //   return false;

  return true;
}

Bool_t ECALAnalyzer::Is_SCA_Maxed()
{
  for (int ihit = 0; ihit < _data.nhit_len; ihit++)
  {
    if(13 < _data.hit_slab[ihit]) return 1;
  }
  
  return 0;

}

std::vector<Float_t> ECALAnalyzer::Mean_SD(int slab, std::vector<Float_t> arr)
{
  static std::vector<Float_t> Mean_SD_vec;
  Float_t sum = 0;
  Int_t nhits = arr.size();
  Mean_SD_vec = {static_cast<float>(slab), static_cast<float>(nhits), -1000.0, -1000.0};

  if (nhits == 0)
  {
    return Mean_SD_vec;
  }

  for (int i = 0; i < nhits; i++)
  {
    sum += arr.at(i);
  }

  Float_t mean = sum / nhits;
  Float_t sigma = 0;
  for (int i = 0; i < nhits; i++)
  {
    sigma += pow(arr.at(i) - mean, 2);
  }

  sigma = sqrt(sigma / nhits);

  Mean_SD_vec[0] = slab;
  Mean_SD_vec[1] = nhits;
  Mean_SD_vec[2] = mean;
  Mean_SD_vec[3] = sigma;

  return Mean_SD_vec;
}
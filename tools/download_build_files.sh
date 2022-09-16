#!/bin/bash

runIDs=( 20 78 75 72 67 65 55 )
energies=( 10 20 40 60 80 100 150 )

for i in "${!energies[@]}"
do
  scp calice@pc-calice:/home/calice/TB2022-06/SiWECAL-TB-monitoring/data/raw_siwecal_903${runIDs[i]}/full_run.root \
  raw_siwecal_903${runIDs[i]}_e-_${energies[i]}.0GeV_build.root
done
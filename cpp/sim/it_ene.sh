#!/bin/bash

for energy in 10 20 40 60 80 100 150
do
    root -l -q run_sim.cc\(${energy}\)
done
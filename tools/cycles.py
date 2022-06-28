#!/usr/bin/env python3

import sys, os
import subprocess, shlex

## to be run in pc-calice at /home/calice/TB2022-06/eudaq_data/raw 
## Which is mounted from the diskserver /home/calice/TB2022-06/eudaq/data/raw

raw_path = "/home/calice/TB2022-06/eudaq_data/raw/"
win_path = "/mnt/win_daq/Run_Data/"

ls = os.listdir(win_path)
#print(ls)

for ifile, filename in enumerate(ls):
    if filename.startswith("eudaq_run_09"):

        run_setting_path = win_path + "/" + filename + "/Run_Settings.txt"
        run_setting_file = open(run_setting_path,'r')
        
        lines = run_setting_file.readlines()

        for iline, line in enumerate(lines):
            if iline == 0 :
                print(line)

        # win_runnumber = filename.split("_")[-1]
        # disk_runnumber = win_runnumber[1:]
        # disk_runname = "raw_siwecal_" + disk_runnumber
        # mkdir = "mkdir -p " + disk_runname
        # p = subprocess.Popen(shlex.split(mkdir))
        
        # for txtfile in ["hitsHistogram", "Run_Settings", "logfile"]:
        #     cp_daq = "cp " + win_path + "/" + filename + "/" + txtfile + ".txt " + disk_runname + "/"
        #     p = subprocess.Popen(shlex.split(cp_daq))
        
        # wd = os.getcwd()
        # os.chdir(disk_runname)
        # if os.path.exists(disk_runname + "_raw.bin"):
        #     p = subprocess.Popen(shlex.split("rm " + disk_runname + "_raw.bin"))
        # # do relative paths here
        # link = "ln -s ../" + disk_runname + ".raw " + disk_runname + "_raw.bin"
        # p = subprocess.Popen(shlex.split(link))
        # os.chdir(wd)

import csv
import numpy as np
import os
import subprocess as sb
# import matplotlib.pyplot as plt
import ROOT




sb.run(["grep","-r","'FeedbackCap:'","~/siw-ecal/TB2022-03/beamData/ascii/3GeV_MIPscan_eudaq_run_050489/Run_Settings.txt"])



'''
reader = csv.reader(open("/afs/cern.ch/work/y/yuokugaw/public/run_list/run_list.csv"), delimiter=",")

with open('tmp1.csv', 'w') as outfile1:

	writer_tmp = csv.writer(outfile1, delimiter=",")

	for i, line in enumerate(reader):

		filename = line[0]
		date     = line[1]
		time     = line[2]

		filenameSP = filename.split("_")
		runID = filenameSP[-1]

		# print(runID,filename,date,time)

		input_string = []
		input_string.append(runID)
		input_string.append(filename)
		input_string.append(date)
		input_string.append(time)
		writer_tmp.writerow(input_string)
'''
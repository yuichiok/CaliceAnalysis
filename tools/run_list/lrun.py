import csv
import numpy as np
import os
import subprocess as sb
import ROOT
import collections

options = {
	2  : 0, # 0.8 pF
	3  : 1, # 1.2 pF
	4  : 2, # 1.6 pF
	5  : 3, # 0.8 & 1.2 pF
	15 : 4, # 6.0 pF
	20 : 5  # 0.8 & 1.2 & 6.0 pF
}

directory='/eos/project-s/siw-ecal/TB2022-03/beamData/ascii/'


with open('tmp1.csv', 'w') as outfile1:

	writer_tmp = csv.writer(outfile1, delimiter=",")

	for runname in os.listdir(directory):

		run_setting = os.path.join(directory, runname, "Run_Settings.txt")

		inputs = []
		FCs=[]

		if os.path.isfile(run_setting):

			reader = csv.reader(open(run_setting,"r"), delimiter=" ")

			for i, line in enumerate(reader):

				if 'FeedbackCap:' in line:
					flg = line.index('FeedbackCap:')
					FCs.append(int(line[flg+1]))

			var = set(FCs)
			add = 0
			for i in var: add += i
			# print(add)
			# print(options[add])

			runnameID = runname.split("_")
			runID = runnameID[-1]
			# print(runID)

			inputs.append(runID)
			inputs.append(runname)
			inputs.append(options[add])

			writer_tmp.writerow(inputs)






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
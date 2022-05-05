import csv
import numpy as np
import matplotlib.pyplot as plt
import ROOT

read_map = csv.reader(open("mapping/fev10_chip_channel_x_y_mapping.txt"), delimiter=" ")

chip=-1
x0=9999
y0=9999
# channel=-1
x=9999
y=9999

mapxy_04     = ROOT.TH2F("mapxy_04","map-xy; x; y",8,-45,0,8,-90,-45)
channel_04 = np.zeros((8,8))

for ix, iy in np.ndindex(channel_04.shape):
	channel_04[ix, iy] = -1

for i, line in enumerate(read_map):

	if i == 0:
		continue

	input_string = [None]*6

	chip = int(line[0])
	x0   = float(line[1])
	y0   = float(line[2])
	chnl = int(line[3])
	x    = float(line[4])
	y    = float(line[5])

	if chip == 4:
		mapxy_04.Fill(-x,-y,chnl)

c1 = TCanvas
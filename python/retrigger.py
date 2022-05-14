import ROOT
import uproot
import numpy as np
import matplotlib.pyplot as plt

file = uproot.open("/home/calice/work/SiWECAL-TB-analysis/converter_SLB/convertedfiles/Run_ILC_11052022_datarate_aq100.root")
siw = file['siwecaldecoded']

print(siw.keys())
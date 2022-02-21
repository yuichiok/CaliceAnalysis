filename = "/home/calice/Work/TBData/ascii/3GeVMIPscan_run_050043/Run_Settings.txt"

with open(filename) as file:
    while line := file.readline():
        print(line.rstrip())

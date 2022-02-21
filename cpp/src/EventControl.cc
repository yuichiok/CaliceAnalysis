#include "../include/EventControl.hh"

void EventControl(){

	TString filename = "/home/calice/Work/TBData/ascii/3GeVMIPscan_run_050043/Run_Settings.txt";

	read_file(filename);

	cout << "aq win = " << detector.acq_window << endl;
	cout << "aq win = " << detector.acq_delay  << endl;


}
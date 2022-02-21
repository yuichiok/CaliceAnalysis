#include "TROOT.h"
#include "TFile.h"

#include "../conf_struct.h"

using std::cout;
using std::endl;

void read_file(TString filename_in){

	read_configuration_file(filename_in,false);

}

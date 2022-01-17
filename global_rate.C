/**************************************************************************************************
 *
 *** Filename: global_rate.C
 *
 *** Date of creation: 23/09/2021
 *
 *** Author(s): @jdani98
 *
 *** Description:
 *   This program reads the tree .root file and returns: number of events, total time interval and
 *   global rate of events.
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L global_rate.C
 *       > rate(<root_file>)
 *      where <root_file> is the .root input file (written in quotes)
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/

#include "CRoot1.h"
#include "TObject.h"
#include "TTree.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

void rate(const char* root_file){
  TTree *tree = new TTree();
  TFile *file;
  if(!(file = gROOT->GetFile())) file = new TFile(root_file);
  file->GetObject("myT", tree);
  
  CScopeEvent *myscope = new CScopeEvent();
  tree->SetBranchAddress("event", &myscope);
  
  Long64_t nentries = tree->GetEntriesFast();
  Long64_t ientry = tree->GetEntry(0);
  unsigned long int T_ini = myscope->GetEventTime();    // initial time (TempoInicial)
  unsigned long int t_prev = T_ini;              // previous time for loop (previousTime)
  unsigned long int time = T_ini;
  Long64_t lastentry = tree->GetEntry(nentries - 1);
  unsigned long int T_fin = myscope->GetEventTime();    // final time (TempoFinal)
  unsigned long int DT_tot = (T_fin - T_ini);           // total time (tempoTotal_uSecs)
  
  Float_t rate = float(nentries-1)/float(DT_tot)*1000000.;
  
  cout << "N events= " << nentries << "  Time interval= " << DT_tot << " us" << 
  "  Global rate= " << rate << " events/second" << endl;
}

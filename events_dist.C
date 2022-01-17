/**************************************************************************************************
 *
 *** Filename: events_dist.C
 *
 *** Date of creation: 30/09/2021
 *
 *** Author(s): @jdani98
 *
 *** Description:
 *   This program reads the tree .root file and divides the time in intervals, then returns 2 
 *   figures and 1 parameter:
 *    - Number of events inside each time interval
 *    - Histogram (ditribution) of number of events per time interval
 *    - Mean rate of events
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L events_dist.C
 *       > events(<fileName>,<[nbins]>,<[nbins2]>)
 *      where <fileName> is the .root input file (written in quotes), <nbins> is the number of time
 *      intervals to plot the counts and <nbins2> is the number of bins of counts histogram
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/

#include "CRoot1.h"
#include "TObject.h"
#include "TTree.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

void events(const char* fileName, int nbins=30, int nbins2=10) {

  /// Fixed variables /////////////////////////////////////////////////////////////////////////////
    // nbins: number of time intervals to plot the counts
    // nbins2: number of bins of counts histogram
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  TTree *tree = new TTree();
  TFile *file;
  if(!(file = gROOT->GetFile())) file = new TFile(fileName);
  file->GetObject("myT", tree);

  Float_t timescale = 1.e-6;
  //ofstream tabla;tabla.open("tabla.txt");

  CPulseEvent *mypulse = new CPulseEvent();
  CScopeEvent *myscope = new CScopeEvent();
  tree->SetBranchAddress("pulse", &mypulse);
  tree->SetBranchAddress("event", &myscope);
  
  Long64_t nentries = tree->GetEntriesFast();           // number of entries
  Long64_t ientry = tree->GetEntry(0);                  // first entry
  unsigned long int T_ini = myscope->GetEventTime();    // initial time
  unsigned long int time;                               
  //time = T_ini;
  Long64_t lastentry = tree->GetEntry(nentries - 1);    // last entry
  unsigned long int T_fin = myscope->GetEventTime();    // final time
  unsigned long int DT_tot = (T_fin-T_ini);             // total time interval
  Float_t Rate_mean = (Float_t)(nentries) / (Float_t)(DT_tot);
  
  
  /// Number of counts per time intervals
  unsigned long int DT;
  int frange=(Float_t)(DT_tot)*timescale;
  Float_t width = (Float_t)(frange) / (Float_t)(nbins);
  
  TCanvas *rate_can = new TCanvas("rate_can");
  TH1F *rate_hist = new TH1F("Stats","Number of events per time intervals",nbins,0,frange);
  for(int i=0; i<nentries; i++){
    tree->GetEntry(i);
    time = myscope->GetEventTime();
    DT = time - T_ini;
    //cout << "T_ini=" << T_ini << " time=" << time << " DT=" << DT << endl;
    rate_hist->Fill((Float_t)(DT)*timescale);
  }
  //cout << "T_ini=" << T_ini << " time=" << time << " DT=" << DT << endl;
  rate_hist->GetXaxis()->SetTitle("Time (s)");
  rate_hist->GetYaxis()->SetTitle("events");
  rate_hist->Draw();
  int maxcount;
  maxcount = rate_hist->GetMaximum();
  rate_hist->GetYaxis()->SetRangeUser(0,maxcount+5);
  //rate_hist->Rebin();
  
  
  /// Counts histogram
  
  TCanvas *rated_can = new TCanvas("rated_can");
  TH1F *rated_hist = new TH1F("Histogram stats","Number of events histogram",nbins2,0,maxcount+1);
  int counts;
  Float_t bwidth;
  Float_t local_rate;
  for(int i=1; i<=nbins; i++){
    counts = rate_hist->GetBinContent(i);
    bwidth = rate_hist->GetBinWidth(i);
    local_rate = (Float_t)(counts) / (Float_t)(bwidth);
    rated_hist->Fill(counts);
    }
  rated_hist->GetXaxis()->SetTitle("events/interv");
  rated_hist->GetYaxis()->SetTitle("counts");
  rated_hist->Draw();
  //rated_hist->Rebin();
  cout << "Mean rate: " << Rate_mean/timescale << endl;
}

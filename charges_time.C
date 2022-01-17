/**************************************************************************************************
 *
 *** Filename: charges_time.C
 *
 *** Date of creation: 30/09/2021
 *
 *** Author(s): @jdani98
 *
 *** Description:
 *   This program reads the tree .root file and returns a figure with four subplots displaying
 *   the charge for each event, situated on the x axis in correspondency with its time.
 *   The charge is defined in arbitrary units as the sum of voltages (inverted in sign) 
 *   of one event.
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L charges_time.C
 *       > charges_time(<fileName>)
 *      where <fileName> is the .root input file (written in quotes)
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/

#include "CRoot1.h"
#include "TObject.h"
#include "TTree.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>


void charges_time(const char* fileName) {
  
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
  
  int ngroup=1;
  /// Number of counts per time intervals
  unsigned long int DT;
  int frange=(Float_t)(DT_tot)*timescale;
  
  Double_t evtime[nentries/ngroup];
  Double_t evchargeA[nentries/ngroup];
  Double_t evchargeB[nentries/ngroup];
  Double_t evchargeC[nentries/ngroup];
  Double_t evchargeD[nentries/ngroup];
  
  for(int i=0; i<nentries/ngroup; i++){
    tree->GetEntry(i);
    time = myscope->GetEventTime();
    DT = time - T_ini;
    //cout << i << "  T_ini=" << T_ini << " time=" << time << " DT=" << DT << endl;
    //rate_hist->Fill((Float_t)(DT)*timescale);
    
    int chargeA = 0;
    int chargeB = 0;
    int chargeC = 0;
    int chargeD = 0;
    
    for(int k=ngroup*i; k<ngroup*(i+1); k++){
      tree->GetEntry(k);
      vector<int> ampA = myscope->GetAmpA();
      for(int i=0;i<ampA.size();i++) chargeA += -ampA[i];
      vector<int> ampB = myscope->GetAmpB();
      for(int i=0;i<ampB.size();i++) chargeB += -ampB[i];
      vector<int> ampC = myscope->GetAmpC();
      for(int i=0;i<ampC.size();i++) chargeC += -ampC[i];
      vector<int> ampD = myscope->GetAmpD();
      for(int i=0;i<ampD.size();i++) chargeD += -ampD[i];
    
    evtime[i] = (Double_t)(DT) * timescale;
    evchargeA[i] = (Double_t)(chargeA);
    evchargeB[i] = (Double_t)(chargeB);
    evchargeC[i] = (Double_t)(chargeC);
    evchargeD[i] = (Double_t)(chargeD);
    
  }}
  //cout << "T_ini=" << T_ini << " time=" << time << " DT=" << DT << endl;
  
  TCanvas *charge_time_can = new TCanvas("charge_time_can"); charge_time_can->Divide(2,2);
  TGraph *charge_time[4];
  charge_time[0] = new TGraph(nentries/ngroup,evtime,evchargeA);
  charge_time[1] = new TGraph(nentries/ngroup,evtime,evchargeB);
  charge_time[2] = new TGraph(nentries/ngroup,evtime,evchargeC);
  charge_time[3] = new TGraph(nentries/ngroup,evtime,evchargeD);
  
  for(int k=0; k<4; k++){
    charge_time_can->cd(k+1);
    charge_time[k]->GetXaxis()->SetTitle("Time (s)");
    charge_time[k]->GetYaxis()->SetTitle("charge");
    charge_time[k]->Draw("AP");
    charge_time[k]->SetMarkerStyle(20);
    charge_time[k]->SetMarkerColor(1+k);
    charge_time[k]->DrawPanel();
    
  }
  
  }

/**************************************************************************************************
 *
 *** Filename: charges_time2.C
 *
 *** Date of creation: 01/10/2021
 *
 *** Author(s): @jdani98
 *
 *** Description:
 *   This program reads the tree .root file and divides the time in intervals, then returns 2
 *   figures:
 *    - Four subplots of the sum of charges falling in each interval
 *    - The number of events per time interval
 *   The charge is defined in arbitrary units as the sum of voltages (inverted in sign) 
 *   of one event.
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L charges_time2.C
 *       > charges_time(<fileName>,<[ngroups]>)
 *      where <fileName> is the .root input file (written in quotes) and <ngroups> is the
 *      (optional) number of time intervals
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/
 
#include "CRoot1.h"
#include "TObject.h"
#include "TTree.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

void charges_time(const char* fileName, int ngroups=50) {
  
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
  
  

  unsigned long int DT;
  //int frange=(Float_t)(DT_tot)*timescale;
  unsigned long int width = DT_tot/ngroups;
  //cout << DT_tot << " " << width << endl;
  
  unsigned long int times[ngroups];
  Double_t times_red[ngroups];
  Double_t ac_chargesA[ngroups];
  Double_t ac_chargesB[ngroups];
  Double_t ac_chargesC[ngroups];
  Double_t ac_chargesD[ngroups];
  Double_t ac_events[ngroups];
  
  unsigned long int limit=0;
  for(int i=0; i<ngroups; i++){
    limit += width;
    times[i] = limit;
    times_red[i] = (Double_t)(limit) * timescale;
    //cout << times_red[i] << endl;
    }
  
  int k=0;
  int ac_chargeA = 0;
  int ac_chargeB = 0;
  int ac_chargeC = 0;
  int ac_chargeD = 0;
  int new_j = 0;
  
  for(int j=0; j<nentries; j++){
    tree->GetEntry(j);
    time = myscope->GetEventTime();
    DT = time - T_ini;
    
    if(DT>times[k]){
      ac_events[k] = (Double_t)(j - new_j);
      ac_chargesA[k] = (Double_t)(ac_chargeA)/(Double_t)(j - new_j);
      ac_chargesB[k] = (Double_t)(ac_chargeB)/(Double_t)(j - new_j);
      ac_chargesC[k] = (Double_t)(ac_chargeC)/(Double_t)(j - new_j);
      ac_chargesD[k] = (Double_t)(ac_chargeD)/(Double_t)(j - new_j);
      //cout << times[k] << " " << ac_chargesA[k] << endl;
      k+=1;
      ac_chargeA = 0;
      ac_chargeB = 0;
      ac_chargeC = 0;
      ac_chargeD = 0;
      new_j = j;
      }
    
    int chargeA = 0;
    int chargeB = 0;
    int chargeC = 0;
    int chargeD = 0;
    
    vector<int> ampA = myscope->GetAmpA();
    for(int i=0;i<ampA.size();i++) chargeA += -ampA[i];
    vector<int> ampB = myscope->GetAmpB();
    for(int i=0;i<ampB.size();i++) chargeB += -ampB[i];
    vector<int> ampC = myscope->GetAmpC();
    for(int i=0;i<ampC.size();i++) chargeC += -ampC[i];
    vector<int> ampD = myscope->GetAmpD();
    for(int i=0;i<ampD.size();i++) chargeD += -ampD[i];
    
    ac_chargeA += chargeA;
    ac_chargeB += chargeB;
    ac_chargeC += chargeC;
    ac_chargeD += chargeD;
    }
  
  
  TCanvas *charge_time_can = new TCanvas("charge_time_can"); charge_time_can->Divide(2,2);
  TGraph *charge_time[4];
  charge_time[0] = new TGraph(ngroups,times_red,ac_chargesA);
  charge_time[1] = new TGraph(ngroups,times_red,ac_chargesB);
  charge_time[2] = new TGraph(ngroups,times_red,ac_chargesC);
  charge_time[3] = new TGraph(ngroups,times_red,ac_chargesD);
  
  
  for(int k=0; k<4; k++){
    charge_time_can->cd(k+1);
    charge_time[k]->GetXaxis()->SetTitle("Time (s)");
    charge_time[k]->GetYaxis()->SetTitle("charge");
    charge_time[k]->Draw(); //"AP"
    charge_time[k]->SetMarkerStyle(20);
    charge_time[k]->SetMarkerColor(1+k);
    charge_time[k]->DrawPanel();
    }
  
  /// #############
  
  TCanvas *entries_time_can = new TCanvas("entries_time_can");
  TGraph *entries_time = new TGraph(ngroups,times_red,ac_events);
  entries_time->GetXaxis()->SetTitle("Time (s)");
  entries_time->GetYaxis()->SetTitle("events");
  entries_time->Draw();
  entries_time->SetMarkerStyle(20);
  entries_time->SetMarkerColor(6);
  }

/**************************************************************************************************
 *
 *** Filename: charges_time.C
 *
 *** Date of creation: 26/09/2021
 *
 *** Author(s): @jdani98
 *
 *** Description:
 *   This program reads the tree .root file and makes a complete analysis of the distribution of
 *   time separation between consecutive events. It returns 6 figures and 1 table:
 *    - Main histogram of time separation in linear scale
 *    - Main histogram in semi-logarithmic scale
 *    - Fit to points (only selected points)
 *    - Contribution of each point to chi2 of the previous fit
 *    - Residuals
 *    - Histogram of residuals
 *    - Summary of obtained numerical results
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L time_dist.C
 *       > time_dist(<fileName>,<[sel_opt]>,<[opt]>,<[mode]>)
 *      where <fileName> is the .root input file (written in quotes), <nbins> is the number of bins
 *      of the histograms, <sel_opt> can be "nbins" or "width" (if "nbins", the next argument <opt>
 *      must indicate the number of bins of the histogram to fit, if "width", <opt> must be the
 *      width (in the used time units) of each bin) and <mode> can be "auto" or "manual" (if
 *      "auto", the points are selected automatically, verifying that the first point is only
 *      if the bin width is greater than 3000000 and the aboslute number of counts per bin is over
 *      a threshold, determined by the variable Nh_th)
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/

#include "CRoot1.h"
#include "TObject.h"
#include "TTree.h"
#include <TCanvas.h>
#include <TH2.h>
#include <TStyle.h>

#include <string.h>
#include <iostream>
#include <TDatime.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
///// PROGRAM TO STUDY THE DISTRIBUTION OF TIMES BETWEEN SUCCESSIVE EVENTS
///// Plots: +main histogram +main histo log +fit to points +contributions to chi2 +residuals +residuals histo
///// Manual selection of points to fit
///////////////////////////////////////////////////////////////////////////////////////////////////


void time_dist(const char* fileName, const char* sel_opt="nbins", int opt=20, const char* mode="auto") {
  
  /// Fixed variables /////////////////////////////////////////////////////////////////////////////
  Float_t Nh_th = 8; // -!- N minimum to include data for plot
  int min_k=0;        // -!- index of first point to fit (in manual selection)
  int max_k=10;        // -!- index of last point to fit (in manual selection)
  Float_t timescale=1.e-6; // -!- scale factor of time. Now: transform us->s
  unsigned long int min_width = 3000000; // -!- minimum width of bins to not include first point in automatic selection of points to fit. Before: 3000000000
  const char* tableName = "OUTPUTS/time_dist_summary.txt"; // name of file with results
  /////////////////////////////////////////////////////////////////////////////////////////////////
  

  TTree *tree = new TTree();
  TFile *file;
  if(!(file = gROOT->GetFile())) file = new TFile(fileName);
  file->GetObject("myT", tree);

  ofstream tabla;tabla.open(tableName,fstream::app); // generates table with outputs information ,fstream::app

  CPulseEvent *mypulse = new CPulseEvent();
  CScopeEvent *myscope = new CScopeEvent();
  tree->SetBranchAddress("pulse", &mypulse);
  tree->SetBranchAddress("event", &myscope);

  // Recall: time in microseconds (us,usecs)
  Long64_t nentries = tree->GetEntriesFast();
  Long64_t ientry = tree->GetEntry(0);
  cout << "t " << timescale << endl;
  unsigned long int T_ini = myscope->GetEventTime();    // initial time
  unsigned long int t_prev = T_ini;                     // previous time for loop
  unsigned long int time = T_ini;
  Long64_t lastentry = tree->GetEntry(nentries - 1);
  unsigned long int T_fin = myscope->GetEventTime();    // final time
  unsigned long int DT_tot = (T_fin - T_ini);           // total time
  Float_t Rate_mean = (Float_t)(DT_tot) / nentries;
  Float_t lambda;
  
  int nbins;     // number of bins
  unsigned long int width; // width of bins (in right units)
  
  
  /// Options of <nbins> or <width> ///////////////////////////////////////////////////////////////
  if ( strncmp(sel_opt,"nbins",5) == 0) {
    nbins = opt;
    width = 5*Rate_mean/opt;
    }
  
  if ( strncmp(sel_opt,"width",5) == 0) {
    width = opt;
    nbins = 5*Rate_mean/opt;
    }
  /////////////////////////////////////////////////////////////////////////////////////////////////
  
  
  Float_t centre_bin[nbins-1]; // medium point of each bin (punto_medio_bin)
  Float_t freq;           // count of each bin
  Float_t R[nbins-1];     // rate freq/dt
  Float_t sR[nbins-1];    // rate uncertainty
  Float_t logR[nbins-1];  // log(R)
  Float_t slogR[nbins-1]; // s(log(R))
  Float_t sbin[nbins-1];  // s(centre_bin)
  Float_t probs[nbins-4];  // array of probabilities of fits
  Float_t max_prob;
  
  Float_t Nh[nbins-1];    // counts of bins
  Float_t sNh[nbins-1];   // uncertainty
  Float_t logNh[nbins-1]; // log(Nh)
  Float_t slogNh[nbins-1];// s(log(Nh))
  
  
  
  //// MAIN HISTOGRAM
  TCanvas *expo_can = new TCanvas("exponential");
  TH1F *h_expo = new TH1F("Exponential stats","Exponential histogram", nbins, 0, 5*Rate_mean); 
  h_expo->Draw();
  
  
  for (int jentry=0; jentry<nentries; jentry++) {
    ientry = tree->GetEntry(jentry);
    time = myscope->GetEventTime();
    h_expo->Fill(time-t_prev);  // previousTime already defined for the first iteration. This stores the time intervals between successives to the histogram
    t_prev=time;
    }
  h_expo->Draw();
  
  
  // Table
  TDatime d;
  int day = d.GetDate();
  int tim = d.GetTime();
  tabla << "\n\n***********************************************************" << endl;
  tabla << " Date and time (AAMMDD HHMMSS): " << day << " " << tim << "  File: " << fileName << endl;
  tabla << " *** HISTOGRAM STATISTICS" << endl;
  cout << " *** HISTOGRAM STATISTICS" << endl;
  tabla << "   * N_entries: " << nentries << "  N_bins: " << nbins << "  width= " << width << endl;
  cout << "   * N_entries: " << nentries << "  N_bins: " << nbins << "  width= " << width << endl;
  tabla << "   * T_ini= " << timescale * T_ini << " s  T_fin= " << timescale * T_fin << " s  DT= " << timescale * DT_tot << " s" << endl;
  cout << "   * T_ini= " << timescale * T_ini << " s  T_fin= " << timescale * T_fin << " s  DT= " << timescale * DT_tot << " s" << endl;
  lambda = h_expo->GetMean();
  tabla << "   * lambda= " << lambda*timescale << " s^-1" << endl;
  cout << "   * lambda= " << lambda*timescale << " s^-1" << endl;
  
  
  
  
  for (int k = 1; k <= nbins; k++) {
    centre_bin[k-1] = h_expo->GetBinCenter(k);
    freq = h_expo->GetBinContent(k);
    R[k-1] = freq/width;
    sR[k-1] = TMath::Sqrt(freq) / width;
    logR[k-1] = TMath::Log(freq/width);
    slogR[k-1] = TMath::Sqrt(freq) / freq;
    
    Nh[k-1] = freq;
    sNh[k-1] = TMath::Sqrt(freq);
    logNh[k-1] = TMath::Log10(freq);
    slogNh[k-1] = TMath::Sqrt(freq)/freq*TMath::Log10(2.7182);
    
    if(freq==0) logR[k-1] = 0, slogR[k-1] = 0; // to avoid NaNs
    }
  
  Float_t sbins[nbins-1];
  
  if (strncmp(mode,"auto",4) == 0) {
    for (int k = 0; k < nbins-1; k++){
      if (Nh[k] >= Nh_th) max_k = k;
    }
    if (width>min_width) min_k=0;
    else min_k=1;
  }
  
  // Table
  tabla << " *** FIT PARAMETERS" << endl;
  cout << " *** FIT PARAMETERS" << endl;
  tabla << "   * min_k= " << min_k << "  max_k= " << max_k << endl; 
  cout << "   * min_k= " << min_k << "  max_k= " << max_k << endl;  
  
  
  
  /// REPRESENTATION OF N in logarithmic scale
  for(int i=0; i<nbins;i++) sbins[i] = 0;
  TCanvas *log10_expo = new TCanvas("log10_expo_dist");
  TGraphErrors *log10_h_expo = new TGraphErrors(nbins,centre_bin,Nh,sbins,sNh);
  log10_h_expo->SetTitle("N vs dT");
  log10_h_expo->Draw("AP");
  log10_h_expo->SetMarkerStyle(20);
  log10_h_expo->SetMarkerColor(4);
  log10_h_expo->GetXaxis()->SetTitle("Delta T (us)");
  log10_h_expo->GetYaxis()->SetTitle("N");
  log10_expo->SetLogy();
  
  
  
  /// REPRESENTATION OF log(R)
  TCanvas *log_expo = new TCanvas("log_expo_dist");
  TGraphErrors *log_h_expo = new TGraphErrors(nbins,centre_bin,logR,sbins,slogR);
  log_h_expo->SetTitle("log(R) vs dT");
  log_h_expo->Draw("AP");
  log_h_expo->SetMarkerStyle(20);
  log_h_expo->SetMarkerColor(4);
  log_h_expo->GetXaxis()->SetTitle("Delta T (us)");
  log_h_expo->GetYaxis()->SetTitle("log(R)");
  //log_h_expo->GetYaxis()->SetRangeUser(-20,-12);
  
  Float_t xmin = centre_bin[min_k];
  Float_t xmax = centre_bin[max_k];
  
  // Table
  tabla << "   * xmin= " << xmin << "  xmax= " << xmax << endl;
  cout << "   * xmin= " << xmin << "  xmax= " << xmax << endl;
  TF1 *loge_fit = new TF1("log_expo_fit","[0]*x+[1]"); // fit log(exponential) points to a straight line
  log_h_expo->Fit(loge_fit,"Fit","Fit",xmin,xmax);
  gStyle->SetOptFit(1111);
  Double_t chisq = loge_fit->GetChisquare();
  // https://root.cern/doc/master/classTH1.html#TH1:Chi2Test
  
  int ndf = (max_k-min_k+1) - 2;
  Float_t pvalue = ROOT::Math::chisquared_cdf(chisq,ndf);
  Float_t dof = loge_fit->GetNDF();
  Float_t prob = loge_fit->GetProb();
  
  tabla << "   * chi2= " << chisq << "  ndf= " << dof << "  p-value= " << pvalue << "  prob= " << prob << endl;
  cout << "   * chi2= " << chisq << "  ndf= " << dof << "  p-value= " << pvalue << "  prob= " << prob << endl; // we show both p-value and prob to see if prob = 1 - p-value
  
  Float_t parma;
  Float_t parmb;
  parma = loge_fit->GetParameter(0);
  parmb = loge_fit->GetParameter(1);
  
  // Table
  tabla << "   * a= " << parma/timescale << " s^-1  b= " << parmb << endl;
  cout << "   * a= " << parma/timescale << " s^-1  b= " << parmb << endl;
  
  
  
  /// CALCULUS OF CHI2 CONTRIBUTIONS
  Float_t chi2_aux;
  Float_t chi2_l[nbins];
  Float_t chi2_c = 0;
  Float_t teoric;
  Float_t x;
  Float_t x_l[nbins];
  Float_t meas;
  Float_t res;
  Float_t residuals[nbins];
  for (int j = 0; j < nbins; j++){
    x = centre_bin[j];
    x_l[j-1] = x;
    meas = logR[j];
    teoric = loge_fit->Eval(x);
    res = meas-teoric;
    residuals[j-1] = res;
    chi2_aux = TMath::Power(res/slogR[j],2); // es esta la verdadera definicion de chi2???
    //cout << "x=" << x << " meas=" << meas << " teo=" << teoric << " aux=" << chi2_aux << " slogR=" << slogR[j] << endl;
    if(j>=min_k and j<=max_k) chi2_c = chi2_c + chi2_aux;
    chi2_l[j-1] = chi2_aux;
  }
  cout << "  -> chi2= " << chi2_c << endl;
  
  TCanvas *chi2_aux_can = new TCanvas("chi2_contributions");
  TGraph *chi2_aux_graph = new TGraph(nbins,x_l,chi2_l);
  chi2_aux_graph->SetTitle("chi2 contribution vs dT");
  chi2_aux_graph->Draw("AP");
  chi2_aux_graph->SetMarkerStyle(20);
  chi2_aux_graph->SetMarkerColor(4);
  chi2_aux_graph->GetYaxis()->SetRangeUser(0,9);
  chi2_aux_graph->GetXaxis()->SetTitle("Delta T (us)");
  chi2_aux_graph->GetYaxis()->SetTitle("chi2 contribution");
  
  TCanvas *res_can = new TCanvas("residuals");
  TGraph *res_graph = new TGraph(nbins,x_l,residuals);
  res_graph->SetTitle("Residuals vs dT");
  res_graph->Draw("AP");
  res_graph->SetMarkerStyle(20);
  res_graph->SetMarkerColor(3);
  res_graph->GetYaxis()->SetRangeUser(-3,3);
  res_can->SetGrid();
  res_graph->GetXaxis()->SetTitle("Delta T (us)");
  res_graph->GetYaxis()->SetTitle("Residual");
  
  TCanvas *hres_can = new TCanvas("residuals histo");
  TH1F *h_res = new TH1F("Histogram stats","Histogram of residuals", 10,-1,1);
  h_res->Draw();
  for (int i=0; i<nbins; i++) h_res->Fill(residuals[i]);
  h_res->GetXaxis()->SetTitle("Residuals");
  h_res->GetYaxis()->SetTitle("counts");
  
    }

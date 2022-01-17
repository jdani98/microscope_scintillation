/**************************************************************************************************
 *
 *** Filename: charges_dist.C
 *
 *** Date of creation: 29/09/2021
 *
 *** Author(s): @jdani98
 *
 *** Description:
 *   This program reads the tree .root file and returns 3 figures:
 *    - Histogram of charges for each one of the four channels
 *    - Histograms of charges inside the same plot
 *    - 2D histograms of each pair of channels to see correlations
 *   and 1 table with correlation coefficients for the pairs of channels.
 *   The charge is defined in arbitrary units as the sum of voltages recorded in one event.
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L charges_dist.C
 *       > charges_time(<fileName>)
 *      where <fileName> is the .root input file (written in quotes)
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/

#include "CRoot1.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TTree.h"
#include "TObject.h"

void charges_dist(const char* fileName){

  /// Fixed variables /////////////////////////////////////////////////////////////////////////////
  const char* tableName = "OUTPUTS/charges_dist_summary.txt";
  /////////////////////////////////////////////////////////////////////////////////////////////////

  TTree *tree = new TTree();
  TFile *file;
  if(!(file = gROOT->GetFile())) file = new TFile(fileName);
  file->GetObject("myT", tree);

  ofstream tabla;tabla.open(tableName,fstream::app);

  CPulseEvent *mypulse = new CPulseEvent();
  CScopeEvent *myscope = new CScopeEvent();
  tree->SetBranchAddress("pulse", &mypulse);
  tree->SetBranchAddress("event", &myscope);

  Long64_t nentries = tree->GetEntriesFast();
  
  TCanvas *oneVar=new TCanvas("1D charge distributions"); oneVar->Divide(2,2);
  TCanvas *oneVar_global =new TCanvas();
  TCanvas *twoVar=new TCanvas("2D charge distributions"); twoVar->Divide(3,2);
  TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
  
  TH1F * h_oneVar[4];
      h_oneVar[0] = new TH1F("h_oneVar_A","h_oneVar_A",50,0,30000);
      h_oneVar[1] = new TH1F("h_oneVar_B","h_oneVar_B",50,0,30000);
      h_oneVar[2] = new TH1F("h_oneVar_C","h_oneVar_C",50,0,30000);
      h_oneVar[3] = new TH1F("h_oneVar_D","h_oneVar_D",50,0,30000);
  
  TH2F *h_twoVar[6];
      h_twoVar[0] = new TH2F("h_A_B","h_A_B",50,0,30000,50,0,30000);
      h_twoVar[1] = new TH2F("h_A_C","h_A_C",50,0,30000,50,0,30000);
      h_twoVar[2] = new TH2F("h_A_D","h_A_D",50,0,30000,50,0,30000);
      h_twoVar[3] = new TH2F("h_B_C","h_B_C",50,0,30000,50,0,30000);
      h_twoVar[4] = new TH2F("h_B_D","h_B_D",50,0,30000,50,0,30000);
      h_twoVar[5] = new TH2F("h_C_D","h_C_D",50,0,30000,50,0,30000);
  
  for (int ev=0; ev<nentries; ev++){
    tree->GetEntry(ev);
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
    
    h_oneVar[0]->Fill(chargeA);
    h_oneVar[1]->Fill(chargeB);
    h_oneVar[2]->Fill(chargeC);
    h_oneVar[3]->Fill(chargeD);
    
    h_twoVar[0]->Fill(chargeA,chargeB);
    h_twoVar[0]->GetXaxis()->SetTitle("ChA"); h_twoVar[0]->GetYaxis()->SetTitle("ChB");
    h_twoVar[1]->Fill(chargeA,chargeC);
    h_twoVar[1]->GetXaxis()->SetTitle("ChA"); h_twoVar[1]->GetYaxis()->SetTitle("ChC");
    h_twoVar[2]->Fill(chargeA,chargeD);
    h_twoVar[2]->GetXaxis()->SetTitle("ChA"); h_twoVar[2]->GetYaxis()->SetTitle("ChD");
    h_twoVar[3]->Fill(chargeB,chargeC);
    h_twoVar[3]->GetXaxis()->SetTitle("ChB"); h_twoVar[3]->GetYaxis()->SetTitle("ChC");
    h_twoVar[4]->Fill(chargeB,chargeD);
    h_twoVar[4]->GetXaxis()->SetTitle("ChB"); h_twoVar[4]->GetYaxis()->SetTitle("ChD");
    h_twoVar[5]->Fill(chargeC,chargeD);
    h_twoVar[5]->GetXaxis()->SetTitle("ChC"); h_twoVar[5]->GetYaxis()->SetTitle("ChD");
    }
  
  for(int i=0;i<4;i++){
    oneVar->cd(i+1);
    h_oneVar[i]->Draw();
    h_oneVar[i]->SetLineColor(861);
    h_oneVar[i]->DrawPanel();
    }
  for(int i=0;i<6;i++){
    twoVar->cd(i+1);
    h_twoVar[i]->Draw("ZCOL");
    }
    
  oneVar_global->cd();
  h_oneVar[0] ->Draw();
  h_oneVar[0] ->SetLineColor(861);
  h_oneVar[0] ->SetLineWidth(2);
  h_oneVar[1] ->Draw("same");
  h_oneVar[1] ->SetLineColor(825);
  h_oneVar[1] ->SetLineWidth(2);
  h_oneVar[2]->Draw("same");
  h_oneVar[2]->SetLineColor(885);
  h_oneVar[2] ->SetLineWidth(2);
  h_oneVar[3]->Draw("same");
  h_oneVar[3]->SetLineColor(807);
  h_oneVar[3] ->SetLineWidth(2);
  legend->AddEntry(h_oneVar[0], "Canal A", "l");
  legend->AddEntry(h_oneVar[1], "Canal B", "l");
  legend->AddEntry(h_oneVar[2], "Canal C", "l");
  legend->AddEntry(h_oneVar[3], "Canal D", "l");
  legend->Draw("");
  
  
  // CORRELATION COEFFICIENTS
  Double_t corr_AB=h_twoVar[0]->GetCorrelationFactor();
  Double_t corr_AC=h_twoVar[1]->GetCorrelationFactor();
  Double_t corr_AD=h_twoVar[2]->GetCorrelationFactor();
  Double_t corr_BC=h_twoVar[3]->GetCorrelationFactor();
  Double_t corr_BD=h_twoVar[4]->GetCorrelationFactor();
  Double_t corr_CD=h_twoVar[5]->GetCorrelationFactor();
  
  TDatime d;
  int day = d.GetDate();
  int tim = d.GetTime();
  
  tabla<<"\n\n*************************************************************"<<endl;
  tabla << " Date and time (AAMMDD HHMMSS): " << day << " " << tim << "  File: " << fileName << endl;
  tabla << "Nevents= " << nentries << endl;
  tabla<<"Correlation coefficients"<<endl;
  tabla<< "A-B: "<<corr_AB<<"\n"<< "A-C: "<<corr_AC<<"\n"<< "A-D: "<<corr_AD<<"\n"<< "B-C: "<<corr_BC<<"\n"<< "B-D: "<<corr_BD<<"\n"<< "C-D: "<<corr_CD<<endl;

  tabla.close();

  }

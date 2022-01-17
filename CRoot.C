/**************************************************************************************************
 *
 *** Filename: CRoot.C
 *
 *** Date of creation: 06/2021
 *
 *** Author(s): Héctor Álvarez-Pol @hapol. Copyright (C) IGFAE (Univ. Santiago de Compostela)
 *
 *** Description:
 *   This program reads a .txt datafile from ps3000aCon software and creates the tree .root file
 *
 *** How to tun?:
 *   1) Open ROOT in the directory where this file is
 *   2) Type the following commands:
 *       > .L CRoot.C
 *       > digitEvents(<inputFile>,<outputFile>)
 *      where <fileName> is the .txt input file (written in quotes) and <outputFile> is the name of
 *      the output file (written in quotes)
 *   If error occurs try to re-run ROOT.
 *
 *************************************************************************************************/

#include "CRoot1.h"

void digitEvents(const char* inputFile, const char* outputFile){
        // Digitization event loop

        gROOT->SetStyle("Default");
        gStyle->SetOptTitle(0);
        gStyle->SetOptStat(0);
        gStyle->SetOptFit(0);

        TFile *hfile = new TFile(outputFile,"RECREATE","Test");

        //reading the input file
        FILE * fp;
        char * line = NULL;
        size_t len = 0;
        ssize_t read;
        unsigned long int trTime = 0;
        int time=0, chA=0, chB=0, chC=0, chD=0;
        CScopeEvent* scopeEvent = 0;
        CPulseEvent* pulseEvent = 0;
        Int_t maxAmp;
        Int_t threshold;
        cout<<"Threshold: "<<endl;
        cin>>threshold;
        cout<<"maxAmp: "<<endl;
        cin>>maxAmp;

        TTree* myT = new TTree("myT","ScopeEvents");
        auto branchScope = myT->Branch("event", &scopeEvent);
        auto branchPulse = myT->Branch("pulse", &pulseEvent);


        // TH1F *hist  = new TH1F("hist","ampA",1000,-2500,0);

        vector<Float_t> x;

        fp = fopen(inputFile, "r");
        if (fp == NULL)
                exit(EXIT_FAILURE);

        while ((read = getline(&line, &len, fp)) != -1) {
                //reading lines one by one and checking the number of words
                if (5 != sscanf( line, "%i %i %i %i %i", &time, &chA, &chB, &chC, &chD)) {
                        if (1 == sscanf(line,"%lu",&trTime)) {
                                if(scopeEvent && scopeEvent->isCorrect()) {
                                        pulseEvent = new CPulseEvent(scopeEvent,maxAmp,threshold);
                                        myT->Fill();
                                        // scopeEvent->Print();
                                        delete scopeEvent;
                                        delete pulseEvent;
                                }
                                scopeEvent = new CScopeEvent(trTime);
                        }
                }
                else{
                        if(time>150) continue;
                        scopeEvent->AddDigits(time, chA, chB, chC, chD);
                        //cout << time << " " << chA <<" " << chB << " "<< chC <<" " << chD<< endl;
                }
        }
        // scopeEvent->Print();
         pulseEvent = new CPulseEvent(scopeEvent,maxAmp,threshold);
        myT->Fill();

// Long64_t nentries = myT->GetEntriesFast();
// for(int i=0; i<nentries;i++){
//   pulseEvent = new CPulseEvent(scopeEvent,maxAmp,threshold);
//   myT->Fill();
// }


        //hfile.Write();
        myT->Write();
        // hist->Write();
        hfile->Close();



        fclose(fp);
        exit(EXIT_SUCCESS);




}



int main(){
  digitEvents("doc.txt","out2.root");
  exit(0);
}

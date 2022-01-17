///////////////////////////////////////////////////////////////////
//*-- AUTHOR : Hector Alvarez-Pol
//*-- Date: 06/2021
//*-- Copyright: IGFAE (Univ. Santiago de Compostela)
//

// #ifndef CROOTT_H
// #define CROOTT_H

#include <TObject.h>
#include <TMath.h>
#include <iostream>
#include "TTree.h"
#include "TROOT.h"
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TControlBar.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TTimer.h>

using namespace std;

Int_t C_DEBUG=0; //A global DEBUG variable:
   //0 absolutly no output (quiet)
   //1 DEBUG mode

class CScopeEvent;
class CPulseEvent;


class CScopeEvent : public TObject {

public:
CScopeEvent();
CScopeEvent(unsigned long int trTime);
~CScopeEvent();

unsigned long int GetEventTime(){
return eventTime;
}
Int_t GetDataPoints(){
return dataPoints;
}

vector<int> GetTimeBase(){
return timeBase;
}
vector<int> GetAmpA(){
return ampA;
}
vector<int> GetAmpB(){
return ampB;
}
vector<int> GetAmpC(){
return ampC;
}
vector<int> GetAmpD(){
return ampD;
}

Bool_t isCorrect(){
return correct;
}

void SetEventTime(Int_t time){
eventTime = time;
}
void SetDataPoints(Int_t data){
dataPoints = data;
}

void AddDigits(int time, int chA, int chB, int chC, int chD);
void Print();

private:
unsigned long int eventTime;
vector<int> timeBase;
vector<int> ampA;
vector<int> ampB;
vector<int> ampC;
vector<int> ampD;
Int_t dataPoints;
Bool_t correct;

ClassDef(CScopeEvent,1);
};


CScopeEvent::CScopeEvent(){
if(C_DEBUG) cout << "Enters CScopeEvent::CScopeEvent()" << endl;
correct=kFALSE;
if(C_DEBUG) cout << "Exits CScopeEvent::CScopeEvent()" << endl;
}

CScopeEvent::CScopeEvent(unsigned long int trTime){
if(C_DEBUG) cout << "Enters CScopeEvent::CScopeEvent(int)" << endl;
dataPoints=0;
eventTime = trTime;
correct=kTRUE;
if(C_DEBUG) cout << "Exits CScopeEvent::CScopeEvent(int)" << endl;
}

CScopeEvent::~CScopeEvent(){
if(C_DEBUG) cout << "Enters CScopeEvent::~CScopeEvent()" << endl;
if(C_DEBUG) cout << "Exits CScopeEvent::~CScopeEvent()" << endl;
}
void CScopeEvent::AddDigits(int time, int chA, int chB, int chC, int chD){
timeBase.push_back(time);
ampA.push_back(chA);
ampB.push_back(chB);
ampC.push_back(chC);
ampD.push_back(chD);
dataPoints++;
}

void CScopeEvent::Print(){
cout << "Event Time: " << eventTime << endl;
cout << "Vector sizes: " << timeBase.size() << " " << ampA.size() << " "
<< ampB.size() << " " << ampC.size() << " " << ampD.size() << endl;
for (int i = 0; i < timeBase.size(); i++)
  cout << timeBase[i] << " " << ampA[i] << " " << ampB[i] << " " << ampC[i] << " " << ampD[i] << endl;
}


class CPulseEvent : public TObject {

public:
CPulseEvent();
CPulseEvent(CScopeEvent* anEvent,Int_t maxAmp,Int_t threshold);
~CPulseEvent();

Int_t GetEventTime(){return eventTime;}

vector<Float_t> GetAmpAtMin_A(){return ampAtMin_A;}
vector<Float_t> GetAmpAtMin_B(){return ampAtMin_B;}
vector<Float_t> GetAmpAtMin_C(){return ampAtMin_C;}
vector<Float_t> GetAmpAtMin_D(){return ampAtMin_D;}

vector<Float_t> GetTimeAtMin_A(){return timeAtMin_A;}
vector<Float_t> GetTimeAtMin_B(){return timeAtMin_B;}
vector<Float_t> GetTimeAtMin_C(){return timeAtMin_C;}
vector<Float_t> GetTimeAtMin_D(){return timeAtMin_D;}

vector<Float_t> GetWidth_A(){return width_A;}
vector<Float_t> GetWidth_B(){return width_B;}
vector<Float_t> GetWidth_C(){return width_C;}
vector<Float_t> GetWidth_D(){return width_D;}

// int GetPeak(){return peak;}

private:
int searchPeak(int threshold,vector<int> time,vector<int> amp,vector<Float_t>* timeAtMin,vector<Float_t>* ampAtMin,vector<Float_t>* widthAtMin);
void funcFitMin(vector<int>* x, vector<int>* y, vector<Float_t>* fitMin);


 unsigned long int eventTime;
// int peak;
vector<Float_t> ampAtMin_A;
vector<Float_t> ampAtMin_B;
vector<Float_t> ampAtMin_C;
vector<Float_t> ampAtMin_D;
vector<Float_t> timeAtMin_A;
vector<Float_t> timeAtMin_B;
vector<Float_t> timeAtMin_C;
vector<Float_t> timeAtMin_D;
vector<Float_t> width_A;
vector<Float_t> width_B;
vector<Float_t> width_C;
vector<Float_t> width_D;



ClassDef(CPulseEvent,1);
};


CPulseEvent::CPulseEvent(){
if(C_DEBUG) cout << "Enters CPulseEvent::CPulseEvent()" << endl;
if(C_DEBUG) cout << "Exits CPulseEvent::CPulseEvent()" << endl;
}


CPulseEvent::~CPulseEvent(){
if(C_DEBUG) cout << "Enters CPulseEvent::CPulseEvent()" << endl;
if(C_DEBUG) cout << "Exits CPulseEvent::CPulseEvent()" << endl;
}






// Cálculo de mínimos: para que sea versátil, voulle meter directamente a amplitude e os tempos, non o scope enteiro
// Quero que devolva unha amplitude no mínimo, un tempo no mínimo e unha anchura, entonces pode devolverme un array de 3 datos

int CPulseEvent::searchPeak(int threshold,vector<int> time,vector<int> amp,vector<Float_t>* timeAtMin,vector<Float_t>* ampAtMin,vector<Float_t>* widthAtMin) {

vector<Float_t>* fitMin;
vector<int>* x;
vector<int>* y;
int time_aux=0;
int i_0=0;
int peak=0;

x= new vector<int>; y= new vector<int>;
fitMin = new  vector<Float_t>;

for(int i=0; i<amp.size()-1; i++) {//entro neste bucle e o primeiro que teño que comprobar é si baixa ou sube no primeiro paso
  // cout<<threshold<< "*************"<<endl;
  // cout<<"******************\n amplitude en i"<<amp[i]<<" quen é i?   "<<i<<endl;
  if(amp[i+1]<amp[i]) {
          while(amp[i+1]<amp[i]) {
                  i++;
                  if(i>amp.size()-2) break;//esto vai a romper o while, pero non rompe o if
          }


            //En caso de que deixáramos de baixar, e sigamos tendo índices no vector de amplitudes, teremos dous casos, pero antes de estudialos, temos
            //que asegurarnos de que non estamos collendo ruido
          if(amp[i]<threshold) {// en caso de que a amplitude non sexa menor que o threshold, non pasará nada (sumamos 20 ns e continuamos co bucle)

                //se si que é menor que o threshold distiguimos os dous casos:
              if(amp[i+1]>amp[i]) { //primeiro caso, que a amplitude posterior (que temos asegurado que existe) sexa estrictamente maior

                    //empezamos a construcción dos vectores. Para iso creamos os obxectos na memoria reservada

                    x->push_back(time[i]); y->push_back(amp[i]); //Metemos o primeiro valor, que debe ser o máis prox ao mínimo

                      //chequeos varios para asegurarnos de que non se collen valores fóra dos vectores sin querer
                    if(0<time[i+1]-time[i]&& time[i+1]-time[i]<20 ) {
                            x->push_back(time[i+1]); y->push_back(amp[i+1]);
                    }

                    if(0<time[i]-time[i-1] && time[i]-time[i-1]<20 ) {
                      x->push_back(time[i-1]); y->push_back(amp[i-1]);
                    }

                    if (i<amp.size()-2) {
                        if(amp[i+2]>amp[i+1]){
                            if( 0<(time[i+2]-time[i+1])&&(time[i+2]-time[i+1])<20 ) {
                                    x->push_back(time[i+2]);
                                    y->push_back(amp[i+2]);
                            }
                        }
                    }

                   if(i>1) {
                      if(amp[i-2]>amp[i-1]){
                          if(0<(time[i-1]-time[i-2])&&(time[i-1]-time[i-2])<20) {
                                    x->push_back(time[i-2]);
                                    y->push_back(amp[i-2]);
                          }
                      }
                  }


               }else if(amp[i+1]==amp[i]) { //segundo caso: pode ser que estemos na aplitude máxima, ou que chegáramos a un val, logo temos que salir del
                      i_0=i;//indice de referencia

                      while(amp[i+1]==amp[i]){
                            i++;
                            if (i>amp.size()-2) break;
                      }//salimos do while, pero tamén temos que salir do if


                      //chegados a este punto, pode ser que volvamos a baixar, ou que empecemos a subir
                      if(amp[i+1]<amp[i]) {//en caso de que volvamos a baixar, empezamos o bucle de novo
                                continue;
                      }
                      else{ //no caso en que ahora estemos en subida, creamos vectores e facemos o fitting

                            x->push_back(time[i_0]);  y->push_back(amp[i_0]);


                            if(0<time[i+1]-time[i] && time[i+1]-time[i]<20 ) {
                                    x->push_back(time[i+1]); y->push_back(amp[i+1]);
                            }

                            if(0<time[i_0]-time[i_0-1] && time[i_0]-time[i_0-1] <20 ) {
                                    x->push_back(time[i_0-1]); y->push_back(amp[i_0-1]);
                            }

                            if (i<amp.size()-2) {
                                if(amp[i+2]>amp[i+1]){
                                      if( 0<(time[i+2]-time[i+1]) && (time[i+2]-time[i+1])<20 ) {
                                              x->push_back(time[i+2]);
                                              y->push_back(amp[i+2]);
                                          }
                                  }
                            }

                            if(i>1) {
                              if(amp[i_0-2]>amp[i_0-1]){
                                    if(0<(time[i_0-1]-time[i_0-2]) && (time[i_0-1]-time[i_0-2])<20 ) {
                                            x->push_back(time[i_0-2]);
                                            y->push_back(amp[i_0-2]);
                                    }
                              }
                            }
                          }

                      }
                  }


                  //No caso de haber construído os vectores (fora da maneira que fora), facemos o fitting e devolvémolo
                  if(x->size()>2) {

                          funcFitMin(x, y,fitMin);
                          // cout<<"primeiro if"<<endl;
                          // cout<<x->at(0)<<"  "<<x->at(1)<<"   "<<x->at(2)<<"   "<<y->at(0)<<"   "<<y->at(1)<<"   "<<y->at(2)<<endl;
                          peak++;
                          timeAtMin->push_back(fitMin->at(0));
                          ampAtMin->push_back(fitMin->at(1));
                          widthAtMin->push_back(fitMin->at(2));
                          fitMin->clear();
                    }

            }

            x->clear();y->clear();
            //sexa cal sexa o resultado de haber feito ou non un fit pol2, temos que sumar os 20 ns
            time_aux=time[i];
            while(time[i]<time_aux+30) {
                    i=i+1;
                    if(i>amp.size()-2) break;
            }

  }

delete x; delete y;
delete fitMin;
return peak;

}

void CPulseEvent::funcFitMin(vector<int>* x,vector<int>* y, vector<Float_t>* fitMin){

int on=1;


if(on==0){
int tamao=x->size();
auto gr = new TGraph();

  for (int i = 0; i < tamao; i++) {
    gr->SetPoint(i, (Float_t)x->at(i),(Float_t)y->at(i));
     // cout<<" points that are being setted"<<" x_i:  "<<x->at(i)<<"   y_i:  "<<y->at(i)<<endl;
  }
  // gr->Fit("pol2","Q");
  // TF1 *gfit = (TF1 *)gr->GetFunction("pol2");
  TFitResultPtr parametros = gr->Fit("pol2","QS");
  // cout<<"Parametro 1: "<<parametros->Value(0)<<"  parametro 2. "<<parametros->Value(1)<< "   parametro 3:  "<<parametros->Value(2)<<endl;
  fitMin->push_back(-parametros->Value(1) / (2*parametros->Value(2)));//cout<<"suponse que é o tempo min"<<fitMin->at(0)<<endl;
  fitMin->push_back(parametros->Value(0) - parametros->Value(1)*parametros->Value(1) / (4*parametros->Value(2)));//cout<<"suponse que é a amp min"<<fitMin->at(1)<<endl;
  fitMin->push_back(sqrt(parametros->Value(2)*parametros->Value(2)*(parametros->Value(1)*parametros->Value(1)-4*parametros->Value(2)*parametros->Value(0)))/(sqrt(2)*parametros->Value(2)*parametros->Value(2)));

delete gr;

}else{
Int_t errorA=0;
Float_t x1=(Float_t)x->at(0); Float_t x2=(Float_t)x->at(1); Float_t x3=(Float_t)x->at(2);
Float_t y1=(Float_t)y->at(0); Float_t y2=(Float_t)y->at(1); Float_t y3=(Float_t)y->at(2);

Float_t denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
if(denom==0)
{
    cout << "ERROR1" << endl;
}

Float_t A     = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;

if(A==0){
      cout << "ERROR2     " <<x1<<"  "<<x2<<"   "<<x3<<"    "<<y1<<" "<<y2<<" "<<y3<< endl;
      if(x->size()>3){
            x3=(Float_t)x->at(3);
            y3=(Float_t)y->at(3);
            A = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;

            if(A==0){
                if(x->size()>4){
                    x2=(Float_t)x->at(4);
                    y2=(Float_t)y->at(4);
                  }
            A = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;

            if(A==0) errorA=1;

            }
          }
       else errorA=1;
  }

if(errorA==0){
    Float_t B     = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3)) / denom;
    Float_t C     = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom;
    fitMin->push_back(-B / (2*A));
    fitMin->push_back(C - B*B / (4*A));
    fitMin->push_back( sqrt(A*A*(B*B-4*A*C))/(sqrt(2)*A*A));
} else{
    fitMin->push_back(-1);fitMin->push_back(1);fitMin->push_back(-1);
}

}

}



CPulseEvent::CPulseEvent(CScopeEvent* anEvent, Int_t maxAmp, Int_t threshold){

if(C_DEBUG) cout << "Enters CPulseEvent::CPulseEvent(CScopeEvent* , Int_t ,unsigned long int ,Int_t )" << endl;
unsigned long int eventTime=anEvent->GetEventTime();

vector<Float_t>* timeAtMin;
vector<Float_t>* ampAtMin;
vector<Float_t>* widthAtMin;




timeAtMin = new vector<Float_t>;
ampAtMin = new vector<Float_t>;
widthAtMin = new vector<Float_t>;

// ******************************** CANAL A ************************************


int peak=searchPeak(threshold,anEvent->GetTimeBase(),anEvent->GetAmpA(),timeAtMin,ampAtMin,widthAtMin);
if (peak==0) {
  // cout<<"\nNot minimum found\n"<<endl;
  // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
  timeAtMin_A.push_back(-1);
  ampAtMin_A.push_back(1);
  width_A.push_back(-1);
}else if(peak>0){
  for(int i=0; i<peak;i++){
          if(timeAtMin->at(i)>0 && ampAtMin->at(i)<0) {
                  if(ampAtMin->at(i)>maxAmp-3000 || timeAtMin->at(i)<1000) {
                          // cout<<"\nFound algún minimum\n"<<"minimo número "<<i+1<<endl;
                          // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
                          timeAtMin_A.push_back(timeAtMin->at(i));
                          ampAtMin_A.push_back(ampAtMin->at(i));
                          width_A.push_back(widthAtMin->at(i));
                  }
          }
          else{
                   cout<<"Problema no cálculo da amplitude do CANAL D no event time :   "<<eventTime<<endl;
                   cout<<" time "<<timeAtMin->at(i)<<"  amp  "<<ampAtMin->at(i)<<endl;
                  timeAtMin_A.push_back(-1);
                  ampAtMin_A.push_back(1);
                  width_A.push_back(-1);
          }
        }

  }


  timeAtMin->clear();
  ampAtMin->clear();
  widthAtMin->clear();
//****************************************** Canal B *******************************************************





peak=searchPeak(threshold,anEvent->GetTimeBase(),anEvent->GetAmpB(),timeAtMin,ampAtMin,widthAtMin);
if (peak==0) {
  // cout<<"\nNot minimum found\n"<<endl;
  // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
  timeAtMin_B.push_back(-1);
  ampAtMin_B.push_back(1);
  width_B.push_back(-1);
}else if(peak>0){
  for(int i=0; i<peak;i++){
          if(timeAtMin->at(i)>0 && ampAtMin->at(i)<0) {
                  if(ampAtMin->at(i)>maxAmp-3000 || timeAtMin->at(i)<1000) {
                          // cout<<"\nFound algún minimum\n"<<"minimo número "<<i+1<<endl;
                          // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
                          timeAtMin_B.push_back(timeAtMin->at(i));
                          ampAtMin_B.push_back(ampAtMin->at(i));
                          width_B.push_back(widthAtMin->at(i));
                  }
          }
          else{
                   cout<<"Problema no cálculo da amplitude do CANAL B no event time :   "<<eventTime<<endl;
                   cout<<" time "<<timeAtMin->at(i)<<"  amp  "<<ampAtMin->at(i)<<endl;
                  timeAtMin_B.push_back(-1);
                  ampAtMin_B.push_back(1);
                  width_B.push_back(-1);
          }
        }

  }


  timeAtMin->clear();
  ampAtMin->clear();
  widthAtMin->clear();


// ***************** CANAL C *******************



peak=searchPeak(threshold,anEvent->GetTimeBase(),anEvent->GetAmpC(),timeAtMin,ampAtMin,widthAtMin);

if (peak==0) {
  // cout<<"\nNot minimum found\n"<<endl;
  // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
  timeAtMin_C.push_back(-1);
  ampAtMin_C.push_back(1);
  width_C.push_back(-1);
}else if(peak>0){
  for(int i=0; i<peak;i++){
          if(timeAtMin->at(i)>0 && ampAtMin->at(i)<0) {
                  if(ampAtMin->at(i)>maxAmp-3000 || timeAtMin->at(i)<1000) {
                          // cout<<"\nFound algún minimum\n"<<"minimo número "<<i+1<<endl;
                          // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
                          timeAtMin_C.push_back(timeAtMin->at(i));
                          ampAtMin_C.push_back(ampAtMin->at(i));
                          width_C.push_back(widthAtMin->at(i));
                  }
          }
          else{
                   cout<<"Problema no cálculo da amplitude do CANAL C no event time :   "<<eventTime<<endl;
                   cout<<" time "<<timeAtMin->at(i)<<"  amp  "<<ampAtMin->at(i)<<endl;
                  timeAtMin_C.push_back(-1);
                  ampAtMin_C.push_back(1);
                  width_C.push_back(-1);
          }
        }

  }




timeAtMin->clear();
ampAtMin->clear();
widthAtMin->clear();

// ******************** CANAL D *************************



peak=searchPeak(threshold,anEvent->GetTimeBase(),anEvent->GetAmpD(),timeAtMin,ampAtMin,widthAtMin);

if (peak==0) {
  // cout<<"\nNot minimum found\n"<<endl;
  // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
  timeAtMin_D.push_back(-1);
  ampAtMin_D.push_back(1);
  width_D.push_back(-1);
}else if(peak>0){
  for(int i=0; i<peak;i++){
          if(timeAtMin->at(i)>0 && ampAtMin->at(i)<0) {
                  if(ampAtMin->at(i)>maxAmp-3000 || timeAtMin->at(i)<1000) {
                          // cout<<"\nFound algún minimum\n"<<"minimo número "<<i+1<<endl;
                          // cout<<"*********************************\n introduciouse un novo dato ahora\n *************************************"<<endl;
                          timeAtMin_D.push_back(timeAtMin->at(i));
                          ampAtMin_D.push_back(ampAtMin->at(i));
                          width_D.push_back(widthAtMin->at(i));
                  }
          }
          else{
                   cout<<"Problema no cálculo da amplitude do CANAL D no event time :   "<<eventTime<<endl;
                   cout<<" time "<<timeAtMin->at(i)<<"  amp  "<<ampAtMin->at(i)<<endl;
                  timeAtMin_D.push_back(-1);
                  ampAtMin_D.push_back(1);
                  width_D.push_back(-1);
          }
        }

  }

  //
  // timeAtMin->clear();
  // ampAtMin->clear();
  // widthAtMin->clear();



delete timeAtMin;
delete ampAtMin;
delete widthAtMin;





if(C_DEBUG) cout << "Exits CPulseEvent::CPulseEvent(CScopeEvent* , Int_t ,unsigned long int ,Int_t )" << endl;
}

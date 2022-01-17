///////////////////////////////////////////////////////////////////
//*-- AUTHOR : Hector Alvarez-Pol
//*-- Date: 06/2021
//*-- Copyright: IGFAE (Univ. Santiago de Compostela)
//
#include <TObject.h>
#include <TMath.h>


using namespace std;

Int_t C_DEBUG=0; //A global DEBUG variable:
                 //0 absolutly no output (quiet)
                 //1 DEBUG mode

class CScopeEvent;
class CPulseEvent;
Float_t* parabola(int x1, int x2, int x3, int y1, int y2, int y3);


class CScopeEvent : public TObject {

public:
CScopeEvent();
CScopeEvent(unsigned long int trTime);
~CScopeEvent();

unsigned long int GetEventTime(){return eventTime;}
Int_t GetDataPoints(){return dataPoints;}

vector<int> GetTimeBase(){return timeBase;}
vector<int> GetAmpA(){return ampA;}
vector<int> GetAmpB(){return ampB;}
vector<int> GetAmpC(){return ampC;}
vector<int> GetAmpD(){return ampD;}

Bool_t isCorrect(){return correct;}

void SetEventTime(Int_t time){eventTime = time;}
void SetDataPoints(Int_t data){dataPoints = data;}

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
  CPulseEvent(CScopeEvent* anEvent,Int_t threshold,unsigned long int trTime,Int_t Amplitude);
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

private:
  Float_t* getParamParabola(int x1, int x2, int x3, int y1, int y2, int y3);

  unsigned long int eventTime;

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


Float_t* CPulseEvent::getParamParabola(int x1, int x2, int x3, int y1, int y2, int y3) {
        // Float_t* parabola = (Float_t *) malloc(3*sizeof(Float_t));
        Float_t* parabola = new Float_t[3];
        //Función que devolve as coordenadas x e y do máximo dunha parábola dada por tres puntos, ademáis de dar a anchura a altura metade
        Float_t denom = (x1 - x2) * (x1 - x3) * (x2 - x3);
        if(denom==0) cout << "ERROR1" << endl;
        //cout<<x1<<" "<<x2<<" "<<x3<<endl;
        //cout<<denom<<endl;
        Float_t A     = (x3 * (y2 - y1) + x2 * (y1 - y3) + x1 * (y3 - y2)) / denom;
        Float_t B     = (x3*x3 * (y1 - y2) + x2*x2 * (y3 - y1) + x1*x1 * (y2 - y3)) / denom;
        Float_t C     = (x2 * x3 * (x2 - x3) * y1 + x3 * x1 * (x3 - x1) * y2 + x1 * x2 * (x1 - x2) * y3) / denom;
        if(A==0) cout << "ERROR2" << endl;
        parabola[0] = -B / (2*A);
        parabola[1] = C - B*B / (4*A);
        parabola[2] = sqrt(A*A*(B*B-4*A*C))/(sqrt(2)*A*A);
        return parabola;
}

CPulseEvent::CPulseEvent(CScopeEvent* anEvent, Int_t threshold,unsigned long int trTime,Int_t Amplitude){

  if(C_DEBUG) cout << "Enters CPulseEvent::CPulseEvent(CScopeEvent* , Int_t ,unsigned long int ,Int_t )" << endl;
  eventTime=trTime;


//************************************CANAL A*********************************

  //Amplitude e tempo no primeiro mínimo para o canal A
  int i=0;
  int k=1;
  while (i<anEvent->GetAmpA().size()-1){
    if(threshold>anEvent->GetAmpA()[i] ){
          if(anEvent->GetAmpA()[i+1]<anEvent->GetAmpA()[i])
          {
            i++;
          }
          else {
            if(i==0) i+1;

            if(anEvent->GetAmpA()[i]<=Amplitude)
            {
              while(anEvent->GetAmpA()[i+k]<=Amplitude)
                {
                  k++;
                  if(k>anEvent->GetAmpA().size()-i) break;
                }
            }
            else
            {
              k=1;
            }

                  Float_t* paramsParabola = getParamParabola(anEvent->GetTimeBase()[i-1],anEvent->GetTimeBase()[i],anEvent->GetTimeBase()[i+k],
                    anEvent->GetAmpA()[i-1],anEvent->GetAmpA()[i],anEvent->GetAmpA()[i+k]);

                  timeAtMin_A.push_back(paramsParabola[0]);
                  ampAtMin_A.push_back(paramsParabola[1]);
                  width_A.push_back(paramsParabola[2]);
                  delete[] paramsParabola;
                  i=i+20;
                  k=1;
          }
        }
        else
        {
          timeAtMin_A.push_back(-1.);
          ampAtMin_A.push_back(1.);
          width_A.push_back(-1.);
          i++;
        }
      }


//************************************CANAL B*********************************

  //Amplitude e tempo no primeiro mínimo para o canal B
  i=0;
  while (i<anEvent->GetAmpB().size()-1){
    if(threshold>anEvent->GetAmpB()[i] ){
          if(anEvent->GetAmpB()[i+1]<anEvent->GetAmpB()[i])
          {
            i++;
          }
          else {
            if(i==0) i+1;

            if(anEvent->GetAmpB()[i]<=Amplitude)
            {
              while(anEvent->GetAmpB()[i+k]<=Amplitude)
                {
                  k++;
                  if(k>anEvent->GetAmpB().size()-i) break;
                }
            }
            else
            {
              k=1;
            }
                  if(i==0) i+1;
                  Float_t* paramsParabola = getParamParabola(anEvent->GetTimeBase()[i-1],anEvent->GetTimeBase()[i],anEvent->GetTimeBase()[i+k],
                    anEvent->GetAmpB()[i-1],anEvent->GetAmpB()[i],anEvent->GetAmpB()[i+k]);

                  timeAtMin_B.push_back(paramsParabola[0]);
                  ampAtMin_B.push_back(paramsParabola[1]);
                  width_B.push_back(paramsParabola[2]);
                  delete[] paramsParabola;
                  i=i+20;
                  k=1;
          }
        }
        else
        {
          timeAtMin_B.push_back(-1.);
          ampAtMin_B.push_back(1.);
          width_B.push_back(-1.);
          i++;
        }
      }



  //************************************CANAL C*********************************

    //Amplitude e tempo no primeiro mínimo para o canal C
    i=0;
    while (i<anEvent->GetAmpC().size()-1){
      if(threshold>anEvent->GetAmpC()[i] ){
            if(anEvent->GetAmpC()[i+1]<anEvent->GetAmpC()[i])
            {
              i++;
            }
            else {
              if(i==0) i+1;

              if(anEvent->GetAmpC()[i]<=Amplitude)
              {
                while(anEvent->GetAmpC()[i+k]<=Amplitude)
                  {
                    k++;
                    if(k>anEvent->GetAmpC().size()-i) break;
                  }
              }
              else
              {
                k=1;
              }
                    if(i==0) i+1;
                    Float_t* paramsParabola = getParamParabola(anEvent->GetTimeBase()[i-1],anEvent->GetTimeBase()[i],anEvent->GetTimeBase()[i+k],
                      anEvent->GetAmpC()[i-1],anEvent->GetAmpC()[i],anEvent->GetAmpC()[i+k]);

                    timeAtMin_C.push_back(paramsParabola[0]);
                    ampAtMin_C.push_back(paramsParabola[1]);
                    width_C.push_back(paramsParabola[2]);
                    delete[] paramsParabola;
                    i=i+20;
                    k=1;
            }
          }
          else
          {
            timeAtMin_C.push_back(-1.);
            ampAtMin_C.push_back(1.);
            width_C.push_back(-1.);
            i++;
          }
        }




    //************************************CANAL D*********************************

      //Amplitude e tempo no primeiro mínimo para o canal D
      i=0;
      while (i<anEvent->GetAmpD().size()-1){
        if(threshold>anEvent->GetAmpD()[i] ){
              if(anEvent->GetAmpD()[i+1]<anEvent->GetAmpD()[i])
              {
                i++;
              }
              else {
                if(i==0) i+1;

                if(anEvent->GetAmpD()[i]<=Amplitude)
                {
                  while(anEvent->GetAmpD()[i+k]<=Amplitude)
                    {
                      k++;
                      if(k>anEvent->GetAmpD().size()-i) break;
                    }
                }
                else
                {
                  k=1;
                }
                      if(i==0) i+1;
                      Float_t* paramsParabola = getParamParabola(anEvent->GetTimeBase()[i-1],anEvent->GetTimeBase()[i],anEvent->GetTimeBase()[i+k],
                        anEvent->GetAmpD()[i-1],anEvent->GetAmpD()[i],anEvent->GetAmpD()[i+k]);

                      timeAtMin_D.push_back(paramsParabola[0]);
                      ampAtMin_D.push_back(paramsParabola[1]);
                      width_D.push_back(paramsParabola[2]);
                      delete[] paramsParabola;
                      i=i+20;
                      k=1;
              }
            }
            else
            {
              timeAtMin_D.push_back(-1.);
              ampAtMin_D.push_back(1.);
              width_D.push_back(-1.);
              i++;
            }
            }

            if(C_DEBUG) cout << "Exits CPulseEvent::CPulseEvent(CScopeEvent* , Int_t ,unsigned long int ,Int_t )" << endl;
}

#include "kuramoto.h"
#include <vector>
#include <random>
using namespace std::complex_literals;

static int N;  //numero di oscillatori
static double K; //parametro di accoppiamento


std::vector<Oscillator> Oscillator::SetSystem(double& a, double& b){  //funzione che genera la fase di tutti gli oscillatori [mi sembra che il fatto che avessero tutti la stessa fase iniziale 
                                                                     //non cambiasse i risultati del modello, da rivedere] e le frequenze tramite g(w) 
    std::vector<Oscillator> System(N);  //vettore di oscillatori
    std::random_device seed; 
    std::random_device rndm;                        
    std::uniform_int_distribution<double> Phases(0, 2*M_PI);
    double THETA = Phases(seed);  //in questo modo tutti gli oscillatori partono con stessa fase; sennò mettere dentro il for e avranno fasi diverse e uniformi
    for(int i=0; i<=N; ++i){
        System[i].SetTheta(THETA);  
    }
    std::cauchy_distribution<double> Lorentz_g(a,b); //a,b parametri della lorentziana (forse non è esattamente questa quella da usare, in caso la definiamo noi)
    for(int i=0; i<=N; ++i){
        double omega = Lorentz_g(rndm);
        System[i].Setw(omega);
    }
    return System;
}

//definire i parametri di campo medio
void Oscillator::MeanField(std::vector<Oscillator>& System) {

double COS;
double SIN;
for(int i=0; i<=N; ++i){
    COS += std::cos(System[i].theta_);  //COS è la somma di tutti i coseni di theta_i
    SIN += std::cos(System[i].theta_);  //stesso per SIN
}
double X = COS/N;
double Y = SIN/N;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY. Da qui lo riportiamo in exp
double r = std::sqrt(X*X + Y*Y);   
double psi = std::atan(Y/X);        //non è così semplice calcolare la fase, non so se esiste qualche funzione che considera i vari casi possibili
std::complex<double> MF = std::polar(r, psi);  //polar costruisce un exp complesso, MF sta per mean field non per Matteo Falcioni

}


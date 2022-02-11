#include "kuramoto.h"
#include <vector>
using namespace std::complex_literals;

static int N;

//definire i parametri di campo medio

void Oscillator::MeanField() {

std::vector<Oscillator> System(N); //insieme degli oscillatori
double COS;
double SIN;

for(int i=0; i<=N; ++i){
    COS += std::cos(System[i].theta_);  //COS è la somma di tutti i coseni di theta_i
    SIN += std::cos(System[i].theta_);  //stesso per SIN
}
double r = std::sqrt(COS*COS + SIN*SIN);   //possiamo vedere la somma dei fasori come COS + iSIN = X + iY. Da qui lo riportiamo in exp
double psi = std::atan(SIN/COS);
std::complex<double> MF = std::polar(r, psi);

std::complex<double> Z;  //Z è il parametro d'ordine, cioè X rinormalizzato a N. Deve essere espresso come re^i*Psi però

}

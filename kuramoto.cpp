#include "kuramoto.h"
#include <vector>

static int N;

//definire i parametri di campo medio

void Oscillator::MeanField() {

std::vector<Oscillator> System(N); //insieme degli oscillatori
std::complex<double> X = 0;
for(int i=0; i<=N; ++i){
    X += System[i].phasor;  //X è la somma di tutti gli esponenziali con fase theta_i
}
std::complex<double> Z = (1/N)*X;  //Z è il parametro d'ordine, cioè X rinormalizzato a N. Deve essere espresso come re^i*Psi però

}

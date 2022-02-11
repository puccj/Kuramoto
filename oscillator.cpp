#include "oscillator.h"
#include <vector>
#include <random>
using namespace std::complex_literals;

//int Oscillator::_N;  //numero di oscillatori
//double Oscillator::_K; //parametro di accoppiamento

Oscillator::Oscillator(double freq, double phase): _freq{freq} {
  if (freq == -1) {    
    std::random_device seed;
    double l1 = 1;
    double l2 = 2;  //non ho idea di quali possano essere valori ragionevoli per i parametri della lorentziana (anche questi random?)
    std::cauchy_distribution<double> lorentzDist(l1,l2); //l1,l2 parametri della lorentziana (forse non è esattamente questa quella da usare, in caso la definiamo noi)
    _freq = lorentzDist(seed);
  }

  if (phase == -1) {
    std::random_device seed;   
    std::uniform_real_distribution<double> phaseDist(0, 2*M_PI);
    _phase = phaseDist(seed);
  }
  else 
    setPhase(phase);
}

void Oscillator::setPhase(double phase) {
  while (phase < 0) {
    phase += 2 * M_PI;
  }
  while (phase >= 2*M_PI) {
    phase -= 2 * M_PI;
  }
  _phase = phase;
}

//definire i parametri di campo medio
std::complex<double> Oscillator::orderParamether(std::vector<Oscillator>& system) {
  double cosAll;
  double sinAll;
  int n = system.size();
  for(int i = 0; i < n; ++i){
    cosAll += std::cos(system[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::cos(system[i].phase());  //stesso per sinAll
  }
  double x = cosAll/n;
  double y = sinAll/n;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY. Da qui lo riportiamo in exp
  double r = std::sqrt(x*x + y*y);   
  double psi = std::atan(y/x);        //non è così semplice calcolare la fase, non so se esiste qualche funzione che considera i vari casi possibili
  return std::polar(r, psi);  //polar costruisce un exp complesso, MF sta per mean field non per Matteo Falcioni
}

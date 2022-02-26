#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <complex>
#include <cassert>
#include <vector>
using namespace std::complex_literals;

enum class Distribution {Lorentz, Gauss, Boltzmann, Expo};

//Lorentz Distribution (allows to solve explicitly for r(K))
double lorentz_g(double freq, double mean = 1, double gamma = 0.5);
double gauss_g(double freq, double mean = 1, double sigma = 1);
double boltzmann_g(double freq, double T = 2);    //manca double mean e non so come metterlo
double exp_g(double freq, double mean = 1);

class Oscillator{
 protected:
  double _freq;     //frequency omega   //nota: lunghezze d'onda comprese tra Lmin = 500 nm e Lmax = 650 nm -> omega tra 2pi*c/Lmax=3.77e15 e 2pi*c/Lmin=2.9e15 Hz 
  double _phase;    //phase phi/theta

  std::complex<double> phasor() { return std::__complex_exp(1i*_phase); }                 //returns directly the exponential form
  
 public:
  Oscillator(double freq, double phase = -1);
  Oscillator(Distribution dist = Distribution::Lorentz, double mean = 1, double param = 998);  //Deve essere un valore improponibile, per capire che non l'ha messo l'utente
  
  double freq() { return _freq; }
  void setFreq(double freq) { _freq = freq; }
  double phase() { return _phase; };
  void setPhase(double phase);
  
  //print the oscillator in ascii
  void print();
  //evolve the oscillator's dinamic considering dt seconds have passed.
  void evolve(double dt);
};

//returns "[random]" if num == -1. Else returns std::to_string(num).
std::string toString(double num);

//returns "true" or "false"
std::string literal(bool b);

#endif //OSCILLATOR_H

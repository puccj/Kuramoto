#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <complex>
#include <cassert>
#include <vector>
using namespace std::complex_literals;

//Lorentz Distribution (allows to solve explicitly for r(K))
double lorentz_g(double freq, double gamma = 0.01); //gamma deve essere >= 0, e più aumenta meno la lorentziana è piccata
                                                      //quando comincia a superare 1.5 è quasi piatta; sotto 0.4 si picca velocemente.
                                                      //per il nostro intervallo di valori (+/- 0.8e15 Hz dalla media) dovremmo avere un gamma di all'incirca 0.01 o inferiore.

class Oscillator{
 protected:
  //static double _K; //coupling strength 
  static double _dt;
  double _freq;   //frequency omega   //nota: lunghezze d'onda comprese tra Lmin = 500 nm e Lmax = 650 nm -> omega tra 2pi*c/Lmax=3.77e15 e 2pi*c/Lmin=2.9e15 Hz 
  double _phase;  //phase phi/theta

  bool phaseNearZero(double limit = _dt*2) { return (_phase < limit || _phase > 2*M_PI - limit); }
  static void setDefaultDt() { _dt = 0.1; }
  std::complex<double> phasor() { return std::exp(1i*_phase); }                 //returns directly the exponential form
  static std::complex<double> orderParameter(std::vector<Oscillator>& system); //returns r: order parameter

 public:
  //if not indicated, frequencies and phases are set randomly
  Oscillator(double freq = -1, double phase = -1);
  
  static void setDt(double dt) { _dt = dt; }
  double freq() { return _freq; }
  void setFreq(double freq) { _freq = freq; }
  double phase() { return _phase; };
  void setPhase(double phase);
  
  //print the oscillator in ascii
  void print();
  //evolve the oscillator's dinamic
  void evolve();
};

#endif //OSCILLATOR_H

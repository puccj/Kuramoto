#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <cmath>
#include <complex>
#include <cassert>
#include <vector>
using namespace std::complex_literals;

class Oscillator{
  //static double _K; //parametro di accoppiamento (in inglese come?)
  double _freq;   //frequency omega
  double _phase;  //phase phi/theta
  
  std::complex<double> phasor() { return std::exp(1i*_phase); }  //returns directly the exponential form

 public:
  //if not indicated, frequencies and phases are set randomly
  Oscillator(double freq = -1, double phase = -1);
  
  double freq() { return _freq; }
  void setFreq(double freq) { _freq = freq; }
  double phase() { return _phase; };
  void setPhase(double phase);
  
  //return r: parameter of order
  static std::complex<double> orderParamether(std::vector<Oscillator>& system);
};


#endif //OSCILLATOR_H
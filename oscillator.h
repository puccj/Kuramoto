#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <complex>
#include <cassert>
using namespace std::complex_literals;

enum class DistName {Lorentz, Gauss, Boltzmann, Exp};
/*double lorentz_g(double freq, double mean = 1, double gamma = 0.5);
double gauss_g(double freq, double mean = 1, double sigma = 1);
double boltzmann_g(double freq, double T = 2);    //manca double mean e non so come metterlo
double exp_g(double freq, double mean = 1); */

class Distribution {
  DistName _name;
  double _mean;
  double _param;
 public: 
  Distribution(DistName name = DistName::Lorentz, double mean = 1, double param = 988);
  double evaluate(double freq);
  double max();
  std::string toString();   //returns (ex) "Lorentz,1,0.5"
};

class Oscillator {
 protected:
  double _freq;     //frequency omega
  double _phase;    //phase phi/theta

  //std::complex<double> phasor() { return std::__complex_exp(1i*_phase); }
 public:
  Oscillator(double freq, double phase = -1);
  Oscillator(Distribution dist = Distribution{DistName::Lorentz, 1, 998});
  
  double freq() { return _freq; }
  void setFreq(double freq) { _freq = freq; }
  double phase() { return _phase; };
  void setPhase(double phase);
  
  //evolve the oscillator's dinamic considering dt seconds have passed.
  void update(double dt);
};

//returns "[random]" if num == -1. Else returns std::to_string(num).
std::string toString(double num);

//returns "True" or "False"
std::string literal(bool b);

#endif //OSCILLATOR_H

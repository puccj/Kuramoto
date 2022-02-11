#ifndef KURAMOTO_H
#define KURAMOTO_H

#include <cmath>
#include <complex>
#include <cassert>
#include <vector>
using namespace std::complex_literals;

class Oscillator{
  //static int _N;   //# of oscillator
  
  static int _gridDim;  //dimension (side) of the grid where oscillators will be placed
  int _position;        //position of oscillator on the grid

  //static double _K; //parametro di accoppiamento (in inglese come?)
  double _freq;   //frequency omega
  double _phase;  //phase phi/theta
  
  static void setDefaultGridDim() { _gridDim = 10; }
  std::complex<double> phasor() { return std::exp(1i*_phase); }  //returns directly the exponential form

 public:
  //if not indicated, frequencies and phases are set random
  Oscillator(double freq = -1, double phase = -1, double position = -1);
  
  static double setGridDim(int gridDim) { _gridDim = gridDim; }
  double freq() { return _freq; }
  void setFreq(double freq) { _freq = freq; }
  double phase() { return _phase; };
  void setPhase(double phase);
  
  //return r: parameter of order
  static std::complex<double> orderParamether(std::vector<Oscillator>& system);
  
  //print system in ascii
  static void print(std::vector<Oscillator>& system);
};


#endif
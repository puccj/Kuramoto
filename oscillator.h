#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <SFML/Graphics.hpp>
//#include <complex>
//using namespace std::complex_literals;

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
  sf::Vector2f _position;         //position of firefly on the window

  //std::complex<double> phasor() { return std::__complex_exp(1i*_phase); }
 public:
  Oscillator(Distribution dist = Distribution{DistName::Lorentz, 1, 998}, sf::Vector2f position = sf::Vector2f(-1,-1));
  Oscillator(double freq, sf::Vector2f position = sf::Vector2f(-1,-1));
  
  double freq() { return _freq; }
  void setFreq(double freq) { _freq = freq; }
  double phase() { return _phase; };
  void setPhase(double phase);
  sf::Vector2f position() { return _position; }

  //evolve the oscillator's dinamic considering dt seconds have passed.
  void update(double dt);
};

//returns "[random]" if num == -1. Else returns std::to_string(num).
std::string toString(double num);

//returns "True" or "False"
std::string literal(bool b);

//std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector);
//std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector);
//bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs);

#endif //OSCILLATOR_H

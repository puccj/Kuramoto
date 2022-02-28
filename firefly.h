#ifndef FIREFLY_H
#define FIREFLY_H

#include "oscillator.h"
#include <SFML/Graphics.hpp>

class Firefly : public Oscillator {
  static double _K;               //coupling strength between firefly
  static sf::Vector2f _windowDim; //dimension (pixels, (x,y)) of the grid where fireflies will be placed
  sf::Vector2f _position;         //position of firefly on the grid

  //makes the firefly (its oscillator) interact with the others. System indicates all the firefly minus this
  void interact(std::vector<Firefly>& system, double dt);

 public:
  //if not indicated, frequencies, phases and position are set randomly
  Firefly(double freq, double phase = -1, sf::Vector2f position = sf::Vector2f(-1,-1));
  Firefly(Distribution dist = Distribution::Lorentz, double mean = 1, double param = 998, sf::Vector2f position = sf::Vector2f(-1,-1));
  
  //get and set functions:
  static void setK(double K) { _K = K; }
  static void setWindowDim(sf::Vector2f windowDim) { _windowDim = windowDim; }
  static void setWindowDim(int x, int y) { setWindowDim(sf::Vector2f(x,y)); }
  static void setWindowDim(int size) { setWindowDim(sf::Vector2f(size, size)); }
  sf::Vector2f position() { return _position; }

  //returns r: order parameter
  static std::complex<double> orderParameter(std::vector<Firefly>& system);
  static double moduleOrderParameter(std::vector<Firefly>& system);

  //void move();   // Da fare: magari con tot velocità ( (static) Vector2f)

  //calls evolve and interact for each element of the vector
  static void evolve(std::vector<Firefly>& syst, double dt, bool interaction = false);

  //draw the system
  static void draw(std::vector<Firefly>& syst);

  //plot the oscillators in a complex plane
  static void plot(std::vector<Firefly>& syst, int windowSize = 300, int drawSize = 5);
};

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector);
std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector);
bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs);

#endif  //FIREFLY_H
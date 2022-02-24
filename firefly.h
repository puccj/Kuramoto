#ifndef FIREFLY_H
#define FIREFLY_H

#include "oscillator.h"
#include <SFML/Graphics.hpp>

class Firefly : public Oscillator {
  static double _K;               //coupling strength between firefly
  static sf::Vector2f _windowDim; //dimension (pixels, (x,y)) of the grid where fireflies will be placed
  //static sf::Vector2f _velocity //velocity of fireflies moving around (Da fare)
  sf::Vector2f _position;         //position of firefly on the grid
  
  static void setDefaultK() { _K = 0.1; }     //Quanto il valore?
  static void setDefaultWindowDim() { _windowDim = sf::Vector2f(200,200); }
 public:
  //if not indicated, frequencies, phases and position are set randomly
  Firefly(double freq, double phase = -1, sf::Vector2f position = sf::Vector2f(-1,-1));
  Firefly(Distribution dist = Distribution::Lorentz, double mean = 1, double param = 998, sf::Vector2f position = sf::Vector2f(-1,-1));

  
  //get and set functions:
  static void setK(double K) { _K = K; }
  static sf::Vector2f windowDim();
  static void setWindowDim(sf::Vector2f windowDim) { _windowDim = windowDim; }
  static void setWindowDim(double x, double y) { setWindowDim(sf::Vector2f(x,y)); }
  static void setWindowDim(int size) { setWindowDim(sf::Vector2f(size, size)); }
  sf::Vector2f position() { return _position; }

  //print system in ascii
  // static void print(std::vector<Firefly> system);  //non funziona più con la posizione fatta con sf::Vector2f

  //move the firefly in the grid
  // per ora non fa, da rivedere perché ora non si aggiorna ogni tot tempo fisso,
  // ma il tempo dipende da quanto tempo impiega il computer per calcolare tutto il frame. (Nota infatti che Oscillator::evolve ha un parametro)
  // void move();

  //returns r: order parameter
  static std::complex<double> orderParameter(std::vector<Firefly>& system);
  static double moduleOrderParameter(std::vector<Firefly>& system);

  //calls evolve for each element of vector
  static void evolve (std::vector<Firefly>& syst, double dt);

  //makes the firefly (its oscillator) interact with the others. System indicates all the firefly apart from this
  void interact(std::vector<Firefly>& system, double dt);
};

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector);
std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector);
bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs);

//return true if lhs comes before rhs (scanning line from left to right, top to bottom)
bool operator<(Firefly& lhs, Firefly& rhs);

#endif  //FIREFLY_H
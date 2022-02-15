#ifndef FIREFLY_H
#define FIREFLY_H

#include "oscillator.h"
#include <SFML/Graphics.hpp>

class Firefly : public Oscillator {
  static sf::Vector2f _windowDim; //dimension (pixels, (x,y)) of the grid where fireflies will be placed
  sf::Vector2f _position;       //position of firefly on the grid
  
  static void setDefaultWindowDim() { _windowDim = sf::Vector2f(200,200); }
 public:
  //if not indicated, frequencies, phases and position are set randomly
  Firefly(double freq = -1, double phase = -1, sf::Vector2f position = sf::Vector2f(-1,-1));
  
  //get and set functions:
  static sf::Vector2f windowDim();
  static void setWindowDim(sf::Vector2f windowDim) { _windowDim = windowDim; }
  static void setWindowDim(int x, int y) {setWindowDim(sf::Vector2f(x,y)); }
  static void setWindowDim(int size) { setWindowDim(sf::Vector2f(size, size)); }
  sf::Vector2f position() { return _position; }

  //print system in ascii
  // static void print(std::vector<Firefly> system);  //non funziona più con la posizione fatta con sf::Vector2f

  //move the firefly in the grid
  // per ora non fa, da rivedere perché ora non si aggiorna ogni tot tempo fisso,
  // ma il tempo dipende da quanto tempo impiega il computer per calcolare tutto il frame. (Nota infatti che Oscillator::evolve ha un parametro)
  // void move();

  //calls evolve for each element of vector
  static void evolve (std::vector<Firefly>& syst, double dt);
};

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector);
std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector);
bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs);

//return true if lhs comes before rhs (scanning line from left to right, top to bottom)
bool operator<(Firefly& lhs, Firefly& rhs);

#endif  //FIREFLY_H
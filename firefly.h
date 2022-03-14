#ifndef FIREFLY_H
#define FIREFLY_H

#include "oscillator.h"
#include <SFML/Graphics.hpp>

class Firefly : public Oscillator {
  sf::Vector2f _position;         //position of firefly on the window

 public:
  //if not indicated, frequencies, phases and position are set randomly
  Firefly(Distribution dist = Distribution{DistName::Lorentz, 1, 998}, sf::Vector2f position = sf::Vector2f(-1,-1));
  Firefly(double freq, sf::Vector2f position = sf::Vector2f(-1,-1));
  
  sf::Vector2f position() { return _position; }
  //void move();   // Da fare: magari con tot velocità ( (static) Vector2f)
};

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector);
std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector);
bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs);

#endif  //FIREFLY_H
#include "firefly.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <thread>


Firefly::Firefly(Distribution dist, sf::Vector2f position) : Oscillator(dist), _position{position} {
  /*if (position >= _windowDim) {
    std::cerr << "WARN (13): position value is too high for the set window dimensions: using a random position instead.\n";
    std::cerr << "Use Firefly::setWindowDim static function if you want to change the window dimensions\n";
    position = sf::Vector2f(-1,-1);
  }
  if (position == sf::Vector2f(-1,-1)) {   //default value -> random setting
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _windowDim.x);
    std::uniform_int_distribution<int> distY(0, _windowDim.y);
    _position.x = distX(seed);
    _position.y = distY(seed);
  }*/
}

Firefly::Firefly(double freq, sf::Vector2f position): Oscillator(freq), _position{position} { 
/*  if (position >= _windowDim) {
    std::cerr << "WARN (12): position value is too high for the set window dimensions: using a random position instead.\n";
    std::cerr << "Use Firefly::setWindowDim static function if you want to change the window dimensions\n";
    position = sf::Vector2f(-1,-1);
  }
  if (position == sf::Vector2f(-1,-1)) {   //default value -> random setting
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _windowDim.x);
    std::uniform_int_distribution<int> distY(0, _windowDim.y);
    _position.x = distX(seed);
    _position.y = distY(seed);
  }*/
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector) {
  os << '(' << vector.x << ", " << vector.y << ')';
  return os;
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector) {
  os << '(' << vector.x << ", " << vector.y << ')';
  return os;
}

bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs) {
  return lhs.x >= rhs.x || lhs.y >= rhs.y;
}
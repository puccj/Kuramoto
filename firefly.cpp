#include "firefly.h"
#include <iostream>
#include <random>
#include <algorithm>

sf::Vector2f Firefly::_windowDim = sf::Vector2f(-1,-1);

Firefly::Firefly(double freq, double phase, sf::Vector2f position): Oscillator(freq, phase), _position{position} {
  if (_windowDim == sf::Vector2f(-1,-1)) {
    std::cerr << "WARN (11): _windowDim value not set: using default value. Use Firefly::setWindowdDim static function if you want to set it manually\n";
    setDefaultWindowDim();
  }
  
  if (position >= _windowDim) {
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
  }
}

sf::Vector2f Firefly::windowDim() { 
  if (_windowDim == sf::Vector2f(-1,-1)) {
    std::cerr << "WARN (31): _windowDim value not set: using default value. Use Firefly::setWindowDim static function if you want to set it manually\n";
    setDefaultWindowDim();
  }
  return _windowDim;
}

/* 
void Firefly::print(std::vector<Firefly> system) {
  std::sort(system.begin(), system.end());

  for (int y = 0; y < _gridDim.y; y++) {
    if (y != 0)
      std::cout << "|\n";
    for (int x = 0; x < _gridDim.x; x++)
      std::cout << "----";
    std::cout << "\n| ";

    bool present = false;
    for (int x = 0; x < _gridDim.x; x++) {
      for (int i = 0; i < system.size(); i++) {
        if (system[i].position().y == y && system[i].position().x == x) {
          system[i]
        }
      }
    }
  }

  for (int n = 0; n < _gridDim.x*_gridDim.y; n++) {
    if (n % (int) _gridDim.x == 0) {
      if (n == 0)
        std::cout << '\n';
      else
        std::cout << "|\n";
      
      for (int i = 0; i < _gridDim.x; i++)
        std::cout << "----";
      std::cout << '\n';
    }

    std::cout << "| ";
    
    bool present = false;
    for(int i = 0; i < system.size(); i++) {
      //converting 2D position in 1D
      int position1D = system[i].position().x + system[i].position().y*_gridDim.x;

      if (position1D == n) {
        system.erase(system.begin()+i); //remove the element from the vector (no problem since the vector has been passed as a copy (not by &))
        present = true;
        system[i].Oscillator::print();
        break;
      }
    }
    if (!present)
      std::cout << "  ";
  }

  std::cout << "|\n";
  for (int i = 0; i < _gridDim.x; i++) {
    std::cout << "----";
  }
}
 */

/*
void Firefly::move() {
  std::random_device seed;
  std::uniform_int_distribution<int> moveDist(0, 3); //4 possibility
  int direction = moveDist(seed);

  switch (direction)
  {
  case 0:     //left
    if (_position % _gridDim != 0) {
        _position--;
    }
    break;
  case 1:     //right
    if (_position % _gridDim != _gridDim-1) {
        _position++;
    }
    break;
  case 2:     //up
    if (_position / _gridDim != 0) {
      _position -= _gridDim;
    }
    break;
  case 3:     //down
    if (_position / _gridDim != _gridDim-1) {
      _position += _gridDim;
    }
    break;
  default:
    std::cerr << "ERR (31): Error in function Firefly::move().\n";
  }

  //_history.push_back(_money);
}
*/


void Firefly::evolve(std::vector<Firefly>& syst, double dt) {
  int size = syst.size();
  for (int i = 0; i < size; i++) {
    syst[i].Oscillator::evolve(dt);
  }
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
  return lhs.x >= rhs.y || lhs.y >= rhs.x;
}

bool operator<(Firefly& lhs, Firefly& rhs) {
  if (lhs.position().y < rhs.position().y)
    return true;
  if (lhs.position().y == rhs.position().y && lhs.position().x < rhs.position().x)
    return true;
  return false;
}
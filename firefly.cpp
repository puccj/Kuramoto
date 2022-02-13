#include "firefly.h"
#include <iostream>
#include <random>
#include <algorithm>

int Firefly::_gridDim = -1;

Firefly::Firefly(double freq, double phase, int position): Oscillator(freq, phase), _position{position} {
  if (_gridDim == -1) {
    std::cerr << "WARN (11): _gridDim value not set: using default value. Use Firefly::setGridDim static function if you want to set it manually\n";
    setDefaultGridDim();
  }
  
  if (position >= _gridDim*_gridDim) {
    std::cerr << "WARN (12): position value is too high for the set grid dimension: using a random position instead.\n";
    std::cerr << "Use Firefly::setGridDim static function if you want to change the grid dimension\n";
    position = -1;
  }

  if (position == -1) {   //default value -> random setting
    std::random_device seed;
    std::uniform_int_distribution<int> dist(0, _gridDim*_gridDim);
    _position = dist(seed);
  }
}

void Firefly::print(std::vector<Firefly> system) {
  std::sort(system.begin(), system.end());
  
  for (int n = 0; n < _gridDim*_gridDim; n++) {
    if (n % _gridDim == 0) {
      if (n == 0)
        std::cout << '\n';
      else
        std::cout << "|\n";
      
      for (int i = 0; i < _gridDim; i++)
        std::cout << "----";
      std::cout << '\n';
    }

    std::cout << "| ";
    
    bool present = false;
    for(int i = 0; i < system.size(); i++) {
      if (system[i].position() == n) {
        system.erase(system.begin()+i);
        present = true;
        system[i].Oscillator::print();
        break;
      }
    }
    if (!present)
      std::cout << "  ";
  }

  std::cout << "|\n";
  for (int i = 0; i < _gridDim; i++) {
    std::cout << "----";
  }
}

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
    std::cerr << "ERR (31): Error in function move().\n";
  }

  //_history.push_back(_money);
}

bool operator<(Firefly& lhs, Firefly& rhs) {
  return lhs.position() < rhs.position();
}
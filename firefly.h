#ifndef FIREFLY_H
#define FIREFLY_H

#include "oscillator.h"

class Firefly : public Oscillator{
  static int _gridDim;  //dimension (side) of the grid where fireflies will be placed
  int _position;        //position of firefly on the grid
  
  //non serve perché la classe Firefly è derivata da Oscillator, quindi ha tutte le cose dell'oscillator:
  //Oscillator _oscill;   //brain of the firefly

  static void setDefaultGridDim() { _gridDim = 10; }
 public:
 //if not indicated, frequencies, phases and position are set randomly
  Firefly(double freq = -1, double phase = -1, int position = -1);
  
  static void setGridDim(int gridDim) { _gridDim = gridDim; }
  int position() { return _position; }

  //print system in ascii
  static void print(std::vector<Firefly> system);
};

bool operator<(Firefly& lhs, Firefly& rhs);

#endif  //FIREFLY_H
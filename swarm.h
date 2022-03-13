#ifndef SWARM_H
#define SWARM_H

#include <SFML/Graphics.hpp>
#include "firefly.h"

class Swarm {
  int _size = 0;              //number of fireflies
  Firefly* _data = nullptr;   //array of fireflies
  double _K = 1;              //coupling strength between firefly
  double _Kc = 0;             //critical value of K
  bool _interaction = false;  //toggle interaction between fireflies
  bool _evolve = true;        //to decide when to stop evolving
  sf::Vector2f _windowDim = sf::Vector2f(1000, 700);  //dimension (pixels, (x,y)) of the window where fireflies will be placed
  
 public:
  Swarm(int size, Distribution dist = Distribution{DistName::Lorentz, 1, 998});
  //Swarm(int size, Firefly data = Firefly{});
  Swarm(Swarm const& other);
  ~Swarm() { delete[] _data; }

  //get and set functions:
  int size() { return _size; }
  Firefly& operator[](int);
  double K() { return _K; }
  void setK(double K) { _K = K; }
  void setWindowDim(sf::Vector2f windowDim) { _windowDim = windowDim; }
  void setWindowDim(int x, int y) { setWindowDim(sf::Vector2f(x,y)); }
  void setWindowDim(int size) { setWindowDim(sf::Vector2f(size, size)); }

  //returns r: order parameter
  std::complex<double> orderParameter();
  double moduleOrderParameter();
  double angleOrderParameter();

  //calls update and interact for each element of the vector
  void evolve(bool saveData = true);

  //break out from evolve loop
  void stopEvolve() { _evolve = false; }

  //draw the system
  void draw();

  //plot the oscillators in a complex plane
  void plot();

  //make more simulations with different K and save the average of last values of r
  //speedFactor make the simulations run faster (or slower if < 1), spacing more the time between each step
  void rkGraph(double kMin, double kMax, double kIncrement = 0.1, double speedFactor = 1, bool saveRT = false);
  
  Swarm& operator=(Swarm const& other);
  void push_back(Firefly const& add);
};


#endif //SWARM_H
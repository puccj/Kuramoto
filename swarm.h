#ifndef SWARM_H
#define SWARM_H

#include "oscillator.h"

struct TitledWindow {
  std::string name;
  int width = 1000;
  int height = 700;
};

struct Complex {
  double mod;
  double phase;
};

class Swarm {
  int _size = 0;                //number of fireflies
  Oscillator* _data = nullptr;  //array of fireflies
  double _K = 1;                //coupling strength between firefly
  double _Kc = 0;               //critical value of K
  bool _interaction = false;    //toggle interaction between fireflies
  bool _evolve = true;          //to decide when to stop evolving
  TitledWindow _window;         //name and size of the window to draw in
  
  //makes the chosen oscillator interact with the system
  void interact(Oscillator& chosen, double dt);
 public:
  Swarm(int size, Distribution dist = Distribution{DistName::Lorentz, 1, 998}, TitledWindow window = {"default", 1000, 700});
  Swarm(int size, double freq, TitledWindow window = {"default", 1000, 700,});
  ~Swarm() { delete[] _data; }

  //get and set functions:
  int size() { return _size; }
  //Oscillator& operator[](int);
  double K() { return _K; }
  void setK(double K) { _K = K; }
  void setWindowDim(int x, int y);
  void setWindowDim(int size) { setWindowDim(size, size); }

  //returns r: order parameter
  enum class Component {Module, Phase};
  Complex orderParameter();
  double orderParameter(Component component); //return module or phase only (made to speeds up things)

  //calls update and interact for each element of the vector
  void evolve(bool saveData = true);

  //break out from evolve loop
  void stopEvolve() { _evolve = false; }

  //draw the system
  void draw();

  //plot the oscillators in a complex plane
  void plot();

  /**
   * @brief Makes more simulations with different K and save the average of last values of r.
   * 
   * @param kMin Value of K from where to start from
   * @param kMax Value of where to stop
   * @param kIncrement Increment of K value between each simulations
   * @param saveRT Decide to save also r(t) graphs for each fixed K value
   * @param timeIncrement Increment of time between each step in the simulations.
   * A lower value results in a better approximated solution, but it require more time to compute.
   * Setting it to 0 (default), the time between each step will be equal to the actual time taken by the CPU to compute the operation.
   * This makes a real-time-faithful simulation, but produces dt intervals unequally spaced between different (but also the same) CPUs.
   */
  void rkGraph(double kMin, double kMax, double kIncrement = 0.1, bool saveRT = false, double timeIncrement = 0);
  
  //Swarm& operator=(Swarm const& other);
  //void push_back(Firefly const& add); //funziona ma fa crashare con multi threads.
};


#endif //SWARM_H
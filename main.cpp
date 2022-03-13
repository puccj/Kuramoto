#include "swarm.h"
#include <thread>
#include <iostream>
//#include <X11/Xlib.h> 

int main() {
  //XInitThreads();  //serve per XServer

  Swarm primo(1000, Distribution{DistName::Lorentz, 1, 0.5});
  
  //primo.setWindowDim(1000,700);
  //primo.setK(8);

  //Draw window, plot window and evolve done in separeted threads
  //std::thread dThread(&Swarm::draw, &primo);
  //std::thread pThread(&Swarm::plot, &primo);
  //std::thread eThread(&Swarm::evolve, &primo);

  //wait for threads to terminate before proceding
  //dThread.join(); 
  //pThread.join();
  //primo.stopEvolve();
  //eThread.join();

  //primo.rkGraph(sciame, 0, 7, 0.1, 1, true);

  return 0;
}
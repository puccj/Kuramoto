#include "swarm.h"
#include <thread>
#include <iostream>

//#include <X11/Xlib.h> 

void draw(Swarm& syst) {
  syst.draw();
}
void plot(Swarm& syst) {
  syst.plot();
}
void evolve(Swarm& syst, bool saveData) {
  syst.evolve(saveData);
}

int main() {
  //XInitThreads();  //serve per XServer

  Swarm lorRK(1000, Distribution{DistName::Lorentz, 1, 0.5}); //array con distribuzione di Lorentz a media 1 e gamma 0.5
  //Swarm primo(1000, 0.5, TitledWindow{"Esempio"});  //array con tutti alla stessa frequenza di 0.5Hz
  //primo.setK(0.1);
  //primo.setWindowDim(1920,1080);

  //Draw window, plot window and evolve done in separeted threads
  // std::thread primoDrawThr(&draw, std::ref(primo));
  // std::thread primoPlotThr(&plot, std::ref(primo));
  // std::thread primoEvolveThr(&evolve, std::ref(primo), false);

  //wait for threads to terminate before proceding
  // primoDrawThr.join(); 
  // primoPlotThr.join();
  // primo.stopEvolve();
  // primoEvolveThr.join();

  lorRK.rkGraph(0, 7, 0.1, true, 0.003);

  return 0;
}
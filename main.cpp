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

  Swarm primo(1000, Distribution{DistName::Lorentz, 1, 0.5}); //array con distribuzione di Lorentz a media 1 e gamma 0.5
  Swarm secondo(500, 0.5);  //array con tutti alla stessa frequenza di 0.5Hz
  
  //primo.setWindowDim(1000,700);
  primo.setK(10);
  secondo.setK(1);

  //Draw window, plot window and evolve done in separeted threads
  std::thread primoDrawThr(&draw, std::ref(primo));
  std::thread primoPlotThr(&plot, std::ref(primo));
  std::thread primoEvolveThr(&evolve, std::ref(primo), false);

  //add them to thrHandler (Da fare: in automatico)
  thrHandler::add(primoDrawThr.get_id(), primo.name());
  thrHandler::add(primoPlotThr.get_id(), primo.name());
  thrHandler::add(primoEvolveThr.get_id(), primo.name());

  std::thread secondoDrawThr(&draw, std::ref(secondo));
  std::thread secondoPlotThr(&plot, std::ref(secondo));
  std::thread secondoEvolveThr(&evolve, std::ref(secondo), false);
  thrHandler::add(secondoDrawThr.get_id(), secondo.name());
  thrHandler::add(secondoPlotThr.get_id(), secondo.name());
  thrHandler::add(secondoEvolveThr.get_id(), secondo.name());

  //wait for threads to terminate before proceding
  primoDrawThr.join(); 
  primoPlotThr.join();
  primo.stopEvolve();
  primoEvolveThr.join();

  secondoDrawThr.join(); 
  secondoPlotThr.join();
  secondo.stopEvolve();
  secondoEvolveThr.join();
  //primo.rkGraph(sciame, 0, 7, 0.1, 1, true);

  return 0;
}
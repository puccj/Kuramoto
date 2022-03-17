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

  Swarm esempio(1000, Distribution{DistName::Lorentz, 1, 0.5}, {"Esempio", 1280, 720}); //array con distribuzione di Lorentz a media 1 e gamma 0.5
  esempio.setK(10);

  //Draw window, plot window and evolve done in separeted threads
  std::thread drawThr(&draw, std::ref(esempio));
  std::thread plotThr(&plot, std::ref(esempio));
  std::thread evolveThr(&evolve, std::ref(esempio), false);

  //wait for threads to terminate before proceding
  drawThr.join(); 
  plotThr.join();
  esempio.stopEvolve();
  evolveThr.join();

  //esempio.rkGraph(10,20,1, true);

  return 0;
}
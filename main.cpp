#include "firefly.h"
#include <thread>
#include <iostream>

int main() {
  Firefly::setWindowDim(1000,700);
  Firefly::setK(10);

  std::vector<Firefly> sciame;
  //std::fstream fout("output.txt",std::ios::out);
  for (int i = 0; i < 1000; i++) {
    //sciame.push_back(Firefly(Distribution::Lorentz,1,0.5));
    sciame.push_back(Firefly(1));
    //fout << sciame[i].freq() << '\n';
  }

  //fout.close();
  //std::cout << "Kc: " << 2/(M_PI*lorentz_g(0,1,0.5));
  
  //Drawwindow and plot separeted threads, passing sciame as reference
  std::thread dThread(&Firefly::draw, std::ref(sciame));  
  std::thread pThread(&Firefly::plot, std::ref(sciame));
  std::thread eThread(&Firefly::evolve, std::ref(sciame));

  dThread.join(); //wait for thread to terminate before proceding
  pThread.join();
  return 0;
}
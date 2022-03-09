#include "firefly.h"
#include <thread>
#include <iostream>

int main() {
  Firefly::setWindowDim(1000,700);
  Firefly::setK(10);

  std::vector<Firefly> sciame;
  //std::fstream fout("output.txt",std::ios::out);
  for (int i = 0; i < 1000; i++) {
    sciame.push_back(Firefly(Distribution::Lorentz,1,0.5));
    //sciame.push_back(Firefly(0.2));
    //fout << sciame[i].freq() << '\n';
  }

  //fout.close();
  //std::cout << "Kc: " << 2/(M_PI*lorentz_g(0,1,0.5));
  
  //Draw window, plot window and evolve done in separeted threads, passing sciame as reference
  std::thread dThread(&Firefly::draw, std::ref(sciame));  
  std::thread pThread(&Firefly::plot, std::ref(sciame));
  std::thread eThread(&Firefly::evolve, std::ref(sciame));

  //wait for threads to terminate before proceding
  dThread.join(); 
  pThread.join();
  return 0;
}
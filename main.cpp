#include "firefly.h"
#include <thread>
#include <iostream>

int main() {
  Firefly::setWindowDim(1000,700);
  Firefly::setK(30);

  std::vector<Firefly> sciame;
  //std::fstream fout("output.txt",std::ios::out);
  for (int i = 0; i < 1000; i++) {
    sciame.push_back(Firefly(Distribution::Lorentz,1,0.5));
    //fout << sciame[i].freq() << '\n';
  }

  //fout.close();
  //std::cout << "Kc: " << 2/(M_PI*lorentz_g(0,1,0.5));
  
  //Drawing window in another thread, passing sciame as reference
  std::thread t(&Firefly::draw, std::ref(sciame));  
  

  
  t.join(); //wait for thread to terminate before proceding
  return 0;
}
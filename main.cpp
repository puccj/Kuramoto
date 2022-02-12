#include "firefly.h"
#include <chrono>
#include <thread>
#include <iostream>

int main(){
  /*int N = 5;
  Firefly::setGridDim(5);
  std::vector<Firefly> system(N);  //create an array of N Firefly (no parameter set -> random)
  Firefly::print(system);*/

  /* in teoria tenendo uguali dt e sleep for si riesce a fare un tempo vero (cioè se la frequenza è 1 lampeggia davvero 1 volta al secondo)
   * In realtà però lagga un po' e più il dt (e lo sleep for) sono piccoli, più si vede questa cosa.
   * Dovrei trovare un'altra cosa che magari invece dello sleep for usa il clock del computer o qualcosa così.
   */
  
  Oscillator::setDt(1);     
  Oscillator prova(0.5,0);

  /*prova.setPhase(M_PI);
  std::cout << prova.phase() << '\n';
  prova.setPhase(2*M_PI);
  std::cout << prova.phase() << '\n';*/

  for (int i = 0; i < 400; i++) {
    prova.print();
    prova.evolve();
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1000ms);
    //system("pause");
  }
  
}
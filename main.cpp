#include "firefly.h"
#include <chrono>
#include <thread>
#include <iostream>

int main(){
  /* in teoria tenendo uguali dt e usToSleep si riesce a fare un tempo vero (cioè se la frequenza è 1 lampeggia davvero 1 volta al secondo)
   * In realtà però lagga un po' e più il dt (e lo sleep for) sono piccoli, più si vede questa cosa.
   * Magari metterò uno slider o qualcosa del genere per regolarne uno affinché effettivamente il tempo di calcolo sia lo stesso che
   * quello reale che passa tra un frame e l'altro
   */
  
  Oscillator::setDt(0.050);   //time (seconds) "di calcolo"
  int sleep = 50000;          //time (microseconds) between each frame
  int simulationLength = 20;  //duration (seconds) of the simulation
  std::chrono::duration<int, std::micro> usToSleep (sleep);

  //int N = 5;
  Firefly::setGridDim(5);
  /*std::vector<Firefly> sciame(N);  //create an array of N Firefly (no parameter set -> random)
  Firefly::print(sciame);
  */

  Firefly prova = Firefly(0.5); //firefly with frequency of 2 and random phase and position

  /*
  Oscillator prova(2,0);
  prova.setPhase(M_PI);
  std::cout << prova.phase() << '\n';
  prova.setPhase(2*M_PI);
  std::cout << prova.phase() << '\n';
  */
  for (int i = 0; i < simulationLength*1000000/sleep ; i++) {
    //start timer..
    auto startClock = std::chrono::high_resolution_clock::now();
    auto endClock = startClock + usToSleep;

    //..do everything..
    system("cls");
    Firefly::print({prova});  //print an array with just prova.
    prova.Oscillator::evolve();
    
    //..stop timer.
    do {
      std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < endClock);
  }
  
}
#include "oscillator.h"

int main(){
  int N = 10;
  Oscillator::setGridDim(5);
  std::vector<Oscillator> system(N);  //create an array of N oscillator (no parameter set -> random)
  Oscillator::print(system);
  
}
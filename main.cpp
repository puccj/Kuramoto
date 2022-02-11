#include "firefly.h"

int main(){
  int N = 5;
  Firefly::setGridDim(5);
  std::vector<Firefly> system(N);  //create an array of N Firefly (no parameter set -> random)
  Firefly::print(system);
  
}
#include "oscillator.h"
#include <vector>
#include <random>
#include <iostream>
#include "swarm.h"

using namespace std::complex_literals;

Distribution::Distribution(DistName name, double mean, double param) : _name{name}, _mean{mean}, _param{param} {
  if (param == 998) {
    switch (name) {
     case DistName::Lorentz:
      param = 0.5;
      break;
     case DistName::Gauss:
      param = 1;
      break;
     case DistName::Boltzmann:
      param = 2;
      break;
    }
  }
  if (name == DistName::Lorentz && param < 0)
    param = -param;
}

double Distribution::evaluate(double freq) {
  switch (_name) {
   case DistName::Lorentz:
    return _param / ( M_PI* (_param*_param + (freq-_mean)*(freq-_mean)) );
   case DistName::Gauss:
    return std::exp( (freq-_mean)*(freq-_mean) / (-2*_param)) / sqrt(2*M_PI*_param);
   case DistName::Boltzmann:
    return std::exp(-freq/_param);
   case DistName::Exp:
    return std::exp(-freq/_mean) / _mean;
  }
  return -1;
}

double Distribution::max() {
  switch (_name) {
   case DistName::Lorentz:
   case DistName::Gauss:
    return evaluate(_mean); 
   case DistName::Boltzmann:
   case DistName::Exp:
    return evaluate(0);
  }
  return -1;
}

std::string Distribution::toString() {
  std::string result;
  switch (_name) {
   case DistName::Lorentz:
    result = "Lorentz";
    break;
   case DistName::Gauss:
    result = "Gauss";
    break; 
   case DistName::Boltzmann:
    result = "Boltzmann";
    break;
   case DistName::Exp:
    result = "Exp";
    break;
  }

  result += ',' + std::to_string(_mean) + ',' + std::to_string(_param);
  return result;
}

/*
double lorentz_g(double freq, double mean, double gamma){
  if(gamma < 0) { gamma = -gamma; }
  if(gamma > 1) { 
    if (gamma != 998)
      std::cout<< "Setting gamma > 1 will flatten the Loretzian density"<<'\n';
    gamma = 0.5;  //ho visto che in varie simulazioni online usavano 0.5 per gamma
  }
  return gamma/ ( M_PI*(gamma*gamma + (freq-mean)*(freq-mean)) );
}

double gauss_g(double freq, double mean, double sigma){
  if (sigma == 998)
    sigma = 1;
  double c = 1/sqrt(2*M_PI*sigma);
  return c*std::exp( (freq-mean)*(freq-mean) / (-2*sigma));
}

//Non ho capito bene che distribuzione sarebbe. Mi servirebbe poi un modo per calcolare la media, in analogia con le altre distribuzioni
/*
*è la distribuzione di maxwell boltzmann, quella del microcanonico in fisica della materia: ti dice che le frequenze 
*sono maggiormente distribuite vicino a Omega (che sarebbe la frequenza media caratteristica del sistema) e la 
*probabilità di avere freq>Omega diminuisce esponenzialmente. La media è Omega^2. Problema che ho realizzato dopo avere scritto tutto: non è simmetrica 
*rispetto all'origine, quindi succederà un macello? da vedere
double boltzmann_g(double freq, double Omega){ //Maxwell-Boltzmann distribution, frequency has the same interpretation as Energy 
  if (Omega == 998)
    Omega = 2;    //Qualcosa di accettabile
  return std::exp(-freq/Omega);
}

double exp_g(double freq, double mean) {  //questa che sarebbe? //Sarebbe un'esponenziale decrescente. L'ho messa così a caso
  return std::exp(-freq/mean) / mean;
}*/

Oscillator::Oscillator(double freq, double phase) : _freq{freq} {
  if (phase == -1) {
    //generate phase randomly
    std::random_device seed;   
    std::uniform_real_distribution<double> phaseDist(0, 2*M_PI);
    _phase = phaseDist(seed);
    return;
  }
  setPhase(phase);
}

Oscillator::Oscillator(Distribution dist) {
  //generate random frequency according to the selected distribution  
  double randomX;
  double randomY;
  std::random_device seed;
  std::uniform_real_distribution<double> xDist(-4,4);
  std::uniform_real_distribution<double> yDist(0, dist.max());
  do {
    randomX = xDist(seed);
    randomY = yDist(seed);
  } while (randomY > dist.evaluate(randomX));

  _freq = randomX;

  //generate random phase
  std::random_device seedPhase;   
  std::uniform_real_distribution<double> phaseDist(0, 2*M_PI);
  _phase = phaseDist(seedPhase);
}

void Oscillator::setPhase(double phase) {
  while (phase < 0) {
    phase += 2 * M_PI;
  }
  while (phase >= 2*M_PI) {
    phase -= 2 * M_PI;
  }
  _phase = phase;
}

void Oscillator::update(double dt) {   
  setPhase(_phase + _freq*2*M_PI*dt); //equals to _phase += _freq*dt  + normalize.
}

std::string toString(double num) {
  if (num == -1)
    return "[random]";
  return std::to_string(num);
}

std::string literal(bool b) {
  if (b) 
    return "True";
  return "False";
}
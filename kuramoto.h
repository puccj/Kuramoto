#ifndef KURAMOTO_H
#define KURAMOTO_H

#include <cmath>
#include <complex>
#include <cassert>
using namespace std::complex_literals;

class Oscillator{

    double w_; //omega
    double theta_; //fase
    std::complex<double> Ctheta = 1i*theta_;  
    std::complex<double> phasor = std::__complex_exp(Ctheta);  //servirà per l'approssimaz. di campo medio

    public:
    Oscillator(double w, double theta): w_{w}, theta_{theta} { assert( theta >= 0 && theta <= 2*M_PI ); }  //anche se in realtà theta potrebbe essere un multiplo di Pi..da rivedere
    Oscillator(){}

    void Setw(double w) {w_ = w;}
    void SetTheta(double theta) {theta_ = theta;}
    double Getw() {return w_;}
    double GetTheta() {return theta_;};

};


#endif
#ifndef KURAMOTO_H
#define KURAMOTO_H

#include <cmath>
#include <complex>
#include <cassert>
using namespace std::complex_literals;

class Oscillator{

    double w_; //omega
    double theta_; //fase
    std::complex<double> phasor = std::exp(1i*theta_);  //potrebbe essere utile avere direttamente la forma esponenziale

    public:
    Oscillator(double w, double theta): w_{w}, theta_{theta} { assert( theta >= 0 && theta <= 2*M_PI ); }  //anche se in realtà theta potrebbe essere un multiplo di Pi..da rivedere
    Oscillator(){}

    void Setw(double w) {w_ = w;}
    void SetTheta(double theta) {theta_ = theta;} //cambiare theta cambia automaticamente anche il fasore
    double Getw() {return w_;}
    double GetTheta() {return theta_;};

    void MeanField();

};


#endif
#include "firefly.h"
#include <iostream>
#include <random>
#include <algorithm>

sf::Vector2f Firefly::_windowDim = sf::Vector2f(-1,-1);
double Firefly::_K = -1;

void Firefly::interact(std::vector<Firefly>& system, double dt) {
  if (_K == -1) {
    std::cerr << "WARN (41): _K value not set: using default value. Use Oscillator::setK static function if you want to set it manually\n";
    setDefaultK();
  }

  int size = system.size();

  double sumSinDiff = 0;
  for (int i = 0; i < size; i++) {
    sumSinDiff += std::sin(system[i].phase() - _phase);   //theta_i - theta
  }

  setPhase(_phase + (_freq + _K*sumSinDiff/size ) * dt);  //sarebbe  phase += ()*dt + normalize
}

Firefly::Firefly(double freq, double phase, sf::Vector2f position): Oscillator(freq, phase), _position{position} {
  if (_windowDim == sf::Vector2f(-1,-1)) {
    std::cerr << "WARN (11): _windowDim value not set: using default value. Use Firefly::setWindowdDim static function if you want to set it manually\n";
    setDefaultWindowDim();
  }
  
  if (position >= _windowDim) {
    std::cerr << "WARN (12): position value is too high for the set window dimensions: using a random position instead.\n";
    std::cerr << "Use Firefly::setWindowDim static function if you want to change the window dimensions\n";
    position = sf::Vector2f(-1,-1);
  }
  if (position == sf::Vector2f(-1,-1)) {   //default value -> random setting
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _windowDim.x);
    std::uniform_int_distribution<int> distY(0, _windowDim.y);
    _position.x = distX(seed);
    _position.y = distY(seed);
  }
}

Firefly::Firefly(Distribution dist, double mean, double param, sf::Vector2f position) : Oscillator(dist, mean, param), _position{position} {
  if (position >= _windowDim) {
    std::cerr << "WARN (13): position value is too high for the set window dimensions: using a random position instead.\n";
    std::cerr << "Use Firefly::setWindowDim static function if you want to change the window dimensions\n";
    position = sf::Vector2f(-1,-1);
  }
  if (position == sf::Vector2f(-1,-1)) {   //default value -> random setting
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _windowDim.x);
    std::uniform_int_distribution<int> distY(0, _windowDim.y);
    _position.x = distX(seed);
    _position.y = distY(seed);
  }
}

sf::Vector2f Firefly::windowDim() { 
  if (_windowDim == sf::Vector2f(-1,-1)) {
    std::cerr << "WARN (31): _windowDim value not set: using default value. Use Firefly::setWindowDim static function if you want to set it manually\n";
    setDefaultWindowDim();
  }
  return _windowDim;
}

/* 
void Firefly::print(std::vector<Firefly> system) {
  std::sort(system.begin(), system.end());

  for (int y = 0; y < _gridDim.y; y++) {
    if (y != 0)
      std::cout << "|\n";
    for (int x = 0; x < _gridDim.x; x++)
      std::cout << "----";
    std::cout << "\n| ";

    bool present = false;
    for (int x = 0; x < _gridDim.x; x++) {
      for (int i = 0; i < system.size(); i++) {
        if (system[i].position().y == y && system[i].position().x == x) {
          system[i]
        }
      }
    }
  }

  for (int n = 0; n < _gridDim.x*_gridDim.y; n++) {
    if (n % (int) _gridDim.x == 0) {
      if (n == 0)
        std::cout << '\n';
      else
        std::cout << "|\n";
      
      for (int i = 0; i < _gridDim.x; i++)
        std::cout << "----";
      std::cout << '\n';
    }

    std::cout << "| ";
    
    bool present = false;
    for(int i = 0; i < system.size(); i++) {
      //converting 2D position in 1D
      int position1D = system[i].position().x + system[i].position().y*_gridDim.x;

      if (position1D == n) {
        system.erase(system.begin()+i); //remove the element from the vector (no problem since the vector has been passed as a copy (not by &))
        present = true;
        system[i].Oscillator::print();
        break;
      }
    }
    if (!present)
      std::cout << "  ";
  }

  std::cout << "|\n";
  for (int i = 0; i < _gridDim.x; i++) {
    std::cout << "----";
  }
}
 */

/*
void Firefly::move() {
  std::random_device seed;
  std::uniform_int_distribution<int> moveDist(0, 3); //4 possibility
  int direction = moveDist(seed);

  switch (direction)
  {
  case 0:     //left
    if (_position % _gridDim != 0) {
        _position--;
    }
    break;
  case 1:     //right
    if (_position % _gridDim != _gridDim-1) {
        _position++;
    }
    break;
  case 2:     //up
    if (_position / _gridDim != 0) {
      _position -= _gridDim;
    }
    break;
  case 3:     //down
    if (_position / _gridDim != _gridDim-1) {
      _position += _gridDim;
    }
    break;
  default:
    std::cerr << "ERR (31): Error in function Firefly::move().\n";
  }

  //_history.push_back(_money);
}
*/

std::complex<double> Firefly::orderParameter(std::vector<Firefly>& system) {
  double cosAll = 0;
  double sinAll = 0;
  int n = system.size();
  for(int i = 0; i < n; ++i){
    cosAll += std::cos(system[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(system[i].phase());  //stesso per sinAll
  }
  double x = cosAll/n;
  double y = sinAll/n;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY. Da qui lo riportiamo in exp
  double r = std::sqrt(x*x + y*y);   
  double psi = std::atan(y/x);        //non è così semplice calcolare la fase, non so se esiste qualche funzione che considera i vari casi possibili
  return std::polar(r, psi);  //polar costruisce un exp complesso
}

double Firefly::moduleOrderParameter(std::vector<Firefly>& system) {
  double cosAll = 0;
  double sinAll = 0;
  int n = system.size();
  for(int i = 0; i < n; ++i){
    cosAll += std::cos(system[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(system[i].phase());  //stesso per sinAll
  }
  double x = cosAll/n;
  double y = sinAll/n;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY. Da qui lo riportiamo in exp
  return std::sqrt(x*x + y*y);

  //return sqrt(Firefly::orderParameter(system).real()*Firefly::orderParameter(system).real() + 
  //Firefly::orderParameter(system).imag()*Firefly::orderParameter(system).imag()) ;
}

void Firefly::evolve(std::vector<Firefly>& syst, double dt, bool interaction) {
  int size = syst.size();

  for (int i = 0; i < size; i++) {
    //evolve
    syst[i].Oscillator::evolve(dt);
  }

  if (interaction) {
    for (int i = 0; i < size; i++) {
      //interact
      std::vector<Firefly> newsciame = syst;  
      newsciame.erase(newsciame.begin() +i);
      syst[i].interact(newsciame, dt);
    }
  }
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2f& vector) {
  os << '(' << vector.x << ", " << vector.y << ')';
  return os;
}

std::ostream& operator<<(std::ostream& os, const sf::Vector2u& vector) {
  os << '(' << vector.x << ", " << vector.y << ')';
  return os;
}

bool operator>=(sf::Vector2f& lhs, sf::Vector2f& rhs) {
  return lhs.x >= rhs.x || lhs.y >= rhs.y;
}

bool operator<(Firefly& lhs, Firefly& rhs) {
  if (lhs.position().y < rhs.position().y)
    return true;
  if (lhs.position().y == rhs.position().y && lhs.position().x < rhs.position().x)
    return true;
  return false;
}
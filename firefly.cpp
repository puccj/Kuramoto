#include "firefly.h"
#include <iostream>
#include <random>
#include <algorithm>

sf::Vector2f Firefly::_windowDim = sf::Vector2f(1000, 700);
double Firefly::_K = 1;
bool Firefly::_interaction = false;

void Firefly::interact(std::vector<Firefly>& system, double dt) {
  int size = system.size();

  double sumSinDiff = 0;
  for (int i = 0; i < size; i++) {
    sumSinDiff += std::sin(system[i].phase() - _phase);   //theta_i - theta
  }

  setPhase(_phase + (_freq + _K*sumSinDiff/size ) * dt);  //sarebbe  phase += ()*dt + normalize
}

Firefly::Firefly(double freq, double phase, sf::Vector2f position): Oscillator(freq, phase), _position{position} { 
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

std::complex<double> Firefly::orderParameter(std::vector<Firefly>& system) {
  double cosAll = 0;
  double sinAll = 0;
  int n = system.size();
  for(int i = 0; i < n; ++i){
    cosAll += std::cos(system[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(system[i].phase());  //stesso per sinAll
  }
  double x = cosAll/n;
  double y = sinAll/n;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY.
  return std::complex<double>{x,y};
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
}

void Firefly::evolve(std::vector<Firefly>& syst) {
  int size = syst.size();
  sf::Clock clock;

  while(true) {
    double dt = 0;
    dt = clock.restart().asSeconds();
    for (int i = 0; i < size; i++) {
      //evolve
      syst[i].Oscillator::evolve(dt);
    }

    if (_interaction) {
      for (int i = 0; i < size; i++) {
        //interact
        //std::vector<Firefly> newsyst = syst;  
        //newsyst.erase(newsyst.begin() +i);
        syst[i].interact(syst, dt);
      }
    }
  }
}

void Firefly::draw(std::vector<Firefly>& syst) {
  int drawSize = 5;

  //load font
  sf::Font arial;
  if (!arial.loadFromFile("arial.ttf"))
    std::cerr << "ERR(21): Couldn't load font. Check if font is present in working folder.\n";
  
  //create static text
  sf::String staticText = "Press 'R' to add a random firefly (random position, phase and frequency).\n"; //Da fare: "according to ... distribution"
  staticText += "Press 'S' to show/hide non-flashing fireflies.\nScroll mouse wheel to change dimension of fireflies.\n";
  sf::Text text(staticText, arial, 12);
  
  //variables for events managing
  bool showOff = false;     //show/hide not-flashing (off) fireflies
  double addFrequency = 1;  //frequency of the new added firefly

  sf::RenderWindow window(sf::VideoMode(_windowDim.x, _windowDim.y), "Fireflies");
  window.setFramerateLimit(30);

  //main loop (or game loop)
  while (window.isOpen())
  {
    //reacts to Events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed) 
        window.close();

      if (event.type == sf::Event::Resized) {
        //view changes with window. Doing so, instead of squeezing/stretchin things, more things will be shown when resizing.
        sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
        window.setView(sf::View(visibleArea));
        
        //Let the place where fireflies spawn be the same of the window
        Firefly::setWindowDim(event.size.width, event.size.height);
      }

      if (event.type == sf::Event::KeyPressed) {
        
        //R : add a random firefly
        if (event.key.code == sf::Keyboard::R)    //Da fare: according to ... distribution
        {
          Firefly temp; //da fare: Firefly temp(dist)
          syst.push_back(temp);
        }

        //S : show/hide not-flashing fireflies
        if (event.key.code == sf::Keyboard::S)  
          showOff = !showOff;

        //P / M : change frequency of added firefly
        if (event.key.code == sf::Keyboard::P)
          addFrequency++;
        if (event.key.code == sf::Keyboard::M) {
          if (addFrequency > -1)
            addFrequency--;
        }

        //I : toggle interraction
        if (event.key.code == sf::Keyboard::I)
        {
          _interaction = !_interaction;
        }
      }

      //Change dimension of fireflies
      if (event.type == sf::Event::MouseWheelScrolled) {
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
          if (drawSize + event.mouseWheelScroll.delta > 1)
            drawSize += event.mouseWheelScroll.delta;
        }
      }

      //Add a fireflies
      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          Firefly temp(addFrequency, -1, sf::Vector2f(event.mouseButton.x - drawSize, event.mouseButton.y - drawSize));
          syst.push_back(temp);
        }
      }
    } //end react to events

    window.clear(); //clear with default color (black)

    //draw fireflies
    int N = syst.size();
    for (int i = 0; i < N; i++) {
      sf::CircleShape circle(drawSize);
      circle.setPosition(syst[i].position());

      if (std::cos(syst[i].phase()) > 0.9) {  //da fare meglio. Facendo così le più lente stanno anche accese per più tempo.
        circle.setFillColor(sf::Color::Yellow);
        window.draw(circle);
      }
      else if (showOff) {
        circle.setFillColor(sf::Color(120,120,120));  //gray
        window.draw(circle);
      }
    }

    //draw static text
    window.draw(text);

    //draw dinamic (changing) text
    sf::String dString = "\n\n\n\nPress 'I' to toggle interaction between fireflies (" + literal(_interaction) +
    ")\nOrder parameter: r = " + std::to_string(Firefly::moduleOrderParameter(syst));
    sf::Text dText(dString, arial,12);
    window.draw(dText);

    //refresh display
    window.display();
  }
}

void Firefly::plot(std::vector<Firefly>& syst) {
  int windowSize = 300;
  int radius = 5;
  int dim = windowSize + radius*2;
  sf::RenderWindow window(sf::VideoMode(dim, dim), "Plot");    //Deve essere non-resizable o comunque deve rimanere quadrata
  window.setPosition(sf::Vector2i(_windowDim));
  window.setFramerateLimit(30); //less lag

  //main loop (or game loop)
  while (window.isOpen())
  {
    //reacts to Events
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed) 
        window.close();
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::I)
        {
          _interaction = !_interaction;
        }
      }
    }

    window.clear();

    //draw oscillators
    int size = syst.size();
    for (int i = 0; i < size; i++) {
      sf::CircleShape circle(radius);
      double phase = syst[i].phase();
      circle.setPosition( (std::cos(phase)+1)*windowSize/2 , (std::sin(phase)+1)*windowSize/2 );
      window.draw(circle);
    }

    //refresh display
    window.display();
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
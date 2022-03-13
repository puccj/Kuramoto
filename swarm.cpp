#include "swarm.h"
#include <iostream>
#include <fstream>
#include <random>

Swarm::Swarm(int size, Distribution dist) : _size(size), _data{new Firefly[_size]} {
  for (int i = 0; i < _size; i++) {
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _windowDim.x);
    std::uniform_int_distribution<int> distY(0, _windowDim.y);
    _data[i] = Firefly(dist, sf::Vector2f(distX(seed), distY(seed)));
  }

  _Kc = 2 / M_PI* dist.evaluate(0);
}

// Swarm::Swarm(int size, Firefly data) : _size{size}, _data{new Firefly[_size]} { 
//   std::fill(_data, _data + _size, data);
// }

Swarm::Swarm(Swarm const& other) : _size{other._size}, _data{new Firefly[_size]} {
  std::copy(other._data, other._data + _size, _data);
}

Firefly& Swarm::operator[](int index) {
  if (index >= _size || index < 0) {
    std::cerr << "ERR (41): trying to access element out of bound.\n";
    return;
  }
  return _data[index];
}

std::complex<double> Swarm::orderParameter() {
  double cosAll = 0;
  double sinAll = 0;
  for(int i = 0; i < _size; ++i){
    cosAll += std::cos(_data[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(_data[i].phase());  //stesso per sinAll
  }
  double x = cosAll/_size;
  double y = sinAll/_size;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY.
  return std::complex<double>{x,y};
}

double Swarm::moduleOrderParameter() {
  double cosAll = 0;
  double sinAll = 0;
  for(int i = 0; i < _size; ++i){
    cosAll += std::cos(_data[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(_data[i].phase());  //stesso per sinAll
  }
  double x = cosAll/_size;
  double y = sinAll/_size;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY. Da qui lo riportiamo in exp
  return std::sqrt(x*x + y*y);
}

double Swarm::angleOrderParameter() {
  double cosAll = 0;
  double sinAll = 0;
  for (int i = 0; i < _size; i++) {
    cosAll += std::cos(_data[i].phase());
    sinAll += std::sin(_data[i].phase());
  }
  double x = cosAll/_size;
  double y = sinAll/_size;
  double angle = std::atan(y/x)*180/M_PI;
  if (x < 0)
    angle += 180;
  return angle;
}

void Swarm::evolve(bool saveData) {
  _evolve = true;
  sf::Clock clock;

  double time = 0;
  std::fstream fout;
  if (saveData)
    fout.open("data/r-t data.txt", std::ios::out);

  double dt = 0;
  while(_evolve) {
    dt = clock.restart().asSeconds();
    for (int i = 0; i < _size; i++) {
      //update
      _data[i].Oscillator::update(dt);
    }

    if (_interaction) {
      for (int i = 0; i < _size; i++) {
        //interact
        _data[i].interact(*this, dt);
      }

      if (saveData) {
        //std::cout << "Saving data\n";
        fout << time << '\t' << moduleOrderParameter() << '\n';
        time += dt;
      }
    }
  } //end while
  
  fout.close();
  std::cout << "r-t data saved\n";
}

void Swarm::draw() {
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
  window.setFramerateLimit(30);   //less lag

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
        Swarm::setWindowDim(event.size.width, event.size.height);
      }

      if (event.type == sf::Event::KeyPressed) {
        
        //R : add a random firefly
        if (event.key.code == sf::Keyboard::R)    //Da fare: according to ... distribution
        {
          Firefly temp; //da fare: Firefly temp(dist)
          push_back(temp);
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
          push_back(temp);
        }
      }
    } //end react to events

    window.clear(); //clear with default color (black)

    //draw fireflies
    for (int i = 0; i < _size; i++) {
      sf::CircleShape circle(drawSize);
      circle.setPosition(_data[i].position());

      if (std::cos(_data[i].phase()) > 0.9) {  //da fare meglio. Facendo così le più lente stanno anche accese per più tempo.
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
    sf::String dString = "\n\n\n\nPress 'I' to toggle interaction between fireflies (" + literal(_interaction) + ')';
    sf::Text dText(dString, arial,12);
    window.draw(dText);

    //refresh display
    window.display();
  }
}

void Swarm::plot() {
  int windowSize = 500;
  int radius = 1;
  int dim = windowSize + radius*2;

  //create window
  sf::RenderWindow window(sf::VideoMode(dim, dim), "Plot");    //Deve essere non-resizable o comunque deve rimanere quadrata
  window.setFramerateLimit(30); //less lag

  //load font
  sf::Font arial;
  if (!arial.loadFromFile("arial.ttf"))
    std::cerr << "ERR(32): Couldn't load font. Check if font is present in working folder.\n";

  //setting background
  sf::RectangleShape xAxis(sf::Vector2f(dim,1));
  xAxis.setPosition(0, dim/2);
  xAxis.setFillColor(sf::Color(130,130,130));
  sf::RectangleShape yAxis(sf::Vector2f(1,dim));
  yAxis.setPosition(dim/2, 0);
  yAxis.setFillColor(sf::Color(130,130,130));
  sf::Text xText("cos θ", arial, 12);
  xText.setPosition(sf::Vector2f(dim-30, dim/2 + 10));
  xText.setFillColor(sf::Color::Black);
  sf::Text yText("sin θ", arial, 12);
  yText.setPosition(sf::Vector2f(dim/2 -10, 30));
  yText.setFillColor(sf::Color::Black);
  yText.rotate(270);

  //circle for fireflies
  sf::CircleShape circle(radius);
  circle.setFillColor(sf::Color::Black);  

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

    window.clear(sf::Color::White);
    window.draw(xAxis);
    window.draw(yAxis);
    window.draw(xText);
    window.draw(yText);

    //draw oscillators
    for (int i = 0; i < _size; i++) {
      double phase = _data[i].phase();
      circle.setPosition( (std::cos(phase)+1)*windowSize/2 , (std::sin(phase)+1)*windowSize/2 );
      window.draw(circle);
    }

    //draw text
    sf::Text text("Order parameter: |r| = " + std::to_string(moduleOrderParameter()),arial,12);
    text.setFillColor(sf::Color::Black);
    window.draw(text);

    //draw arrow (r)
    sf::RectangleShape line(sf::Vector2f(moduleOrderParameter()*dim/2, 2));
    line.setPosition(dim/2, dim/2 +1);
    line.setFillColor(sf::Color::Black);
    line.rotate(angleOrderParameter());
    window.draw(line);

    //refresh display
    window.display();
  }
}

void Swarm::rkGraph(double kMin, double kMax, double kIncrement, double speedFactor, bool saveRT) {
  _interaction = true;

  if (kMin > kMax) {
    std::cerr << "WARN(21): You set kMin higher than kMax, I'm swapping them\n";
    double t = kMin;
    kMin = kMax;
    kMax = t;
  }

  //copy the begin state of the system before evolving it and preparing the file
  Swarm beginSyst = *this;
  std::fstream rkOut;
  rkOut.open("data/r-k data.txt", std::ios::out);

  //cicle throw different values of K
  for (double kValue = kMin; kValue < kMax; kValue += kIncrement) {
    std::fstream rtOut;
    if (saveRT)
      rtOut.open("data/r-t data (K = " + std::to_string(kValue) + ").txt", std::ios::out);

    setK(kValue);
    sf::Clock clock;
    double time = 0;
    double dt = 0;
    double sum = 0;
    double steps = 0;

    //evolve untill time < 12
    while(time < 12) {
      dt = clock.restart().asSeconds() *speedFactor;
      for (int i = 0; i < _size; i++)    //update
        _data[i].Oscillator::update(dt); 
      for (int i = 0; i < _size; i++)    //interact
        _data[i].interact(*this, dt);   
      time += dt;

      //average values of K from time=6 to time=12
      if (time > 6) {
        sum += moduleOrderParameter();
        steps++;
      }

      if (saveRT) {
        //std::cout << "Saving data\n";
        rtOut << time << '\t' << moduleOrderParameter() << '\n';
        time += dt;
      }
    } //end while

    //done r-t, save average |r|
    rtOut.close();
    rkOut << kValue << '\t' << sum/steps << '\n';
    std::cout << "Done K = " << kValue << '\n';

    //reset the system
    *this = beginSyst;
  }
  rkOut.close();
  std::cout << "r-k data saved\n";
}

Swarm& Swarm::operator=(Swarm const& other) { 
  if (this != &other) { 
    delete[] _data;
    _size = other._size;
    _data = new Firefly[_size]; 
    std::copy(other._data, other._data + _size, _data); 
  } 
  return *this;
}

void Swarm::push_back(Firefly const& add) {
  Firefly temp[_size+1];
  std::copy(_data, _data + _size, temp);
  temp[_size] = add;
  _size++;
  delete[] _data;
  _data = new Firefly[_size];
  std::copy(temp, temp + _size, _data);  
}
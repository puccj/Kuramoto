#include "swarm.h"
#include <iostream>
#include <fstream>
#include <random>

void Swarm::interact(Oscillator& chosen, double dt) {
  double sumSinDiff = 0;
  double phase = chosen.phase();
  for (int i = 0; i < _size; i++) {
    sumSinDiff += std::sin(_data[i].phase() - phase);   //theta_i - theta
  }

  chosen.setPhase(phase + (chosen.freq() + _K*sumSinDiff/_size ) * dt);  //sarebbe  phase += ()*dt + normalize
}

Swarm::Swarm(int size, Distribution dist, TitledWindow window) : _size(size), _data{new Oscillator[_size]}, _window{window} {
  for (int i = 0; i < _size; i++) {
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _window.width);
    std::uniform_int_distribution<int> distY(0, _window.height);
    _data[i] = Oscillator(dist, sf::Vector2f(distX(seed), distY(seed)));
  }

  _Kc = 2 / (M_PI* dist.evaluate(0));

  if (window.name == "default") {
    _window.name = std::to_string(size) + '-' + dist.toString();
  }
}

Swarm::Swarm(int size, double freq, TitledWindow window) : _size{size}, _data{new Oscillator[_size]}, _window{window} {
  for (int i = 0; i < _size; i++) {
    std::random_device seed;
    std::uniform_int_distribution<int> distX(0, _window.width);
    std::uniform_int_distribution<int> distY(0, _window.height);
    _data[i] = Oscillator(freq, sf::Vector2f(distX(seed),distY(seed)));
  }

  if (window.name == "default") {
    _window.name = std::to_string(size) + '-' + std::to_string(freq) + "Hz";
  }
}

/*
Oscillator& Swarm::operator[](int index) {
  if (index >= _size || index < 0) {
    std::cerr << "ERR (41): trying to access element out of bound. Exiting...\n";
    exit(0);
  }
  return _data[index];
}
*/

void Swarm::setWindowDim(int x, int y) {
  _window.width = x;
  _window.height = y;
}

Complex Swarm::orderParameter() {
  double cosAll = 0;
  double sinAll = 0;
  for(int i = 0; i < _size; ++i){
    cosAll += std::cos(_data[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(_data[i].phase());  //stesso per sinAll
  }
  double x = cosAll/_size;
  double y = sinAll/_size;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY.
  double mod = std::sqrt(x*x + y*y);
  double phase = std::atan(y/x)*180/M_PI;
  if(x < 0)
    phase += 180;
  return {mod,phase};
}

double Swarm::orderParameter(Component component) {
  double cosAll = 0;
  double sinAll = 0;
  for(int i = 0; i < _size; ++i){
    cosAll += std::cos(_data[i].phase());  //cosAll è la somma di tutti i coseni di theta_i
    sinAll += std::sin(_data[i].phase());  //stesso per sinAll
  }
  double x = cosAll/_size;
  double y = sinAll/_size;           //possiamo vedere la somma dei fasori come COS/N + i*SIN/N = X + iY. Da qui lo riportiamo in exp
  
  if(component == Component::Module)
    return std::sqrt(x*x + y*y);
  
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
    fout.open("./data/" + _window.name + " - r-t_data.txt", std::ios::out);

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
        interact(_data[i], dt);
      }

      if (saveData) {
        //std::cout << "Saving data\n";
        fout << time << '\t' << orderParameter(Component::Module) << '\n';
        time += dt;
      }
    }
  } //end while
  
  if (saveData) {
    fout.close();
    std::cout << "r-t data saved\n";
  }
}

void Swarm::draw() {
  int drawSize = 5;

  //load font
  sf::Font arial;
  if (!arial.loadFromFile("arial.ttf"))
    std::cerr << "ERR(21): Couldn't load font. Check if font is present in working folder.\n";
  
  //create static text
  sf::Text text("Press 'S' to show/hide non-flashing fireflies.\nScroll mouse wheel to change dimension of fireflies.\n", arial, 12);
  
  sf::RenderWindow window(sf::VideoMode(_window.width, _window.height), "Fireflies: " + _window.name);
  window.setFramerateLimit(30);   //less lagsf::Image icon;
  
  // Icon from <a href="https://www.flaticon.com/free-icons/firefly" title="firefly icons">
  // Firefly icons created by Vitaly Gorbachev - Flaticon</a>
  sf::Image icon;
  if (icon.loadFromFile("fireflyIcon.png"))
    window.setIcon(512,512,icon.getPixelsPtr());
  else
    std::cerr << "WARN(31): Could not load icon image. Check if icon file is in the working folder.\n";

  bool showOff = false;     //show/hide not-flashing (off) fireflies

  //main loop (or game loop)
  while (window.isOpen())
  {
    //check if dimension changed:
    if (window.getSize() != sf::Vector2u(_window.width, _window.height)) {
      window.setSize(sf::Vector2u(_window.width, _window.height));
    }

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
        
        //S : show/hide not-flashing fireflies
        if (event.key.code == sf::Keyboard::S)  
          showOff = !showOff;

        //I : toggle interraction
        if (event.key.code == sf::Keyboard::I)
        {
          _interaction = !_interaction;
        }
      }
      
      if (window.hasFocus()) {
        //Change dimension of fireflies
        if (event.type == sf::Event::MouseWheelScrolled) {
          if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
            if (drawSize + event.mouseWheelScroll.delta > 1)
              drawSize += event.mouseWheelScroll.delta;
          }
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
    sf::String dString = "\n\nPress 'I' to toggle interaction between fireflies (" + literal(_interaction) + ')';
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
  sf::Text xText(L"cos θ", arial, 12);
  xText.setPosition(sf::Vector2f(dim-30, dim/2 + 10));
  xText.setFillColor(sf::Color::Black);
  sf::Text yText(L"sin θ", arial, 12);
  yText.setPosition(sf::Vector2f(dim/2 -10, 30));
  yText.setFillColor(sf::Color::Black);
  yText.rotate(270);

  //circle for fireflies
  sf::CircleShape circle(radius);
  circle.setFillColor(sf::Color::Black);  

  //create window
  sf::RenderWindow window(sf::VideoMode(dim, dim), "Plot: " + _window.name);    //Deve essere non-resizable o comunque deve rimanere quadrata
  window.setFramerateLimit(30); //less lag

  // Icon from: <a href="https://www.flaticon.com/free-icons/scatter-plot" title="scatter plot icons">
  // Scatter plot icons created by Flowicon - Flaticon</a>
  sf::Image icon;
  if (icon.loadFromFile("plotIcon.png"))
    window.setIcon(512,512,icon.getPixelsPtr());
  else
    std::cerr << "WARN(31): Could not load icon image. Check if icon file is in the working folder.\n";

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
      if (event.type == sf::Event::Resized) {
        int smaller = event.size.height;
        if (event.size.width < event.size.height)
          smaller = event.size.width;
        window.setSize(sf::Vector2u(smaller, smaller));
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

    Complex r = orderParameter();

    //draw text
    sf::Text text("Order parameter: |r| = " + std::to_string(orderParameter().mod),arial,12);
    text.setFillColor(sf::Color::Black);
    window.draw(text);

    //draw arrow (r)
    sf::RectangleShape line(sf::Vector2f(orderParameter().mod*dim/2, 2));
    line.setPosition(dim/2, dim/2 +1);
    line.setFillColor(sf::Color::Red);
    line.rotate(orderParameter().phase);
    window.draw(line);

    //refresh display
    window.display();
  }
}

void Swarm::rkGraph(double kMin, double kMax, double kIncrement, bool saveRT, double timeIncrement) {
  _interaction = true;

  if (kMin > kMax) {
    std::cerr << "WARN(21): You set kMin higher than kMax, I'm swapping them\n";
    double t = kMin;
    kMin = kMax;
    kMax = t;
  }

  //copy the begin state of the system before evolving it and preparing the file
  Oscillator beginData[_size];
  std::copy(_data, _data + _size, beginData);
  std::fstream rkOut;
  rkOut.open("./data/" + _window.name + " - r-k data.txt" , std::ios::out);
  rkOut << "Kc = " << _Kc << '\n';

  //cicle throught different values of K
  for (double kValue = kMin; kValue < kMax; kValue += kIncrement) {
    std::fstream rtOut;
    if (saveRT)
      rtOut.open("./data/" + _window.name + " - r-t_data(K=" + std::to_string(kValue) + ").txt", std::ios::out);

    setK(kValue);
    sf::Clock clock;
    double time = 0;
    double dt = 0;
    double sum = 0;
    double steps = 0;

    //width of progress bar
    int barWidth = 70;

    //evolve untill time < 15 seconds
    int timeMax = 15;
    std::cout << "Simulating K = " << kValue << '\r';
    while(time < timeMax) {
      if (timeIncrement == 0)
        dt = clock.restart().asSeconds();
      else
        dt = timeIncrement;

      for (int i = 0; i < _size; i++)    //update
        _data[i].Oscillator::update(dt); 
      for (int i = 0; i < _size; i++)    //interact
        interact(_data[i], dt);

      double r = orderParameter(Component::Module);

      //average values of K from time=12 to time=15
      if (time > timeMax*4/5) {
        sum += r;
        steps++;
      }

      if (saveRT) {
        //std::cout << "Saving data\n";
        rtOut << time << '\t' << r << '\n';
      }
      
      time += dt;

      //print progress
      /*
      int pos = barWidth * time/timeMax;
      for (int i = 0; i < barWidth; i++) {
        if (i < pos) std::cout << '=';
        else if (i == pos) std::cout << '>';
        else std::cout << ' ';
      }
      std::cout << "] " << int(time* 100/timeMax) << " %\r";
      std::cout.flush();
      */

    } //end while

    //done r-t, save average |r|
    rtOut.close();
    rkOut << kValue << '\t' << sum/steps << '\n';

    std::cout.flush();
    std::cout << "      Done K = " << kValue << "\t ";
    // for (int i = 0; i < barWidth; i++)
    //   std::cout << ' ';
    std::cout << "      \n";

    //reset the system
    std::copy(beginData, beginData + _size, _data);
  }
  rkOut.close();
  std::cout << "r-k data saved\n";
}

/*Swarm& Swarm::operator=(Swarm const& other) { 
  if (this != &other) { 
    delete[] _data;
    _size = other._size;
    _data = new Firefly[_size]; 
    std::copy(other._data, other._data + _size, _data); 
  } 
  return *this;
}*/

/*
void Swarm::push_back(Firefly const& add) {
  Firefly temp[_size+1];
  std::copy(_data, _data + _size, temp);
  temp[_size] = add;
  _size++;
  
  delete[] _data;
  _data = new Firefly[_size];
  std::copy(temp, temp + _size, _data); 
}*/
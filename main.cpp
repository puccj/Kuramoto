#include "firefly.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <SFML/Graphics.hpp>

int main(){
  /* in teoria tenendo uguali dt e usToSleep si riesce a fare un tempo vero (cioè se la frequenza è 1 lampeggia davvero 1 volta al secondo)
   * In realtà però lagga un po' e più il dt (e lo sleep for) sono piccoli, più si vede questa cosa.
   * Magari metterò uno slider o qualcosa del genere per regolarne uno affinché effettivamente il tempo di calcolo sia lo stesso che
   * quello reale che passa tra un frame e l'altro
   */
  
  // Oscillator::setDt(0.050);   //time (seconds) "di calcolo"
  // int sleep = 50000;          //time (microseconds) between each frame
  // int simulationLength = 20;  //duration (seconds) of the simulation
  // std::chrono::duration<int, std::micro> usToSleep (sleep);

  Firefly::setWindowDim(1000,700);   //analogus to Firefly::setWindowDim(sf::Vector2f(300,200));
  double drawSize = 10;
  
  std::vector<Firefly> sciame(5);  //create an array of N Firefly (no parameter set -> random)
  for (int i = 0; i < 5; i++)
    sciame[i].setFreq(i+0.5);
  
  // Firefly prova = Firefly(0.5); //firefly with frequency of 2 and random phase and position
  // std::cout << "Frequency: " << prova.freq() << '\n';
  // std::cout << "Position: (" << prova.position().x << ", " << prova.position().y << ")\n";


  // Oscillator prova(2,0);
  // prova.setPhase(M_PI);
  // std::cout << prova.phase() << '\n';
  // prova.setPhase(2*M_PI);
  // std::cout << prova.phase() << '\n';
  /*  
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
   */

  sf::Clock clock;
  sf::RenderWindow window(sf::VideoMode(Firefly::windowDim().x, Firefly::windowDim().y), "Fireflies");

  //load font
  sf::Font arial;
  if (!arial.loadFromFile("arial.ttf"))
    std::cerr << "ERR(21): Couldn't load font. Check font is present in working folder.\n";
  
  //create text
  sf::String staticText = "Press 'R' to add a random firefly (random position, phase and frequency).\n";
  staticText += "Press 'S' to show/hide not-flashing fireflies.\nScroll your mouse wheel to change dimension of fireflies.\n";
  sf::Text text(staticText, arial, 12);
  //text.setStyle(sf::Text::Bold | sf::Text::Underlined);

  //debug:
  std::cout << "Freq\tPosition\n";
  for (int i = 0; i < 5; i++) {
    std::cout << sciame[i].freq() << '\t' << sciame[i].position() << '\n';
  }
  
  //variables for KeyPressed
  bool showOff = false;                             //show/hide not-flashing (off) fireflies
  //sf::Vector2f addPosition = sf::Vector2f(-1,-1);   //position where to add new firefly pressing 'A'
  double addFrequency = 1;                         //frequency of the new added firefly

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

        //for debug:
        std::cout << "New sizes: " << Firefly::windowDim() << '\n';
        std::cout << "New sizes: " << window.getSize() << '\n';
      }

      if (event.type == sf::Event::KeyPressed) {
        
        //add a random firefly
        if (event.key.code == sf::Keyboard::R)  
        {
          Firefly temp;
          sciame.push_back(temp);
          
          //debug:
          std::cout << "\nFreq\tPosition\n";
          int size = sciame.size();
          for (int i = 0; i < size; i++)
            std::cout << sciame[i].freq() << '\t' << sciame[i].position() << '\n';
        }

        //show/hide not-flashing fireflies
        if (event.key.code == sf::Keyboard::S)  
        {
          showOff = !showOff;
          //debug:
          std::cout << "Showing not-flashing fireflies: " << showOff << '\n';
        }
        
        /*
        //Arrows: change position where to add firefly
        if (event.key.code == sf::Keyboard::Left)   //addPosition x--
        {
          if (addPosition.x > 0)
            addPosition.x--;
          if (addPosition == sf::Vector2f(0,0))
            addPosition = sf::Vector2f(-1,-1);
        }
        if (event.key.code == sf::Keyboard::Right)  //addPosition x++
        {
          if (addPosition.x < window.getSize().x - drawSize)
            addPosition.x++;
        }
        if (event.key.code == sf::Keyboard::Up)     //addPosition y--
        {
          if (addPosition.y > 0)
            addPosition.y--;
          if (addPosition == sf::Vector2f(0,0))
            addPosition = sf::Vector2f(-1,-1);
        }
        if (event.key.code == sf::Keyboard::Down)   //addPosition y++
        {
          if (addPosition.y < window.getSize().y - drawSize)
            addPosition.y--;
        }

        //P / M : change frequency of added firefly
        if (event.key.code == sf::Keyboard::P)
          addFrequency++;
        if (event.key.code == sf::Keyboard::M) {
          if (addFrequency > -1)
            addFrequency--;
        }

        //A : add a specific firefly
        if (event.key.code == sf::Keyboard::A)
        {
          Firefly temp(addFrequency, -1, addPosition);
          sciame.push_back(temp);
          
          //debug:
          std::cout << "\nFreq\tPosition\n";
          int size = sciame.size();
          for (int i = 0; i < size; i++)
            std::cout << sciame[i].freq() << '\t' << sciame[i].position() << '\n';
        }
        */
      }

      if (event.type == sf::Event::MouseWheelScrolled) {  //change dimension of fireflies
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
          if (drawSize + event.mouseWheelScroll.delta > 1)
            drawSize += event.mouseWheelScroll.delta;
        }
      }

      if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
          Firefly temp(addFrequency, -1, sf::Vector2f(event.mouseButton.x - drawSize, event.mouseButton.y - drawSize));
          sciame.push_back(temp);

          //debug:
          std::cout << "Mouse: " << sf::Vector2f(event.mouseButton.x, event.mouseButton.y) << '\n';
          std::cout << "Window: " << window.getSize() << '\n';
          std::cout << "Grid: " << Firefly::windowDim() << '\n';
        }
      }
    }

    window.clear(); //clear with default color (black)

    //draw static text
    window.draw(text);

    //draw changing/dinamic text
    sf::Text addText("\n\n\n\nClick everywhere to add a firefly with frequency of " + 
    toString(addFrequency) + " Hz.\n(Use arrow to change position and 'P' / 'M' key to change frequency)", arial, 12);
    
    window.draw(addText);

    //draw fireflies
    int N = sciame.size();
    for (int i = 0; i < N; i++) {
      sf::CircleShape circle(drawSize);
      circle.setPosition(sciame[i].position());

      if (std::sin(sciame[i].phase()) > 0.9) {  //da fare meglio. Facendo così le più lente stanno anche accese per più tempo.
        circle.setFillColor(sf::Color::Yellow);
        window.draw(circle);
      }
      else if (showOff) {
        circle.setFillColor(sf::Color(120,120,120));  //gray
        window.draw(circle);
      }
    }
    window.display();
    sf::Time elapsed = clock.restart();
    Firefly::evolve(sciame, elapsed.asSeconds());
  }
  
}
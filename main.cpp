#include "firefly.h"
#include <chrono>
#include <thread>
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
  Firefly::setWindowDim(1000,700);
  double drawSize = 10;

  std::vector<Firefly> sciame(5);  //create an array of N Firefly (no parameter -> dist Lorentz)
  for (int i = 0; i < 5; i++)
    sciame[i].setFreq(i+0.5);

  //load font
  sf::Font arial;
  if (!arial.loadFromFile("arial.ttf"))
    std::cerr << "ERR(21): Couldn't load font. Check font is present in working folder.\n";
  
  //create static text
  sf::String staticText = "Press 'R' to add a random firefly (random position, phase and frequency).\n"; //Da fare: "according to ... distribution"
  staticText += "Press 'S' to show/hide not-flashing fireflies.\nScroll your mouse wheel to change dimension of fireflies.\n";
  sf::Text text(staticText, arial, 12);
  
  //variables for events managing
  bool showOff = false;     //show/hide not-flashing (off) fireflies
  double addFrequency = 1;  //frequency of the new added firefly
  //double addDist = Distribution::Lorentz

  sf::Clock clock;
  sf::RenderWindow window(sf::VideoMode(Firefly::windowDim().x, Firefly::windowDim().y), "Fireflies");

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
        
        //add a random firefly
        if (event.key.code == sf::Keyboard::R)    //Da fare: according to ... distribution
        {
          Firefly temp; //da fare: Firefly temp(dist)
          sciame.push_back(temp);
        }

        //show/hide not-flashing fireflies
        if (event.key.code == sf::Keyboard::S)  
          showOff = !showOff;

        //P / M : change frequency of added firefly
        if (event.key.code == sf::Keyboard::P)
          addFrequency++;
        if (event.key.code == sf::Keyboard::M) {
          if (addFrequency > -1)
            addFrequency--;
        }

        /*
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
        }
      }
    }

    window.clear(); //clear with default color (black)

    //draw static text
    window.draw(text);

    //draw changing/dinamic text
    //sf::Text addText("\n\n\n\nClick everywhere to add a firefly with frequency of " + 
    //toString(addFrequency) + " Hz.\n(Use arrow to change position and 'P' / 'M' key to change frequency)", arial, 12);
    
    //window.draw(addText);

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

    /*for(std::vector<Firefly>::iterator it = sciame.begin(); it != sciame.end(); ++it){
      std::vector<Firefly> newsciame = sciame;
      newsciame.erase(it);
      sciame[it]->interact(newsciame, elapsed.asSeconds());
    }
    * Ti dà errore perché it è un puntatore di un elemento di sciame. Quindi prima cosa sarebbe it->interract (non sciame[it]).
    * Poi non puoi usare il puntatore di sciame per cancellare l'elemento di newsciame.
    */
    
    int size = sciame.size();
    for (int i = 0; i < size; i++) {
      std::vector<Firefly> newsciame = sciame;
      newsciame.erase(newsciame.begin() +i);
      sciame[i].interact(newsciame, elapsed.asSeconds());
    }

    Firefly::evolve(sciame, elapsed.asSeconds());
  }
  
}
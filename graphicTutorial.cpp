#include "firefly.h"
#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
  sf::Clock clock;
  sf::RenderWindow window(sf::VideoMode(200, 200), "Fireflies");

  //main loop (or game loop)
  while (window.isOpen())
  {
    //reacts to Events
    sf::Event event;
    while (window.pollEvent(event))
    {
      switch (event.type)
      {
      case sf::Event::Closed:
        window.close();
        break;
      case sf::Event::Resized:
        //Make the grid bigger (also, I need to let the grid be rectangular, other than a square)
        // std::cout << "new width: " << event.size.width << std::endl;
        // std::cout << "new height: " << event.size.height << std::endl;
        break;
      case sf::Event::LostFocus:
        //Qui si può fare qualcosa? (tipo stoppare la simulazione)
        break;
      case sf::Event::GainedFocus:
        //E magari qui riprenderla
        break;
      case sf::Event::TextEntered:
        // This is different from KeyPressed. 
        // Pressing '^' then 'e' on a French keyboard is 2 KeyPressed events, but 1 TextEntered event containing the 'ê' character.
        if (event.text.unicode < 128)
          std::cout << "ASCII character typed: " << static_cast<char>(event.text.unicode) << std::endl;
        break;
      case sf::Event::KeyPressed: //To disable repeated KeyPressed events, you can call window.setKeyRepeatEnabled(false)
      {
        if (event.key.code == sf::Keyboard::Escape)
        {
        std::cout << "the escape key was pressed" << std::endl;
        std::cout << "control:" << event.key.control << std::endl;
        std::cout << "alt:" << event.key.alt << std::endl;
        std::cout << "shift:" << event.key.shift << std::endl;
        std::cout << "system:" << event.key.system << std::endl;
        }
      }
        break;
      case sf::Event::KeyReleased:
        break;
      case sf::Event::MouseWheelScrolled: // We can use this to zoom (i.e. make each square of the board bigger.)
        if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
          std::cout << "wheel type: vertical" << std::endl;
        else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
          std::cout << "wheel type: horizontal" << std::endl;
        else
          std::cout << "wheel type: unknown" << std::endl;
        std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
        std::cout << "mouse x: " << event.mouseWheelScroll.x << std::endl;
        std::cout << "mouse y: " << event.mouseWheelScroll.y << std::endl;
      case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Right)
        {
            std::cout << "the right button was pressed" << std::endl;
            std::cout << "mouse x: " << event.mouseButton.x << std::endl;
            std::cout << "mouse y: " << event.mouseButton.y << std::endl;
        }

      default:
        break;

      } //end switch (event.type)
    } //end while (event managing)

    window.clear(); //clear with default color (black)
    
    //window.draw(shape);
    window.display();

    sf::Time elapsed = clock.restart();
    //updateGame(elapsed)
  }
  
}
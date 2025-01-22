#include <SFML/Graphics.hpp>
#include "./include/env.hpp"

#include <SFML/Graphics.hpp>
#include "./include/env.hpp"
#include <string>

int main() {
    sf::Font font;
    // Load Font
    if (!font.loadFromFile("Game Paused DEMO.otf")) {
        std::cout << "Font Open ERROR" << std::endl;
        return -1;
    }
sf::Font number_font;
    // Load Font
    if (!number_font.loadFromFile("ARIAL.TTF")) {
        std::cout << "Font Open ERROR" << std::endl;
        return -1;
    }
    sf::RenderWindow window(sf::VideoMode(700, 800), "Tetris");
    Environnement env;
    window.setFramerateLimit(60);

    int Counter = 0; // For time counting depending on the delay
    while (window.isOpen()) {
        sf::Event event;
        int action = 0;

        // Handle events
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    action = 1;
                } else if (event.key.code == sf::Keyboard::Right) {
                    action = 2;
                } else if (event.key.code == sf::Keyboard::Up) {
                    action = 3;
                } else if (event.key.code == sf::Keyboard::Down) {
                    action = 4;
                }
            }
        }

        // Update environment based on action
        env.Step(action);

        // Update timer logic
        if (Counter == 5) {  
            env.Update_Sec();  
            Counter = 0;
        } else {
            Counter++;
        }

        
        window.clear();

       
        env.Render(&window,0,0);

       
        sf::RectangleShape Timer_Container({350, 150}); 
        Timer_Container.setFillColor(sf::Color(212, 206, 205));
        Timer_Container.setPosition({400, 0});
        window.draw(Timer_Container);

       
        sf::Text Timer_Text; // Timer Text
        Timer_Text.setFont(font);
        Timer_Text.setString("Timer");
        Timer_Text.setCharacterSize(40);
        Timer_Text.setPosition({500, 10});
        window.draw(Timer_Text);

        // Draw the timer value
        sf::Text Timer_Value; // Timer Value
        if(env.timer->sec < 10){
            Timer_Value.setString(std::to_string(env.timer->min) + " : 0" + std::to_string(env.timer->sec));
        }
        else
        {
            Timer_Value.setString(std::to_string(env.timer->min) + " : " + std::to_string(env.timer->sec));
        }
        Timer_Value.setFont(number_font);
        Timer_Value.setCharacterSize(40);
        Timer_Value.setPosition({500, 60});
        window.draw(Timer_Value);

       
        window.display();

       
        sf::sleep(sf::milliseconds(200));
    }

    return 0;
}

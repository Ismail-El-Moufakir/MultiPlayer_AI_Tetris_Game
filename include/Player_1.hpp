#include "SFML/Network.hpp"
#include <iostream>
#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "env.hpp"
#include "Network.hpp"


using namespace sf ;
class Player_1_Window: public RenderWindow
{
    private:
        int GameMod ;
    public:
        int width ;
        int height;
        sf::Texture BackGround_Texture ;
        sf::Font font ;

        Player_1_Window(int w,int h,const char* Title);
        UdpSocket* socket ;
};
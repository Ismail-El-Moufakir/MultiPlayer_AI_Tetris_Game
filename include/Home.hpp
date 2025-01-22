#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


using namespace sf ;
class HomeWindow: public RenderWindow
{
    private:
        int GameMod ;
    public:
        int width ;
        int height;
        sf::Texture BackGround_Texture ;
        sf::Font font ;

        HomeWindow(int w,int h,const char* Title);
        void Draw_Home();
};
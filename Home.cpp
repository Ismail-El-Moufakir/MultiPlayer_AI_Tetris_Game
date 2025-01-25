#include "../include/Home.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../include/sfmlbutton.hpp"
#include <thread>



void runProgram(const std::string& command) {
    int result = system(command.c_str());
    if (result != 0) {
        std::cerr << "Erreur lors de l'exécution de : " << command << "\n";
    } else {
        std::cout << command << " a été exécuté avec succès.\n";
    }
}

HomeWindow::HomeWindow(int w, int h, const char* Title):
RenderWindow(sf::VideoMode(w,h),Title),width(w),height(h)
{
    //Loading Background Texture
    BackGround_Texture.loadFromFile("img/Background.png");
    //loading Font
    if (!font.loadFromFile("Game Paused DEMO.otf"))
    {
        std::cout<< "Font Open ERROR" <<std::endl ;
        return ;
    }
    

}
void HomeWindow::Draw_Home()
{
    RectButton Vs_Lan_Btn(font,{200,50},{width/7 + 120,250});
    RectButton Vs_AI_Btn(font,{200,50},{width/7 + 120,350});
    sf::Event event ;
    //event handeling loop/switch case 
    while(this->pollEvent(event))
    {
        Vs_Lan_Btn.getButtonStatus(*this,event);
        Vs_AI_Btn.getButtonStatus(*this,event);
        switch (event.type)
        {
        case Event::Closed :
            this->close() ;
            break;
            
        }
        if(Vs_Lan_Btn.isPressed)
        {
            this->GameMod = 0 ;
            std::thread thread1(runProgram, "./player1");
            std::thread thread2(runProgram, "./player2");
            thread1.join();
            thread2.join();
    }
        }
        if(Vs_AI_Btn.isPressed)
        {
            this->GameMod = 1 ;
            std::cout<<"Vs AI Button Pressed : GameMod "<<this->GameMod<<std::endl;
        }
         
    this->clear();
    // Background of HOME screen
    sf::Sprite Background(BackGround_Texture);
    this->draw(Background);
    //Title of Home Screen
    sf::Text Title ;
    sf::Color Title_Background_Color(50, 94, 135,155); 
    sf:RectangleShape Title_Rec({420,75});
    Title_Rec.setFillColor(Title_Background_Color); // set the Title background with little opacity 
    Title_Rec.setPosition(width/7 +10,100);
    this->draw(Title_Rec);
    Title.setString("My BOMBO Tetris"); //Title Drawing
    Title.setFont(font);
    Title.setCharacterSize(50);
    Title.setPosition(width/6 +10 ,100);
    this->draw(Title);
    // Button and GameMod Panel
    sf::RectangleShape Btn_Panel_Rec({420,350});//Panel for Game Buttons
    Btn_Panel_Rec.setPosition({width/7 +10,200});
    Btn_Panel_Rec.setFillColor(Title_Background_Color);
    this->draw(Btn_Panel_Rec);
    Vs_Lan_Btn.setButtonLabel(30,"Vs Lan");
    Vs_Lan_Btn.draw(*this);
    Vs_AI_Btn.setButtonLabel(30,"Vs AI");
    Vs_AI_Btn.draw(*this);

    



    this->display();
}


int main()
{
    HomeWindow win(600,600,"Titris Game");
    while(win.isOpen())
    {
        win.Draw_Home();
    }
}

#include "../include/Player_1.hpp"
#include "../include/env.hpp"



Player_1_Window::Player_1_Window(int w, int h, const char* Title):
RenderWindow(sf::VideoMode(w,h),Title),width(w),height(h)
{
    //loading Font
    if (!font.loadFromFile("Game Paused DEMO.otf"))
    {
        std::cout<< "Font Open ERROR" <<std::endl ;
        return ;
    }
    

}
//Waiting Connection function
void Player_1_Window::Waiting_Connection()
{
    while ((true))
    {
        this->clear() ;
        sf::RectangleShape Wait_Connection_Panel(sf::Vector2f(1400,800)) ;
        Wait_Connection_Panel.setFillColor(sf::Color::Black) ;
        sf::Text Wait_Connection_Text("Waiting for Connection",font,50) ;
        Wait_Connection_Text.setFillColor(sf::Color::White) ;
        Wait_Connection_Text.setPosition(450,350) ;
        this->draw(Wait_Connection_Panel) ;
        this->draw(Wait_Connection_Text) ;
        this->display() ;
        //checking for connection 
        if(CheckConnection(socket))
        {
            break ;
        }
    
    }
    
}
//function for oponent drawing

void Player_1_Window::OponentRender(int Width_Marge,int Height_Marge,int** Mtx_Grid,char PieceHolder[8])
{
    // draw a grid 
      for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            sf::RectangleShape Cell({40.f, 40.f});
            Cell.setPosition({Width_Marge+j * 40.f,Height_Marge + i * 40.f});
            Cell.setOutlineColor(sf::Color::Black);
            Cell.setOutlineThickness(1.f);
            Cell.setFillColor(sf::Color::White);
            this->draw(Cell);
        }
      }
    //Render Piece 
    for (int i = 0; i < 4; i++) {
        sf::RectangleShape Cell({40.f, 40.f});
        float x = Width_Marge + PieceHolder[i * 2 + 1] * 40.f;
        float y = Height_Marge + PieceHolder[i * 2] * 40.f;
        Cell.setPosition({x, y});
        Cell.setOutlineColor(sf::Color::Black);
        Cell.setOutlineThickness(1.f);
        Cell.setFillColor(sf::Color::Magenta);
        this->draw(Cell);
    }
    //draw the matrix
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 10; j++) {
            if (Mtx_Grid[i][j] != 0) {
                sf::RectangleShape Cell({40.f, 40.f});
                Cell.setPosition({Width_Marge+j * 40.f,Height_Marge + i * 40.f});
                Cell.setOutlineColor(sf::Color::Black);
                Cell.setOutlineThickness(1.f);
                Cell.setFillColor(COLORS[Mtx_Grid[i][j] - 1]);
                this->draw(Cell);
            }
        }
    }
}

int main()
{
    Environnement Player_Env;
    Environnement Opponent_Env;
    Player_1_Window Player_1_win(1400,800,"Player One") ;
    Player_1_win.socket = new sf::UdpSocket() ;
    //Binding the socket to port 54000
    if(Player_1_win.socket->bind(54000) != sf::Socket::Done)
    {
        std::cerr << "Error: Could not bind socket to port." << std::endl;
        return 1;
    }
    //Waiting for connection
    Player_1_win.Waiting_Connection() ;
    //Connection Established entering the game loop
    while(Player_1_win.isOpen())
    {
        //receiving oponent data 
        char dataHolder[8] ;
        int **Mtx_Grid = new int*[20] ;
        for (int i = 0; i < 20; i++) {
            Mtx_Grid[i] = new int[10];
            for (int j = 0; j < 10; j++) {
                Mtx_Grid[i][j] = 0;
            }
        }
        receivingMatrix(Player_1_win.socket,Mtx_Grid) ;
        receiveCoordinates(Player_1_win.socket,dataHolder) ;
        //Rendering the oponent
        Player_1_win.OponentRender(600,0,Mtx_Grid,dataHolder) ;
        //
        sf::Event event;
        int action = 0;
        while (Player_1_win.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                Player_1_win.close();
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

       

        Player_1_win.clear() ;
        Player_1_win.display() ;
    }

}

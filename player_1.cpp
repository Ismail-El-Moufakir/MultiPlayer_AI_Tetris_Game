// player1.cpp
#include "./include/env.hpp"
#include "./include/Network.hpp"
#include <SFML/Graphics.hpp>
#include <thread>
#include <chrono>

#define PLAYER_PORT 54000
#define OPPONENT_PORT 54001
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define GRID_OFFSET_X 420
#define GRID_OFFSET_Y 0
sf::Color COLORS1[7] = {
    sf::Color::Magenta,      // 1
    sf::Color::Green,        // 2
    sf::Color::Yellow,       // 3
    sf::Color::Blue,         // 4
    sf::Color::Red,          // 5
    sf::Color::Cyan,         // 6
    sf::Color(135, 35, 48)   // 7
};
void UpdateOpponentData(Environnement* opponentEnv, sf::UdpSocket* socket) {
    char opponentPieceData[8];
    ReceiveCoordinates(socket, opponentPieceData);
    for (int i = 0; i < 4; ++i) {
        opponentEnv->Current_Piece->Coordinates[i][0] = static_cast<int>(opponentPieceData[i * 2]);
        opponentEnv->Current_Piece->Coordinates[i][1] = static_cast<int>(opponentPieceData[i * 2 + 1]);
    }

    ReceiveMatrix(socket, opponentEnv->Grid_Mtx);
}
void DrawScore(Environnement env)
{

}
int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Player 1");
    Environnement env;
    Environnement opponentEnv;
    int action = 0;
    sf::UdpSocket socket;
    socket.bind(PLAYER_PORT);
    sf::Font font ;
    font.loadFromFile("ARIAL.TTF");
    std::thread networkThread([&]() {
        while (window.isOpen()) {
            UpdateOpponentData(&opponentEnv, &socket);
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    });

    while (window.isOpen()) {
        sf::Event event;
          // Handle events
          action = 0;
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

        // Game logic and rendering
        if(env.checkLose())
        {
            cout << "game over"<<endl ;
        }
        env.Remove_Lines();
        int res = env.Step(action); 
        if (res == 2)
        {
            exit(0);
             std::cerr << "game over" << std::endl;
        }
        SendCoordinates(&env, &socket, OPPONENT_PORT);
        SendMatrix(&env, &socket, OPPONENT_PORT, sf::IpAddress("127.0.0.1"));

        window.clear();
        env.Render(&window);

        // Render opponent grid
        for (int i = 0; i < Environnement::Rows; ++i) {
            for (int j = 0; j < Environnement::Cols; ++j) {
                if (opponentEnv.Grid_Mtx[i][j] != 0) {
                    sf::RectangleShape Cell({40.f, 40.f});
                    Cell.setPosition(GRID_OFFSET_X + j * 40.f, GRID_OFFSET_Y + i * 40.f);
                    Cell.setFillColor(COLORS1[opponentEnv.Grid_Mtx[i][j] - 1]);
                    window.draw(Cell);
                }
            }
        }

        // Render opponent falling piece
        for (int i = 0; i < 4; ++i) {
            sf::RectangleShape Cell({40.f, 40.f});
            Cell.setPosition(GRID_OFFSET_X + opponentEnv.Current_Piece->Coordinates[i][1] * 40.f,
                             GRID_OFFSET_Y + opponentEnv.Current_Piece->Coordinates[i][0] * 40.f);
            Cell.setFillColor(COLORS1[opponentEnv.Current_Piece->type - 1]);
            window.draw(Cell);
        }
         //render the score
        sf::Text ScoreText ;
        ScoreText.setFont(font);
        ScoreText.setString("Score : "+to_string(env.score));
        ScoreText.setCharacterSize(50);
        ScoreText.setPosition(0,750);
        ScoreText.setFillColor(sf::Color::Black);
        window.draw(ScoreText);
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
       
    }

    networkThread.join();
    return 0;
}

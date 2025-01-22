#ifndef ENV_HPP
#define ENV_HPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace std;


// Piece class representing each dropped Piece
class Piece {
public:
    int Coordinates[4][2] = {{0, 0}, {0, 0}, {0, 0}, {0, 0}};
    int type = 0;
    int Origin[2] = {0, 0};
};
//Timer Class 
class Timer
{
    public:
        int sec  = 0;
        int min = 0 ;
       
}
;
class Environnement {
public:
    static const int Cols = 10;
    static const int Rows = 20; 
    int Placing = 1 ;
    
    
    int** Grid_Mtx;

    Timer* timer ;
    Piece* Holding_Piece;
    Piece* Current_Piece;
    int score = 0 ;

    
    Environnement();
    ~Environnement();

    int Step(int action);                
    void Render(sf::RenderWindow* win);
    void Draw_Grid(sf::RenderWindow* win);
    bool Check_Collision(Piece* p);
    bool Check_Horizontal_Mouvement(Piece* p, int deltaCol);
    bool Rotate_Piece(Piece* p);
    void Update_Sec();
    void Update_Min();
    void Remove_Lines();
    bool checkLose();
};

// Utility functions
Piece* generate_Random_Piece(); 
void Adjust_Piece(Piece* p, int Coor[4][2], int Origin[2]);

#endif

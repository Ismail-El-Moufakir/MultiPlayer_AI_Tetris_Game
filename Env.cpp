#include "./include/env.hpp"

using namespace sf;

// Global color map for Tetris pieces
//COLORS
sf::Color COLORS[7] = {
    sf::Color::Magenta,      // 1
    sf::Color::Green,        // 2
    sf::Color::Yellow,       // 3
    sf::Color::Blue,         // 4
    sf::Color::Red,          // 5
    sf::Color::Cyan,         // 6
    sf::Color(135, 35, 48)   // 7
};

//////////////////////////////////////////
//           Utility Functions          //
//////////////////////////////////////////

// Adjust piece coordinates and origin
void Adjust_Piece(Piece* p, int Coor[4][2], int Origin[2]) {
    for (int i = 0; i < 4; i++) {
        p->Coordinates[i][0] = Coor[i][0];  // row
        p->Coordinates[i][1] = Coor[i][1];  // col
    }
    p->Origin[0] = Origin[0];
    p->Origin[1] = Origin[1];
}

// Generate a random Tetris piece
Piece* generate_Random_Piece() {
    int coor[7][4][2] = {
        {{0, 4}, {0, 5}, {1, 4}, {1, 5}}, // O
        {{0, 3}, {0, 4}, {0, 5}, {0, 6}}, // I
        {{0, 4}, {0, 5}, {1, 3}, {1, 4}}, // S
        {{0, 3}, {0, 4}, {1, 4}, {1, 5}}, // Z
        {{0, 3}, {1, 3}, {1, 4}, {1, 5}}, // L
        {{0, 5}, {1, 3}, {1, 4}, {1, 5}}, // J
        {{0, 4}, {1, 3}, {1, 4}, {1, 5}}  // T
    };
    
    int origins[7][2] = {
        {1, 4}, // O
        {0, 4}, // I
        {0, 4}, // S
        {1, 4}, // Z
        {1, 4}, // L
        {1, 4}, // J
        {1, 4}  // T
    };

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, 6);
    Piece* p = new Piece();
    int Random_Piece = distrib(gen);
    p->type = Random_Piece + 1; // 1..7
    Adjust_Piece(p, coor[Random_Piece], origins[Random_Piece]);
    return p;
}

//////////////////////////////////////////
//         Environnement Class          //
//////////////////////////////////////////

Environnement::Environnement() {
    Grid_Mtx = new int*[Rows];
    for (int i = 0; i < Rows; i++) {
        Grid_Mtx[i] = new int[Cols];
        for (int j = 0; j < Cols; j++) {
            Grid_Mtx[i][j] = 0;
        }
    }

    timer = new Timer();
    Holding_Piece = generate_Random_Piece();
    Current_Piece = generate_Random_Piece();
}
Environnement::~Environnement() {
    for (int i = 0; i < Rows; i++) {
        delete[] Grid_Mtx[i];
    }
    delete[] Grid_Mtx;

    
    delete Holding_Piece;
    delete Current_Piece;
}
//check lose function
bool Environnement::checkLose()
{
    for(int i = 0 ; i<4;i++)
    {
        int r = Holding_Piece->Coordinates[i][0];
        int c = Holding_Piece->Coordinates[i][1];
        if(Grid_Mtx[r][c] != 0)
        {
            return true ;
        }
    }
    return false ;
}
//remove filled line 
void Environnement::Remove_Lines()
{
    //remove filled line and add to score
    for (int i = Rows - 1; i >= 0; i--) {
        bool isFilled = true;
        for (int j = 0; j < Cols; j++) {
            if (Grid_Mtx[i][j] == 0) {
                isFilled = false;
                break;
            }
        }
        if (isFilled) {
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < Cols; j++) {
                    Grid_Mtx[k][j] = Grid_Mtx[k - 1][j];
                }
            }
            score += 10;
            i++;
        }
    }

}
// Draw the empty grid
void Environnement::Draw_Grid(sf::RenderWindow* win) {
    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            sf::RectangleShape Cell({40.f, 40.f});
            Cell.setPosition({j * 40.f,i * 40.f});
            Cell.setOutlineColor(sf::Color::Black);
            Cell.setOutlineThickness(1.f);
            Cell.setFillColor(sf::Color::White);
            win->draw(Cell);
        }
    }
}
// Check if the piece p collides with the grid or boundaries
bool Environnement::Check_Collision(Piece* p) {
    for (int i = 0; i < 4; i++) {
        int r = p->Coordinates[i][0];
        int c = p->Coordinates[i][1];
        if (r < 0 || r >= Rows || c < 0 || c >= Cols || Grid_Mtx[r][c] != 0) {
            return true;
        }
    }
    return false;
}

// Check horizontal mouvement (left/right). 
// deltaCol = -1 for left, +1 for right
bool Environnement::Check_Horizontal_Mouvement(Piece* p, int deltaCol) {
    for (int i = 0; i < 4; i++) {
        int r = p->Coordinates[i][0];
        int c = p->Coordinates[i][1] + deltaCol; 
        if (c < 0 || c >= Cols || Grid_Mtx[r][c] != 0) {
            return false;
        }
    }
    return true;
}
// rotation around the piece's origin
bool Environnement::Rotate_Piece(Piece* p) {
    // Backup coordinates in case rotation is invalid
    int backupCoordinates[4][2];
    for (int i = 0; i < 4; i++) {
        backupCoordinates[i][0] = p->Coordinates[i][0];
        backupCoordinates[i][1] = p->Coordinates[i][1];
    }

    // Perform the rotation
    for (int i = 0; i < 4; i++) {
        int relativeRow = p->Coordinates[i][0] - p->Origin[0];
        int relativeCol = p->Coordinates[i][1] - p->Origin[1];

        int newRow = -relativeCol;
        int newCol = relativeRow;

        p->Coordinates[i][0] = newRow + p->Origin[0];
        p->Coordinates[i][1] = newCol + p->Origin[1];
    }

    if (Check_Collision(p)) {
        for (int j = 0; j < 4; j++) {
            p->Coordinates[j][0] = backupCoordinates[j][0];
            p->Coordinates[j][1] = backupCoordinates[j][1];
        }
        return false;
    }
    return true;
}

// The main "step" function: apply action, move piece downward, etc.
int Environnement::Step(int action) {
    // Action reference:
    // 0: no action (just gravity)
    // 1: move left
    // 2: move right
    // 3: rotate
    // 4: hard drop
 if(checkLose() == true)
        {
            return 2 ;
        }
    // Temporarily move the piece down by 1 
    for (int i = 0; i < 4; i++) {
        Current_Piece->Coordinates[i][0] += 1;
    }
    Current_Piece->Origin[0] += 1;

    // Now check collision
    if (Check_Collision(Current_Piece)) {
        // Revert the gravity step
        for (int i = 0; i < 4; i++) {
            Current_Piece->Coordinates[i][0] -= 1;
        }
        Current_Piece->Origin[0] -= 1;

        // Place the piece in the grid
        for (int i = 0; i < 4; i++) {
            int r = Current_Piece->Coordinates[i][0];
            int c = Current_Piece->Coordinates[i][1];
            Grid_Mtx[r][c] = Current_Piece->type;
        }

        // Swap to the next piece
        delete Current_Piece;          
        Current_Piece = Holding_Piece;  
        Holding_Piece = generate_Random_Piece(); 
        return 0; 
    }

    // 2. If we did not collide after gravity, handle the action input
    // -----------------------------------
    if (action == 1) { 
        // move left
        if (Check_Horizontal_Mouvement(Current_Piece, -1)) {
            for (int i = 0; i < 4; i++) {
                Current_Piece->Coordinates[i][1] -= 1;
            }
            Current_Piece->Origin[1] -= 1;
        }
    }
    else if (action == 2) {
        // move right
        if (Check_Horizontal_Mouvement(Current_Piece, +1)) {
            for (int i = 0; i < 4; i++) {
                Current_Piece->Coordinates[i][1] += 1;
            }
            Current_Piece->Origin[1] += 1;
        }
    }
    else if (action == 3) {
        // rotate
        Rotate_Piece(Current_Piece);
    }
    else if (action == 4) {
        // hard drop: keep moving down until collision
        while (!Check_Collision(Current_Piece)) {
            for (int i = 0; i < 4; i++) {
                Current_Piece->Coordinates[i][0] += 1;
            }
            Current_Piece->Origin[0] += 1;
        }
        // Revert last invalid move
        for (int i = 0; i < 4; i++) {
            Current_Piece->Coordinates[i][0] -= 1;
        }
        Current_Piece->Origin[0] -= 1;

        // Place the piece
        for (int i = 0; i < 4; i++) {
            int r = Current_Piece->Coordinates[i][0];
            int c = Current_Piece->Coordinates[i][1];
            Grid_Mtx[r][c] = Current_Piece->type;
        }

        // Swap pieces
        delete Current_Piece;
        if(checkLose())
        {
            return 2;
        }
        Current_Piece = Holding_Piece;
        Holding_Piece = generate_Random_Piece();
       
        return 0; 

    }

    return 1; 
}

// Render everything
void Environnement::Render(sf::RenderWindow* win) {
    win->clear();
    Draw_Grid(win);

    if (Current_Piece) {
        for (int i = 0; i < 4; i++) {
            sf::RectangleShape Cell({40.f, 40.f});
            float x =   Current_Piece->Coordinates[i][1] * 40.f;
            float y = Current_Piece->Coordinates[i][0] * 40.f;
            Cell.setPosition({x, y});
            Cell.setOutlineColor(sf::Color::Black);
            Cell.setOutlineThickness(1.f);
            Cell.setFillColor(COLORS[Current_Piece->type - 1]);
            win->draw(Cell);
        }
    }

    for (int i = 0; i < Rows; i++) {
        for (int j = 0; j < Cols; j++) {
            if (Grid_Mtx[i][j] != 0) {
                sf::RectangleShape Cell({40.f, 40.f});
                Cell.setPosition({j * 40.f, i * 40.f});
                Cell.setOutlineColor(sf::Color::Black);
                Cell.setOutlineThickness(1.f);
                Cell.setFillColor(COLORS[Grid_Mtx[i][j] - 1]);
                win->draw(Cell);
            }
        }
    }

}

//Update Sec
void Environnement::Update_Sec()
{
    if(timer->sec == 60)
    {
        timer->sec = 0 ;
        Update_Min();
    }
    else{
        timer->sec++;
    }
}
//Update Min 
void Environnement::Update_Min()
{
    timer->min++ ;
}
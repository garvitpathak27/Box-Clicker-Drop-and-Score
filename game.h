#pragma once

#include <iostream>
#include <vector>
#include <ctime> // timing
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

using namespace std;
using namespace sf;

class game
{
private:
    // variable
    // windows
    void initvariable();
    void initwindow();
    RenderWindow* window;
    Event ev;
    VideoMode videomode;

    // mouse position
    Vector2i mouseposwindow;
    Vector2f mousepositionview; // this one is always used ...

    // resources
    Font font;

    // text
    Text uitext;

    // game logic
    unsigned points; // positive values only
    int health;
    float enemyspawntimer;
    float enemyspawntimermax;
    int maxenemies;
    bool mouseheld;
    bool endgame;
    void inittext();
    void initfonts();

    // game objects
    vector<RectangleShape> enemies;
    RectangleShape enemy;

public:
    // constructor
    game();
    ~game();
    const bool running() const;
    const bool getendgame() const;

    void pollevent();

    void updatetext();
    void updateenemeis();
    void update();

    void updatemouseposition();

    void rendertext(RenderTarget& target); // dont have to render to the main window ...
    void renderenemeis(RenderTarget& target);
    void render();

    void initenemies();
    void spawnenemy();
};

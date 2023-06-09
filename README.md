# aimer_gameAIMER PRACTICE GAME 
By Garvit Pathak 
Reference YouTube video link : https://www.youtube.com/watch?v=PXnhYBG0AEA&t=4245s
So recently I made game using YouTube as my reference in which there are boxes of several sizes which drop from the top of the window and if we click on them we will get some points ... if we are unable to click them when the boxes fall then we lose health. this is a very simple game and doesnot have any front end development but the back end is written in c++ and an primary sfml library is used to show the out come of the game 
Time taken to complete the project – 2 days (5 hrs each )

Following are some of the screenshots from the game and its code 
The code first has a game.cpp file which is 
the main game file 
#include "game.h"
void game::inittext()
{
    this->uitext.setFont(this->font);
    this->uitext.setCharacterSize(24);
    this->uitext.setFillColor(Color::White);
    this->uitext.setString("NONE");
}
void game::initfonts()
{
    if (this->font.loadFromFile("Fonts/Dosis-Light.ttf"))
    {
        cout << "failed to load font ! " << endl;
    }
}
void game::rendertext(RenderTarget& target)
{
    target.draw(this->uitext);
}
void game::updatetext()
{
    stringstream ss;
    ss << "point : " << this->points << endl;
    ss << "health : " << this->health << endl;
    this->uitext.setString(ss.str());
}
// CONSTRUCTOR AND DESTRUCTOR
game::game()
{
    this->initvariable();
    this->initwindow();
    this->initfonts();
    this->inittext();
    this->initenemies();
}
game::~game()
{
    delete this->window;
}

// PRIVATE FUNCTION
void game::initvariable()
{
    this->window = nullptr;
    this->points = 0;
    this->health = 20;
    this->enemyspawntimermax = 20.f;
    this->enemyspawntimer = this->enemyspawntimermax;
    this->maxenemies = 5;
    this->mouseheld = false;
    this->endgame = false;
}

void game::initwindow()
{
    // control all the properties of game window
    this->videomode.height = 600;
    this->videomode.width = 800;
    this->window = new RenderWindow(this->videomode, "aimer", Style::Titlebar | Style::Close | Style::Resize);
    this->window->setFramerateLimit(60);
}

// PUBLIC FUNCTIONS
const bool game::running() const
{
    // verify if the window is running or not (basically check if the window is open or not )
    return this->window->isOpen();
}

const bool game::getendgame() const
{
    return this->endgame;
}

void game::pollevent()
{
    // handles input by the user hardware
    while (this->window->pollEvent(this->ev))
    {
        switch (this->ev.type)
        {
        case Event::Closed:
            this->window->close();
            break;
        case Event::KeyPressed:
            if (this->ev.key.code == Keyboard::Escape)
            {
                this->window->close();
                break;
            }
        default:
            break;
        }
    }
}

void game::updateenemeis()
{ /*
     @return void
         updates the enemy spawn timer ...
         when the total amount of enemies is smaller than the maximum
         moves the enemy downwards
         removes the enemies at eh edge of the screen
  */

  // uspdates the timer for enemy spawing
    if (this->enemies.size() < this->maxenemies)
    {

        if (this->enemyspawntimer >= this->enemyspawntimermax)
        {
            // spawn the enemy and reset the timer
            this->spawnenemy();
            this->enemyspawntimer = 0.f;
        }
        else
        {
            this->enemyspawntimer += 1.f;
        }
    }
    for (int i = 0; i < this->enemies.size(); i++)
    {
        this->enemies[i].move(0.f, 3.f);
        bool deleted = false;
        if (this->enemies[i].getPosition().y > this->window->getSize().y)
        {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            cout << "health : " << this->health << endl;
        }
    } // this for loop is only to check if the enemy is below the screen

    // check if clicked upon
    if (Mouse::isButtonPressed(Mouse::Left))
    {
        if (this->mouseheld == false)
        {                           // if the mouse is not clicked ...
            this->mouseheld = true; // then if the mouse is clicked ...
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && deleted == false; i++)
            {
                if (this->enemies[i].getGlobalBounds().contains(this->mousepositionview))
                {
                    if (this->enemies[i].getFillColor() == Color::Red)
                        this->points += 20;
                    else if (this->enemies[i].getFillColor() == Color::Magenta)
                        this->points += 15;
                    else if (this->enemies[i].getFillColor() == Color::Cyan)
                        this->points += 10;
                    else if (this->enemies[i].getFillColor() == Color::Yellow)
                        this->points += 5;
                    else if (this->enemies[i].getFillColor() == Color::Green)
                        this->points += 1;

                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);

                    cout << "points : " << this->points << endl;
                }
            }
        }
    }
    else
    {
        this->mouseheld = false;
    }
}

void game::update()
{
    // takes the input by the user and update the objects and process in the window... like a flip book
    this->pollevent();
    if (this->endgame == false)
    {
        this->updatemouseposition();
        this->updatetext();
        this->updateenemeis();
    }

    if (this->health <= 0)
    {
        this->endgame = true;
    }
}

void game::updatemouseposition()
{
    /**
    @return void

        updates the mouse position
            mouse position relative to window ...
            is a vector2i
    */
    this->mouseposwindow = Mouse::getPosition(*this->window);
    // we are taking the mouse position from the mouseposition window and then using the same thing in mouse position view

    this->mousepositionview = this->window->mapPixelToCoords(this->mouseposwindow);
}

void game::renderenemeis(RenderTarget& target)
{
    for (auto& e : this->enemies)
    {
        target.draw(e);
    }
}

void game::render()
{
    /*
return void .

    * clear old frame
    * render objects
    * diplay frame in window
    renders the game objects .

    */
    this->window->clear(); // r , g , b , alpha

    this->renderenemeis(*this->window);

    this->rendertext(*this->window);

    this->window->display();
}

void game::initenemies()
{
    /*
    in sfml
        the initial position of any object is at origin ..

        remember the coordinate system and flip is horizontally ..
        not the positive y direction is below the x axis and the origin is at the top left corner

    */

    // handles all the properties of an object ... (enemy)
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(Vector2f(100.f, 100.f));
    this->enemy.setScale(Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(Color::Cyan);
    // this->enemy.setOutlineColor(Color::Green);
    // this->enemy.setOutlineThickness(1.f);
}

void game::spawnenemy()
{
    /*
    spawns enemy and set there types , color and position randomly
    - set a random position
    - sets a random color
    - adds enemy to the vector
    */
    this->enemy.setPosition(
        static_cast<float>(rand() % static_cast<int>(this->window->getSize().x - this->enemy.getSize().x)),
        0.f // y should not be random
    );      // have no idea what this line does ...

    // random enemy type

    int type = rand() % 5;
    switch (type)
    {

    case 0:
        this->enemy.setSize(Vector2f(30.f, 30.f));
        this->enemy.setFillColor(Color::Red);
        break;
    case 1:
        this->enemy.setSize(Vector2f(50.f, 50.f));
        this->enemy.setFillColor(Color::Magenta);
        break;
    case 2:
        this->enemy.setSize(Vector2f(70.f, 70.f));
        this->enemy.setFillColor(Color::Cyan);
        break;
    case 3:
        this->enemy.setSize(Vector2f(90.f, 90.f));
        this->enemy.setFillColor(Color::Yellow);
        break;
    case 4:
        this->enemy.setSize(Vector2f(120.f, 120.f));
        this->enemy.setFillColor(Color::Green);
        break;
    default:
        this->enemy.setSize(Vector2f(120.f, 120.f));
        this->enemy.setFillColor(Color::Green);
        break;
    }

    // spawn the enemy
    this->enemies.push_back(this->enemy);
}
Main game file 
// VIDEO LINK https://www.youtube.com/watch?v=PXnhYBG0AEA&t=4245s
#include <iostream>
#include "game.h" // this is a pre compile header ... ///
/*
WHY USE PRE COMPILE HEADER ??

    according to wikipedia

            To reduce compilation times, some compilers allow header files to be compiled into a form that is faster for the compiler to process. This intermediate form is known as a precompiled header, and is commonly held in a file named with the extension . pch or similar, such as . gch under the GNU Compiler Collection.

*/

using namespace std;
using namespace sf;

int main()
{   
    // initializing random
    srand(static_cast<unsigned>(time(NULL)));

    game g;
    while (g.running() && !g.getendgame())
    {
        g.update();
        g.render();
    }
    return 0;
}








Game header file 
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

Output screen shot 
  

# Game README

## Overview

This game, developed by Garvit Pathak, is a simple clicking game where boxes of various sizes drop from the top of the window. The objective is to click on these boxes to score points. Failing to click on a box before it reaches the bottom will result in a loss of health. This project is built using C++ and SFML (Simple and Fast Multimedia Library) and focuses on backend functionality without frontend development.

### YouTube Reference

For more details on the development process, refer to this [YouTube video](https://www.youtube.com/watch?v=PXnhYBG0AEA&t=4245s).

## Project Details

- **Duration:** 2 days (5 hours each day)
- **Main Technologies:** C++, SFML
- **Backend:** C++ with SFML library

### Screenshots

(Include relevant screenshots here)

## Code Overview

### `game.cpp`

This file contains the main game logic.

```cpp
#include "game.h"

// Initializes the text settings
void game::inittext() {
    this->uitext.setFont(this->font);
    this->uitext.setCharacterSize(24);
    this->uitext.setFillColor(Color::White);
    this->uitext.setString("NONE");
}

// Loads the font
void game::initfonts() {
    if (!this->font.loadFromFile("Fonts/Dosis-Light.ttf")) {
        cout << "Failed to load font!" << endl;
    }
}

// Renders the text
void game::rendertext(RenderTarget& target) {
    target.draw(this->uitext);
}

// Updates the text displayed on screen
void game::updatetext() {
    stringstream ss;
    ss << "Points: " << this->points << endl;
    ss << "Health: " << this->health << endl;
    this->uitext.setString(ss.str());
}

// Constructor and Destructor
game::game() {
    this->initvariable();
    this->initwindow();
    this->initfonts();
    this->inittext();
    this->initenemies();
}

game::~game() {
    delete this->window;
}

// Initializes game variables
void game::initvariable() {
    this->window = nullptr;
    this->points = 0;
    this->health = 20;
    this->enemyspawntimermax = 20.f;
    this->enemyspawntimer = this->enemyspawntimermax;
    this->maxenemies = 5;
    this->mouseheld = false;
    this->endgame = false;
}

// Initializes the game window
void game::initwindow() {
    this->videomode.height = 600;
    this->videomode.width = 800;
    this->window = new RenderWindow(this->videomode, "Aimer", Style::Titlebar | Style::Close | Style::Resize);
    this->window->setFramerateLimit(60);
}

// Checks if the window is running
const bool game::running() const {
    return this->window->isOpen();
}

// Checks if the game has ended
const bool game::getendgame() const {
    return this->endgame;
}

// Handles user input events
void game::pollevent() {
    while (this->window->pollEvent(this->ev)) {
        switch (this->ev.type) {
            case Event::Closed:
                this->window->close();
                break;
            case Event::KeyPressed:
                if (this->ev.key.code == Keyboard::Escape) {
                    this->window->close();
                }
                break;
            default:
                break;
        }
    }
}

// Updates enemy positions and checks for collisions
void game::updateenemies() {
    if (this->enemies.size() < this->maxenemies) {
        if (this->enemyspawntimer >= this->enemyspawntimermax) {
            this->spawnenemy();
            this->enemyspawntimer = 0.f;
        } else {
            this->enemyspawntimer += 1.f;
        }
    }

    for (int i = 0; i < this->enemies.size(); i++) {
        this->enemies[i].move(0.f, 3.f);
        if (this->enemies[i].getPosition().y > this->window->getSize().y) {
            this->enemies.erase(this->enemies.begin() + i);
            this->health -= 1;
            cout << "Health: " << this->health << endl;
        }
    }

    if (Mouse::isButtonPressed(Mouse::Left)) {
        if (!this->mouseheld) {
            this->mouseheld = true;
            bool deleted = false;
            for (int i = 0; i < this->enemies.size() && !deleted; i++) {
                if (this->enemies[i].getGlobalBounds().contains(this->mousepositionview)) {
                    switch (this->enemies[i].getFillColor().toInteger()) {
                        case Color::Red.toInteger():
                            this->points += 20;
                            break;
                        case Color::Magenta.toInteger():
                            this->points += 15;
                            break;
                        case Color::Cyan.toInteger():
                            this->points += 10;
                            break;
                        case Color::Yellow.toInteger():
                            this->points += 5;
                            break;
                        case Color::Green.toInteger():
                            this->points += 1;
                            break;
                    }
                    deleted = true;
                    this->enemies.erase(this->enemies.begin() + i);
                    cout << "Points: " << this->points << endl;
                }
            }
        }
    } else {
        this->mouseheld = false;
    }
}

// Updates game logic
void game::update() {
    this->pollevent();
    if (!this->endgame) {
        this->updatemouseposition();
        this->updatetext();
        this->updateenemies();
    }
    if (this->health <= 0) {
        this->endgame = true;
    }
}

// Updates mouse position
void game::updatemouseposition() {
    this->mouseposwindow = Mouse::getPosition(*this->window);
    this->mousepositionview = this->window->mapPixelToCoords(this->mouseposwindow);
}

// Renders enemies
void game::renderenemies(RenderTarget& target) {
    for (auto& e : this->enemies) {
        target.draw(e);
    }
}

// Renders the game window
void game::render() {
    this->window->clear();
    this->renderenemies(*this->window);
    this->rendertext(*this->window);
    this->window->display();
}

// Initializes enemies
void game::initenemies() {
    this->enemy.setPosition(10.f, 10.f);
    this->enemy.setSize(Vector2f(100.f, 100.f));
    this->enemy.setScale(Vector2f(0.5f, 0.5f));
    this->enemy.setFillColor(Color::Cyan);
}

// Spawns an enemy with random properties
void game::spawnenemy() {
    this->enemy.setPosition(static_cast<float>(rand() % (this->window->getSize().x - static_cast<int>(this->enemy.getSize().x))), 0.f);

    int type = rand() % 5;
    switch (type) {
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

    this->enemies.push_back(this->enemy);
}

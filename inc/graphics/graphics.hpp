#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../models/world.hpp"
#include "../tools/containers.hpp"
#include "../tools/algorythms.hpp"
#include "../logger/logger.hpp"
#include "../models/animals/animals.hpp"
#include "../graphics/button.hpp"


class SFMLManager{ // This class encapsulates SFML
private:
    sf::RenderWindow window;
    BufferedLogger logger;
    void drawButtonBackground(Button& button, sf::Uint8* pixels);
    void drawButtonText(Button& button);
    static void RGB(sf::Uint8* pixels, int x, int y, int r, int g, int b); // draw a dot into SFML buffer
public:
    static const int width = 1000;
    static const int height = 500;
    static const int framerate = 60;
    static const bool DRAW_HEIGHT = false; // whether to show terrain height on the map or not
    static const bool SMOOTH_LANDSCAPE = false;

    SFMLManager(std::string title);
    bool checkCloseEvent(); // close the window if the user says so
    bool windowIsOpen();
    void render(); // draw the SFML buffer to the screen
    void drawCenteredText(string textString, int x, int y, int size, sf::Color color);

    void drawWorld(World &world); // draw the world into SFML buffer
};
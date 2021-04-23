// Moves.cpp
// code to show possible moves of a selected piece
#include <SFML/Graphics.hpp>
#include <set>
#include "Moves.h"
#include <iostream>

Moves::Moves(std::set<std::string> validMoves)
{
    int xLocation, yLocation;
    redTexture.loadFromFile("content/red.png");
    for(auto elem: validMoves)
    {
        std::vector<char> cstr(elem.c_str(), elem.c_str() + elem.size() + 1);
        squareLocation(cstr[0], cstr[1], xLocation, yLocation);
        sf::RectangleShape circleShape;
        circleShape.setSize(sf::Vector2f(52,52));
        circleShape.setTexture(&redTexture);
        circleShape.setPosition(xLocation, yLocation);
        squares.push_back(circleShape);
    }
}

void Moves::draw(sf::RenderWindow &window)
{
    for (sf::RectangleShape shape : squares)
    {
        window.draw(shape);
    }
}

void Moves::squareLocation(char file, char rank, int &xLocation, int &yLocation)
{
    if (rank == '8') {
        yLocation = 42;
    }
    else if (rank == '7') {
        yLocation = 94;
    }
    else if (rank == '6') {
        yLocation = 146;
    }
    else if (rank == '5') {
        yLocation = 198;
    }
    else if (rank == '4') {
        yLocation = 250;
    }
    else if (rank == '3') {
        yLocation = 302;
    }
    else if (rank == '2') {
        yLocation = 354;
    }
    else if (rank == '1') {
        yLocation = 406;
    }
    else {
        yLocation = 0;;
    }
    if (file == 'a'){
        xLocation = 42;
    }
    else if (file == 'b') {
        xLocation = 94;
    }
    else if (file == 'c') {
        xLocation = 146;
    }
    else if (file == 'd') {
        xLocation = 198;
    }
    else if (file == 'e') {
        xLocation = 250;
    }
    else if (file == 'f') {
        xLocation = 302;
    }
    else if (file == 'g') {
        xLocation = 354;
    }
    else if (file == 'h') {
        xLocation = 406;
    }
    else {
        xLocation = 0;
    }
}
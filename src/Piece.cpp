// Piece.cpp
#include <SFML/Graphics.hpp>
#include <set>
#include "Piece.h"
#include <iostream>


Piece::Piece(std::string location, std::string fileLocation)
{
    std::string delim = "src/res/";
    type = fileLocation.substr(delim.length(),fileLocation.find(".png")-delim.length());
    position = location;
    std::vector<char> cstr(position.c_str(), position.c_str() + position.size() + 1);
    squareLocation(cstr[0], cstr[1], xLocation, yLocation);
    pShape.setSize(sf::Vector2f(52,52));
    pieceTexture.loadFromFile(fileLocation);
    pShape.setTexture(&pieceTexture);
    pShape.setPosition(xLocation, yLocation);
}

void Piece::squareLocation(char file, char rank, int &xLocation, int &yLocation)
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

void Piece::selectPiece()
{
    pShape.setOutlineThickness(5);
    pShape.setOutlineColor(sf::Color::Yellow);
    selected = true;
}

// void Piece::highlight()
// {
//     pShape.setOutlineThickness(5);
//     pShape.setOutlineColor(sf::Color::Red);
// }

// void Piece::unhighlight()
// {
//     pShape.setOutlineThickness(0);
// }

void Piece::deselectPiece()
{
    pShape.setOutlineThickness(0);
    pShape.setOutlineColor(sf::Color::Yellow);
    selected = false;
}

bool Piece::isSelected()
{
    return selected;
}

bool Piece::hasMoved()
{
    return hasMovedB;
}

std::string Piece::getType()
{
    return type;
}

void Piece::movePiece(std::string location)
{
    position = location;
    std::vector<char> cstr(location.c_str(), location.c_str() + location.size() + 1);
    squareLocation(cstr[0], cstr[1], xLocation, yLocation);
    pShape.setPosition(xLocation, yLocation);
    hasMovedB = true;
}

void Piece::draw(sf::RenderWindow &window)
{
    window.draw(pShape);
    // window.draw(highlightR);
}

std::set<std::string> Piece::getValidMoves(std::set<std::string> whitePieceLocations, std::set<std::string> blackPieceLocations, bool attackMovesOnly){
    std::set<std::string> moves;
    if(type == "pawnW") //add en passant and promotion in later
    {
        char rank = position.at(1);
        char file = position.at(0);
        //check if piece in front of pawn, if not then let pawn move forward
        std::string pieceInFront = std::string() + file + (char)(rank + 1);
        if(!attackMovesOnly)
        {
            if((whitePieceLocations.count(pieceInFront) == 0 && (blackPieceLocations.count(pieceInFront) == 0)))
            {
                moves.insert(std::string() + file + (char)(rank + 1));
                if((hasMoved() == false) && (blackPieceLocations.count(std::string() + file + (char)(rank + 2)) == 0))
                {
                    moves.insert(std::string() + file + (char)(rank + 2));
                }
            }
        }
        //check if pawn can capture left or right
        std::string captureLeft = std::string() + (char)(file - 1) + (char)(rank + 1);
        std::string captureRight = std::string() + (char)(file + 1) + (char)(rank + 1);
        if(blackPieceLocations.count(captureLeft))
        {
            moves.insert(captureLeft);
        }
        if(blackPieceLocations.count(captureRight))
        {
            moves.insert(captureRight);
        }
    }
    if(type == "kingW") //need to add logic so king can't move into check
    {
        char rank = position.at(1);
        char file = position.at(0);
        
        std::string upLeft = std::string() + (char)(file - 1) + (char)(rank + 1);
        std::string up = std::string() + file + (char)(rank + 1);
        std::string upRight = std::string() + (char)(file + 1) + (char)(rank + 1);
        std::string downLeft = std::string() + (char)(file - 1) + (char)(rank - 1);
        std::string down = std::string() + file + (char)(rank - 1);
        std::string downRight = std::string() + (char)(file + 1) + (char)(rank - 1);
        std::string right = std::string() + (char)(file + 1) + rank;
        std::string left = std::string() + (char)(file - 1) + rank;
        if(blackPieceLocations.count(upLeft) || !whitePieceLocations.count(upLeft))
        {
            if(((rank - '0') + 1 <=8) && ((char)(file - 1) >= 'a'))
            {
                moves.insert(upLeft);
            }
        }
        if(blackPieceLocations.count(up) || !whitePieceLocations.count(up))
        {
            if((rank - '0') + 1 <=8)
            {
                moves.insert(up);
            }
        }
        if(blackPieceLocations.count(upRight) || !whitePieceLocations.count(upRight))
        {
            if(((rank - '0') + 1 <=8) && ((char)(file + 1) <= 'h'))
            {
                moves.insert(upRight);
            }
        }
        if(blackPieceLocations.count(downLeft) || !whitePieceLocations.count(downLeft))
        {
            if(((rank - '0') - 1 >= 1) && ((char)(file - 1) >= 'a'))
            {
                moves.insert(downLeft);
            }
        }
        if(blackPieceLocations.count(down) || !whitePieceLocations.count(down))
        {
            if((rank - '0') - 1 >= 1)
            {
                moves.insert(down);
            }
        }
        if(blackPieceLocations.count(downRight) || !whitePieceLocations.count(downRight))
        {
            if(((rank - '0') - 1 >= 1) && ((char)(file + 1) <= 'h'))
            {
                moves.insert(downRight);
            }
        }
        if(blackPieceLocations.count(right) || !whitePieceLocations.count(right))
        {
            if((char)(file + 1) <= 'h')
            {
                moves.insert(right);
            }
        }
        if(blackPieceLocations.count(left) || !whitePieceLocations.count(left))
        {
            if((char)(file - 1) >= 'a')
            {
                moves.insert(left);
            }
        }
    }
    if(type == "bishopW" || type == "queenW")
    {
        char rank = position.at(1);
        char file = position.at(0);
        bool upLeftB = true;
        bool upRightB = true;
        bool downLeftB = true;
        bool downRightB = true;
        int counter = 1;
        while (upLeftB == true)
        {
            if(((rank - '0') + counter <= 8) && ((char)(file - counter) >= 'a'))
            {
                std::string upLeft = std::string() + (char)(file - counter) + (char)(rank + counter);
                if((blackPieceLocations.count(upLeft) == 0) && (whitePieceLocations.count(upLeft) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(upLeft);
                    // std::cout << upLeft << "no pieces in the way" << std::endl;
                }
                else if(blackPieceLocations.count(upLeft)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(upLeft);
                    upLeftB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    upLeftB = false;
                }
                counter = counter + 1;
            }
            else
            {
                upLeftB = false;
            }
        }
        counter = 1;
        while (upRightB == true)
        {
            if(((rank - '0') + counter <= 8) && ((char)(file + counter) <= 'h'))
            {
                std::string upRight = std::string() + (char)(file + counter) + (char)(rank + counter);
                if((blackPieceLocations.count(upRight) == 0) && (whitePieceLocations.count(upRight) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(upRight);
                    // std::cout << upRight << "no pieces in the way" << std::endl;
                }
                else if(blackPieceLocations.count(upRight)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(upRight);
                    upRightB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    upRightB = false;
                }
                counter = counter + 1;
            }
            else
            {
                upRightB = false;
            }
        }
        counter = 1;
        while (downLeftB == true)
        {
            if(((rank - '0') - counter >= 1) && ((char)(file - counter) >= 'a'))
            {
                std::string downLeft = std::string() + (char)(file - counter) + (char)(rank - counter);
                if((blackPieceLocations.count(downLeft) == 0) && (whitePieceLocations.count(downLeft) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(downLeft);
                    // std::cout << downLeft << "no pieces in the way" << std::endl;
                }
                else if(blackPieceLocations.count(downLeft)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(downLeft);
                    downLeftB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    downLeftB = false;
                }
                counter = counter + 1;
            }
            else
            {
                downLeftB = false;
            }
        }
        counter = 1;
        while (downRightB == true)
        {
            if(((rank - '0') - counter >= 1) && ((char)(file + counter) <= 'h'))
            {
                std::string downRight = std::string() + (char)(file + counter) + (char)(rank - counter);
                if((blackPieceLocations.count(downRight) == 0) && (whitePieceLocations.count(downRight) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(downRight);
                    // std::cout << downRight << "no pieces in the way" << std::endl;
                }
                else if(blackPieceLocations.count(downRight)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(downRight);
                    downRightB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    downRightB = false;
                }
                counter = counter + 1;
            }
            else
            {
                downRightB = false;
            }
        }
    }
    if(type == "rookW" || type == "queenW")
    {
        char rank = position.at(1);
        char file = position.at(0);
        bool upB = true;
        bool downB = true;
        bool leftB = true;
        bool rightB = true;
        int counter = 1;
        while (upB == true)
        {
            if((rank - '0') + counter <= 8)
            {
                std::string up = std::string() + file + (char)(rank + counter);
                if((blackPieceLocations.count(up) == 0) && (whitePieceLocations.count(up) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(up);
                }
                else if(blackPieceLocations.count(up)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(up);
                    upB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    upB = false;
                }
                counter = counter + 1;
            }
            else
            {
                upB = false;
            }
        }
        counter = 1;
        while (downB)
        {
            if((rank - '0') - counter >= 1)
            {
                std::string down = std::string() + file + (char)(rank - counter);
                if(blackPieceLocations.count(down) == 0 && whitePieceLocations.count(down) == 0) //if the square is empty it can move there or past
                {
                    moves.insert(down);
                }
                else if(blackPieceLocations.count(down)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(down);
                    downB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    downB = false;
                }
                counter = counter + 1;
            }
            else
            {
                downB = false;
            }
        }
        counter = 1;
        while (leftB)
        {
            if((char)(file - counter) >= 'a')
            {
                std::string left = std::string() + (char)(file - counter) + rank;
                if(blackPieceLocations.count(left) == 0 && whitePieceLocations.count(left) == 0)
                {
                    moves.insert(left);
                }
                else if(blackPieceLocations.count(left))
                {
                    moves.insert(left);
                    leftB = false;
                }
                else
                {
                    leftB = false;
                }
                counter = counter + 1;
            }
            else
            {
                leftB = false;
            }
        }
        counter = 1;
        while (rightB)
        {
            if((char)(file + counter) <= 'h')
            {
                std::string right = std::string() + (char)(file + counter) + rank;
                if(blackPieceLocations.count(right) == 0 && whitePieceLocations.count(right) == 0)
                {
                    moves.insert(right);
                }
                else if(blackPieceLocations.count(right))
                {
                    moves.insert(right);
                    rightB = false;
                }
                else
                {
                    rightB = false;
                }
                counter = counter + 1;
            }
            else
            {
                rightB = false;
            }
        }
    }
    if(type == "knightW")
    {
        char rank = position.at(1);
        char file = position.at(0);
        std::string upupLeft = std::string() + (char)(file - 1) + (char)(rank + 2);
        std::string upupRight = std::string() + (char)(file + 1) + (char)(rank + 2);
        std::string upLeftLeft = std::string() + (char)(file - 2) + (char)(rank + 1);
        std::string upRightRight = std::string() + (char)(file + 2) + (char)(rank + 1);
        std::string downdownLeft = std::string() + (char)(file - 1) + (char)(rank - 2);
        std::string downdownRight = std::string() + (char)(file + 1) + (char)(rank - 2);
        std::string downLeftLeft = std::string() + (char)(file - 2) + (char)(rank - 1);
        std::string downRightRight = std::string() + (char)(file + 2) + (char)(rank - 1);
        if(blackPieceLocations.count(upupLeft) || !whitePieceLocations.count(upupLeft))
        {
            moves.insert(upupLeft);
        }
        if(blackPieceLocations.count(upupRight) || !whitePieceLocations.count(upupRight))
        {
            moves.insert(upupRight);
        }
        if(blackPieceLocations.count(upLeftLeft) || !whitePieceLocations.count(upLeftLeft))
        {
            moves.insert(upLeftLeft);
        }
        if(blackPieceLocations.count(upRightRight) || !whitePieceLocations.count(upRightRight))
        {
            moves.insert(upRightRight);
        }
        if(blackPieceLocations.count(downdownLeft) || !whitePieceLocations.count(downdownLeft))
        {
            moves.insert(downdownLeft);
        }
        if(blackPieceLocations.count(downdownRight) || !whitePieceLocations.count(downdownRight))
        {
            moves.insert(downdownRight);
        }
        if(blackPieceLocations.count(downLeftLeft) || !whitePieceLocations.count(downLeftLeft))
        {
            moves.insert(downLeftLeft);
        }
        if(blackPieceLocations.count(downRightRight) || !whitePieceLocations.count(downRightRight))
        {
            moves.insert(downRightRight);
        }
    }

    if(type == "pawnB") //add en passant and promotion in later
    {
        char rank = position.at(1);
        char file = position.at(0);
        //check if piece in front of pawn, if not then let pawn move forward
        std::string pieceInFront = std::string() + file + (char)(rank - 1);
        if(!attackMovesOnly)
        {
            if((blackPieceLocations.count(pieceInFront) == 0 && (whitePieceLocations.count(pieceInFront) == 0)))
            {
                moves.insert(std::string() + file + (char)(rank - 1));
                if((hasMoved() == false) && (whitePieceLocations.count(std::string() + file + (char)(rank - 2)) == 0))
                {
                    moves.insert(std::string() + file + (char)(rank - 2));
                }
            }
        }
        //check if pawn can capture left or right
        std::string captureLeft = std::string() + (char)(file - 1) + (char)(rank - 1);
        std::string captureRight = std::string() + (char)(file + 1) + (char)(rank - 1);
        if(whitePieceLocations.count(captureLeft))
        {
            moves.insert(captureLeft);
        }
        if(whitePieceLocations.count(captureRight))
        {
            moves.insert(captureRight);
        }
    }
    if(type == "kingB") //need to add logic so king can't move into check
    {
        char rank = position.at(1);
        char file = position.at(0);
        std::string upLeft = std::string() + (char)(file - 1) + (char)(rank + 1);
        std::string up = std::string() + file + (char)(rank + 1);
        std::string upRight = std::string() + (char)(file + 1) + (char)(rank + 1);
        std::string downLeft = std::string() + (char)(file - 1) + (char)(rank - 1);
        std::string down = std::string() + file + (char)(rank - 1);
        std::string downRight = std::string() + (char)(file + 1) + (char)(rank - 1);
        std::string right = std::string() + (char)(file + 1) + rank;
        std::string left = std::string() + (char)(file - 1) + rank;
        if(whitePieceLocations.count(upLeft) || !blackPieceLocations.count(upLeft))
        {
            if(((rank - '0') + 1 <=8) && ((char)(file - 1) >= 'a'))
            {
                moves.insert(upLeft);
            }
        }
        if(whitePieceLocations.count(up) || !blackPieceLocations.count(up))
        {
            if((rank - '0') + 1 <=8)
            {
                moves.insert(up);
            }
        }
        if(whitePieceLocations.count(upRight) || !blackPieceLocations.count(upRight))
        {
            if(((rank - '0') + 1 <=8) && ((char)(file + 1) <= 'h'))
            {
                moves.insert(upRight);
            }
        }
        if(whitePieceLocations.count(downLeft) || !blackPieceLocations.count(downLeft))
        {
            if(((rank - '0') - 1 >= 1) && ((char)(file - 1) >= 'a'))
            {
                moves.insert(downLeft);
            }
        }
        if(whitePieceLocations.count(down) || !blackPieceLocations.count(down))
        {
            if((rank - '0') - 1 >= 1)
            {
                moves.insert(down);
            }
        }
        if(whitePieceLocations.count(downRight) || !blackPieceLocations.count(downRight))
        {
            if(((rank - '0') - 1 >= 1) && ((char)(file + 1) <= 'h'))
            {
                moves.insert(downRight);
            }
        }
        if(whitePieceLocations.count(right) || !blackPieceLocations.count(right))
        {
            if((char)(file + 1) <= 'h')
            {
                moves.insert(right);
            }
        }
        if(whitePieceLocations.count(left) || !blackPieceLocations.count(left))
        {
            if((char)(file - 1) >= 'a')
            {
                moves.insert(left);
            }
        }
    }
    if(type == "bishopB" || type == "queenB")
    {
        char rank = position.at(1);
        char file = position.at(0);
        bool upLeftB = true;
        bool upRightB = true;
        bool downLeftB = true;
        bool downRightB = true;
        int counter = 1;
        while (upLeftB == true)
        {
            if(((rank - '0') + counter <= 8) && ((char)(file - counter) >= 'a'))
            {
                std::string upLeft = std::string() + (char)(file - counter) + (char)(rank + counter);
                if((whitePieceLocations.count(upLeft) == 0) && (blackPieceLocations.count(upLeft) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(upLeft);
                }
                else if(whitePieceLocations.count(upLeft)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(upLeft);
                    upLeftB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    upLeftB = false;
                }
                counter = counter + 1;
            }
            else
            {
                upLeftB = false;
            }
        }
        counter = 1;
        while (upRightB == true)
        {
            if(((rank - '0') + counter <= 8) && ((char)(file + counter) <= 'h'))
            {
                std::string upRight = std::string() + (char)(file + counter) + (char)(rank + counter);
                if((whitePieceLocations.count(upRight) == 0) && (blackPieceLocations.count(upRight) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(upRight);
                }
                else if(whitePieceLocations.count(upRight)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(upRight);
                    upRightB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    upRightB = false;
                }
                counter = counter + 1;
            }
            else
            {
                upRightB = false;
            }
        }
        counter = 1;
        while (downLeftB == true)
        {
            if(((rank - '0') - counter >= 1) && ((char)(file - counter) >= 'a'))
            {
                std::string downLeft = std::string() + (char)(file - counter) + (char)(rank - counter);
                if((whitePieceLocations.count(downLeft) == 0) && (blackPieceLocations.count(downLeft) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(downLeft);
                }
                else if(whitePieceLocations.count(downLeft)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(downLeft);
                    downLeftB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    downLeftB = false;
                }
                counter = counter + 1;
            }
            else
            {
                downLeftB = false;
            }
        }
        counter = 1;
        while (downRightB == true)
        {
            if(((rank - '0') - counter >= 1) && ((char)(file + counter) <= 'h'))
            {
                std::string downRight = std::string() + (char)(file + counter) + (char)(rank - counter);
                if((whitePieceLocations.count(downRight) == 0) && (blackPieceLocations.count(downRight) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(downRight);
                }
                else if(whitePieceLocations.count(downRight)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(downRight);
                    downRightB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    downRightB = false;
                }
                counter = counter + 1;
            }
            else
            {
                downRightB = false;
            }
        }
    }
    if(type == "rookB" || type == "queenB")
    {
        char rank = position.at(1);
        char file = position.at(0);
        bool upB = true;
        bool downB = true;
        bool leftB = true;
        bool rightB = true;
        int counter = 1;
        while (upB == true)
        {
            if((rank - '0') + counter <= 8)
            {
                std::string up = std::string() + file + (char)(rank + counter);
                if((whitePieceLocations.count(up) == 0) && (blackPieceLocations.count(up) == 0)) //if the square is empty it can move there or past
                {
                    moves.insert(up);
                }
                else if(whitePieceLocations.count(up)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(up);
                    upB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    upB = false;
                }
                counter = counter + 1;
            }
            else
            {
                upB = false;
            }
        }
        counter = 1;
        while (downB)
        {
            if((rank - '0') - counter >= 1)
            {
                std::string down = std::string() + file + (char)(rank - counter);
                if(whitePieceLocations.count(down) == 0 && blackPieceLocations.count(down) == 0) //if the square is empty it can move there or past
                {
                    moves.insert(down);
                }
                else if(whitePieceLocations.count(down)) //if there is an enemy piece, it can capture but not move past
                {
                    moves.insert(down);
                    downB = false;
                }
                else //if there is a allie piece there it can't move any farther
                {
                    downB = false;
                }
                counter = counter + 1;
            }
            else
            {
                downB = false;
            }
        }
        counter = 1;
        while (leftB)
        {
            if((char)(file - counter) >= 'a')
            {
                std::string left = std::string() + (char)(file - counter) + rank;
                if(whitePieceLocations.count(left) == 0 && blackPieceLocations.count(left) == 0)
                {
                    moves.insert(left);
                }
                else if(whitePieceLocations.count(left))
                {
                    moves.insert(left);
                    leftB = false;
                }
                else
                {
                    leftB = false;
                }
                counter = counter + 1;
            }
            else
            {
                leftB = false;
            }
        }
        counter = 1;
        while (rightB)
        {
            if((char)(file + counter) <= 'h')
            {
                std::string right = std::string() + (char)(file + counter) + rank;
                if(whitePieceLocations.count(right) == 0 && blackPieceLocations.count(right) == 0)
                {
                    moves.insert(right);
                }
                else if(whitePieceLocations.count(right))
                {
                    moves.insert(right);
                    rightB = false;
                }
                else
                {
                    rightB = false;
                }
                counter = counter + 1;
            }
            else
            {
                rightB = false;
            }
        }
    }
    if(type == "knightB")
    {
        char rank = position.at(1);
        char file = position.at(0);
        std::string upupLeft = std::string() + (char)(file - 1) + (char)(rank + 2);
        std::string upupRight = std::string() + (char)(file + 1) + (char)(rank + 2);
        std::string upLeftLeft = std::string() + (char)(file - 2) + (char)(rank + 1);
        std::string upRightRight = std::string() + (char)(file + 2) + (char)(rank + 1);
        std::string downdownLeft = std::string() + (char)(file - 1) + (char)(rank - 2);
        std::string downdownRight = std::string() + (char)(file + 1) + (char)(rank - 2);
        std::string downLeftLeft = std::string() + (char)(file - 2) + (char)(rank - 1);
        std::string downRightRight = std::string() + (char)(file + 2) + (char)(rank - 1);
        if(whitePieceLocations.count(upupLeft) || !blackPieceLocations.count(upupLeft))
        {
            moves.insert(upupLeft);
        }
        if(whitePieceLocations.count(upupRight) || !blackPieceLocations.count(upupRight))
        {
            moves.insert(upupRight);
        }
        if(whitePieceLocations.count(upLeftLeft) || !blackPieceLocations.count(upLeftLeft))
        {
            moves.insert(upLeftLeft);
        }
        if(whitePieceLocations.count(upRightRight) || !blackPieceLocations.count(upRightRight))
        {
            moves.insert(upRightRight);
        }
        if(whitePieceLocations.count(downdownLeft) || !blackPieceLocations.count(downdownLeft))
        {
            moves.insert(downdownLeft);
        }
        if(whitePieceLocations.count(downdownRight) || !blackPieceLocations.count(downdownRight))
        {
            moves.insert(downdownRight);
        }
        if(whitePieceLocations.count(downLeftLeft) || !blackPieceLocations.count(downLeftLeft))
        {
            moves.insert(downLeftLeft);
        }
        if(whitePieceLocations.count(downRightRight) || !blackPieceLocations.count(downRightRight))
        {
            moves.insert(downRightRight);
        }
    }

    return moves;
}
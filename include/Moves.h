//Moves.h
#ifndef MOVES_H
#define MOVES_H

class Moves 
{
    public:
        Moves(std::set<std::string> validMoves);
        void draw(sf::RenderWindow &window);
        void squareLocation(char file, char rank, int &xLocation, int &yLocation);
    private:
        // std::set<std::string> validMoves;
        std::vector<sf::RectangleShape> squares;
        sf::Texture redTexture;
};

#endif


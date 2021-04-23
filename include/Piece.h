//Piece.h
#ifndef PIECE_H
#define PIECE_H

class Piece 
{
    public:
        Piece(std::string location, std::string fileLocation);

        void squareLocation(char file, char rank, int &xLocation, int &yLocation);

        void selectPiece();

        // void highlight();

        // void unhighlight();

        void deselectPiece();

        bool isSelected();

        bool hasMoved();

        std::string getType();

        void movePiece(std::string location);

        void draw(sf::RenderWindow &window);

        std::set<std::string> getValidMoves(std::set<std::string> whitePieceLocations, std::set<std::string> blackPieceLocations, bool attackMovesOnly);
    private:
        int xLocation, yLocation;
        std::string type;
        std::string position;
        bool selected = false;
        bool hasMovedB = false;
        sf::RectangleShape pShape;
        sf::Texture pieceTexture;
        std::set<std::string> squaresUnderAttack;
};

#endif


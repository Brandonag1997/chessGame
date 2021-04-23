#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>
#include <string>
#include <set>
#include "Piece.h"
#include "config.h"


//get the square that the mouse is clicking on
//updates the file and rank pointers with the square details
void testPosition(int x, int y, char &file, char &rank)
{
    if ((y > 42) && (y < 94)) {
        rank = '8';
    }
    else if ((y > 94) && (y < 146)) {
        rank = '7';
    }
    else if ((y > 146) && (y < 198)) {
        rank = '6';
    }
    else if ((y > 198) && (y < 250)) {
        rank = '5';
    }
    else if ((y > 250) && (y < 302)) {
        rank = '4';
    }
    else if ((y > 302) && (y < 354)) {
        rank = '3';
    }
    else if ((y > 354) && (y < 406)) {
        rank = '2';
    }
    else if ((y > 406) && (y < 458)) {
        rank = '1';
    }
    else {
        rank = ' ';
    }
    if ((x > 42) && (x < 94)){
        file = 'a';
    }
    else if ((x > 94) && (x < 146)) {
        file = 'b';
    }
    else if ((x > 146) && (x < 198)) {
        file = 'c';
    }
    else if ((x > 198) && (x < 250)) {
        file = 'd';
    }
    else if ((x > 250) && (x < 302)) {
        file = 'e';
    }
    else if ((x > 302) && (x < 354)) {
        file = 'f';
    }
    else if ((x > 354) && (x < 406)) {
        file = 'g';
    }
    else if ((x > 406) && (x < 458)) {
        file = 'h';
    }
    else {
        file = ' ';
    }
}

//updates the piece location sets
void updateLocations(std::unordered_map<std::string, Piece> &whitePieces, std::unordered_map<std::string, Piece> &blackPieces, std::set<std::string> &whitePieceLocations, std::set<std::string> &blackPieceLocations)
{
    whitePieceLocations.clear();
    blackPieceLocations.clear();
    for (auto itr = whitePieces.begin(); itr != whitePieces.end(); itr++) { 
        whitePieceLocations.insert(itr->first);
    } 
    for (auto itr = blackPieces.begin(); itr != blackPieces.end(); itr++) { 
        blackPieceLocations.insert(itr->first);
    }
}

//returns true if the king is in check
//also computes squares attacked by enemy pieces
bool isKingInCheck(std::unordered_map<std::string, Piece> enemyPieces, std::set<std::string> whitePieceLocations, std::set<std::string>blackPieceLocations, std::string allyKingLocation, std::set<std::string> &squaresAttackedByEnemy)
{
    squaresAttackedByEnemy.clear();
    //find what squares are being attacked
    for (auto itr = enemyPieces.begin(); itr != enemyPieces.end(); itr++) 
    {
        std::set<std::string> s = itr->second.getValidMoves(whitePieceLocations, blackPieceLocations, true);
        squaresAttackedByEnemy.insert(s.begin(), s.end());
    } 

    if(squaresAttackedByEnemy.find(allyKingLocation) != squaresAttackedByEnemy.end())
    {
        return true; //King is in check!
    }
    else
    {
        return false;
    }
}

bool isGameOver(std::unordered_map<std::string, Piece> &whitePieces, std::unordered_map<std::string, Piece> &blackPieces, std::string whiteKingLocation, std::string blackKingLocation, bool whiteTurn)
{

    // std::unordered_map<std::string, Piece> whitePieces = &whitePiecesR;
    std::set<std::string> whitePieceLocations;
    std::set<std::string> blackPieceLocations;
    updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
    //test all posible moves
    if(whiteTurn)
    {    
        for (auto itr : whitePieces) 
        { 
            std::string currentSquare = itr.first;
            std::set<std::string> possibleMoves = itr.second.getValidMoves(whitePieceLocations, blackPieceLocations, false);
            for (auto moveN : possibleMoves)
            {
                std::unordered_map<std::string, Piece> whitePiecesTemp = whitePieces;
                std::unordered_map<std::string, Piece> blackPiecesTemp = blackPieces;
                std::string whiteKingLocationTemp = whiteKingLocation;
                std::string blackKingLocationTemp = blackKingLocation;
                if (blackPieces.find(moveN) != blackPieces.end())
                {
                    blackPiecesTemp.erase(moveN);
                }
                whitePiecesTemp.at(currentSquare).movePiece(moveN);
                auto wp = whitePiecesTemp.extract(currentSquare);
                wp.key() = moveN;
                whitePiecesTemp.insert(std::move(wp));
                if(whitePiecesTemp.at(moveN).getType() == "kingW")
                {
                    whiteKingLocationTemp = moveN;
                }
                std::set<std::string> whitePieceLocationsTemp;
                std::set<std::string> blackPieceLocationsTemp;
                updateLocations(whitePiecesTemp, blackPiecesTemp, whitePieceLocationsTemp, blackPieceLocationsTemp);
                std::set<std::string> squaresAttackedByBlackTemp;   
                if(!isKingInCheck(blackPiecesTemp, whitePieceLocationsTemp, blackPieceLocationsTemp, whiteKingLocationTemp, squaresAttackedByBlackTemp))
                {
                    // std::cout << currentSquare << moveN << std::endl;
                    return false;
                }
            }
        }
    }
    else
    {
        for (auto itr = blackPieces.begin(); itr != blackPieces.end(); itr++) 
        { 
            std::string currentSquare = itr->first;
            std::set<std::string> possibleMoves = itr->second.getValidMoves(whitePieceLocations, blackPieceLocations, false);
            for (auto moveN : possibleMoves)
            {
                std::unordered_map<std::string, Piece> whitePiecesTemp = whitePieces;
                std::unordered_map<std::string, Piece> blackPiecesTemp = blackPieces;
                std::string whiteKingLocationTemp = whiteKingLocation;
                std::string blackKingLocationTemp = blackKingLocation;
                if (whitePieces.find(moveN) != whitePieces.end())
                {
                    whitePiecesTemp.erase(moveN);
                }
                blackPiecesTemp.at(currentSquare).movePiece(moveN);
                auto bp = blackPiecesTemp.extract(currentSquare);
                bp.key() = moveN;
                blackPiecesTemp.insert(std::move(bp));
                if(blackPiecesTemp.at(moveN).getType() == "kingB")
                {
                    blackKingLocationTemp = moveN;
                }
                std::set<std::string> whitePieceLocationsTemp;
                std::set<std::string> blackPieceLocationsTemp;
                updateLocations(whitePiecesTemp, blackPiecesTemp, whitePieceLocationsTemp, blackPieceLocationsTemp);
                std::set<std::string> squaresAttackedByWhiteTemp;   
                if(!isKingInCheck(whitePiecesTemp, whitePieceLocationsTemp, blackPieceLocationsTemp, blackKingLocationTemp, squaresAttackedByWhiteTemp))
                {
                    // std::cout << "false" << std::endl;
                    return false;
                }
            }
        }
    } 
    return true; //no moves left not resulting in check, game is over    
}





int main()
{

    std::string source_dir = RESOURCE_PATH; 
    char pawnDouble; //var to hold file of pawn that moved 2 spaces on it's first turn
    char rank, file;
    std::string selectedSquare;
    bool whiteTurn = true;
    std::string winningPlayer = "";
    std::string whiteKingLocation = "e1";
    std::string blackKingLocation = "e8";
    bool whiteKingInCheck;
    bool blackKingInCheck;
    bool canCastleK = false; //can castle king side
    bool canCastleQ = false; //can castle queen side
    
    // int xLocation, yLocation;
    sf::RenderWindow window(sf::VideoMode(500, 500), "Chess Game!");
    sf::RectangleShape board(sf::Vector2f(500, 500));
    sf::Texture boardTexture;
    boardTexture.loadFromFile(source_dir + "board.png");
    board.setTexture(&boardTexture);
    window.setFramerateLimit(60); //reduces cpu usage

    std::unordered_map<std::string, Piece> whitePieces;
    std::unordered_map<std::string, Piece> blackPieces;

    //pawns
    Piece a2Piece("a2",source_dir + "pawnW.png");
    whitePieces.insert({"a2", a2Piece});
    Piece b2Piece("b2",source_dir + "pawnW.png");
    whitePieces.insert({"b2", b2Piece});
    Piece c2Piece("c2",source_dir + "pawnW.png");
    whitePieces.insert({"c2", c2Piece});
    Piece d2Piece("d2",source_dir + "pawnW.png");
    whitePieces.insert({"d2", d2Piece});
    Piece e2Piece("e2",source_dir + "pawnW.png");
    whitePieces.insert({"e2", e2Piece});
    Piece f2Piece("f2",source_dir + "pawnW.png");
    whitePieces.insert({"f2", f2Piece});
    Piece g2Piece("g2",source_dir + "pawnW.png");
    whitePieces.insert({"g2", g2Piece});
    Piece h2Piece("h2",source_dir + "pawnW.png");
    whitePieces.insert({"h2", h2Piece});
    Piece a7Piece("a7",source_dir + "pawnB.png");
    blackPieces.insert({"a7", a7Piece});
    Piece b7Piece("b7",source_dir + "pawnB.png");
    blackPieces.insert({"b7", b7Piece});
    Piece c7Piece("c7",source_dir + "pawnB.png");
    blackPieces.insert({"c7", c7Piece});
    Piece d7Piece("d7",source_dir + "pawnB.png");
    blackPieces.insert({"d7", d7Piece});
    Piece e7Piece("e7",source_dir + "pawnB.png");
    blackPieces.insert({"e7", e7Piece});
    Piece f7Piece("f7",source_dir + "pawnB.png");
    blackPieces.insert({"f7", f7Piece});
    Piece g7Piece("g7",source_dir + "pawnB.png");
    blackPieces.insert({"g7", g7Piece});
    Piece h7Piece("h7",source_dir + "pawnB.png");
    blackPieces.insert({"h7", h7Piece});

    //rooks
    Piece a1Piece("a1",source_dir + "rookW.png");
    whitePieces.insert({"a1", a1Piece});
    Piece h1Piece("h1",source_dir + "rookW.png");
    whitePieces.insert({"h1", h1Piece});
    Piece a8Piece("a8",source_dir + "rookB.png");
    blackPieces.insert({"a8", a8Piece});
    Piece h8Piece("h8",source_dir + "rookB.png");
    blackPieces.insert({"h8", h8Piece});

    //knights
    Piece b1Piece("b1",source_dir + "knightW.png");
    whitePieces.insert({"b1", b1Piece});
    Piece g1Piece("g1",source_dir + "knightW.png");
    whitePieces.insert({"g1", g1Piece});
    Piece b8Piece("b8",source_dir + "knightB.png");
    blackPieces.insert({"b8", b8Piece});
    Piece g8Piece("g8",source_dir + "knightB.png");
    blackPieces.insert({"g8", g8Piece});

    //bishops
    Piece c1Piece("c1",source_dir + "bishopW.png");
    whitePieces.insert({"c1", c1Piece});
    Piece f1Piece("f1",source_dir + "bishopW.png");
    whitePieces.insert({"f1", f1Piece});
    Piece c8Piece("c8",source_dir + "bishopB.png");
    blackPieces.insert({"c8", c8Piece});
    Piece f8Piece("f8",source_dir + "bishopB.png");
    blackPieces.insert({"f8", f8Piece});

    //queens
    Piece d1Piece("d1",source_dir + "queenW.png");
    whitePieces.insert({"d1", d1Piece});
    Piece d8Piece("d8",source_dir + "queenB.png");
    blackPieces.insert({"d8", d8Piece});

    //kings
    Piece e1Piece("e1",source_dir + "kingW.png");
    whitePieces.insert({"e1", e1Piece});
    Piece e8Piece("e8",source_dir + "kingB.png");
    blackPieces.insert({"e8", e8Piece});

    //objects for promotion
    Piece newQueenW("nq",source_dir + "queenW.png");
    Piece newQueenB("nq",source_dir + "queenB.png");
    Piece newKnightW("nk",source_dir + "knightW.png");
    Piece newKnightB("nk",source_dir + "knightB.png");
    Piece newRookW("nr",source_dir + "rookW.png");
    Piece newRookB("nr",source_dir + "rookB.png");
    Piece newBishopW("nb",source_dir + "bishopW.png");
    Piece newBishopB("nb",source_dir + "bishopB.png");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::MouseButtonPressed:
                    testPosition(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y, file, rank);
                    //set storing all valid moves for a specific piece
                    std::set<std::string> validMovesP;
                    //sets to hold locations of every piece on the board
                    std::set<std::string> whitePieceLocations;
                    std::set<std::string> blackPieceLocations;
                    //squares under attack by each player
                    std::set<std::string> squaresAttackedByWhite;
                    std::set<std::string> squaresAttackedByBlack;
                    
                    //updates piece locations and determines if the game is over
                    //need to add in stalemate and other draws
                    updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                    if(isGameOver(whitePieces, blackPieces, whiteKingLocation, blackKingLocation, whiteTurn))
                    {
                        std::cout << "Game over!" << std::endl;
                        if(whiteTurn)
                        {
                            winningPlayer = "Black";
                        }
                        else
                        {
                            winningPlayer = "White";
                        }
                    }          

                    // whiteKingInCheck = isKingInCheck(blackPieces,whitePieceLocations,blackPieceLocations,whiteKingLocation,squaresAttackedByBlack);
                    // blackKingInCheck = isKingInCheck(whitePieces,whitePieceLocations,blackPieceLocations,blackKingLocation,squaresAttackedByWhite);

                    if ((file == ' ') || (rank == ' ')) //can't select square off the board
                    {
                        break;
                    }
                    std::string key = {file,rank};
                    //if it's white's turn and a white piece is clicked on select it
                    if ((whiteTurn) && (whitePieces.find(key) != whitePieces.end())) //picking white up piece
                    {
                        //if a white piece is already selected deselect it first
                        if (selectedSquare != ""){
                            whitePieces.at(selectedSquare).deselectPiece();
                            selectedSquare = "";
                        }
                        whitePieces.at(key).selectPiece();
                        selectedSquare = key;
                    }
                    //if it's black's turn and a black piece is clicked on select it
                    else if ((!whiteTurn) && (blackPieces.find(key) != blackPieces.end())) //picking up black piece
                    {
                        //if a black piece is already selected deselect it first
                        if (selectedSquare != ""){
                            blackPieces.at(selectedSquare).deselectPiece();
                            selectedSquare = "";
                        }
                        blackPieces.at(key).selectPiece();
                        selectedSquare = key;
                    }
                    else //moving and capturing
                    {
                        //if it's white's turn
                        if (whiteTurn)
                        {
                            //if a white piece is selected
                            if (selectedSquare != "")
                            {
                                if(pawnDouble != ' ')
                                {
                                    std::string doublP = std::string() + pawnDouble + '7' + '5';
                                    blackPieceLocations.insert(doublP);
                                }
                                validMovesP = whitePieces.at(selectedSquare).getValidMoves(whitePieceLocations, blackPieceLocations, false);
                                //logic to check if castling posible
                                whiteKingInCheck = isKingInCheck(blackPieces,whitePieceLocations,blackPieceLocations,whiteKingLocation,squaresAttackedByBlack);
                                if(whitePieces.at(selectedSquare).getType() == "kingW" && whitePieces.at(selectedSquare).hasMoved() == false && whiteKingInCheck == false)
                                {
                                    if((whitePieces.find("h1") != whitePieces.end()) && (whitePieces.at("h1").hasMoved() == false) && (whitePieces.find("f1") == whitePieces.end()) && (whitePieces.find("g1") == whitePieces.end()) && (blackPieces.find("f1") == blackPieces.end()) && (blackPieces.find("g1") == blackPieces.end()) && (squaresAttackedByBlack.find("f1") == squaresAttackedByBlack.end()) && (squaresAttackedByBlack.find("g1") == squaresAttackedByBlack.end()))//king side castle
                                    {
                                        canCastleK = true;
                                    }
                                    else
                                    {
                                        canCastleK = false;
                                    }
                                    if((whitePieces.find("a1") != whitePieces.end()) && (whitePieces.at("a1").hasMoved() == false) && (whitePieces.find("b1") == whitePieces.end()) && (whitePieces.find("c1") == whitePieces.end()) && (whitePieces.find("d1") == whitePieces.end()) && (blackPieces.find("b1") == blackPieces.end()) && (blackPieces.find("c1") == blackPieces.end()) && (blackPieces.find("d1") == blackPieces.end()) && (squaresAttackedByBlack.find("b1") == squaresAttackedByBlack.end()) && (squaresAttackedByBlack.find("c1") == squaresAttackedByBlack.end()) && (squaresAttackedByBlack.find("d1") == squaresAttackedByBlack.end())) //queen side castle
                                    {
                                        canCastleQ = true;
                                    }
                                    else
                                    {
                                        canCastleQ = false;
                                    }
                                }

                                if(whitePieces.at(selectedSquare).getType() == "kingW" && (squaresAttackedByBlack.find(key) != squaresAttackedByBlack.end()))
                                {
                                    std::cout << "White king cannot move into check!" << std::endl;
                                    whitePieces.at(selectedSquare).deselectPiece();
                                    selectedSquare = "";
                                    break;
                                }

                                //if moving the selected piece to the new square (key) is valid
                                if (validMovesP.count(key))
                                {
                                    bool captureEnemy = false;
                                    if (blackPieces.find(key) != blackPieces.end()) 
                                    {
                                        //capture black piece
                                        captureEnemy = true;
                                        blackPieces.at(key).movePiece("zz");
                                        auto bh = blackPieces.extract(key);
                                        bh.key() = "zz";
                                        blackPieces.insert(std::move(bh));
                                        // blackPieces.erase(key); //do this if doesn't result in king being in check
                                    }
                                    //move piece then deselect
                                    whitePieces.at(selectedSquare).movePiece(key);
                                    whitePieces.at(selectedSquare).deselectPiece();
                                    //update key of piece moved
                                    auto nh = whitePieces.extract(selectedSquare);
                                    nh.key() = key;
                                    whitePieces.insert(std::move(nh));
                                    if(whitePieces.at(key).getType() == "kingW")
                                    {
                                        whiteKingLocation = key;
                                    }
                                    updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                                    if(isKingInCheck(blackPieces,whitePieceLocations,blackPieceLocations,whiteKingLocation,squaresAttackedByBlack))
                                    {
                                        //move pieces back
                                        std::cout << "Your king can not be in check!" << std::endl;
                                        if(captureEnemy)
                                        {
                                            blackPieces.at("zz").movePiece(key);
                                            auto bh = blackPieces.extract("zz");
                                            bh.key() = key;
                                            blackPieces.insert(std::move(bh));
                                        }
                                        whitePieces.at(key).movePiece(selectedSquare);
                                        auto wh = whitePieces.extract(key);
                                        wh.key() = selectedSquare;
                                        whitePieces.insert(std::move(wh));
                                        selectedSquare = "";
                                        // updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                                        break;
                                    }
                                    else
                                    {
                                        if(captureEnemy)
                                        {
                                            blackPieces.erase("zz");
                                        }
                                        //logic for promotion of pawn, just make a queen for now
                                        if(whitePieces.at(key).getType() == "pawnW")
                                        {
                                            char file = key.at(0);
                                            char rank = key.at(1);
                                            char startFile = selectedSquare.at(0);
                                            char startRank = selectedSquare.at(1);
                                            if (rank == '8')
                                            {
                                                whitePieces.erase(key);
                                                whitePieces.insert({key, newQueenW});
                                                whitePieces.at(key).movePiece(key);
                                            }
                                            if(startRank == '2' && rank == '4')
                                            {
                                                pawnDouble = file;
                                            }
                                            else
                                            {
                                                pawnDouble = ' ';
                                            }
                                            //if en pacant capture erase black piece
                                            if(rank == '6' && (((char)(startFile - 1) == file) || ((char)(startFile + 1) == file)) && captureEnemy == false)
                                            {
                                                std::string bp = std::string() + file + (char)(rank - 1);
                                                blackPieces.erase(bp);
                                            }
                                        }
                                        else
                                        {
                                            pawnDouble = ' ';
                                        }
                                    }
                                    // updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);

                                    
                                }
                                else if((canCastleK == true) && (key == "g1")) //if player can castle king side and chooses to
                                {
                                    //move piece then deselect
                                    whitePieces.at(selectedSquare).movePiece(key);
                                    whitePieces.at("h1").movePiece("f1");
                                    whitePieces.at(selectedSquare).deselectPiece();
                                    whiteKingLocation = key;
                                    //update key of piece moved
                                    auto nh1 = whitePieces.extract(selectedSquare);
                                    nh1.key() = key;
                                    whitePieces.insert(std::move(nh1));
                                    auto nh2 = whitePieces.extract("h1");
                                    nh2.key() = "f1";
                                    whitePieces.insert(std::move(nh2));
                                }
                                else if((canCastleQ == true) && (key == "c1")) //if player can castle queen side and chooses to
                                {
                                    //move piece then deselect
                                    whitePieces.at(selectedSquare).movePiece(key);
                                    whitePieces.at("a1").movePiece("d1");
                                    whitePieces.at(selectedSquare).deselectPiece();
                                    whiteKingLocation = key;
                                    //update key of piece moved
                                    auto nh1 = whitePieces.extract(selectedSquare);
                                    nh1.key() = key;
                                    whitePieces.insert(std::move(nh1));
                                    auto nh2 = whitePieces.extract("a1");
                                    nh2.key() = "d1";
                                    whitePieces.insert(std::move(nh2));
                                }
                                // updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                                selectedSquare = "";
                                whiteTurn = !whiteTurn;
                                blackKingInCheck = isKingInCheck(whitePieces,whitePieceLocations,blackPieceLocations,blackKingLocation,squaresAttackedByWhite);
                            }                            
                        }
                        else {
                            if (selectedSquare != "")
                            {
                                if(pawnDouble != ' ')
                                {
                                    std::string doublP = std::string() + pawnDouble + '2' + '4';
                                    whitePieceLocations.insert(doublP);
                                }
                                validMovesP = blackPieces.at(selectedSquare).getValidMoves(whitePieceLocations, blackPieceLocations, false);
                                //logic to check if castling posible
                                blackKingInCheck = isKingInCheck(whitePieces,whitePieceLocations,blackPieceLocations,blackKingLocation,squaresAttackedByWhite);
                                if(blackPieces.at(selectedSquare).getType() == "kingB" && blackPieces.at(selectedSquare).hasMoved() == false && blackKingInCheck == false)
                                {
                                    if((blackPieces.find("h8") != blackPieces.end()) && (blackPieces.at("h8").hasMoved() == false) && (whitePieces.find("f8") == whitePieces.end()) && (whitePieces.find("g8") == whitePieces.end()) && (blackPieces.find("f8") == blackPieces.end()) && (blackPieces.find("g8") == blackPieces.end()) && (squaresAttackedByWhite.find("f8") == squaresAttackedByWhite.end()) && (squaresAttackedByWhite.find("g8") == squaresAttackedByWhite.end()))//king side castle
                                    {
                                        canCastleK = true;
                                    }
                                    else
                                    {
                                        canCastleK = false;
                                    }
                                    if((blackPieces.find("a8") != blackPieces.end()) && (blackPieces.at("a8").hasMoved() == false) && (whitePieces.find("b8") == whitePieces.end()) && (whitePieces.find("c8") == whitePieces.end()) && (whitePieces.find("d8") == whitePieces.end()) && (blackPieces.find("b8") == blackPieces.end()) && (blackPieces.find("c8") == blackPieces.end()) && (blackPieces.find("d8") == blackPieces.end()) && (squaresAttackedByWhite.find("b8") == squaresAttackedByWhite.end()) && (squaresAttackedByWhite.find("c8") == squaresAttackedByWhite.end()) && (squaresAttackedByWhite.find("d8") == squaresAttackedByWhite.end())) //queen side castle
                                    {
                                        canCastleQ = true;
                                    }
                                    else
                                    {
                                        canCastleQ = false;
                                    }
                                }

                                if(blackPieces.at(selectedSquare).getType() == "kingB" && (squaresAttackedByWhite.find(key) != squaresAttackedByWhite.end()))
                                {
                                    std::cout << "Black king cannot move into check!" << std::endl;
                                    blackPieces.at(selectedSquare).deselectPiece();
                                    selectedSquare = "";
                                    break;
                                }
                                //if moving the selected piece to the new square (key) is valid
                                if (validMovesP.count(key))
                                {

                                    bool captureEnemy = false; //is the move a capture move?
                                    if (whitePieces.find(key) != whitePieces.end()) 
                                    {
                                        //capture white piece
                                        captureEnemy = true;
                                        whitePieces.at(key).movePiece("zz");
                                        auto wh = whitePieces.extract(key);
                                        wh.key() = "zz";
                                        whitePieces.insert(std::move(wh));
                                    }
                                    //move piece then deselect
                                    blackPieces.at(selectedSquare).movePiece(key);
                                    blackPieces.at(selectedSquare).deselectPiece();
                                    //update key of piece moved
                                    auto nh = blackPieces.extract(selectedSquare);
                                    nh.key() = key;
                                    blackPieces.insert(std::move(nh));
                                    if(blackPieces.at(key).getType() == "kingB")
                                    {
                                        blackKingLocation = key;
                                    }
                                    updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                                    if(isKingInCheck(whitePieces,whitePieceLocations,blackPieceLocations,blackKingLocation,squaresAttackedByWhite))
                                    {
                                        //move pieces back
                                        std::cout << "Your king can not be in check!" << std::endl;
                                        if(captureEnemy)
                                        {
                                            whitePieces.at("zz").movePiece(key);
                                            auto wh = whitePieces.extract("zz");
                                            wh.key() = key;
                                            whitePieces.insert(std::move(wh));
                                        }
                                        blackPieces.at(key).movePiece(selectedSquare);
                                        auto bh = blackPieces.extract(key);
                                        bh.key() = selectedSquare;
                                        blackPieces.insert(std::move(bh));
                                        selectedSquare = "";
                                        // updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                                        break;
                                    }
                                    else
                                    {
                                        if(captureEnemy)
                                        {
                                            whitePieces.erase("zz");
                                        }
                                        //logic for promotion of pawn, just make a queen for now
                                        if(blackPieces.at(key).getType() == "pawnB")
                                        {
                                            char file = key.at(0);
                                            char rank = key.at(1);
                                            char startFile = selectedSquare.at(0);
                                            char startRank = selectedSquare.at(1);
                                            if (rank == '1')
                                            {
                                                blackPieces.erase(key);
                                                blackPieces.insert({key, newQueenB});
                                                blackPieces.at(key).movePiece(key);
                                            }
                                            if(startRank == '7' && rank == '5')
                                            {
                                                pawnDouble = file;
                                            }
                                            else
                                            {
                                                pawnDouble = ' ';
                                            }
                                            //if en pacant capture erase white piece
                                            if(rank == '3' && (((char)(startFile - 1) == file) || ((char)(startFile + 1) == file)) && captureEnemy == false)
                                            {
                                                std::string wp = std::string() + file + (char)(rank + 1);
                                                whitePieces.erase(wp);
                                            }
                                        }
                                        else
                                        {
                                            pawnDouble = ' ';
                                        }
                                    }

                                }
                                else if((canCastleK == true) && (key == "g8")) //if player can castle king side and chooses to
                                {
                                    //move piece then deselect
                                    blackPieces.at(selectedSquare).movePiece(key);
                                    blackPieces.at("h8").movePiece("f8");
                                    blackPieces.at(selectedSquare).deselectPiece();
                                    blackKingLocation = key;
                                    //update key of piece moved
                                    auto nh1 = blackPieces.extract(selectedSquare);
                                    nh1.key() = key;
                                    blackPieces.insert(std::move(nh1));
                                    auto nh2 = blackPieces.extract("h8");
                                    nh2.key() = "f8";
                                    blackPieces.insert(std::move(nh2));
                                }
                                else if((canCastleQ == true) && (key == "c8")) //if player can castle queen side and chooses to
                                {
                                    //move piece then deselect
                                    blackPieces.at(selectedSquare).movePiece(key);
                                    blackPieces.at("a8").movePiece("d8");
                                    blackPieces.at(selectedSquare).deselectPiece();
                                    blackKingLocation = key;
                                    //update key of piece moved
                                    auto nh1 = blackPieces.extract(selectedSquare);
                                    nh1.key() = key;
                                    blackPieces.insert(std::move(nh1));
                                    auto nh2 = blackPieces.extract("a8");
                                    nh2.key() = "d8";
                                    blackPieces.insert(std::move(nh2));
                                }
                                // updateLocations(whitePieces, blackPieces, whitePieceLocations, blackPieceLocations);
                                selectedSquare = "";
                                whiteTurn = !whiteTurn;
                                whiteKingInCheck = isKingInCheck(blackPieces,whitePieceLocations,blackPieceLocations,whiteKingLocation,squaresAttackedByBlack);
                            }                            
                        }
                    }
            }
        }

        

        window.clear();
        window.draw(board);
        // window.draw(moves);

        for (auto itr = whitePieces.begin(); itr != whitePieces.end(); itr++) { 
            itr->second.draw(window);
        } 

        for (auto itr = blackPieces.begin(); itr != blackPieces.end(); itr++) { 
            itr->second.draw(window);
        }

        if(winningPlayer != "")
        {
            window.setTitle(winningPlayer + " Wins!");
        }
        else
        {
            if(whiteTurn)
            {
                window.setTitle("White's turn");
            }
            else
            {
                window.setTitle("Black's turn");
            }
        }
        
        window.display();

    }

    return 0;
}


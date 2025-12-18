#include "State.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

using json = nlohmann::json;
using std::ofstream;
using std::ifstream;
using std::string;

State::State() {}

State::State(Piece* board[8][8], MoveNode* prevMove) {
    // Copy board pieces
    updateBoard(board);

    // Copy prev move
    _prevMove = prevMove;
}

void State::updateBoard(Piece* board[8][8]) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            currentBoard[i][j] = board[i][j];
        }
    }
}

Piece* State::makePiece(string pieceStr) {
    /* 
     * Takes "pieceType, moveCount, color" as input
     * Also returns nullptr if string is empty
     * Creates & returns dynamically allocated
     */

    // Initialise vars
    Piece* piece = nullptr;
    bool typeRead = false;
    char pieceType = '\0';
    bool movesCounted = false;
    int moveCount = 0;
    char pieceColour = '\0';

    // Iterate through string
    for (int i = 0; i < (int)pieceStr.length(); i++) {
        if (pieceStr[i] != ' ') {
            if (!typeRead) { // Get piece type
                if (pieceStr[i] != ',') pieceType = pieceStr[i];
                else typeRead = true;
            } else if (!movesCounted) { // Get moves counted
                if (pieceStr[i] == ',') movesCounted = true;
                else {
                    moveCount *= 10;
                    moveCount += (int)pieceStr[i] - '0';
                }
            } else if (pieceColour == '\0') { // Get colour
                pieceColour = pieceStr[i];
            }
        }
    }

    // Construct piece
    switch(pieceType) {
        case 'r':
            piece = new Rook(pieceColour);
            break;
        case 'b':
            piece = new Bishop(pieceColour);
            break;
        case 'n':
            piece = new Knight(pieceColour);
            break;
        case 'k':
            piece = new King(pieceColour);
            break;
        case 'q':
            piece = new Queen(pieceColour);
            break;
        case 'p':
            piece = new Pawn(pieceColour);
            break;
    }

    // Make moves
    if (piece != nullptr) piece->setMoveCount(moveCount);

    return piece;
}

string State::storePiece(Piece* piece) {
    // Store a piece as a string
    string returnStr = "";
    if (piece == nullptr) return returnStr;

    // Get piece type
    returnStr += piece->getType();
    returnStr += ", ";

    // Calculate move count
    int moveCount = piece->getMoveCount();
    string moveString = "";
    if (moveCount == 0) moveString += '0';

    while (moveCount > 0) {
        // Get each multiple of 10
        moveString += (char)(moveCount % 10) + '0';
        moveCount -= moveCount % 10;
        moveCount /= 10;
    }

    while (!moveString.empty()) {
        // Reverse string
        returnStr += moveString.back();
        moveString.pop_back();
    }

    // Store color
    returnStr += ", ";
    returnStr += piece->getColor();
    return returnStr;
}

bool State::loadCurrentBoard() {
    // Load board from JSON file
    ifstream file("./build/board.json");
    if (!file.is_open()) return false;
    
    try {
        json j;
        file >> j;
        file.close();
        
        // Load board from JSON
        // Note: JSON stores rank 0 as bottom (white's home rank)
        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                auto& square = j[rank][file];
                std::string type = square["type"].get<std::string>();
                
                if (!type.empty()) {
                    char pieceType = type[0];
                    char color = square["color"].get<std::string>()[0];
                    int moved = square["moved"];
                    
                    Piece* piece = nullptr;
                    switch(pieceType) {
                        case 'p': piece = new Pawn(color); break;
                        case 'r': piece = new Rook(color); break;
                        case 'n': piece = new Knight(color); break;
                        case 'b': piece = new Bishop(color); break;
                        case 'q': piece = new Queen(color); break;
                        case 'k': piece = new King(color); break;
                    }
                    
                    if (piece) {
                        piece->setMoveCount(moved);
                        currentBoard[file][rank] = piece;
                    }
                } else {
                    currentBoard[file][rank] = nullptr;
                }
            }
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading board: " << e.what() << std::endl;
        return false;
    }
}

bool State::loadPrevMoves() {
    // Load moves from JSON file
    ifstream file("./build/moves.json");
    if (!file.is_open()) return false;
    
    try {
        json j;
        file >> j;
        file.close();
        
        _prevMove = new MoveNode();
        
        // Apply moves from JSON
        for (const auto& moveJson : j) {
            int startRow = moveJson["startRow"];
            int startCol = moveJson["startCol"];
            int endRow = moveJson["endRow"];
            int endCol = moveJson["endCol"];
            bool isSpecial = moveJson["isSpecial"];
            
            // Handle captured piece
            Piece* capturedPiece = nullptr;
            if (!moveJson["capturedPiece"].is_null()) {
                char type = moveJson["capturedPiece"].get<std::string>()[0];
                // For captured pieces, we don't know the color from the JSON
                // We'll need to determine it from context or store it properly
                char color = 'B'; // Default - we need better logic here
                
                switch(type) {
                    case 'p': capturedPiece = new Pawn(color); break;
                    case 'r': capturedPiece = new Rook(color); break;
                    case 'n': capturedPiece = new Knight(color); break;
                    case 'b': capturedPiece = new Bishop(color); break;
                    case 'q': capturedPiece = new Queen(color); break;
                    case 'k': capturedPiece = new King(color); break;
                }
            }
            
            // Handle promoted piece
            Piece* promotedPiece = nullptr;
            if (!moveJson["promotionPiece"].is_null()) {
                char type = moveJson["promotionPiece"].get<std::string>()[0];
                // Promotion piece color should be the same as the moving piece
                // We need to track this better
                char color = 'W'; // Default
                
                switch(type) {
                    case 'p': promotedPiece = new Pawn(color); break;
                    case 'r': promotedPiece = new Rook(color); break;
                    case 'n': promotedPiece = new Knight(color); break;
                    case 'b': promotedPiece = new Bishop(color); break;
                    case 'q': promotedPiece = new Queen(color); break;
                    case 'k': promotedPiece = new King(color); break;
                }
            }
            
            _prevMove->addMove(startCol, startRow, endCol, endRow, isSpecial, promotedPiece, capturedPiece);
        }
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading moves: " << e.what() << std::endl;
        return false;
    }
}

bool State::loadGame(Piece* board[8][8], MoveNode** node) {
    // Load from JSON files
    if (!loadCurrentBoard()) return false;
    if (!loadPrevMoves()) return false;

    // Output loaded game to input variables
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = currentBoard[i][j];
        }
    }
    *node = this->_prevMove;
    return true;
}

void State::saveState() {
    // Save board and moves to JSON files
    saveBoard();
    saveAllMoves(_prevMove);
}

void State::saveState(Piece* board[8][8]) {
    // Update board and save
    updateBoard(board);
    saveBoard();
    
    // Save moves
    saveMove(_prevMove);
}

void State::saveBoard() {
    // Save board as JSON
    json j = json::array();
    
    for (int rank = 0; rank < 8; rank++) {
        json row = json::array();
        for (int file = 0; file < 8; file++) {
            json pieceJson;
            if (currentBoard[file][rank] != nullptr) {
                pieceJson["type"] = std::string(1, currentBoard[file][rank]->getType());
                pieceJson["color"] = std::string(1, currentBoard[file][rank]->getColor());
                pieceJson["moved"] = currentBoard[file][rank]->getMoveCount();
            } else {
                pieceJson["type"] = "";
                pieceJson["color"] = "";
                pieceJson["moved"] = 0;
            }
            row.push_back(pieceJson);
        }
        j.push_back(row);
    }
    
    // Save to file
    ofstream file("./build/board.json");
    if (file.is_open()) {
        file << j.dump(2);  // Pretty print with 2-space indent
        file.close();
    }
}

void State::saveAllMoves(MoveNode* node) {
    // Collect all moves in reverse chronological order
    std::vector<MoveNode*> moves;
    MoveNode* current = node;
    
    while (current != nullptr && current->prev() != nullptr) {
        moves.push_back(current);
        current = current->prev();
    }
    
    // Reverse to get chronological order
    std::reverse(moves.begin(), moves.end());
    
    // Create JSON array
    json j = json::array();
    
    for (MoveNode* move : moves) {
        json moveJson;
        moveJson["startRow"] = move->getOldRank();
        moveJson["startCol"] = move->getOldFile();
        moveJson["endRow"] = move->getNewRank();
        moveJson["endCol"] = move->getNewFile();
        
        if (move->getCapturedPiece() != nullptr) {
            moveJson["capturedPiece"] = std::string(1, move->getCapturedPiece()->getType());
        } else {
            moveJson["capturedPiece"] = nullptr;
        }
        
        moveJson["isSpecial"] = move->enPassant();
        
        if (move->getPromotedPiece() != nullptr) {
            moveJson["promotionPiece"] = std::string(1, move->getPromotedPiece()->getType());
        } else {
            moveJson["promotionPiece"] = nullptr;
        }
        
        j.push_back(moveJson);
    }
    
    // Save to file
    ofstream file("./build/moves.json");
    if (file.is_open()) {
        file << j.dump(2);
        file.close();
    }
}

void State::saveMove(MoveNode* node) {
    // Just call saveAllMoves to rewrite the entire file
    saveAllMoves(node);
}
#pragma once

#include "Pawn.h"
#include "King.h"
#include "Queen.h"
#include "Knight.h"
#include "Rook.h"
#include "Bishop.h"
#include "MoveNode.h"
#include <fstream>
#include <string>
using std::fstream;
using std::string;

class State {//խաղի ամբողջ վիճակը պահելու, թարմացնելու և ֆայլերում պահպանելու համար.
    private:
  
        Piece* currentBoard[8][8];//pahpanum e yntacik taxtaky

        MoveNode* _prevMove;//cucich depi verjin qayl
       
        fstream _file;//fstream object save ev loadi hamar

        Piece* makePiece(string pieceStr);//Piece object faylic kardalu hamar
        string storePiece(Piece* piece);//string vory pahuma piece-i masin data

        void updateBoard(Piece* board[8][8]);//popoxuma taxtaky

        // Load currentBoard from file
        bool loadCurrentBoard();//faylic load a anum currentboard
      
        bool loadPrevMoves();//load a anum naxord sharjumy

        // Save board to file (overwrite save)
        void saveBoard();
        // Save most recent move to file
        void saveMove(MoveNode* node);
        // Save all moves to file (overwrite save)
        void saveAllMoves(MoveNode* node);

    public:
        // Constructors

        // Load information from file
        State();
        // Take information from input
        State(Piece* board[8][8], MoveNode* prevMove);

        // Save to file

        // Overwrite existing save
        void saveState();
        // Update existing save
        void saveState(Piece* board[8][8]);

        // Load currentBoard and _prevMove to input board & node
        bool loadGame(Piece* board[8][8], MoveNode** node);
};

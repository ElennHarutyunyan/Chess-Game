#pragma once 

#include "Piece.h"

class MoveNode {
    /* LINKED LIST */
    private:
        // Stores move information
        int _oldFile;//hin syuni dirqy
        int _oldRank;//hin toxi dirqna
        int _newFile;//nor syuni dirq
        int _newRank;//nor toxi dirqy

        // Stores the captured piece
        Piece* _capturedPiece;//ete vercnenq hakarakordi qary

        // Stores the promoted piece
        Piece* _promotedPiece;//zinvory hasav verjin vandak hakarakordi

        // Did this move take by en passant?
        bool _enPassant;//zinvori koxancik vercman gaxapar

        // Stores a pointer to the previous node
        MoveNode* _prevNode;//pahuma naxord nodei hascen

    public:
    
        // Constructors
        MoveNode();
        MoveNode(int oldFile, int oldRank, int newFile, int newRank, 
                bool enPassant, Piece* promotedPiece, Piece* capturedPiece);//hanguyc aranc prev
        MoveNode(int oldFile, int oldRank, int newFile, int newRank, 
                bool enPassant, Piece* promotedPiece, Piece* capturedPiece, MoveNode*);//nor sharjum

        // Add move to list
        void addMove(int oldFile, int oldRank, int newFile, int newRank, 
                bool enPassant, Piece* promotedPiece, Piece* capturedPiece);

        // Access previous node
        MoveNode* prev();
        MoveNode* prev(int distance);
        // Access information
        int getOldFile();
        int getOldRank();
        int getNewFile();
        int getNewRank();
        Piece* getCapturedPiece();//vercvac xaxaqar
        bool enPassant();//koxancik vercnel
        Piece* getPromotedPiece();//hasela verjin rank

        // Reverse the input board by input moves
        void reverseBoard(Piece* board[8][8], int moves);
        // Undo the above function
        void unreverseBoard(Piece* board[8][8], int moves);

        // Destructor
        ~MoveNode();
};

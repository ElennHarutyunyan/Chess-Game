#pragma once//header fayly nermucvum e mek angam kanxum e krknaki sahmanumy

#include "Piece.h"

class Bishop : public Piece {//Piece classi jarang Bishop
    public:
        // Constructor. Set color of piece
        Bishop(char color);//set a anum colory

        // Check whether a move is valid
        bool checkMoveValidity(int oldFile, int oldRank, int newFile, int newRank);//stuguma qayly valida te che
};

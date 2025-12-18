#pragma once////header fayly nermucvum e mek angam kanxum e krknaki sahmanumy

#include "Piece.h"

class King : public Piece {
    public:
        // Constructor. Set color of piece
        King(char color);

        // Check whether a move is valid
        bool checkMoveValidity(int oldFile, int oldRank, int newFile, int newRank);
};

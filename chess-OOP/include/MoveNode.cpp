#include "MoveNode.h"
#include <cstdlib>

// default Constructors
MoveNode::MoveNode() {
    _oldFile = -1;//skzbnakan -1
    _newFile = -1;//skzbnakan -1
    _oldRank = -1;//skzbnakan -1
    _newRank = -1;//skzbnakan -1
    _enPassant = false;//hazvadep qayla dra hamar false
    _promotedPiece = nullptr;//zinvory hasnum a verjin rank
    _capturedPiece = nullptr;//cucich depi vercrac xaxaqar
    _prevNode = nullptr;//cucich depi naxord qayl
}

MoveNode::MoveNode(int oldFile, int oldRank, int newFile, int newRank, 
                    bool enPassant, Piece* promotedPiece, Piece* capturedPiece) {
    _oldFile = oldFile;
    _oldRank = oldRank;
    _newFile = newFile;
    _newRank = newRank;
    _enPassant = enPassant;
    _promotedPiece = promotedPiece;
    _capturedPiece = capturedPiece;
    _prevNode = nullptr;
}

MoveNode::MoveNode(int oldFile, int oldRank, int newFile, int newRank, 
                    bool enPassant, Piece* promotedPiece, Piece* capturedPiece, MoveNode* prevNode) {
    _oldFile = oldFile;
    _oldRank = oldRank;
    _newFile = newFile;
    _newRank = newRank;
    _enPassant = enPassant;
    _promotedPiece = promotedPiece;
    _capturedPiece = capturedPiece;
    _prevNode = prevNode;
}

// Add move
void MoveNode::addMove(int oldFile, int oldRank, int newFile, int newRank,
                    bool enPassant, Piece* promotedPiece, Piece* capturedPiece) {
    MoveNode* prevMove = new MoveNode(_oldFile, _oldRank, _newFile, _newRank,
                                    _enPassant, _promotedPiece, _capturedPiece, _prevNode);
    _prevNode = prevMove;
    _oldFile = oldFile;
    _oldRank = oldRank;
    _newFile = newFile;
    _newRank = newRank;
    _enPassant = enPassant;
    _promotedPiece = promotedPiece;
    _capturedPiece = capturedPiece;
}

// Access previous node
MoveNode* MoveNode::prev() {
    return _prevNode;
}

MoveNode* MoveNode::prev(int distance) {//qani qayl piti het gnanq
    if (_prevNode == nullptr) {//voch mi qayl naxkinum chi exel
        return this;//kmna nuyny 
    } else if (distance > 1) {//ete yndameny mi qayla katarvel chi mtni
        return prev()->prev(distance-1);//ete mekic shat rekursiv het kgna
    } else {
        return prev();//ete mi qayla exel kveradarcni prev
    }
}

// Access information
int MoveNode::getOldFile() {return _oldFile;}//veradarcnuma naxord syuny
int MoveNode::getOldRank() {return _oldRank;}//veradarcnuma naxord toxy
int MoveNode::getNewFile() {return _newFile;}//veradarcnuma nor syuny
int MoveNode::getNewRank() {return _newRank;}//veradarcnuma nor toxy
Piece* MoveNode::getCapturedPiece() {return _capturedPiece;}//veradarcnuma vercrac xaxaqary
bool MoveNode::enPassant() {return _enPassant;}//veradarcnuma koxancik vercnum ka te che
Piece* MoveNode::getPromotedPiece() {return _promotedPiece;}//veradarcnuma zinvory verjin ranki vra te che

// Reverse 
void MoveNode::reverseBoard(Piece* board[8][8], int moves) {
    // Reverses the input board by [moves] moves
    Piece* sourcePiece = board[_newFile][_newRank];//nor dirqi cucich
    board[_oldFile][_oldRank] = sourcePiece;//nory darcnum enq hin

    // Reverse capture
    if (_enPassant) {//ete enPassant linuma
        board[_newFile][_newRank] = nullptr;//nor dirqy darnuma datark
        board[_newFile][_oldRank] = _capturedPiece;//hakarakordi zinvory galisa nor dirq
    } else {
        board[_newFile][_newRank] = _capturedPiece;//sovorakan qayla anum
    }
    
    // Reverse pieces
    sourcePiece->reverseMove();
    if (_capturedPiece != nullptr) _capturedPiece->reverseMove();//vercrac qarernel enq veradarcnum

    // Castle 
    if (sourcePiece->getType() == 'k') {//stuguma xaxaqary kinga te che 
        if (abs(_newFile - _oldFile) == 2) {//ete king erku qayla arel syunov poxatexuma exel
            if (_newFile > _oldFile) { // Castling short
                board[7][_oldRank] = board[_newFile-1][_newRank];//navaky dnum enq skzbnakan dirqum
                board[_newFile-1][_newRank] = nullptr;//hin dirqy datarka linum
                board[7][_oldRank]->reverseMove();//navaki hin dirqy verakangnum enq
            } else { // castling long
                board[0][_oldRank] = board[_newFile+1][_newRank];//veradarcvuma dax dirqi skzbnakan dirqin
                board[_newFile+1][_newRank] = nullptr;//hin dirqy datarkum enq 
                board[0][_oldRank]->reverseMove();//verakangnum enq hin dirqy
            }
        }
    }

    // Promotion
    if (_promotedPiece != nullptr) {//zinvory xaxaqar a darel
        board[_oldFile][_oldRank] = _promotedPiece;//zinvory drvuma hin dirqum
        _promotedPiece = sourcePiece;//pahum enq popoxvac xaxaqary
    }

    // Finish reversing this move
    if (moves == 1) return;//ete mi qay enq arel het gnalu kariq chka
    else _prevNode->reverseBoard(board, moves-1);//het enq gnum amen angam poqracnelov moves-y
}

void MoveNode::unreverseBoard(Piece* board[8][8], int moves) {
    // Un-reverses the input board by [moves] moves
    if (moves > 1) _prevNode->unreverseBoard(board, moves-1);//het enq gnum minchev arajin qayl
    if (moves == 0) return;//ban chenq anum ete vabshe qayl chenq arel

    Piece* sourcePiece = board[_oldFile][_oldRank];//talis enq hin dirqery

    // Make move
    board[_newFile][_newRank] = sourcePiece;//nor dashtum dnum enq xaxaqar
    board[_oldFile][_oldRank] = nullptr;//hin dashty datarkum enq

    // Update pieces
    sourcePiece->move();//xaxaqary sharjvuma
    if (_capturedPiece != nullptr) {//ete vercrac lini chi masnakci xaxin
        _capturedPiece->capture();//ays xaxaqary vercrac e 
    }

    // En passant
    if (_enPassant) {//ete en passant a exel 
        board[_newFile][_oldRank] = nullptr;//hin dirqy darcnuma datark
    }

    // Castle 
    if (sourcePiece->getType() == 'k') {//ete xaxaqary tagavor e
        if (abs(_newFile - _oldFile) == 2) {//syunov 2 qayla anum
            if (_newFile > _oldFile) { // Castling short  aj masov
                board[_newFile-1][_oldRank] = board[7][_oldRank];
                board[7][_oldRank] = nullptr;
                board[_newFile-1][_oldRank]->move();
            } else { // castling long
                board[_newFile+1][_oldRank] = board[0][_oldRank];
                board[0][_oldRank] = nullptr;
                board[_newFile+1][_oldRank]->move();
            }
        }
    }

    // Promote
    if (_promotedPiece != nullptr) {//ardyoq darela nor xaxaqar
        board[_newFile][_newRank] = _promotedPiece;//nor texum dnum enq xaxaqary
        _promotedPiece->move();//nshum enq vor sharjvela xaxaqary
        _promotedPiece = sourcePiece;//pahum enq hin zivory
    }

    return;
}

// Destructor
MoveNode::~MoveNode() {
    if (_prevNode != nullptr) {//stuguma exel a naxkin qayl
        delete _prevNode;//jnjum enq 
    }
}